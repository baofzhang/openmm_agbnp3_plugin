#ifndef AGBNP3_KERNELS_H_
#define AGBNP3_KERNELS_H_

/* -------------------------------------------------------------------------- *
 *                              OpenMM-AGBNP3                                 *
 * -------------------------------------------------------------------------- */

#include "AGBNP3Force.h"
#include "openmm/KernelImpl.h"
#include "openmm/Platform.h"
#include "openmm/System.h"
#include <string>

namespace AGBNP3Plugin {

/**
 * This kernel is invoked by AGBNP3Force to calculate the forces acting on the system and the energy of the system.
 */
class CalcAGBNP3ForceKernel : public OpenMM::KernelImpl {
public:
    static std::string Name() {
        return "CalcAGBNP3Force";
    }
    CalcAGBNP3ForceKernel(std::string name, const OpenMM::Platform& platform) : OpenMM::KernelImpl(name, platform) {
    }
    /**
     * Initialize the kernel.
     * 
     * @param system     the System this kernel will be applied to
     * @param force      the AGBNP3Force this kernel will be used for
     */
    virtual void initialize(const OpenMM::System& system, const AGBNP3Force& force) = 0;
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @return the potential energy due to the force
     */
    virtual double execute(OpenMM::ContextImpl& context, bool includeForces, bool includeEnergy) = 0;
    /**
     * Copy changed parameters over to a context.
     *
     * @param context    the context to copy parameters to
     * @param force      the AGBNP3Force to copy the parameters from
     */
    virtual void copyParametersToContext(OpenMM::ContextImpl& context, const AGBNP3Force& force) = 0;
};

} // namespace AGBNP3Plugin

#endif /*AGBNP3_KERNELS_H_*/
