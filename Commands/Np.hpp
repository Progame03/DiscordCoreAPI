// Np.hpp - Header for the "now playing" command.
// Sep 4, 2021
// Chris M.
// https://gibhub.com/RealTimeChris

#pragma once

#ifndef _NP_
#define _NP_

#include "../DiscordCoreClient02.hpp"

namespace DiscordCoreAPI {

	class Np :public BaseFunction {
	public:
		Np() {
			this->commandName = "np";
			this->helpDescription = "Displays the currently playing song.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nSimply enter !np or /np!\n------");
			msgEmbed.setTitle("__**Now Playing Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		Np* create() {
			return new Np;
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

			EmbedData newEmbed;
			newEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
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
				ReplyMessageData dataPackage(args->eventData);
				dataPackage.addMessageEmbed(newEmbed);
				auto newEvent02 = InputEvents::respondToEvent(dataPackage);
			}
			else {
				CreateInteractionResponseData dataPackage(args->eventData);
				dataPackage.addMessageEmbed(newEmbed);
				auto newEvent02 = InputEvents::respondToEvent(dataPackage);
			}


			co_return;
		}
	};

}
#endif
