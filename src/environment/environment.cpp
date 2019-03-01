#include "environment.h"

using namespace LGen;

Environment::Environment(const std::shared_ptr<Terrain> terrain, const size_t maxIterations) :
	terrain(std::move(terrain)),
	maxIterations(maxIterations) {

}

void Environment::addAgent(const Agent &agent) {
	agents.push_back(agent);
}

void Environment::clearAgents() {
	agents.clear();
}

std::shared_ptr<LRender::Scene> Environment::makeScene(std::mt19937 &randomizer) const {
	const std::shared_ptr<Terrain> &terrainSource = terrain;

	LRender::Terrain renderTerrain(
		terrain->getWidth(),
		terrain->getHeight(),
		terrainSource,
		[terrainSource](const float x, const float y) {
			return terrainSource->get(x, y);
		});

	std::shared_ptr<LRender::Scene> scene(new LRender::Scene(renderTerrain));

	for(const auto &agent : agents)
		scene->addAgent(LRender::Agent(
			LRender::Vector(10, terrain->get(10, 10), 10),
			agent.generate(maxIterations, randomizer)->getString()));

	return scene;
}