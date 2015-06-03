#ifndef OPENMM_REFERENCEAGBNP3KERNELFACTORY_H_
#define OPENMM_REFERENCEAGBNP3KERNELFACTORY_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM-AGBNP3                            *
 * -------------------------------------------------------------------------- */

#include "openmm/KernelFactory.h"

namespace OpenMM {

/**
 * This KernelFactory creates kernels for the reference implementation of the 
 * AGBNP3 plugin.
 */

class ReferenceAGBNP3KernelFactory : public KernelFactory {
public:
    KernelImpl* createKernelImpl(std::string name, const Platform& platform, ContextImpl& context) const;
};

} // namespace OpenMM

#endif /*OPENMM_REFERENCEAGBNP3KERNELFACTORY_H_*/
