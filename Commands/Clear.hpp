// Clear.hpp - Header for the "clear" command.
// Aug 19, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#ifndef _CLEAR_
#define _CLEAR_

#include "../DiscordCoreClient02.hpp"

namespace DiscordCoreAPI {

	class Clear :public BaseFunction {
	public:
		Clear() {
			this->commandName = "clear";
			this->helpDescription = "Clears the current song queue.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nSimply enter !clear or /clear! And that's it!\n------");
			msgEmbed.setTitle("__**Clear Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		Clear* create() {
			return new Clear;
		}

		virtual task<void> execute(shared_ptr<BaseFunctionArguments> args) {
			Channel channel = Channels::getChannelAsync({ .channelId = args->eventData.getChannelId() }).get();

			bool areWeInADm = areWeInADM(args->eventData, channel);

			if (areWeInADm) {
				co_return;
			}

			InputEvents::deleteInputEventResponseAsync(args->eventData).get();

			Guild guild = Guilds::getGuildAsync({ .guildId = args->eventData.getGuildId() }).get();

			DiscordGuild discordGuild(guild);

			bool checkIfAllowedInChannel = checkIfAllowedPlayingInChannel(args->eventData, discordGuild);

			if (!checkIfAllowedInChannel) {
				co_return;
			}

			GuildMember guildMember = GuildMembers::getGuildMemberAsync({ .guildMemberId = args->eventData.getAuthorId(),.guildId = args->eventData.getGuildId() }).get();

			bool doWeHaveControl = checkIfWeHaveControl(args->eventData, discordGuild, guildMember);

			if (!doWeHaveControl) {
				co_return;
			}

			shared_ptr<VoiceConnection>* voiceConnectionRaw = guild.connectToVoice(guildMember.voiceData.channelId);

			if (voiceConnectionRaw == nullptr) {
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("------\n__**Sorry, but there is no voice connection that is currently held by me!**__\n------");
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Connection Issue:**__");
				newEmbed.setColor(discordGuild.data.borderColor);
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::RegularMessage;
					auto newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
					dataPackage.addMessageEmbed(newEmbed);
					auto newEvent = InputEvents::respondToEvent(dataPackage);
				}
				co_return;
			}

			auto voiceConnection = *voiceConnectionRaw;

			if (guildMember.voiceData.channelId == "" || guildMember.voiceData.channelId != voiceConnection->getChannelId()) {
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("------\n__**Sorry, but you need to be in a correct voice channel to issue those commands!**__\n------");
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Clear Issue:**__");
				newEmbed.setColor(discordGuild.data.borderColor);
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::RegularMessage;
					dataPackage.addMessageEmbed(newEmbed);
					auto newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
					dataPackage.addMessageEmbed(newEmbed);
					auto newEvent = InputEvents::respondToEvent(dataPackage);
				}
				co_return;
			}

			auto playlist = SongAPI::getPlaylist(guild.id);
			playlist.songQueue.clear();
			SongAPI::setPlaylist(playlist, guild.id);

			EmbedData msgEmbed;
			msgEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
			msgEmbed.setColor(discordGuild.data.borderColor);
			msgEmbed.setDescription("\n------\n__**You have cleared the song queue!**__\n------");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setTitle("__**Queue Cleared:**__");
			if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
				RespondToInputEventData dataPackage(args->eventData);
				dataPackage.type = DesiredInputEventResponseType::RegularMessage;
				dataPackage.addMessageEmbed(msgEmbed);
				auto newEvent = InputEvents::respondToEvent(dataPackage);
				InputEvents::deleteInputEventResponseAsync(newEvent, 20000);
			}
			else {
				RespondToInputEventData dataPackage(args->eventData);
				dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
				dataPackage.addMessageEmbed(msgEmbed);
				auto newEvent = InputEvents::respondToEvent(dataPackage);
			}

			co_return;
		}
	};

}

#endif
