#pragma once

#include "console/command.h"

namespace LGen {
	class Command::System final : public Command {
	public:
		System();

		class New;

	private:
		static const std::string KEYWORD;
		static const std::string ALIAS;
	};
};