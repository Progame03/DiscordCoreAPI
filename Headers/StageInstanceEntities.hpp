// StageInstanceEntities.hpp - Source file for the Stage Instance related stuff.
// Nov 29, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include "IndexInitial.hpp"
#include "FoundationEntities.hpp"

namespace DiscordCoreAPI {

	/// For creating a StageInstance. \brief For creating a StageInstance.
	struct DiscordCoreAPI_Dll CreateStageInstanceData {
		StageInstancePrivacyLevel privacyLevel{};///< The privacy level of the Stage instance(default GUILD_ONLY).
		string channelId{ "" };///< The id of the Stage channel.
		string reason{ "" };///< The reason for starting it.
		string topic{ "" };///< The topic of the Stage instance(1 - 120 characters).
	};
	
	/// A single StageInstance. \brief A single StageInstance.
	class DiscordCoreAPI_Dll StageInstance : public StageInstanceData {
	public:

		friend class DiscordCoreInternal::HttpRequestAgent;
		friend class DiscordCoreInternal::DataParser;
		template<typename returnValueType>
		friend class CoRoutine;
		friend class DiscordCoreClient;
		friend class Reactions;
		friend class Users;
		friend class Guild;

		StageInstance();

		virtual ~StageInstance() {};

	protected:

		StageInstance(StageInstanceData dataNew);

	};
	
	/**
	* \addtogroup discord_core_client
	* @{
	*/
	/// An interface class for the StageInstance related Discord endpoints. \brief An interface class for the StageInstance related Discord endpoints.
	class DiscordCoreAPI_Dll StageInstances {
	public:

		/// Creates a StageInstance. \brief Creates a StageInstance.
		/// \param dataPackage A CreateStageInstanceData structure.
		/// \returns A CoRoutine containing a StageInstance.
		static CoRoutine<StageInstance> createStageInstanceAsync(CreateStageInstanceData dataPackage);
	};

	/**@}*/
}