// Test.hpp - Header for the "test" command.
// https://github.com/RealTimeChris

#pragma once

#ifndef _TEST_
#define _TEST_

#include "../DiscordCoreClient02.hpp"

namespace DiscordCoreAPI {

	class Test : public BaseFunction {
	public:
		Test() {
			this->commandName = "test";
			this->helpDescription = "__**Test:**__ Enter !test or /test to run this command!";
		}

		Test* create() {
			return new Test;
		}

		virtual  task<void> execute(shared_ptr<BaseFunctionArguments> args) {

			ModifyGuildMemberData dataPackage01;
			dataPackage01.currentChannelId = "";
			dataPackage01.deaf = false;
			dataPackage01.mute = false;
			dataPackage01.newVoiceChannelId = "";
			dataPackage01.nick = "TEST NICK";
			dataPackage01.roleIds = vector<string>{ "" };
			dataPackage01.guildId = args->eventData.getGuildId();
			dataPackage01.guildMemberId = args->eventData.getAuthorId();

			GuildMember guildMember01 = args->eventData.discordCoreClient->guildMembers->modifyGuildMemberAsync(dataPackage01).get();

			co_return;
		}
	};
}
#endif