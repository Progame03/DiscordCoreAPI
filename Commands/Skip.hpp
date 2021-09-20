// Skip.hpp - Header for the "skip" command.
// Aug 18, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#ifndef _SKIP_
#define _SKIP_

#include "../DiscordCoreClient02.hpp"

namespace DiscordCoreAPI {
	class Skip : public BaseFunction{
	public:
		Skip() {
			this->commandName = "skip";
			this->helpDescription = "Skips to the next song in the queue.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nSimply enter !skip or /skip.\n------");
			msgEmbed.setTitle("__**Skip Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		Skip* create() {
			return new Skip;
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

			bool areWeAllowed = checkIfAllowedPlayingInChannel(args->eventData, discordGuild);

			if (!areWeAllowed) {
				co_return;
			}

			GuildMember guildMember = GuildMembers::getGuildMemberAsync({ .guildMemberId = args->eventData.getAuthorId(),.guildId = args->eventData.getGuildId() }).get();

			bool doWeHaveControl = checkIfWeHaveControl(args->eventData, discordGuild, guildMember);

			if (!doWeHaveControl) {
				co_return;
			}

			InputEventData newEvent = args->eventData;
			if (args->eventData.eventType == InputEventType::SLASH_COMMAND_INTERACTION) {
				CreateDeferredInteractionResponseData dataPackage(args->eventData);
				newEvent = InputEvents::respondToEvent(dataPackage);
			}

			shared_ptr<VoiceConnection>* voiceConnection = guild.connectToVoice(guildMember.voiceData.channelId);

			if (guildMember.voiceData.channelId == "" || guildMember.voiceData.channelId != (*voiceConnection)->getChannelId()) {
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("------\n__**Sorry, but you need to be in a correct voice channel to issue those commands!**__\n------");
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Skipping Issue:**__");
				newEmbed.setColor(discordGuild.data.borderColor);
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					ReplyMessageData dataPackage(newEvent);
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					CreateFollowUpMessageData dataPackage(newEvent);
					dataPackage.addContent("Empty Content");
					newEvent = InputEvents::respondToEvent(dataPackage);
					CreateEphemeralFollowUpMessageData dataPackage02(newEvent);
					dataPackage02.addMessageEmbed(newEmbed);
					InputEvents::respondToEvent(dataPackage02);
					InputEvents::deleteInputEventResponseAsync(newEvent);
				}
				
				co_return;
			}

			if (!(*voiceConnection)->areWeConnected() || !(*voiceConnection)->areWeCurrentlyPlaying()) {
				string msgString = "------\n**There's no music playing to be skipped!**\n------";
				EmbedData msgEmbed;
				msgEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				msgEmbed.setColor(discordGuild.data.borderColor);
				msgEmbed.setDescription(msgString);
				msgEmbed.setTimeStamp(getTimeAndDate());
				msgEmbed.setTitle("__**Skipping Issue:**__");
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					ReplyMessageData dataPackage(newEvent);
					dataPackage.addMessageEmbed(msgEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000);
				}
				else {
					CreateFollowUpMessageData dataPackage(newEvent);
					dataPackage.addContent("Empty Content");
					newEvent = InputEvents::respondToEvent(dataPackage);
					CreateEphemeralFollowUpMessageData dataPackage02(newEvent);
					dataPackage02.addMessageEmbed(msgEmbed);
					InputEvents::respondToEvent(dataPackage02);
					InputEvents::deleteInputEventResponseAsync(newEvent);
				}
				
				co_return;
			}

			if (!SongAPI::isThereAnySongs(guild.id)) {
				string msgString = "------\n**There's no more songs for us to skip to!**\n------";
				EmbedData msgEmbed02;
				msgEmbed02.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				msgEmbed02.setColor(discordGuild.data.borderColor);
				msgEmbed02.setTimeStamp(getTimeAndDate());
				msgEmbed02.setDescription(msgString);
				msgEmbed02.setTitle("__**Song Queue Issue:**__");
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					ReplyMessageData dataPackage(newEvent);
					dataPackage.addMessageEmbed(msgEmbed02);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					CreateFollowUpMessageData dataPackage(newEvent);
					dataPackage.addContent("Empty Content");
					newEvent = InputEvents::respondToEvent(dataPackage);
					CreateEphemeralFollowUpMessageData dataPackage02(newEvent);
					dataPackage02.addMessageEmbed(msgEmbed02);
					InputEvents::respondToEvent(dataPackage02);
					InputEvents::deleteInputEventResponseAsync(newEvent).get();
				}
				
				co_return;
			}
			else {
				
				if ((*voiceConnection)->areWeCurrentlyPlaying() &&SongAPI::isThereAnySongs(guild.id)) {
					SongAPI::skip(guild.id);
					(*voiceConnection)->skip();
					string msgString = "------\n**We're skipping to the next song!**\n------";
					EmbedData msgEmbed02;
					msgEmbed02.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
					msgEmbed02.setColor(discordGuild.data.borderColor);
					msgEmbed02.setTimeStamp(getTimeAndDate());
					msgEmbed02.setDescription(msgString);
					msgEmbed02.setTitle("__**Song Skip:**__");
					if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
						ReplyMessageData dataPackage(newEvent);
						dataPackage.addMessageEmbed(msgEmbed02);
						newEvent = InputEvents::respondToEvent(dataPackage);
						InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
					}
					else {
						CreateFollowUpMessageData dataPackage(newEvent);
						dataPackage.addContent("Empty Content");
						newEvent = InputEvents::respondToEvent(dataPackage);
						CreateEphemeralFollowUpMessageData dataPackage02(newEvent);
						dataPackage02.addMessageEmbed(msgEmbed02);
						auto newEvent02 = InputEvents::respondToEvent(dataPackage02);
						InputEvents::deleteInputEventResponseAsync(newEvent);
						InputEvents::deleteInputEventResponseAsync(newEvent02, 20000);
					}
					GuildMember guildMember02 = GuildMembers::getGuildMemberAsync({ .guildMemberId = SongAPI::getCurrentSong(guild.id).addedById,.guildId = args->eventData.getGuildId() }).get();
					(*voiceConnection)->play();
					EmbedData newEmbed;
					newEmbed.setAuthor(guildMember02.user.userName, guildMember02.user.avatar);
					newEmbed.setDescription("__**Title:**__ [" + SongAPI::getCurrentSong(guild.id).title + "](" + SongAPI::getCurrentSong(guild.id).url + ")" + "\n__**Description:**__ " + SongAPI::getCurrentSong(guild.id).description + "\n__**Duration:**__ " +
						SongAPI::getCurrentSong(guild.id).duration + "\n__**Added By:**__ <@!" + SongAPI::getCurrentSong(guild.id).addedById + "> (" + SongAPI::getCurrentSong(guild.id).addedByUserName + ")");
					newEmbed.setImage(SongAPI::getCurrentSong(guild.id).imageURL);
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Now Playing:**__");
					newEmbed.setColor(discordGuild.data.borderColor);
					if (SongAPI::isLoopAllEnabled(guild.id) && SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
					}
					if (!SongAPI::isLoopAllEnabled(guild.id) && SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
					}
					if (SongAPI::isLoopAllEnabled(guild.id) && !SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
					}
					if (!SongAPI::isLoopAllEnabled(guild.id) && !SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
					}
					if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
						ReplyMessageData dataPackage(newEvent);
						dataPackage.addMessageEmbed(newEmbed);
						auto newEvent02 = InputEvents::respondToEvent(dataPackage);
					}
					else {
						CreateFollowUpMessageData dataPackage(newEvent);
						dataPackage.addMessageEmbed(newEmbed);
						auto newEvent02 = InputEvents::respondToEvent(dataPackage);
					}
					
				}
				else if (!SongAPI::isThereAnySongs(guild.id)) {
					discordGuild.getDataFromDB();
					EmbedData newEmbed;
					newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
					newEmbed.setDescription("------\n__**Sorry, but there's nothing left to play here!**__\n------");
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Now Playing:**__");
					newEmbed.setColor(discordGuild.data.borderColor);
					if (SongAPI::isLoopAllEnabled(guild.id) && SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
					}
					else if (!SongAPI::isLoopAllEnabled(guild.id) && SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
					}
					else if (SongAPI::isLoopAllEnabled(guild.id) && !SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
					}
					else if (!SongAPI::isLoopAllEnabled(guild.id) && !SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
					}
					if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
						ReplyMessageData dataPackage(newEvent);
						dataPackage.addMessageEmbed(newEmbed);
						auto newEvent02 = InputEvents::respondToEvent(dataPackage);
					}
					else {
						CreateEphemeralFollowUpMessageData dataPackage(newEvent);
						dataPackage.addMessageEmbed(newEmbed);
						auto newEvent02 = InputEvents::respondToEvent(dataPackage);
					}
					
					co_return;
				}
				else {
					string msgString = "------\n**There's no music playing to be skipped!**\n------";
					EmbedData msgEmbed02;
					msgEmbed02.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
					msgEmbed02.setColor(discordGuild.data.borderColor);
					msgEmbed02.setTimeStamp(getTimeAndDate());
					msgEmbed02.setDescription(msgString);
					msgEmbed02.setTitle("__**Skipping Issue:**__");
					if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
						ReplyMessageData dataPackage(newEvent);
						dataPackage.addMessageEmbed(msgEmbed02);
						newEvent = InputEvents::respondToEvent(dataPackage);
						InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
					}
					else {
						CreateFollowUpMessageData dataPackage(newEvent);
						dataPackage.addContent("Empty Content");
						newEvent = InputEvents::respondToEvent(dataPackage);
						CreateEphemeralFollowUpMessageData dataPackage02(newEvent);
						dataPackage02.addMessageEmbed(msgEmbed02);
						InputEvents::respondToEvent(dataPackage02);
						InputEvents::deleteInputEventResponseAsync(newEvent).get();
					}
					
					co_return;
				}
			}
			
			co_return;
		}

	};
}

#endif