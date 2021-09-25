// PlayRN.hpp - Header for the "play right now" command.
// Sep 9, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#ifndef _PLAY_RN_
#define _PLAY_RN_

#include "../DiscordCoreClient02.hpp"

namespace DiscordCoreAPI {

	class PlayRN : public BaseFunction {
	public:

		PlayRN() {
			this->commandName = "playrn";
			this->helpDescription = "Add a song to the front of the queue and play it if nothing is playing.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nSimply enter !playrn = SONGNAME OR URL, or /playrn SONGNAME OR URL.\n------");
			msgEmbed.setTitle("__**PlayRN Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		PlayRN* create() {
			return new PlayRN;
		}

		virtual task<void> execute(shared_ptr<BaseFunctionArguments> args) {
			Channel channel = Channels::getChannelAsync({ args->eventData.getChannelId() }).get();

			bool areWeInADm = areWeInADM(args->eventData, channel);

			if (areWeInADm) {
				co_return;
			}

			InputEvents::deleteInputEventResponseAsync(args->eventData).get();

			Guild guild = Guilds::getGuildAsync({ args->eventData.getGuildId() }).get();
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
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
				}
				co_return;
			}

			auto voiceConnection = *voiceConnectionRaw;

			if (guildMember.voiceData.channelId == "" || guildMember.voiceData.channelId != voiceConnection->getChannelId()) {
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("------\n__**Sorry, but you need to be in a correct voice channel to issue those commands!**__\n------");
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Playing Issue:**__");
				newEmbed.setColor(discordGuild.data.borderColor);
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::RegularMessage;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
				}
				co_return;
			}

			vector<Song> searchResults;
			if (args->argumentsArray.size() > 0) {
				if (args->eventData.eventType == InputEventType::SLASH_COMMAND_INTERACTION) {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::DeferredResponse;
					newEvent = InputEvents::respondToEvent(dataPackage);
				}

				searchResults = SongAPI::searchForSong(args->argumentsArray[0], guild.id);
			}
			else if (SongAPI::isThereAnySongs(guild.id) && !voiceConnection->areWeCurrentlyPlaying()) {
				if (args->eventData.eventType == InputEventType::SLASH_COMMAND_INTERACTION) {
					RespondToInputEventData dataPackage(args->eventData);
					dataPackage.type = DesiredInputEventResponseType::DeferredResponse;
					newEvent = InputEvents::respondToEvent(dataPackage);
				}
			}
			else if (!SongAPI::isThereAnySongs(guild.id)) {
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("------\n__**Sorry, but there's nothing to play!**__\n------");
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Playing Issue:**__");
				newEmbed.setColor(discordGuild.data.borderColor);
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.type = DesiredInputEventResponseType::RegularMessage;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
				}
				co_return;
			}
			else if (voiceConnection->areWeCurrentlyPlaying()) {
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("------\n__**Sorry, but there's already something playing!**__\n------");
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Playing Issue:**__");
				newEmbed.setColor(discordGuild.data.borderColor);
				if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.type = DesiredInputEventResponseType::RegularMessage;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
					InputEvents::deleteInputEventResponseAsync(newEvent, 20000).get();
				}
				else {
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.type = DesiredInputEventResponseType::EphemeralInteractionResponse;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
			
				}
				co_return;
			}

			vector<EmbedData> embedsFromSearch;
			unsigned int x = 0;
			for (auto value : searchResults) {
				x += 1;
				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("__**Title:**__ [" + value.songTitle + "](" + value.viewURL + ")" + "\n__**Description:**__ " + value.description + "\n__**Duration:**__ " + value.duration);
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**" + to_string(x) + " of " + to_string(searchResults.size()) + "**__");
				newEmbed.setImage(value.thumbnailURL);
				newEmbed.setColor(discordGuild.data.borderColor);
				embedsFromSearch.push_back(newEmbed);
			}

			if (embedsFromSearch.size() > 0) {
				Song song{};
				song = SongAPI::addSongToQueue(guildMember, guild.id, searchResults[0]);
				SongAPI::stop(guild.id);
				auto playlist = SongAPI::getPlaylist(guild.id);

				vector<Song> songVector{};
				playlist.songQueue.erase(playlist.songQueue.end() - 1);
				songVector.push_back(song);
				for (auto value : playlist.songQueue) {
					songVector.push_back(value);
				}
				playlist.songQueue = songVector;
				playlist.currentSong = Song();
				SongAPI::setPlaylist(playlist, guild.id);
			}

			if (!voiceConnection->areWeCurrentlyPlaying()) {
				voiceConnection->onSongCompletion([=](SongCompletionEventData eventData) mutable noexcept ->task<void> {
					co_await resume_background();
					if (SongAPI::isThereAnySongs(guild.id)) {

						EmbedData newEmbed;
						if (!eventData.isThisAReplay) {
							if (!SongAPI::sendNextSong(guild.id, guildMember)) {
								InputEvents::deleteInputEventResponseAsync(newEvent);
								co_return;
							};
							eventData.voiceConnection->play();
							newEmbed.setAuthor(guildMember.user.userName, guildMember.user.avatar);
							newEmbed.setDescription("__**Title:**__ [" + SongAPI::getCurrentSong(guild.id).songTitle + "](" + SongAPI::getCurrentSong(guild.id).viewURL + ")" + "\n__**Description:**__ " + SongAPI::getCurrentSong(guild.id).description + "\n__**Duration:**__ " +
								SongAPI::getCurrentSong(guild.id).duration + "\n__**Added By:**__ <@!" + SongAPI::getCurrentSong(guild.id).addedByUserId + "> (" + SongAPI::getCurrentSong(guild.id).addedByUserName + ")");
							newEmbed.setImage(SongAPI::getCurrentSong(guild.id).thumbnailURL);
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
							RespondToInputEventData dataPackage(args->eventData);
							dataPackage.type = DesiredInputEventResponseType::RegularMessage;
							dataPackage.addMessageEmbed(newEmbed);
							auto newEvent02 = InputEvents::respondToEvent(dataPackage);
						}
						else {
							eventData.voiceConnection->play();
							
							newEmbed.setAuthor(guildMember.user.userName, guildMember.user.avatar);
							newEmbed.setDescription("__**It appears as though there was an error when trying to play the following track!**__\n__**Title:**__ [" + eventData.previousSong.songTitle + "](" + eventData.previousSong.viewURL + ")" + "\n__**Description:**__ " + eventData.previousSong.description + "\n__**Duration:**__ " +
								eventData.previousSong.duration + "\n__**Added By:**__ <@!" + eventData.previousSong.addedByUserId + "> (" + eventData.previousSong.addedByUserName + ")");
							newEmbed.setImage(eventData.previousSong.thumbnailURL);
							newEmbed.setTimeStamp(getTimeAndDate());
							newEmbed.setTitle("__**Playing Error:**__");
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
							RespondToInputEventData dataPackage(args->eventData);
							dataPackage.type = DesiredInputEventResponseType::RegularMessage;
							dataPackage.addMessageEmbed(newEmbed);
							auto newEvent02 = InputEvents::respondToEvent(dataPackage);
							
						}
					}
					else {
						discordGuild.getDataFromDB();
						EmbedData newEmbed;
						newEmbed.setAuthor(guildMember.user.userName, guildMember.user.avatar);
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
						RespondToInputEventData dataPackage(args->eventData);
						dataPackage.type = DesiredInputEventResponseType::RegularMessage;
						dataPackage.addMessageEmbed(newEmbed);
						auto newEvent02 = InputEvents::respondToEvent(dataPackage);
					}
					});
			}
			if (SongAPI::isThereAnySongs(guild.id)) {
				if (!SongAPI::sendNextSong(guild.id, guildMember)) {
					InputEvents::deleteInputEventResponseAsync(newEvent);
					co_return;
				};
				DiscordGuild discordGuildNew = discordGuild;

				EmbedData newEmbed;
				newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
				newEmbed.setDescription("__**Title:**__ [" + SongAPI::getCurrentSong(guild.id).songTitle + "](" + SongAPI::getCurrentSong(guild.id).viewURL + ")" + "\n__**Description:**__ " + SongAPI::getCurrentSong(guild.id).description + "\n__**Duration:**__ " +
					SongAPI::getCurrentSong(guild.id).duration + "\n__**Added By:**__ <@!" + SongAPI::getCurrentSong(guild.id).addedByUserId + "> (" + SongAPI::getCurrentSong(guild.id).addedByUserName + ")");
				newEmbed.setImage(SongAPI::getCurrentSong(guild.id).thumbnailURL);
				newEmbed.setTimeStamp(getTimeAndDate());
				newEmbed.setTitle("__**Now Playing:**__");
				newEmbed.setColor(discordGuildNew.data.borderColor);
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
				if (newEvent.eventType == InputEventType::REGULAR_MESSAGE) {
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.type = DesiredInputEventResponseType::RegularMessage;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
				}
				else {
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.type = DesiredInputEventResponseType::InteractionResponseEdit;
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToEvent(dataPackage);
				}
				voiceConnection->play();
			}
			co_return;
		};

	};

}

#endif