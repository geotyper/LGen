#include "simulation.h"
#include "simulation/candidate.h"
#include "simulation/densityMap.h"

using namespace LGen;

Simulation::Simulation(
	std::unique_ptr<Mutator> mutator,
	std::unique_ptr<const State> initial) :
	mutator(std::move(mutator)),
	initial(std::move(initial)),
	generation(0) {
	
}

Simulation::Simulation(
	std::unique_ptr<Mutator> mutator,
	std::unique_ptr<const State> initial,
	std::unique_ptr<const State> state,
	const size_t generation) :
	mutator(std::move(mutator)),
	initial(std::move(initial)),
	state(std::move(state)),
	generation(generation) {

}

Mutator &Simulation::getMutator() const {
	return *mutator;
}

const State &Simulation::getInitial() const {
	return *initial;
}

const State &Simulation::getState() const {
	if(state)
		return *state;

	return *initial;
}

size_t Simulation::getGeneration() const {
	return generation;
}

void Simulation::advance(Console &console) {
	LParse::Randomizer randomizer = getState().getRandomizer();
	const auto task = getState().getTaskSceneReport(&randomizer);

	console.getMonitor()->enqueue(task);

	const auto report = task->getReport();
	auto environment = getState().getEnvironment().makeEmptyCopy();
	std::vector<Candidate> candidates;

	for(size_t i = 0; i < getState().getEnvironment().getAgents().size(); ++i) {
		const auto &agent = getState().getEnvironment().getAgents()[i];
		const auto &reportAgent = report->getAgents()[i];

		for(const auto &reportSeed : reportAgent.getSeeds())
			if(
				reportSeed.getLocation().x > 0 &&
				reportSeed.getLocation().z > 0 &&
				reportSeed.getLocation().x < getState().getEnvironment().getTerrain().getWidth() &&
				reportSeed.getLocation().z < getState().getEnvironment().getTerrain().getHeight())
				candidates.emplace_back(Candidate(
					reportSeed.getLocation().x,
					reportSeed.getLocation().z,
					agent.getSystem(),
					reportAgent.getLimits(),
					0));
	}

	DensityMap densityMap(
		getState().getEnvironment().getTerrain().getWidth(),
		getState().getEnvironment().getTerrain().getHeight());

	for(const auto &candidate : candidates) {
		if(densityMap.sample(candidate.getX(), candidate.getY()) > 1.5f)
			continue;

		environment->addAgent(Agent(
			mutator->mutate(candidate.getSystem()),
			candidate.getX(),
			candidate.getY()));

		densityMap.add(candidate);
	}

	state = std::make_unique<State>(std::move(environment), randomizer);
	++generation;
}

void Simulation::revert() {
	state = nullptr;
	generation = 0;
}