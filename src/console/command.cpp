#include "console/command.h"

using namespace LGen;

const std::string Command::MSG_HELP_LIST = "Available commands:";
const std::string Command::MSG_NEED_MORE_ARGUMENTS = "This command needs more arguments.";
const std::string Command::MSG_THIS_COMMAND_TAKES = "This command needs ";
const std::string Command::MSG_ARGUMENT = " argument.";
const std::string Command::MSG_ARGUMENTS = " arguments.";
const std::string Command::MSG_INVALID_INPUT = "The provided input is invalid.";

Command::Command(const std::vector<std::string> triggers, const char args) :
	triggers(triggers),
	hasHelp(false),
	args(args) {

}

Command::Command(const std::vector<std::string> triggers, const std::string help, const char args) :
	triggers(triggers),
	help(help),
	hasHelp(true),
	args(args) {

}

Command::Command(const std::vector<std::string> triggers, const std::vector<Command*> commands, const char args) :
	triggers(triggers),
	hasHelp(true),
	commandList(new CommandList(commands)),
	args(args) {

}

bool Command::apply(const Input &input, Console &console, Workspace &workspace) {
	for(const std::string &trigger : triggers) {
		if(input.getKeyword().rfind(trigger, 0) == 0) {
			if(input.getKeyword().size() == trigger.size()) {
				if(commandList && input.getArguments().size())
					return commandList->apply(input.getArguments(), console, workspace);
				else {
					if(args == -1 || args == input.getArguments().size())
						application(input.getArguments(), console, workspace);
					else {
						if(args == 1)
							console << MSG_THIS_COMMAND_TAKES << std::to_string(args) << MSG_ARGUMENT << std::endl;
						else
							console << MSG_THIS_COMMAND_TAKES << std::to_string(args) << MSG_ARGUMENTS << std::endl;
					}
				}

				return true;
			}
			
			else if(input.getKeyword()[trigger.size()] == '?') {
				if(commandList) {
					console << MSG_HELP_LIST << std::endl;
					
					commandList->enumerateKeywords(console);
				}
				else if(hasHelp)
					showHelp(console);

				return true;
			}
		}
	}

	return false;
}

const std::string &Command::getTrigger() const {
	return triggers[0];
}

std::vector<std::string> Command::getAliases() const {
	std::vector<std::string> aliases;

	for(auto trigger = triggers.begin() + 1; trigger < triggers.end(); ++trigger)
		aliases.push_back(*trigger);

	return aliases;
}

void Command::application(
	const std::vector<std::string> arguments,
	Console &console,
	Workspace &workspace) {
	console << MSG_NEED_MORE_ARGUMENTS << std::endl;
}

void Command::showHelp(const Console &console) const {
	console.dumpFile(help);
}