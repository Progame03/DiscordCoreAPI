// SongEncoder.hpp - Header for the song encoder class.
// Aug 22, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include "IndexInitial.hpp"
#include "FoundationEntities.hpp"

namespace DiscordCoreAPI {

	class DiscordCoreAPI_Dll SongEncoder {
	public:

		SongEncoder();

		concurrent_vector<AudioFrameData> encodeFrames(concurrent_vector<RawFrameData> rawFrames);

		~SongEncoder();

	protected:
		OpusEncoder* encoder{ nullptr };
		const __int32 maxBufferSize{ 1276 };
		const __int32 sampleRate{ 48000 };
		const __int32 nChannels{ 2 };

		EncodedFrameData encodeSingleAudioFrame(RawFrameData inputFrame);

	};

}
