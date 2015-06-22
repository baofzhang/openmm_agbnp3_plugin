#ifndef OPENCL_AGBNP3_KERNELS_H_
#define OPENCL_AGBNP3_KERNELS_H_

/* -------------------------------------------------------------------------- *
 *                           OpenMM-AGBNP3                                    *
 * -------------------------------------------------------------------------- */

#include "AGBNP3Kernels.h"
#include "openmm/opencl/OpenCLContext.h"
#include "openmm/opencl/OpenCLArray.h"

namespace AGBNP3Plugin {

/**
 * This kernel is invoked by AGBNP3Force to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLCalcAGBNP3ForceKernel : public CalcAGBNP3ForceKernel {
public:
    OpenCLCalcAGBNP3ForceKernel(std::string name, const OpenMM::Platform& platform, OpenMM::OpenCLContext& cl, const OpenMM::System& system) :
            CalcAGBNP3ForceKernel(name, platform), hasInitializedKernel(false), cl(cl), system(system), params(NULL) {
    }

    ~OpenCLCalcAGBNP3ForceKernel();
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
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
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
    bool hasInitializedKernel;
    OpenMM::OpenCLContext& cl;
    const OpenMM::System& system;
    OpenMM::OpenCLArray* params;
};

} // namespace AGBNP3Plugin

#endif /*OPENCL_AGBNP3_KERNELS_H_*/
