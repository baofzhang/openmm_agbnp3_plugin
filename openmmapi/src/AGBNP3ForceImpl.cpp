/* -------------------------------------------------------------------------- *
 *                            OpenMM-AGBNP3                                   *
 * -------------------------------------------------------------------------- */

#ifdef WIN32
  #define _USE_MATH_DEFINES // Needed to get M_PI
#endif
#include "internal/AGBNP3ForceImpl.h"
#include "AGBNP3Kernels.h"
#include "openmm/OpenMMException.h"
#include "openmm/internal/ContextImpl.h"
#include <cmath>
#include <map>
#include <set>
#include <sstream>

using namespace AGBNP3Plugin;
using namespace OpenMM;
using namespace std;

AGBNP3ForceImpl::AGBNP3ForceImpl(const AGBNP3Force& owner) : owner(owner) {
}

AGBNP3ForceImpl::~AGBNP3ForceImpl() {
}

void AGBNP3ForceImpl::initialize(ContextImpl& context) {
    kernel = context.getPlatform().createKernel(CalcAGBNP3ForceKernel::Name(), context);
    kernel.getAs<CalcAGBNP3ForceKernel>().initialize(context.getSystem(), owner);
}

double AGBNP3ForceImpl::calcForcesAndEnergy(ContextImpl& context, bool includeForces, bool includeEnergy, int groups) {
  return kernel.getAs<CalcAGBNP3ForceKernel>().execute(context, includeForces, includeEnergy);
}

std::vector<std::string> AGBNP3ForceImpl::getKernelNames() {
    std::vector<std::string> names;
    names.push_back(CalcAGBNP3ForceKernel::Name());
    return names;
}

void AGBNP3ForceImpl::updateParametersInContext(ContextImpl& context) {
    kernel.getAs<CalcAGBNP3ForceKernel>().copyParametersToContext(context, owner);
}
