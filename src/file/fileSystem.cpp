#include "file/fileSystem.h"
#include "utils/stringUtils.h"

#include <sstream>
#include <vector>

using namespace LGen::File;

const std::string System::KEY_AXIOM = "axiom";
const std::string System::KEY_RULE_COUNT = "rule-count";
const std::string System::KEY_RULE_PREFIX = "rule-";

System::System(const std::string &file) :
	Config(file) {

}

System::System(const LParse::System &system) {
	std::stringstream axiom;
	std::stringstream ruleCount;

	axiom << system.getAxiom();
	ruleCount << system.getRules().size();

	set(KEY_AXIOM, axiom.str());
	set(KEY_RULE_COUNT, ruleCount.str());

	for(size_t i = 0; i < system.getRules().size(); ++i) {
		std::stringstream rule;

		rule << system.getRules()[i];

		set(KEY_RULE_PREFIX + std::to_string(i), rule.str());
	}
}

LParse::System System::getSystem() const {
	LParse::System system;
	std::vector<LParse::Rule> rules;
	const size_t ruleCount = std::stoi(get(KEY_RULE_COUNT));

	system.setAxiom(get(KEY_AXIOM));

	for(size_t i = 0; i < ruleCount; ++i) {
		std::string lhs, rhs;

		Utils::String::split(
			get(KEY_RULE_PREFIX + std::to_string(i)),
			LParse::Rule::CONNECTIVE,
			lhs,
			rhs);

		rules.push_back(LParse::Rule(
			Utils::String::trim(lhs),
			Utils::String::trim(rhs)));
	}

	system.setRules(rules);

	return system;
}