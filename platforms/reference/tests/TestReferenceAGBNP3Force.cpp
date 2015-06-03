/* -------------------------------------------------------------------------- *
 *                              OpenMM-AGBNP3                                 *
 * -------------------------------------------------------------------------- */

/**
 * This tests the Reference implementation of AGBNP3Force.
 */

#include "AGBNP3Force.h"
#include "openmm/internal/AssertionUtilities.h"
#include "openmm/Context.h"
#include "openmm/Platform.h"
#include "openmm/System.h"
#include "openmm/VerletIntegrator.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace AGBNP3Plugin;
using namespace OpenMM;
using namespace std;

extern "C" OPENMM_EXPORT void registerAGBNP3ReferenceKernelFactories();

void testForce() {
    // Create a chain of particles connected by bonds.
    const int numBonds = 10;
    const int numParticles = numBonds+1;
    System system;
    vector<Vec3> positions(numParticles);
    for (int i = 0; i < numParticles; i++) {
        system.addParticle(1.0);
        positions[i] = Vec3(0.1*i, 0.1*i, -0.1*i);
    }
    AGBNP3Force* force = new AGBNP3Force();
    system.addForce(force);
    double radius = 0.15;
    double charge = 1.0;
    double gamma = 0.1;
    double alpha = -1.0;
    int hbtype = 0;
    double hbcorr = 0.0;
    int ishydrogen = 0;
    for (int i = 0; i < numParticles; i++){
      force->addParticle(radius, charge, gamma, alpha, hbtype, hbcorr, ishydrogen);
      charge *= -1.0;
    }
    for (int i = 0; i < numBonds; i++)
        force->addParticleConnection(i, i+1);
    
    // Compute the forces and energy.

    VerletIntegrator integ(1.0);
    Platform& platform = Platform::getPlatformByName("Reference");
    Context context(system, integ, platform);
    context.setPositions(positions);
    State state = context.getState(State::Energy | State::Forces);
    
    std::cout << "Energy:" <<  state.getPotentialEnergy()  << std::endl;

}

int main() {
    try {
        registerAGBNP3ReferenceKernelFactories();
        testForce();
	//        testChangingParameters();
    }
    catch(const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Done" << std::endl;
    return 0;
}
