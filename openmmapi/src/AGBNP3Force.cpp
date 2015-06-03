/* -------------------------------------------------------------------------- *
 *                             OpenMM-AGBNP3                                  *
 * -------------------------------------------------------------------------- */

#include "AGBNP3Force.h"
#include "internal/AGBNP3ForceImpl.h"
#include "openmm/OpenMMException.h"
#include "openmm/internal/AssertionUtilities.h"

using namespace AGBNP3Plugin;
using namespace OpenMM;
using namespace std;

AGBNP3Force::AGBNP3Force() {
}

int AGBNP3Force::addParticle(double radius, double charge, double gamma, double alpha,  int hbtype, double hbcorr, int ishydrogen){
  ParticleInfo particle(radius, charge, gamma, alpha, hbtype, hbcorr, ishydrogen);
  particles.push_back(particle);
  return particles.size()-1;
}

void AGBNP3Force::getParticleParameters(int index,  double& radius, double& charge, double& gamma, double& alpha, 
					int& hbtype, double& hbcorr, int& ishydrogen, 
					std::vector<int>& neighbors ) const {
    ASSERT_VALID_INDEX(index, particles);
    radius = particles[index].radius;
    charge = particles[index].charge;
    gamma = particles[index].gamma;
    alpha = particles[index].alpha;
    hbtype = particles[index].hbtype;
    hbcorr = particles[index].hbcorr;
    ishydrogen = particles[index].ishydrogen;
    neighbors = particles[index].neighbors;
}

/* insert p1,p2 pair into connection table */
int AGBNP3Force::addParticleConnection(int p1, int p2) {
  ASSERT_VALID_INDEX(p1, particles);
  ASSERT_VALID_INDEX(p2, particles);
  particles[p1].neighbors.push_back(p2);
  particles[p2].neighbors.push_back(p1);
  return particles[p1].neighbors.size();
}

ForceImpl* AGBNP3Force::createImpl() const {
    return new AGBNP3ForceImpl(*this);
}

void AGBNP3Force::updateParametersInContext(Context& context) {
    dynamic_cast<AGBNP3ForceImpl&>(getImplInContext(context)).updateParametersInContext(getContextImpl(context));
}
