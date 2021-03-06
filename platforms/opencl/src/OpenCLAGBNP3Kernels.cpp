/* -------------------------------------------------------------------------- *
 *                                   OpenMM-AGBNP3                            *
 * -------------------------------------------------------------------------- */

#include "OpenCLAGBNP3Kernels.h"
#include "OpenCLAGBNP3KernelSources.h"
#include "openmm/internal/ContextImpl.h"
#include "openmm/opencl/OpenCLNonbondedUtilities.h"
#include "openmm/opencl/OpenCLForceInfo.h"

using namespace AGBNP3Plugin;
using namespace OpenMM;
using namespace std;

class OpenCLAGBNP3ForceInfo : public OpenCLForceInfo {
public:
    OpenCLAGBNP3ForceInfo(const AGBNP3Force& force) : OpenCLForceInfo(0), force(force) {
    }
    int getNumParticleGroups() {
      return force.getNumParticles();//each particle is in a different group?
    }
    void getParticlesInGroup(int index, vector<int>& particles) {
      particles.push_back(index);
    }
    bool areGroupsIdentical(int group1, int group2) {
        return (group1 == group2);
    }
private:
    const AGBNP3Force& force;
};

OpenCLCalcAGBNP3ForceKernel::~OpenCLCalcAGBNP3ForceKernel() {
    if (params != NULL)
        delete params;
}

void OpenCLCalcAGBNP3ForceKernel::initialize(const System& system, const AGBNP3Force& force) {
    OpenCLNonbondedUtilities& nb = cl.getNonbondedUtilities();
    int numContexts = cl.getPlatformData().contexts.size();
    int startIndex = cl.getContextIndex()*force.getNumParticles()/numContexts;
    int endIndex = (cl.getContextIndex()+1)*force.getNumParticles()/numContexts;
    numParticles = endIndex-startIndex;
    if (numParticles == 0)
        return;
    // charges
    OpenMM::OpenCLArray* chargeParam = OpenCLArray::create<cl_float>(cl, numParticles, "chargeParam");
    OpenMM::OpenCLArray* bornrParam = OpenCLArray::create<cl_float>(cl, numParticles, "bornrParam");
    vector<cl_float> chargeVector(numParticles);
    vector<cl_float> bornrVector(numParticles);
    for (int i = 0; i < numParticles; i++) {
      double radius, charge, gamma, alpha, hbcorr;
      int ishydrogen, hbtype;
      vector<int> neighbors;
      force.getParticleParameters(startIndex+i, radius, charge, gamma, alpha, hbtype,
				  hbcorr, ishydrogen, neighbors);
      chargeVector[i] = (cl_float) charge;
      bornrVector[i] = (cl_float)radius;
    }
    chargeParam->upload(chargeVector);
    bornrParam->upload(bornrVector);
    nb.addParameter(OpenCLNonbondedUtilities::ParameterInfo("chargeParam", "float", 1, sizeof(cl_float),chargeParam->getDeviceBuffer()));
    nb.addParameter(OpenCLNonbondedUtilities::ParameterInfo("bornrParam", "float", 1, sizeof(cl_float), bornrParam->getDeviceBuffer()));

    bool usesCutoff = false;
    bool usesPeriodic = false;
    bool usesExclusions = false;
    double cutoffDistance = 1.0; //default? Error if set to a different value
    vector< vector<int> > exclusionList;
    map<string, string> replacements;
    nb.addInteraction(usesCutoff, usesPeriodic, usesExclusions, cutoffDistance, exclusionList, cl.replaceStrings(OpenCLAGBNP3KernelSources::AGBNP3Force, replacements), force.getForceGroup());
    cl.addForce(new OpenCLAGBNP3ForceInfo(force));
}

double OpenCLCalcAGBNP3ForceKernel::execute(ContextImpl& context, bool includeForces, bool includeEnergy) {
    return 0.0;
}

void OpenCLCalcAGBNP3ForceKernel::copyParametersToContext(ContextImpl& context, const AGBNP3Force& force) {
    int numContexts = cl.getPlatformData().contexts.size();
    int startIndex = cl.getContextIndex()*force.getNumParticles()/numContexts;
    int endIndex = (cl.getContextIndex()+1)*force.getNumParticles()/numContexts;
    if (numParticles != endIndex-startIndex)
        throw OpenMMException("updateParametersInContext: The number of bonds has changed");
    if (numParticles == 0)
        return;
    
    // Record the per-bond parameters.
    
    //vector<mm_float2> paramVector(numBonds);
    //for (int i = 0; i < numBonds; i++) {
    //    int atom1, atom2;
    //   double length, k;
    //   force.getBondParameters(startIndex+i, atom1, atom2, length, k);
    //    paramVector[i] = mm_float2((cl_float) length, (cl_float) k);
    // }
    //params->upload(paramVector);
    
    // Mark that the current reordering may be invalid.
    
    cl.invalidateMolecules();
}

