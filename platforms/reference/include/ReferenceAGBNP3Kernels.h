#ifndef REFERENCE_AGBNP3_KERNELS_H_
#define REFERENCE_AGBNP3_KERNELS_H_

/* -------------------------------------------------------------------------- *
 *                           OpenMM-AGBNP3                                    *
 * -------------------------------------------------------------------------- */

#include "AGBNP3Kernels.h"
#include "openmm/Platform.h"
#include <vector>

namespace AGBNP3Plugin {

/**
 * This kernel is invoked by AGBNP3Force to calculate the forces acting 
 * on the system and the energy of the system.
 */
class ReferenceCalcAGBNP3ForceKernel : public CalcAGBNP3ForceKernel {
public:
    ReferenceCalcAGBNP3ForceKernel(std::string name, const OpenMM::Platform& platform) : CalcAGBNP3ForceKernel(name, platform) {
    }
    /**
     * Initialize the kernel.
     * 
     * @param system     the System this kernel will be applied to
     * @param force      the AGBNP3Force this kernel will be used for
     */
    void initialize(const OpenMM::System& system, const AGBNP3Force& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @return the potential energy due to the force
     */
    double execute(OpenMM::ContextImpl& context, bool includeForces, bool includeEnergy);
    /**
     * Copy changed parameters over to a context.
     *
     * @param context    the context to copy parameters to
     * @param force      the AGBNP3Force to copy the parameters from
     */
    void copyParametersToContext(OpenMM::ContextImpl& context, const AGBNP3Force& force);
private:
    int numParticles;
    int agbnp3_tag; //the id of the agbnp3 instance from the agbnp3 library
    // inputs
    std::vector<double> x, y, z;
    std::vector<int> hbtype, ishydrogen;
    std::vector<double> radius, charge, hbcorr;
    std::vector<double> gamma, alpha;
    std::vector<double> sgamma, salpha;
    //outputs
    double mol_volume, egb, evdw, ecorr_vdw, ecav, ecorr_cav, ehb;
    std::vector<double> sp, br, surf_area;
    double (*dgbdr)[3], (*dvwdr)[3], (*decav)[3], (*dehb)[3];
};

} // namespace AGBNP3Plugin

#endif /*REFERENCE_AGBNP3_KERNELS_H_*/
