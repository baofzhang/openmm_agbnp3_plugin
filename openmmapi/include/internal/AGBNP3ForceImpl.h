#ifndef OPENMM_AGBNP3FORCEIMPL_H_
#define OPENMM_AGBNP3FORCEIMPL_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM-AGBNP3                            *
 * -------------------------------------------------------------------------- */

#include "AGBNP3Force.h"
#include "openmm/internal/ForceImpl.h"
#include "openmm/Kernel.h"
#include <utility>
#include <set>
#include <string>

namespace AGBNP3Plugin {

class System;

/**
 * This is the internal implementation of AGBNP3Force.
 */

class OPENMM_EXPORT_AGBNP3 AGBNP3ForceImpl : public OpenMM::ForceImpl {
public:
    AGBNP3ForceImpl(const AGBNP3Force& owner);
    ~AGBNP3ForceImpl();
    void initialize(OpenMM::ContextImpl& context);
    const AGBNP3Force& getOwner() const {
        return owner;
    }
    void updateContextState(OpenMM::ContextImpl& context) {
        // This force field doesn't update the state directly.
    }
    double calcForcesAndEnergy(OpenMM::ContextImpl& context,  bool includeForces, bool includeEnergy, int groups);
    std::map<std::string, double> getDefaultParameters() {
        return std::map<std::string, double>(); // This force field doesn't define any parameters.
    }
    std::vector<std::string> getKernelNames();
    void updateParametersInContext(OpenMM::ContextImpl& context);
private:
    const AGBNP3Force& owner;
    OpenMM::Kernel kernel;
};

} // namespace AGBNP3Plugin

#endif /*OPENMM_AGBNP3FORCEIMPL_H_*/
