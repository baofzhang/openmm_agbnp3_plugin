#ifndef OPENMM_OPENCLAGBNP3KERNELFACTORY_H_
#define OPENMM_OPENCLAGBNP3KERNELFACTORY_H_

/* -------------------------------------------------------------------------- *
 *                              OpenMM-AGBNP3                                 *
 * -------------------------------------------------------------------------- */

#include "openmm/KernelFactory.h"

namespace AGBNP3Plugin {

/**
 * This KernelFactory creates kernels for the OpenCL implementation of the AGBNP3 plugin.
 */

class OpenCLAGBNP3KernelFactory : public OpenMM::KernelFactory {
public:
    OpenMM::KernelImpl* createKernelImpl(std::string name, const OpenMM::Platform& platform, OpenMM::ContextImpl& context) const;
};

} // namespace AGBNP3Plugin

#endif /*OPENMM_OPENCLAGBNP3KERNELFACTORY_H_*/
