%module AGBNP3plugin

%import(module="simtk.openmm") "OpenMMSwigHeaders.i"


/*
 * The following lines are needed to handle std::vector.
 * Similar lines may be needed for vectors of vectors or
 * for other STL types like maps.
 */

%include "std_vector.i"
namespace std {
  %template(vectord) vector<double>;
  %template(vectori) vector<int>;
};

%{
#include "AGBNP3Force.h"
#include "OpenMM.h" 
#include "OpenMMAmoeba.h"
#include "OpenMMDrude.h"
%}



/*
 * The code below strips all units before the wrapper
 * functions are called. This code also converts numpy
 * arrays to lists.
*/

%pythoncode %{
import simtk.openmm as mm
import simtk.unit as unit
%}


/* strip the units off of all input arguments */
%pythonprepend %{
try:
    args=mm.stripUnits(args)
except UnboundLocalError:
    pass
%}


/*
 * Add units to function outputs.
*/
%pythonappend AGBNP3Plugin::AGBNP3Force::getParticleParameters(int index, double& radius, double& charge, double& gamma, double& alpha,
							       int& hbtype, double& hbcorr, int& ishydrogen,
							       std::vector<int>& neighbors) const %{
    val[2] = unit.Quantity(val[2], unit.nanometer)
    val[3] = unit.Quantity(val[3], unit.elementary_charge)
    val[4] = unit.Quantity(val[4], unit.kilojoule_per_mole / (unit.nanometer * unit.nanometer))
    val[7] = unit.Quantity(val[7], unit.kilojoule_per_mole)
%}


namespace AGBNP3Plugin {

class AGBNP3Force : public OpenMM::Force {
public:
    AGBNP3Force();

    int getNumParticles() const;

    int addParticleConnection(int p1, int p2);

    void addParticle(double radius, double charge, double gamma, double alpha, 
		    int hbtype, double hbcorr, int ishydrogen);

    void updateParametersInContext(OpenMM::Context& context);

    /*
     * The reference parameters to this function are output values.
     * Marking them as such will cause swig to return a tuple.
    */
    %apply double& OUTPUT {double& radius};
    %apply double& OUTPUT {double& charge};
    %apply double& OUTPUT {double& gamma};
    %apply double& OUTPUT {double& alpha};
    %apply int& OUTPUT {int& hbtype};
    %apply double& OUTPUT {double& hbcorr};
    %apply int& OUTPUT {int& ishydrogen};
    %apply std::vector<int>& OUTPUT {std::vector<int>& neighbors}
    void getParticleParameters(int index, double& radius, double& charge, double& gamma, double& alpha, 
			       int& hbtype, double& hbcorr, int& ishydrogen, 
			       std::vector<int>& neighbors) const;
    //    %clear int& particle1;
    //    %clear int& particle2;
    //    %clear double& length;
    //    %clear double& k;
};

}

