// SoundCloudAPI.hpp - Header for the SoundCloud api related stuff.
// Aug 25, 2021
// Chris M.
// https://github.com/RealTimeChris
#pragma once

#include "IndexInitial.hpp"
#include "CoRoutine.hpp"
#include "SongEncoder.hpp"
#include "SongDecoder.hpp"

namespace DiscordCoreAPI {
	
	class DiscordCoreAPI_Dll SoundCloudAPI {
	public:

		friend class DiscordCoreClient;
		friend class VoiceConnection;
		friend class SoundCloudAPI;
		friend class SongAPI;
		friend class SongAPI;
		friend class Guild;

		SoundCloudAPI(string guildId) noexcept;

		~SoundCloudAPI();

	protected:

		unique_ptr<SongDecoder> songDecoder{ nullptr };
		unique_ptr<SongEncoder> songEncoder{ nullptr };
		unique_ptr<CoRoutine<void>> theTask{ nullptr };
		const int32_t maxBufferSize{ 8192 };
		Event<void> readyToQuitEventOut{};
		Event<void> readyToQuitEventIn{};
		SoundCloudSong theSong{ };
		string guildId{ "" };

		bool stop();

		void sendNextSong(Song newSong);

		void cancelCurrentSong();

		CoRoutine<void> downloadAndStreamAudio(Song newSong, SoundCloudAPI* soundCloudAPI);

		void sendEmptyingFrames(UnboundedMessageBlock<vector<uint8_t>>& sendAudioDataBufferNew);

		static vector<SoundCloudSong> searchForSong(string searchQuery, string guildId);

	};
	
};