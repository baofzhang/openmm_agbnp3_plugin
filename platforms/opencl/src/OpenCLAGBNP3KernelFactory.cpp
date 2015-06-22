/* -------------------------------------------------------------------------- *
 *                            OpenMM-AGBNP3                                   *
 * -------------------------------------------------------------------------- */

#include <exception>

#include "OpenCLAGBNP3KernelFactory.h"
#include "OpenCLAGBNP3Kernels.h"
#include "openmm/internal/windowsExport.h"
#include "openmm/internal/ContextImpl.h"
#include "openmm/OpenMMException.h"

using namespace AGBNP3Plugin;
using namespace OpenMM;

extern "C" OPENMM_EXPORT void registerPlatforms() {
}

extern "C" OPENMM_EXPORT void registerKernelFactories() {
    try {
        Platform& platform = Platform::getPlatformByName("OpenCL");
        OpenCLAGBNP3KernelFactory* factory = new OpenCLAGBNP3KernelFactory();
        platform.registerKernelFactory(CalcAGBNP3ForceKernel::Name(), factory);
    }
    catch (std::exception ex) {
        // Ignore
    }
}

extern "C" OPENMM_EXPORT void registerAGBNP3OpenCLKernelFactories() {
    try {
        Platform::getPlatformByName("OpenCL");
    }
    catch (...) {
        Platform::registerPlatform(new OpenCLPlatform());
    }
    registerKernelFactories();
}

KernelImpl* OpenCLAGBNP3KernelFactory::createKernelImpl(std::string name, const Platform& platform, ContextImpl& context) const {
    OpenCLContext& cl = *static_cast<OpenCLPlatform::PlatformData*>(context.getPlatformData())->contexts[0];
    if (name == CalcAGBNP3ForceKernel::Name())
        return new OpenCLCalcAGBNP3ForceKernel(name, platform, cl, context.getSystem());
    throw OpenMMException((std::string("Tried to create kernel with illegal kernel name '")+name+"'").c_str());
}
