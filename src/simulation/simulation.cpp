#include "simulation.h"
#include "simulation/candidate.h"
#include "simulation/planter.h"

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
	std::unique_ptr<State> state,
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
	auto randomizer = getState().getRandomizer();
	const auto task = getState().getTaskSceneReport(&randomizer);

	console.getMonitor()->enqueue(task);

	const auto report = task->getReport();
	const auto planter = Planter(getState().getEnvironment().getAgents(), report->getAgents(), randomizer);
	auto environment = getState().getEnvironment().makeEmptyCopy();

	planter.plant(*environment, *mutator, randomizer);

	state = std::make_unique<State>(std::move(environment), randomizer);
	++generation;
}

void Simulation::revert() {
	state = nullptr;
	generation = 0;
}

void Simulation::reseed() const {
	state->setRandomizer(LParse::Randomizer());
}