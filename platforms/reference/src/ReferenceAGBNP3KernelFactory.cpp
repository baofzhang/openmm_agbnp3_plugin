/* -------------------------------------------------------------------------- *
 *                              OpenMM-AGBNP3                                 *
 * -------------------------------------------------------------------------- */

#include "ReferenceAGBNP3KernelFactory.h"
#include "ReferenceAGBNP3Kernels.h"
#include "openmm/reference/ReferencePlatform.h"
#include "openmm/internal/ContextImpl.h"
#include "openmm/OpenMMException.h"

using namespace AGBNP3Plugin;
using namespace OpenMM;

extern "C" OPENMM_EXPORT void registerPlatforms() {
}

extern "C" OPENMM_EXPORT void registerKernelFactories() {
    for (int i = 0; i < Platform::getNumPlatforms(); i++) {
        Platform& platform = Platform::getPlatform(i);
        if (dynamic_cast<ReferencePlatform*>(&platform) != NULL) {
            ReferenceAGBNP3KernelFactory* factory = new ReferenceAGBNP3KernelFactory();
            platform.registerKernelFactory(CalcAGBNP3ForceKernel::Name(), factory);
        }
    }
}

extern "C" OPENMM_EXPORT void registerAGBNP3ReferenceKernelFactories() {
    registerKernelFactories();
}

KernelImpl* ReferenceAGBNP3KernelFactory::createKernelImpl(std::string name, const Platform& platform, ContextImpl& context) const {
    ReferencePlatform::PlatformData& data = *static_cast<ReferencePlatform::PlatformData*>(context.getPlatformData());
    if (name == CalcAGBNP3ForceKernel::Name())
        return new ReferenceCalcAGBNP3ForceKernel(name, platform);
    throw OpenMMException((std::string("Tried to create kernel with illegal kernel name '")+name+"'").c_str());
}
