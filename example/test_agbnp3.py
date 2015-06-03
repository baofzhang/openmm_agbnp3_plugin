from simtk.openmm.app import *
from simtk.openmm import *
from simtk.unit import *
from sys import stdout
import os, time, shutil
from simtk.openmm.app.desmonddmsfile import *
from datetime import datetime


print("Started at: " + str(time.asctime()))
start=datetime.now()
shutil.copyfile('trpcage.dms','trpcage2.dms')
testDes = DesmondDMSFile('trpcage2.dms')
system = testDes.createSystem(nonbondedMethod=NoCutoff, OPLS = True, implicitSolvent='AGBNP')
integrator = LangevinIntegrator(300*kelvin, 1/picosecond, 0.002*picoseconds)
platform = Platform.getPlatformByName('Reference')
simulation = Simulation(testDes.topology, system, integrator,platform)
simulation.context.setPositions(testDes.positions)
simulation.context.setVelocities(testDes.velocities)
print "Using platform %s" % simulation.context.getPlatform().getName()
state = simulation.context.getState(getEnergy = True)
print(state.getPotentialEnergy())

#simulation.minimizeEnergy()
#simulation.reporters.append(StateDataReporter(stdout, 1000, step=True, potentialEnergy=True,totalEnergy=True,temperature=True))
#simulation.reporters.append(DCDReporter('5dfr.dcd', 1000))
#simulation.step(40000)
#positions = simulation.context.getState(getPositions=True).getPositions()
#velocities = simulation.context.getState(getVelocities=True).getVelocities()
##print "Updating positions and velocities"
#testDes.setPositions(positions)
#testDes.setVelocities(velocities)
#testDes.close()#
#
#end=datetime.now()
#elapsed=end - start
#print("elapsed time="+str(elapsed.seconds+elapsed.microseconds*1e-6)+"s")
