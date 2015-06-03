#ifndef OPENMM_AGBNP3FORCE_H_
#define OPENMM_AGBNP3FORCE_H_

/* -------------------------------------------------------------------------- *
 *                         OpenMM-AGBNP3                                      *
 * -------------------------------------------------------------------------- */

#include "openmm/Context.h"
#include "openmm/Force.h"
#include <vector>
#include "internal/windowsExportAGBNP3.h"

namespace AGBNP3Plugin {

/* HB types */
#define AGBNP_HB_INACTIVE   (0)
/* donor polar hydrogen */
#define AGBNP_HB_POLARH       (1)
/* acceptor geometries */
#define AGBNP_HB_TRIGONAL    (10)
#define AGBNP_HB_TRIGONAL_S  (30)
#define AGBNP_HB_TETRAHEDRAL (20)
/* HB subtypes */
#define AGBNP_HB_TRIGONAL1    (11)
#define AGBNP_HB_TRIGONAL2    (12)
#define AGBNP_HB_TRIGONAL_OOP (13) /* out of plane */
#define AGBNP_HB_TETRAHEDRAL1 (21) /* like O in sulfones */
#define AGBNP_HB_TETRAHEDRAL2 (22) /* like sp3 O */
#define AGBNP_HB_TETRAHEDRAL3 (23) /* like sp3 N */

/* This class implements the AGBNP3 implicit solvation model */

class OPENMM_EXPORT_AGBNP3 AGBNP3Force : public OpenMM::Force {
public:
    /**
     * Create an AGBNP3Force.
     */
    AGBNP3Force();
    /**
     * Add a particle to AGBNP3
     *
     * This should be called once for each particle in the System. When it is called for the i'th time, 
     * it specifies the parameters for the i'th particle.
     *
     * @param radius      the van der Waals radius of the particle, measured in nm
     * @param charge      the charge of the particle, measured in units of the proton charge
     * @param gamma       the surface tension parameter, measured in kJ/mol/nm^2
     * @param alpha       the solute-solvent dispersion energy parameter, dimensionless
     * @param hbtype      hydrogen-bond to solvent type index, see above
     * @param hbcorr      hydrogen bond to solvent strength parameter, measured in kJ/mol
     * @param ishydrogen  if not zero, this particle is a hydrogen atom 
     * @return the index of the particle that was added
     */
    int addParticle(double radius, double charge, double gamma, double alpha, 
		    int hbtype, double hbcorr, int ishydrogen);
    /**
     * Insert p1,p2 pair into connection table 
     * 
     * @param p1          index of first particle
     * @param p2          index of second particle
     * @return the number of neighbors of p1
     */
    int addParticleConnection(int p1, int p2) ;

    /**
     * Get the AGBNP3 parameters for a particle
     * 
     * @param index       the index of the particle
     * @param radius      the van der Waals radius of the particle, measured in nm
     * @param charge      the charge of the particle, measured in units of the proton charge
     * @param gamma       the surface tension parameter, measured in kJ/mol/nm^2
     * @param alpha       the solute-solvent dispersion energy parameter, dimensionless
     * @param hbtype      hydrogen-bond to solvent type index, see below
     * @param hbcorr      hydrogen bond to solvent strength parameter, measured in kJ/mol
     * @param ishydrogen  if not zero, this particle is a hydrogen atom
     * @param neighbors   the list of neighbors, covalently connected
     */
    void getParticleParameters(int index, double& radius, double& charge, double& gamma, double& alpha, 
			       int& hbtype, double& hbcorr, int& ishydrogen, 
			       std::vector<int>& neighbors) const;
    /**
     * Get the number of particles defined for AGBNP3
     */
    int getNumParticles() const {
        return particles.size();
    }

    void updateParametersInContext(OpenMM::Context& context);
protected:
    OpenMM::ForceImpl* createImpl() const;
private:
    class ParticleInfo;
    std::vector<ParticleInfo> particles;
};

/**
 * This is an internal class used to record information about particles.
 * @private
 */
class AGBNP3Force::ParticleInfo {
 public:
  int ishydrogen, hbtype;
  double radius, charge, gamma, alpha, hbcorr;
  std::vector<int> neighbors; //connection table 
  ParticleInfo() {
    ishydrogen = 0;
    radius = 0.15;
    charge = 0.0;
    gamma = 0.0;
    alpha = 0.0;
    hbtype = 1;
    hbcorr = 0.0;
  }
 ParticleInfo(double radius, double charge, double gamma, double alpha, 
	      int hbtype, double hbcorr, int ishydrogen) :
  radius(radius), charge(charge), gamma(gamma), alpha(alpha), hbtype(hbtype), 
    hbcorr(hbcorr), ishydrogen(ishydrogen) {  }
 };
 
} // namespace AGBNP3Plugin

#endif /*OPENMM_AGBNP3FORCE_H_*/
