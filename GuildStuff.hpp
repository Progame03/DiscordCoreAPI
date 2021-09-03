// May 12, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#ifndef _GUILD_STUFF_
#define _GUILD_STUFF_

#include "../pch.h"
#include "VoiceConnectionStuff.hpp"
#include "DataParsingFunctions.hpp"
#include "HttpStuff.hpp"
#include "ChannelStuff.hpp"
#include "MessageStuff.hpp"
#include "GuildMemberStuff.hpp"
#include "UserStuff.hpp"
#include "RoleStuff.hpp"
#include "YouTubeStuff.hpp"

namespace DiscordCoreAPI {

	class Guild :public GuildData {
	public:
		friend struct Concurrency::details::_ResultHolder<Guild>;
		friend class DiscordCoreInternal::GuildManagerAgent;
		friend class DiscordCoreInternal::GuildManager;
		friend struct OnGuildCreationData;
		friend struct OnGuildDeletionData;
		friend struct OnGuildUpdateData;
		friend class DiscordCoreClient;

		shared_ptr<VoiceConnection> connectToVoice(string channelId) {
			shared_ptr<VoiceConnection> voiceConnectionPtr{ nullptr };
			if (DiscordCoreClientBase::voiceConnectionMap->contains(this->id)) {
				voiceConnectionPtr = DiscordCoreClientBase::voiceConnectionMap->at(this->id);
				return voiceConnectionPtr;
			}
			else if (channelId != "") {
				if ((voiceConnectionPtr == nullptr || voiceConnectionPtr->voiceConnectionData.channelId != channelId)) {
					auto voiceConnectData = DiscordCoreClientBase::webSocketConnectionAgent->getVoiceConnectionData(channelId, this->id);
					voiceConnectData.channelId = channelId;
					voiceConnectData.guildId = this->id;
					voiceConnectData.endpoint = "wss://" + voiceConnectData.endpoint + "/?v=4";
					voiceConnectData.userId = this->discordCoreClientBase->currentUser.id;
					if (DiscordCoreClientBase::audioBuffersMap.contains(this->id)) {
						voiceConnectionPtr = make_shared<VoiceConnection>(DiscordCoreInternal::ThreadManager::getThreadContext(DiscordCoreInternal::ThreadType::Music).get(), voiceConnectData, DiscordCoreClientBase::audioBuffersMap.at(this->id), this->discordCoreClientBase);
						shared_ptr<YouTubeAPI> youtubeAPI;
						if (DiscordCoreClientBase::youtubeAPIMap->contains(this->id)) {
							youtubeAPI = DiscordCoreClientBase::youtubeAPIMap->at(this->id);
						}
						else {
							youtubeAPI = make_shared<YouTubeAPI>(DiscordCoreClientBase::audioBuffersMap.at(this->id), this->id, DiscordCoreClientBase::youtubeAPIMap, DiscordCoreInternal::ThreadManager::getThreadContext().get());
						}
						DiscordCoreClientBase::youtubeAPIMap->insert_or_assign(this->id, youtubeAPI);
					}
					else {
						auto sharedPtr = make_shared<unbounded_buffer<AudioFrameData>>();
						DiscordCoreClientBase::audioBuffersMap.insert(make_pair(this->id, sharedPtr));
						voiceConnectionPtr = make_shared<VoiceConnection>(DiscordCoreInternal::ThreadManager::getThreadContext(DiscordCoreInternal::ThreadType::Music).get(), voiceConnectData, DiscordCoreClientBase::audioBuffersMap.at(this->id), this->discordCoreClientBase);
						shared_ptr<YouTubeAPI> youtubeAPI;
						if (DiscordCoreClientBase::youtubeAPIMap->contains(this->id)) {
							youtubeAPI = DiscordCoreClientBase::youtubeAPIMap->at(this->id);
						}
						else {
							youtubeAPI = make_shared<YouTubeAPI>(DiscordCoreClientBase::audioBuffersMap.at(this->id), this->id, DiscordCoreClientBase::youtubeAPIMap, DiscordCoreInternal::ThreadManager::getThreadContext().get());
						}
						DiscordCoreClientBase::youtubeAPIMap->insert_or_assign(this->id, youtubeAPI);
					}
					DiscordCoreClientBase::voiceConnectionMap->insert(make_pair(this->id, voiceConnectionPtr));
					return voiceConnectionPtr;
				}
			}
			return voiceConnectionPtr;
		}

		shared_ptr<YouTubeAPI> getYouTubeAPI() {
			if (DiscordCoreClientBase::youtubeAPIMap->contains(this->id)) {
				return DiscordCoreClientBase::youtubeAPIMap->at(this->id);
			}
			else {
				return nullptr;
			}
		}

	protected:

		Guild() {};

		Guild(GuildData dataNew) {
			this->icon = dataNew.icon;
			this->name = dataNew.name;
			this->id = dataNew.id;
			this->iconHash = dataNew.iconHash;
			this->splash = dataNew.splash;
			this->discoverySplash = dataNew.discoverySplash;
			this->preferredLocale = dataNew.preferredLocale;
			this->publicUpdatesChannelID = dataNew.publicUpdatesChannelID;
			this->vanityURLCode = dataNew.vanityURLCode;
			this->description = dataNew.description;
			this->banner = dataNew.banner;
			this->ruleChannelID = dataNew.ruleChannelID;
			this->applicationID = dataNew.applicationID;
			this->createdAt = dataNew.createdAt;
			this->joinedAt = dataNew.joinedAt;
			this->widgetChannelID = dataNew.widgetChannelID;
			this->systemChannelID = dataNew.systemChannelID;
			this->region = dataNew.region;
			this->afkChannelID = dataNew.afkChannelID;
			this->ownerID = dataNew.ownerID;
			this->features = dataNew.features;
			this->threads = dataNew.threads;
			this->permissions = dataNew.permissions;
			this->owner = dataNew.owner;
			this->afkTimeOut = dataNew.afkTimeOut;
			this->widgetEnabled = dataNew.widgetEnabled;
			this->verificationLevel = dataNew.verificationLevel;
			this->defaultMessageNotifications = dataNew.defaultMessageNotifications;
			this->explicitContentFilter = dataNew.explicitContentFilter;
			this->emoji = dataNew.emoji;
			this->roles = dataNew.roles;
			this->systemChannelFlags = dataNew.systemChannelFlags;
			this->mfaLevel = dataNew.mfaLevel;
			this->large = dataNew.large;
			this->voiceStates = dataNew.voiceStates;
			this->presences = dataNew.presences;
			this->maxPresences = dataNew.maxPresences;
			this->maxMembers = dataNew.maxMembers;
			this->premiumSubscriptionCount = dataNew.premiumSubscriptionCount;
			this->premiumTier = dataNew.premiumTier;
			this->maxVideoChannelUsers = dataNew.maxVideoChannelUsers;
			this->approximateMemberCount = dataNew.approximateMemberCount;
			this->unavailable = dataNew.unavailable;
			this->memberCount = dataNew.memberCount;
			this->approximatePresenceCount = dataNew.approximatePresenceCount;
			this->nsfwLevel= dataNew.nsfwLevel;
			this->welcomeScreen = dataNew.welcomeScreen;
			this->members= dataNew.members;
			this->channels = dataNew.channels;
			this->stageInstances = dataNew.stageInstances;
			this->discordCoreClient = dataNew.discordCoreClient;
			this->discordCoreClientBase = dataNew.discordCoreClientBase;
			if (DiscordCoreClientBase::audioBuffersMap.contains(this->id)) {
				if (!DiscordCoreClientBase::youtubeAPIMap->contains(this->id)) {
					shared_ptr<YouTubeAPI> sharedPtr = make_shared<YouTubeAPI>(DiscordCoreClientBase::audioBuffersMap.at(this->id), this->id, DiscordCoreClientBase::youtubeAPIMap, DiscordCoreInternal::ThreadManager::getThreadContext().get());
					DiscordCoreClientBase::youtubeAPIMap->insert_or_assign(this->id, sharedPtr);
				}
			}
			else {
				shared_ptr<unbounded_buffer<AudioFrameData>> sharedPtrBuffer = make_shared<unbounded_buffer<AudioFrameData>>();
				DiscordCoreClientBase::audioBuffersMap.insert(make_pair(this->id, sharedPtrBuffer));
				if (!DiscordCoreClientBase::youtubeAPIMap->contains(this->id)) {
					shared_ptr<YouTubeAPI> sharedPtr = make_shared<YouTubeAPI>(DiscordCoreClientBase::audioBuffersMap.at(this->id), this->id, DiscordCoreClientBase::youtubeAPIMap, DiscordCoreInternal::ThreadManager::getThreadContext().get());
					DiscordCoreClientBase::youtubeAPIMap->insert_or_assign(this->id, sharedPtr);
				}
			}
			return;
		}

		void initialize() {
			try {
				cout << "Caching guild: " << this->name << endl;
				cout << "Caching channels for guild: " << this->name << endl;
				for (auto value : channels) {
					value.guildId = this->id;
					ChannelData channelData = value;
					channelData.discordCoreClient = this->discordCoreClient;
					Channel channel(channelData);
					this->discordCoreClientBase->channels->insertChannelAsync(channel).get();
				}
				cout << "Caching guild members for guild: " << this->name << endl;
				for (unsigned int x = 0; x < this->members.size(); x += 1) {
					GuildMemberData guildMemberData = this->members.at(x);
					for (auto value : this->voiceStates) {
						if (value.userId == guildMemberData.user.id) {
							guildMemberData.voiceData = value;
						}
					}
					guildMemberData.guildId = this->id;
					guildMemberData.discordCoreClient = this->discordCoreClient;
					DiscordGuildMember discordGuildMember(guildMemberData);
					discordGuildMember.writeDataToDB();
					GuildMember guildMember(guildMemberData, this->id);
					this->discordCoreClientBase->guildMembers->insertGuildMemberAsync(guildMember, this->id).get();
				}
				cout << "Caching roles for guild: " << this->name << endl;
				for (auto const& value : roles) {
					RoleData roleData = value;
					roleData.discordCoreClient = this->discordCoreClient;
					Role role(roleData);
					this->discordCoreClientBase->roles->insertRoleAsync(role).get();
				}
				cout << "Caching users for guild: " << this->name << endl << endl;
				for (auto value : members) {
					UserData userData = value.user;
					userData.discordCoreClient = this->discordCoreClient;
					User user(userData);
					this->discordCoreClientBase->users->insertUserAsync(user).get();
				}
			}
			catch (exception error) {
				cout << "Error: " << error.what() << endl;
			}
			return;
		}
	};

	struct CreateGuildBanData {
		string guildId{ "" };
		string guildMemberId{ "" };
		int deleteMessageDays{ 0 };
		string reason{ "" };
	};

	struct GetGuildData {
		string guildId{ "" };
	};

	struct FetchVanityInviteData {
		string guildId{ "" };
	};

	struct FetchGuildData {
		string guildId{ "" };
	};

	struct FetchAuditLogData {
		AuditLogEvent actionType{};
		string guildId{ "" };
		unsigned int limit{ 0 };
		string userId{ "" };
	};

	struct FetchInviteData {
		string inviteId{ "" };
	};

	struct FetchInvitesData {
		string guildId{ "" };
	};
};

namespace DiscordCoreInternal	{

	class GuildManagerAgent : agent {
	protected:
		friend class DiscordCoreAPI::DiscordCoreClient;
		friend class DiscordCoreAPI::EventHandler;
		friend class GuildManager;

		static overwrite_buffer<map<string, DiscordCoreAPI::Guild>> cache;
		static shared_ptr<ThreadContext> threadContext;

		shared_ptr<DiscordCoreAPI::DiscordCoreClientBase> discordCoreClientBase{ nullptr };
		unbounded_buffer<vector<DiscordCoreAPI::InviteData>> outInvitesBuffer{ nullptr };
		unbounded_buffer<GetVanityInviteData> requestGetVanityInviteBuffer{ nullptr };
		unbounded_buffer<DiscordCoreAPI::AuditLogData> outAuditLogBuffer{ nullptr };
		shared_ptr<DiscordCoreAPI::DiscordCoreClient> discordCoreClient{ nullptr };
		unbounded_buffer<DiscordCoreAPI::InviteData> outInviteBuffer{ nullptr };
		unbounded_buffer<CollectGuildData> requestCollectGuildBuffer{ nullptr };
		unbounded_buffer<PutGuildBanData> requestPutGuildBanBuffer{ nullptr };
		unbounded_buffer<GetAuditLogData> requestGetAuditLogBuffer{ nullptr };
		unbounded_buffer<GetInvitesData> requestGetInvitesBuffer{ nullptr };
		unbounded_buffer<DiscordCoreAPI::BanData> outBanBuffer{ nullptr };
		unbounded_buffer<DiscordCoreAPI::Guild> outGuildBuffer{ nullptr };
		unbounded_buffer<GetInviteData> requestGetInviteBuffer{ nullptr };
		unbounded_buffer<GetGuildData> requestGetGuildBuffer{ nullptr };
		concurrent_queue<DiscordCoreAPI::Guild> guildsToInsert{};
		unbounded_buffer<exception> errorBuffer{ nullptr };
		HttpAgentResources agentResources{};		
		
		GuildManagerAgent(HttpAgentResources agentResourcesNew,  shared_ptr<DiscordCoreAPI::DiscordCoreClient> coreClientNew, shared_ptr<DiscordCoreAPI::DiscordCoreClientBase> coreClientBaseNew)
			:agent(*GuildManagerAgent::threadContext->scheduler->scheduler) {
			this->agentResources = agentResourcesNew;
			this->discordCoreClient = coreClientNew;
			this->discordCoreClientBase = coreClientBaseNew;
			return;
		}

		static void initialize(shared_ptr<ThreadContext> threadContextNew) {
			GuildManagerAgent::threadContext = threadContextNew;
		}

		static void cleanup() {
			GuildManagerAgent::threadContext->releaseGroup();
		}

		void getError(string stackTrace) {
			exception error;
			while (try_receive(errorBuffer, error)) {
				cout << stackTrace + "::GuildManagerAgent Error: " << error.what() << endl << endl;
			}
			return;
		}

		DiscordCoreAPI::Guild getObjectData(GetGuildData dataPackage) {
			HttpWorkload workload;
			workload.workloadClass = HttpWorkloadClass::GET;
			workload.workloadType = HttpWorkloadType::GET_GUILD;
			workload.relativePath = "/guilds/" + dataPackage.guildId;
			HttpRequestAgent requestAgent(dataPackage.agentResources);
			send(requestAgent.workSubmissionBuffer, workload);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManagerAgent::getObjectData_00");
			HttpData returnData;
			try_receive(requestAgent.workReturnBuffer, returnData);
			if (returnData.returnCode != 204 && returnData.returnCode != 201 && returnData.returnCode != 200) {
				cout << "GuildManagerAgent::getObjectData_00 Error: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			else {
				cout << "GuildManagerAgent::getObjectData_00 Success: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			DiscordCoreAPI::GuildData guildData;
			guildData.discordCoreClient = this->discordCoreClient;
			guildData.discordCoreClientBase = this->discordCoreClientBase;
			DataParser::parseObject(returnData.data, &guildData);
			DiscordCoreAPI::Guild guildNew(guildData);
			return guildNew;
		}

		DiscordCoreAPI::InviteData getObjectData(GetInviteData dataPackage) {
			HttpWorkload workload;
			workload.workloadClass = HttpWorkloadClass::GET;
			workload.workloadType = HttpWorkloadType::GET_INVITE;
			workload.relativePath = "/invites/" + dataPackage.inviteId + "?with_counts=true&with_expiration=true";
			HttpRequestAgent requestAgent(dataPackage.agentResources);
			send(requestAgent.workSubmissionBuffer, workload);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManagerAgent::getObjectData_01");
			HttpData returnData;
			try_receive(requestAgent.workReturnBuffer, returnData);
			if (returnData.returnCode != 204 && returnData.returnCode != 201 && returnData.returnCode != 200) {
				cout << "GuildManagerAgent::getObjectData_01 Error: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			else {
				cout << "GuildManagerAgent::getObjectData_01 Success: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			DiscordCoreAPI::InviteData inviteData;
			DataParser::parseObject(returnData.data, &inviteData);
			return inviteData;
		}

		vector<DiscordCoreAPI::InviteData> getObjectData(GetInvitesData dataPackage) {
			HttpWorkload workload;
			workload.workloadClass = HttpWorkloadClass::GET;
			workload.workloadType = HttpWorkloadType::GET_INVITES;
			workload.relativePath = "/guilds/" + dataPackage.guildId + "/invites";
			HttpRequestAgent requestAgent(dataPackage.agentResources);
			send(requestAgent.workSubmissionBuffer, workload);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManagerAgent::getObjectData_02");
			HttpData returnData;
			try_receive(requestAgent.workReturnBuffer, returnData);
			if (returnData.returnCode != 204 && returnData.returnCode != 201 && returnData.returnCode != 200) {
				cout << "GuildManagerAgent::getObjectData_02 Error: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			else {
				cout << "GuildManagerAgent::getObjectData_02 Success: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			vector<DiscordCoreAPI::InviteData> inviteData;
			for (auto value : returnData.data) {
				DiscordCoreAPI::InviteData inviteDataNew;
				DataParser::parseObject(value, &inviteDataNew);
				inviteData.push_back(inviteDataNew);
			}
			return inviteData;
		}

		DiscordCoreAPI::InviteData getObjectData(GetVanityInviteData dataPackage) {
			HttpWorkload workload;
			workload.workloadClass = HttpWorkloadClass::GET;
			workload.workloadType = HttpWorkloadType::GET_VANITY_INVITE;
			workload.relativePath = "/guilds/" + dataPackage.guildId + "/vanity-url";
			HttpRequestAgent requestAgent(dataPackage.agentResources);
			send(requestAgent.workSubmissionBuffer, workload);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManagerAgent::getObjectData_03");
			HttpData returnData;
			try_receive(requestAgent.workReturnBuffer, returnData);
			if (returnData.returnCode != 204 && returnData.returnCode != 201 && returnData.returnCode != 200) {
				cout << "GuildManagerAgent::getObjectData_03 Error: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			else {
				cout << "GuildManagerAgent::getObjectData_03 Success: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			DiscordCoreAPI::InviteData inviteData;
			DataParser::parseObject(returnData.data, &inviteData);
			return inviteData;
		}

		DiscordCoreAPI::AuditLogData getObjectData(GetAuditLogData dataPackage) {
			HttpWorkload workload;
			workload.workloadClass = HttpWorkloadClass::GET;
			workload.workloadType = HttpWorkloadType::GET_AUDIT_LOG;
			workload.relativePath = "/guilds/" + dataPackage.guildId + "/audit-logs";
			if (dataPackage.userId != "") {
				workload.relativePath += "?user_id=" + dataPackage.userId;
				if (to_string((int)dataPackage.actionType) != "") {
					workload.relativePath += "&action_type=" + to_string((int)dataPackage.actionType);
				}
				if (dataPackage.limit != 0) {
					workload.relativePath += "&limit=" + to_string(dataPackage.limit);
				}
			}
			else if (to_string((int)dataPackage.actionType) != "") {
				workload.relativePath += "?action_type=" + to_string((int)dataPackage.actionType);
				if (dataPackage.limit != 0) {
					workload.relativePath += "&limit=" + to_string(dataPackage.limit);
				}
			}
			else if (dataPackage.limit != 0) {
				workload.relativePath += "?limit=" + to_string(dataPackage.limit);
			}
			HttpRequestAgent requestAgent(dataPackage.agentResources);
			send(requestAgent.workSubmissionBuffer, workload);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManagerAgent::getObjectData_04");
			HttpData returnData;
			try_receive(requestAgent.workReturnBuffer, returnData);
			if (returnData.returnCode != 204 && returnData.returnCode != 201 && returnData.returnCode != 200) {
				cout << "GuildManagerAgent::getObjectData_04 Error: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			else {
				cout << "GuildManagerAgent::getObjectData_04 Success: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			DiscordCoreAPI::AuditLogData auditLogData;
			DataParser::parseObject(returnData.data, &auditLogData);
			return auditLogData;
		}

		DiscordCoreAPI::BanData putObjectData(PutGuildBanData dataPackage) {
			HttpWorkload workload;
			workload.workloadClass = HttpWorkloadClass::PUT;
			workload.workloadType = HttpWorkloadType::PUT_GUILD_BAN;
			workload.relativePath = "/guilds/" + dataPackage.guildId + "/bans/" + dataPackage.guildMemberId;
			workload.content = getAddBanPayload(dataPackage);
			HttpRequestAgent requestAgent(dataPackage.agentResources);
			send(requestAgent.workSubmissionBuffer, workload);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManagerAgent::putObjectData_00");
			HttpData returnData;
			try_receive(requestAgent.workReturnBuffer, returnData);
			DiscordCoreAPI::BanData banData;
			if (returnData.returnCode != 204 && returnData.returnCode != 201 && returnData.returnCode != 200) {
				cout << "GuildManagerAgent::putObjectData_00 Error: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
				banData.failedDueToPerms = true;
			}
			else {
				cout << "GuildManagerAgent::putObjectData_00 Success: " << returnData.returnCode << ", " << returnData.returnMessage << endl << endl;
			}
			DataParser::parseObject(returnData.data, &banData);
			return banData;
		}

		void run() {
			try {
				CollectGuildData dataPackage01;
				if (try_receive(this->requestCollectGuildBuffer, dataPackage01)) {
					map<string, DiscordCoreAPI::Guild> cacheTemp;
					if (try_receive(GuildManagerAgent::cache, cacheTemp)) {
						if (cacheTemp.contains(dataPackage01.guildId)) {
							DiscordCoreAPI::Guild guild = cacheTemp.at(dataPackage01.guildId);
							send(this->outGuildBuffer, guild);
						}
					}
				}
				GetGuildData dataPackage02;
				if (try_receive(this->requestGetGuildBuffer, dataPackage02)) {
					map<string, DiscordCoreAPI::Guild> cacheTemp;
					if (try_receive(GuildManagerAgent::cache, cacheTemp)) {
						if (cacheTemp.contains(dataPackage02.guildId)) {
							cacheTemp.erase(dataPackage02.guildId);
						}
					}
					DiscordCoreAPI::Guild guild = getObjectData(dataPackage02);
					cacheTemp.insert(make_pair(dataPackage02.guildId, guild));
					send(this->outGuildBuffer, guild);
					send(GuildManagerAgent::cache, cacheTemp);
				}
				GetAuditLogData dataPackage03;
				if (try_receive(this->requestGetAuditLogBuffer, dataPackage03)) {
					DiscordCoreAPI::AuditLogData auditLog = getObjectData(dataPackage03);
					send(this->outAuditLogBuffer, auditLog);
				}
				GetInvitesData dataPackage04;
				if (try_receive(this->requestGetInvitesBuffer, dataPackage04)) {
					vector<DiscordCoreAPI::InviteData> inviteData = getObjectData(dataPackage04);
					send(this->outInvitesBuffer, inviteData);
				}
				GetInviteData dataPackage05;
				if (try_receive(this->requestGetInviteBuffer, dataPackage05)) {
					DiscordCoreAPI::InviteData inviteData = getObjectData(dataPackage05);
					send(this->outInviteBuffer, inviteData);
				}
				GetVanityInviteData dataPackage06;
				if (try_receive(this->requestGetVanityInviteBuffer, dataPackage06)) {
					DiscordCoreAPI::InviteData inviteData = getObjectData(dataPackage06);
					send(this->outInviteBuffer, inviteData);
				}
				PutGuildBanData dataPackage07;
				if (try_receive(this->requestPutGuildBanBuffer, dataPackage07)) {
					DiscordCoreAPI::BanData inviteData = putObjectData(dataPackage07);
					send(this->outBanBuffer, inviteData);
				}
				DiscordCoreAPI::Guild guildNew;
				while (this->guildsToInsert.try_pop(guildNew)) {
					map<string, DiscordCoreAPI::Guild> cacheTemp;
					try_receive(GuildManagerAgent::cache, cacheTemp);
					if (cacheTemp.contains(guildNew.id)) {
						cacheTemp.erase(guildNew.id);
					}
					guildNew.initialize();
					cacheTemp.insert(make_pair(guildNew.id, guildNew));
					send(GuildManagerAgent::cache, cacheTemp);
				}
			}
			catch (const exception& e) {
				send(this->errorBuffer, e);
			}
			done();
			return;
		}
	};

	class GuildManager {
	public:

		template <class _Ty>
		friend _CONSTEXPR20_DYNALLOC void std::_Destroy_in_place(_Ty& _Obj) noexcept;
		friend class DiscordCoreAPI::PermissionsConverter;
		friend class DiscordCoreAPI::DiscordCoreClient;
		friend class DiscordCoreAPI::Guilds;

		GuildManager(GuildManager* pointer) {
			if (pointer != nullptr) {
				*this = *pointer;
			}
		}

	protected:

		shared_ptr<DiscordCoreAPI::DiscordCoreClientBase> discordCoreClientBase{ nullptr };
		shared_ptr<DiscordCoreAPI::DiscordCoreClient> discordCoreClient{ nullptr };
		shared_ptr<ThreadContext> threadContext{ nullptr };
		HttpAgentResources agentResources{};

		GuildManager initialize(HttpAgentResources agentResourcesNew, shared_ptr<ThreadContext> threadContextNew, shared_ptr<DiscordCoreAPI::DiscordCoreClient> discordCoreClientNew, shared_ptr<DiscordCoreAPI::DiscordCoreClientBase> discordCoreClientBaseNew) {
			this->discordCoreClientBase = discordCoreClientBaseNew;
			this->discordCoreClient = discordCoreClientNew;
			this->agentResources = agentResourcesNew;
			this->threadContext = threadContextNew;
			return *this;
		}

		task<DiscordCoreAPI::Guild> fetchAsync(DiscordCoreAPI::FetchGuildData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			GetGuildData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.guildId = dataPackage.guildId;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestGetGuildBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::fetchAsync");
			DiscordCoreAPI::GuildData guildData;
			guildData.discordCoreClient = this->discordCoreClient;
			guildData.discordCoreClientBase = this->discordCoreClientBase;
			DiscordCoreAPI::Guild guildNew(guildData);
			try_receive(requestAgent.outGuildBuffer, guildNew);
			co_await mainThread;
			co_return guildNew;
		}

		task<vector<DiscordCoreAPI::InviteData>> fetchInvitesAsync(DiscordCoreAPI::FetchInvitesData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			GetInvitesData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.guildId = dataPackage.guildId;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestGetInvitesBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::getInvitesAsync");
			vector<DiscordCoreAPI::InviteData> inviteData;
			try_receive(requestAgent.outInvitesBuffer, inviteData);
			co_await mainThread;
			co_return inviteData;
		}

		task<DiscordCoreAPI::BanData> createGuildBanAsync(DiscordCoreAPI::CreateGuildBanData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			PutGuildBanData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.guildId = dataPackage.guildId;
			dataPackageNew.guildMemberId = dataPackage.guildMemberId;
			dataPackageNew.deleteMessageDays = dataPackage.deleteMessageDays;
			dataPackageNew.reason = dataPackage.reason;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestPutGuildBanBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::createGuildBanAsync");
			DiscordCoreAPI::BanData banData;
			try_receive(requestAgent.outBanBuffer, banData);
			co_await mainThread;
			co_return banData;
		}

		task<DiscordCoreAPI::InviteData> fetchVanityInviteAsync(DiscordCoreAPI::FetchVanityInviteData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			GetVanityInviteData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.guildId = dataPackage.guildId;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestGetVanityInviteBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::getVanityInviteAsync");
			DiscordCoreAPI::InviteData inviteData;
			try_receive(requestAgent.outInviteBuffer, inviteData);
			co_await mainThread;
			co_return inviteData;
		}

		task<DiscordCoreAPI::InviteData> fetchInviteAsync(DiscordCoreAPI::FetchInviteData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			GetInviteData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.inviteId = dataPackage.inviteId;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestGetInviteBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::getInviteAsync");
			DiscordCoreAPI::InviteData inviteData;
			try_receive(requestAgent.outInviteBuffer, inviteData);
			co_await mainThread;
			co_return inviteData;
		}

		task<DiscordCoreAPI::AuditLogData> fetchAuditLogDataAsync(DiscordCoreAPI::FetchAuditLogData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			GetAuditLogData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.guildId = dataPackage.guildId;
			dataPackageNew.actionType = (AuditLogEvent)dataPackage.actionType;
			dataPackageNew.limit = dataPackage.limit;
			dataPackageNew.userId = dataPackage.userId;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestGetAuditLogBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::getAuditLogDataAsync");
			DiscordCoreAPI::AuditLogData auditLog;
			try_receive(requestAgent.outAuditLogBuffer, auditLog);
			co_await mainThread;
			co_return auditLog;
		}

		task<DiscordCoreAPI::Guild> getGuildAsync(DiscordCoreAPI::GetGuildData dataPackage) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			CollectGuildData dataPackageNew;
			dataPackageNew.agentResources = this->agentResources;
			dataPackageNew.guildId = dataPackage.guildId;
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			send(requestAgent.requestCollectGuildBuffer, dataPackageNew);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::getGuildAsync");
			DiscordCoreAPI::GuildData guildData;
			guildData.discordCoreClient = this->discordCoreClient;
			guildData.discordCoreClientBase = this->discordCoreClientBase;
			DiscordCoreAPI::Guild guildNew(guildData);
			try_receive(requestAgent.outGuildBuffer, guildNew);
			co_await mainThread;
			co_return guildNew;
		}

		task<vector<DiscordCoreAPI::Guild>> getAllGuildsAsync() {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			map<string, DiscordCoreAPI::Guild>cache;
			try_receive(GuildManagerAgent::cache, cache);
			vector<DiscordCoreAPI::Guild> guildVector;
			for (auto [key, value] : cache) {
				guildVector.push_back(value);
			}
			send(GuildManagerAgent::cache, cache);
			co_await mainThread;
			co_return guildVector;
		}

		task<void> insertGuildAsync(DiscordCoreAPI::Guild guild) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			GuildManagerAgent requestAgent(this->agentResources, this->discordCoreClient, this->discordCoreClientBase);
			requestAgent.guildsToInsert.push(guild);
			requestAgent.start();
			agent::wait(&requestAgent);
			requestAgent.getError("GuildManager::insertGuildAsync");
			co_await mainThread;
			co_return;
		}

		task<void> removeGuildAsync(string guildId) {
			apartment_context mainThread;
			co_await resume_foreground(*this->threadContext->dispatcherQueue.get());
			map<string, DiscordCoreAPI::Guild> cache;
			try_receive(GuildManagerAgent::cache, cache);
			if (cache.contains(guildId)) {
				cache.erase(guildId);
			}
			send(GuildManagerAgent::cache, cache);
			co_await mainThread;
			co_return;
		}

		~GuildManager() {}
	};
	overwrite_buffer<map<string, DiscordCoreAPI::Guild>> GuildManagerAgent::cache{ nullptr };
	shared_ptr<ThreadContext> GuildManagerAgent::threadContext{ nullptr };
}
#endif