// HttpStuff.hpp - Header for all of the HTTP related stuff.
// May 12, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#ifndef _HTTP_STUFF_
#define _HTTP_STUFF_

#include "../pch.h"
#include "JSONifier.hpp"

namespace DiscordCoreInternal {

	class HttpRequestAgent : public agent {
	public:

		static shared_ptr<ThreadContext> threadContext;

		HttpRequestAgent()
			: agent(*HttpRequestAgent::threadContext->scheduler->scheduler)
		{
			try {
				this->baseURLInd = HttpRequestAgent::baseURL;
			
				Filters::HttpBaseProtocolFilter filter;
				Filters::HttpCacheControl cacheControl{ nullptr };
				cacheControl = filter.CacheControl();
				cacheControl.ReadBehavior(Filters::HttpCacheReadBehavior::NoCache);
				cacheControl.WriteBehavior(Filters::HttpCacheWriteBehavior::NoCache);
				this->getHttpClient = HttpClient(filter);
				this->getHeaders = this->getHttpClient.DefaultRequestHeaders();
				this->putHttpClient = HttpClient(filter);
				this->putHeaders = this->putHttpClient.DefaultRequestHeaders();
				this->postHttpClient = HttpClient(filter);
				this->postHeaders = this->postHttpClient.DefaultRequestHeaders();
				this->patchHttpClient = HttpClient(filter);
				this->patchHeaders = this->patchHttpClient.DefaultRequestHeaders();
				this->deleteHttpClient = HttpClient(filter);
				this->deleteHeaders = this->deleteHttpClient.DefaultRequestHeaders();
				hstring headerString = L"Bot ";
				hstring headerString2 = headerString + to_hstring(HttpRequestAgent::botToken);
				HttpCredentialsHeaderValue credentialValue(nullptr);
				credentialValue = credentialValue.Parse(headerString2.c_str());
				this->getHeaders.Authorization(credentialValue);
				this->putHeaders.Authorization(credentialValue);
				this->postHeaders.Authorization(credentialValue);
				this->patchHeaders.Authorization(credentialValue);
				this->deleteHeaders.Authorization(credentialValue);
			}
			catch (...) {
				DiscordCoreAPI::rethrowException("HttpRequestAgent::HttpRequestAgent() Error: ");
			}
		}

		HttpRequestAgent(HttpAgentResources agentResources)
			: agent(*HttpRequestAgent::threadContext->scheduler->scheduler)
		{
			try {
				if (agentResources.baseURL == "") {
					this->baseURLInd = HttpRequestAgent::baseURL;
				}
				else {
					this->baseURLInd = agentResources.baseURL;
				}
				Filters::HttpBaseProtocolFilter filter;
				Filters::HttpCacheControl cacheControl{ nullptr };
				cacheControl = filter.CacheControl();
				cacheControl.ReadBehavior(Filters::HttpCacheReadBehavior::NoCache);
				cacheControl.WriteBehavior(Filters::HttpCacheWriteBehavior::NoCache);
				this->getHttpClient = HttpClient(filter);
				this->getHeaders = this->getHttpClient.DefaultRequestHeaders();
				if (agentResources.userAgent != "") {
					this->getHeaders.UserAgent().TryParseAdd(to_hstring(agentResources.userAgent));
				}
				this->putHttpClient = HttpClient(filter);
				this->putHeaders = this->putHttpClient.DefaultRequestHeaders();
				if (agentResources.userAgent != "") {
					this->putHeaders.UserAgent().TryParseAdd(to_hstring(agentResources.userAgent));
				}
				this->postHttpClient = HttpClient(filter);
				this->postHeaders = this->postHttpClient.DefaultRequestHeaders();
				if (agentResources.userAgent != "") {
					this->postHeaders.UserAgent().TryParseAdd(to_hstring(agentResources.userAgent));
				}
				this->patchHttpClient = HttpClient(filter);
				this->patchHeaders = this->patchHttpClient.DefaultRequestHeaders();
				if (agentResources.userAgent != "") {
					this->patchHeaders.UserAgent().TryParseAdd(to_hstring(agentResources.userAgent));
				}
				this->deleteHttpClient = HttpClient(filter);
				this->deleteHeaders = this->deleteHttpClient.DefaultRequestHeaders();
				if (agentResources.userAgent != "") {
					this->deleteHeaders.UserAgent().TryParseAdd(to_hstring(agentResources.userAgent));
				}
				if (agentResources.headers.size() > 0) {
					if (agentResources.headers.at(0).headerType == HeaderTypes::Bot_Auth) {
						hstring headerString = L"Bot ";
						hstring headerString2 = headerString + to_hstring(HttpRequestAgent::botToken);
						HttpCredentialsHeaderValue credentialValue(nullptr);
						credentialValue = credentialValue.Parse(headerString2.c_str());
						this->getHeaders.Authorization(credentialValue);
						this->putHeaders.Authorization(credentialValue);
						this->postHeaders.Authorization(credentialValue);
						this->patchHeaders.Authorization(credentialValue);
						this->deleteHeaders.Authorization(credentialValue);
					}
				}
			}
			catch (...) {
				DiscordCoreAPI::rethrowException("HttpRequestAgent::HttpRequestAgent() Error: ");
			}
		}

		static void initialize(string botTokenNew, string baseURLNew) {
			HttpRequestAgent::threadContext = ThreadManager::getThreadContext().get();
			HttpRequestAgent::botToken = botTokenNew;
			HttpRequestAgent::baseURL = baseURLNew;
		};

		HttpData submitWorkloadAndGetResult(HttpWorkloadData workload, string callStack) {
			send(this->workSubmissionBuffer, workload);
			this->start();
			wait(this);
			DiscordCoreAPI::StopWatch stopWatch{ 10000 };
			while (!stopWatch.hasTimePassed()) {
				HttpData returnData;
				if (try_receive(this->workReturnBuffer, returnData)) {
					return returnData;
				};
			}
			return HttpData();
		}

		~HttpRequestAgent() {}

	protected:

		static map<HttpWorkloadType, string> rateLimitDataBucketValues;
		static map<string, RateLimitData> rateLimitData;
		static string botToken;
		static string baseURL;

		unbounded_buffer<HttpWorkloadData> workSubmissionBuffer{ nullptr };
		unbounded_buffer<HttpData> workReturnBuffer{ nullptr };
		HttpRequestHeaderCollection deleteHeaders{ nullptr };
		HttpRequestHeaderCollection patchHeaders{ nullptr };
		unbounded_buffer<exception> errorBuffer{ nullptr };
		HttpRequestHeaderCollection postHeaders{ nullptr };
		HttpRequestHeaderCollection putHeaders{ nullptr };
		HttpRequestHeaderCollection getHeaders{ nullptr };
		HttpClient deleteHttpClient{ nullptr };
		HttpClient patchHttpClient{ nullptr };
		HttpClient postHttpClient{ nullptr };
		HttpClient putHttpClient{ nullptr };
		HttpClient getHttpClient{ nullptr };
		string baseURLInd{ "" };

		static bool executeByRateLimitData(DiscordCoreInternal::RateLimitData* rateLimitDataNew) {
			if (rateLimitDataNew->getsRemaining <= 0) {
				float loopStartTime = rateLimitDataNew->timeStartedAt;
				float targetTime = loopStartTime + rateLimitDataNew->msRemain;
				float currentTime = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
				float timeRemaining = targetTime - currentTime;
				if (timeRemaining > 0.0f) {
					if (rateLimitDataNew->nextExecutionTime == 0) {
						rateLimitDataNew->nextExecutionTime = currentTime;
					}
					rateLimitDataNew->nextExecutionTime += timeRemaining;
					timeRemaining = (float)rateLimitDataNew->nextExecutionTime - currentTime;
					cout << "Waiting on rate-limit, Time Remainiing: " << timeRemaining << "ms." << endl << endl;
					while (timeRemaining > 0.0f) {
						currentTime = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
						timeRemaining = rateLimitDataNew->nextExecutionTime - currentTime;
					}
					rateLimitDataNew->nextExecutionTime = 0;
					rateLimitDataNew->msRemain = 0.0f;
					rateLimitDataNew->timeStartedAt = 0.0f;
				}
			}
			rateLimitDataNew->getsRemaining -= 1;
			return false;
		}

		void run() {
			try {
				transformer<HttpWorkloadData, HttpData> completeHttpRequest([this](HttpWorkloadData workload) -> HttpData {
					RateLimitData rateLimitData;
					rateLimitData.workloadType = workload.workloadType;
					auto bucketIterator = HttpRequestAgent::rateLimitDataBucketValues.find(workload.workloadType);
					if (bucketIterator != end(HttpRequestAgent::rateLimitDataBucketValues)) {
						string bucket = HttpRequestAgent::rateLimitDataBucketValues.at(workload.workloadType);
						rateLimitData = HttpRequestAgent::rateLimitData.at(bucket);
					}
					if (rateLimitData.getsRemaining <= 0) {
						float loopStartTime = rateLimitData.timeStartedAt;
						float targetTime = loopStartTime + rateLimitData.msRemain;
						float currentTime = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
						float timeRemaining = targetTime - currentTime;
						if (timeRemaining > 0.0f) {
							if (rateLimitData.nextExecutionTime == 0) {
								rateLimitData.nextExecutionTime = currentTime;
							}
							rateLimitData.nextExecutionTime += timeRemaining;
							timeRemaining = (float)rateLimitData.nextExecutionTime - currentTime;
							cout << "Waiting on rate-limit, Time Remainiing: " << timeRemaining << "ms." << endl << endl;
							while (timeRemaining > 0.0f) {
								currentTime = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
								timeRemaining = rateLimitData.nextExecutionTime - currentTime;
							}
							rateLimitData.nextExecutionTime = 0;
							rateLimitData.msRemain = 0.0f;
							rateLimitData.timeStartedAt = 0.0f;
						}
					}
					rateLimitData.getsRemaining -= 1;
					HttpData returnData;
					if (workload.workloadClass == HttpWorkloadClass::GET) {
						returnData = httpGETObjectData(workload, &rateLimitData);
					}
					else if (workload.workloadClass == HttpWorkloadClass::POST) {
						returnData = httpPOSTObjectData(workload.relativePath, workload.content, &rateLimitData);
					}
					else if (workload.workloadClass == HttpWorkloadClass::PUT) {
						returnData = httpPUTObjectData(workload.relativePath, workload.content, &rateLimitData);
					}
					else if (workload.workloadClass == HttpWorkloadClass::PATCH) {
						returnData = httpPATCHObjectData(workload.relativePath, workload.content, &rateLimitData);
					}
					else if (workload.workloadClass == HttpWorkloadClass::DELETED) {
						returnData = httpDELETEObjectData(workload.relativePath, &rateLimitData, workload.workloadType);
					}
					auto bucketValueIterator = HttpRequestAgent::rateLimitDataBucketValues.find(workload.workloadType);
					if (bucketValueIterator != end(HttpRequestAgent::rateLimitDataBucketValues)) {
						HttpRequestAgent::rateLimitDataBucketValues.erase(workload.workloadType);
					}
					HttpRequestAgent::rateLimitDataBucketValues.insert_or_assign(workload.workloadType, rateLimitData.bucket);
					auto rateLimitIterator = HttpRequestAgent::rateLimitData.find(rateLimitData.bucket);
					if (rateLimitIterator != end(HttpRequestAgent::rateLimitData)) {
						HttpRequestAgent::rateLimitData.erase(rateLimitData.bucket);
					}
					HttpRequestAgent::rateLimitData.insert_or_assign(rateLimitData.bucket, rateLimitData);
					return returnData;
					});
				completeHttpRequest.link_target(&this->workReturnBuffer);
				HttpWorkloadData workload = receive(&this->workSubmissionBuffer);
				send(&completeHttpRequest, workload);
			}
			catch (...) {
				DiscordCoreAPI::rethrowException("HttpRequestAgent::run() Error: ");
			}
			done();
		}

		HttpData httpGETObjectData(HttpWorkloadData workloadData, RateLimitData* pRateLimitData) {
			HttpData getData;
			string connectionPath = this->baseURLInd + workloadData.relativePath;
			winrt::Windows::Foundation::Uri requestUri{ to_hstring(connectionPath) };
			HttpResponseMessage httpResponse;
			httpResponse = getHttpClient.GetAsync(requestUri).get();
			string returnMessage{};
			for (unsigned int x = 0; x < httpResponse.Content().ReadAsBufferAsync().get().Length(); x += 1) {
				returnMessage.push_back(httpResponse.Content().ReadAsBufferAsync().get().data()[x]);
			}
			getData.returnCode = (unsigned int)httpResponse.StatusCode();
			getData.returnMessage = returnMessage;
			for (auto [key, value] : workloadData.headerValuesToCollect) {
				if (httpResponse.Headers().HasKey(to_hstring(key))) {
					workloadData.headerValuesToCollect.insert_or_assign(key, to_string(httpResponse.Headers().TryLookup(to_hstring(key)).value().c_str()));
				}
			}
			json jsonValue;
			if (returnMessage != "") {
				if (returnMessage.find("var ytInitialData = ") != string::npos) {
					string newString = returnMessage.substr(returnMessage.find("var ytInitialData = ") + to_string(L"var ytInitialData = ").length());
					const char* stringSequence = ";</script><script nonce=";
					newString = newString.substr(0, newString.find(stringSequence));
					jsonValue = jsonValue.parse(newString);
				}
				else if ((returnMessage[0] == to_string(L"{")[0] && returnMessage[returnMessage.size() - 1] == to_string(L"}")[0]) || (returnMessage[0] == to_string(L"[")[0] && returnMessage[returnMessage.size() - 1] == to_string(L"]")[0])) {
					try {
						jsonValue = jsonValue.parse(returnMessage);
						
					}
					catch (...) {
						DiscordCoreAPI::rethrowException("HttpRequestAgent::httpGETObjectData() Error: ");
					}
				}
			}
			getData.data = jsonValue;
			if (httpResponse.Headers().HasKey(L"x-ratelimit-reset-after")) {
				pRateLimitData->msRemain = stof(httpResponse.Headers().TryLookup(L"x-ratelimit-reset-after").value().c_str()) * 1000;
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-remaining")) {
				pRateLimitData->getsRemaining = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-remaining").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-bucket")) {
				pRateLimitData->bucket = to_string(httpResponse.Headers().TryLookup(L"x-ratelimit-bucket").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-limit")) {
				pRateLimitData->totalGets = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-limit").value().c_str());
			}
			pRateLimitData->timeStartedAt = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
			if ((int)httpResponse.StatusCode() == 429) {
				cout << "httpGETObjectDataAsync(), We've hit rate limit! Time Remaining: " << to_string(pRateLimitData->msRemain) << endl << endl;
				if (executeByRateLimitData(pRateLimitData)) {
					HttpData returnData;
					return returnData;
				}
				getData = httpGETObjectData(workloadData, pRateLimitData);
			}
			if (returnMessage != "") {
				if (jsonValue.contains("retry-after") && !jsonValue.at("retry-after").is_null()) {
					pRateLimitData->msRemain = jsonValue.at("retry-after") * 1000.0f;
				}
			}
			return getData;
		}

		HttpData httpPUTObjectData(string relativeURL, string content, RateLimitData* pRateLimitData) {
			HttpData putData;
			string connectionPath = this->baseURLInd + relativeURL;
			winrt::Windows::Foundation::Uri requestUri = winrt::Windows::Foundation::Uri(to_hstring(connectionPath.c_str()));
			HttpContentHeaderCollection contentHeaderCollection;
			HttpContentDispositionHeaderValue headerValue(L"payload_json");
			contentHeaderCollection.ContentDisposition(headerValue);
			HttpMediaTypeHeaderValue typeHeaderValue(L"application/json");
			contentHeaderCollection.ContentType(typeHeaderValue);
			HttpStringContent contents(to_hstring(content), UnicodeEncoding::Utf8);
			contents.Headers().ContentDisposition(headerValue);
			contents.Headers().ContentType(typeHeaderValue);
			HttpResponseMessage httpResponse;
			httpResponse = putHttpClient.PutAsync(requestUri, contents).get();
			string returnMessage = to_string(httpResponse.Content().ReadAsStringAsync().get());
			putData.returnCode = (unsigned int)httpResponse.StatusCode();
			putData.returnMessage = returnMessage;
			json jsonValue;
			if (returnMessage != "") {
				try {
					jsonValue = jsonValue.parse(returnMessage);
				}
				catch (...) {
					DiscordCoreAPI::rethrowException("HttpRequestAgent::httpPUTObjectData() Error: ");
				}
			}
			putData.data = jsonValue;
			if (httpResponse.Headers().HasKey(L"x-ratelimit-reset-after")) {
				pRateLimitData->msRemain = stof(httpResponse.Headers().TryLookup(L"x-ratelimit-reset-after").value().c_str()) * 1000;
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-remaining")) {
				pRateLimitData->getsRemaining = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-remaining").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-bucket")) {
				pRateLimitData->bucket = to_string(httpResponse.Headers().TryLookup(L"x-ratelimit-bucket").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-limit")) {
				pRateLimitData->totalGets = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-limit").value().c_str());
			}
			pRateLimitData->timeStartedAt = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
			if ((int)httpResponse.StatusCode() == 429) {
				cout << "httpPUTObjectDataAsync(), We've hit rate limit! Time Remaining: " << to_string(pRateLimitData->msRemain) << endl << endl;
				if (executeByRateLimitData(pRateLimitData)) {
					HttpData returnData;
					return returnData;
				}
				putData = httpPUTObjectData(relativeURL, content, pRateLimitData);
			}
			if (returnMessage != "") {
				if (jsonValue.contains("retry-after") && !jsonValue.at("retry-after").is_null()) {
					pRateLimitData->msRemain = jsonValue.at("retry-after") * 1000.0f;
				}
			}
			return putData;
		}

		HttpData httpPOSTObjectData(string relativeURL, string content, RateLimitData* pRateLimitData) {
			HttpData postData;
			string connectionPath = this->baseURLInd + relativeURL;
			winrt::Windows::Foundation::Uri requestUri = winrt::Windows::Foundation::Uri(to_hstring(connectionPath.c_str()));
			HttpContentHeaderCollection contentHeaderCollection;
			HttpContentDispositionHeaderValue headerValue(L"payload_json");
			contentHeaderCollection.ContentDisposition(headerValue);
			HttpMediaTypeHeaderValue typeHeaderValue(L"application/json");
			contentHeaderCollection.ContentType(typeHeaderValue);
			HttpStringContent contents(to_hstring(content), UnicodeEncoding::Utf8);
			contents.Headers().ContentDisposition(headerValue);
			contents.Headers().ContentType(typeHeaderValue);
			HttpResponseMessage httpResponse;
			httpResponse = postHttpClient.PostAsync(requestUri, contents).get();
			string returnMessage = to_string(httpResponse.Content().ReadAsStringAsync().get());
			postData.returnCode = (unsigned int)httpResponse.StatusCode();
			postData.returnMessage = returnMessage;
			json jsonValue;
			if (returnMessage != "") {
				try {
					jsonValue = jsonValue.parse(returnMessage);
				}
				catch (...) {
					DiscordCoreAPI::rethrowException("HttpRequestAgent::httpPOSTObjectData() Error: ");
				}
			}
			postData.data = jsonValue;
			if (httpResponse.Headers().HasKey(L"x-ratelimit-reset-after")) {
				pRateLimitData->msRemain = stof(httpResponse.Headers().TryLookup(L"x-ratelimit-reset-after").value().c_str()) * 1000;
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-remaining")) {
				pRateLimitData->getsRemaining = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-remaining").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-bucket")) {
				pRateLimitData->bucket = to_string(httpResponse.Headers().TryLookup(L"x-ratelimit-bucket").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-limit")) {
				pRateLimitData->totalGets = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-limit").value().c_str());
			}
			pRateLimitData->timeStartedAt = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
			if ((int)httpResponse.StatusCode() == 429) {
				cout << "httpPOSTObjectDataAsync(), We've hit rate limit! Time Remaining: " << to_string(pRateLimitData->msRemain) << endl << endl;
				if (executeByRateLimitData(pRateLimitData)) {
					HttpData returnData;
					return returnData;
				}
				postData = httpPOSTObjectData(relativeURL, content, pRateLimitData);
			}
			if (returnMessage != "") {
				if (jsonValue.contains("retry-after") && !jsonValue.at("retry-after").is_null()) {
					pRateLimitData->msRemain = jsonValue.at("retry-after") * 1000.0f;
				}
			}
			return postData;
		}

		HttpData httpPATCHObjectData(string relativeURL, string content, RateLimitData* pRateLimitData) {
			HttpData patchData;
			string connectionPath = this->baseURLInd + relativeURL;
			winrt::Windows::Foundation::Uri requestUri = winrt::Windows::Foundation::Uri(to_hstring(connectionPath.c_str()));
			HttpContentDispositionHeaderValue headerValue(L"payload_json");
			HttpMediaTypeHeaderValue typeHeaderValue(L"application/json");
			auto contentHeaderCollection = HttpRequestHeaderCollection(nullptr);
			HttpStringContent contents(to_hstring(content), UnicodeEncoding::Utf8);
			contents.Headers().ContentDisposition(headerValue);
			contents.Headers().ContentType(typeHeaderValue);
			HttpRequestMessage httpRequest;
			httpRequest.Method(HttpMethod::Patch());
			httpRequest.Content(contents);
			httpRequest.RequestUri(requestUri);
			HttpResponseMessage httpResponse;
			HttpCompletionOption completionOption;
			httpResponse = patchHttpClient.SendRequestAsync(httpRequest, completionOption).get();
			string returnMessage = to_string(httpResponse.Content().ReadAsStringAsync().get());
			patchData.returnCode = (unsigned int)httpResponse.StatusCode();
			patchData.returnMessage = returnMessage;
			json jsonValue;
			if (returnMessage != "") {
				try {
					jsonValue = jsonValue.parse(returnMessage);
				}
				catch (...) {
					DiscordCoreAPI::rethrowException("HttpRequestAgent::httpPATCHObjectData() Error: ");
				}
			}
			patchData.data = jsonValue;
			if (httpResponse.Headers().HasKey(L"x-ratelimit-reset-after")) {
				pRateLimitData->msRemain = stof(httpResponse.Headers().TryLookup(L"x-ratelimit-reset-after").value().c_str()) * 1000;
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-remaining")) {
				pRateLimitData->getsRemaining = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-remaining").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-bucket")) {
				pRateLimitData->bucket = to_string(httpResponse.Headers().TryLookup(L"x-ratelimit-bucket").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-limit")) {
				pRateLimitData->totalGets = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-limit").value().c_str());
			}
			pRateLimitData->timeStartedAt = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
			if ((int)httpResponse.StatusCode() == 429) {
				cout << "httpPATCHObjectDataAsync(), We've hit rate limit! Time Remaining: " << to_string(pRateLimitData->msRemain) << endl << endl;
				if (executeByRateLimitData(pRateLimitData)) {
					HttpData returnData;
					return returnData;
				}
				patchData = httpPATCHObjectData(relativeURL, content, pRateLimitData);
			}
			if (returnMessage != "") {
				if (jsonValue.contains("retry-after") && !jsonValue.at("retry-after").is_null()) {
					pRateLimitData->msRemain = jsonValue.at("retry-after") * 1000.0f;
				}
			}
			return patchData;
		}

		HttpData httpDELETEObjectData(string relativeURL, RateLimitData* pRateLimitData, HttpWorkloadType workloadType) {
			HttpData deleteData;
			string connectionPath = this->baseURLInd + relativeURL;
			winrt::Windows::Foundation::Uri requestUri = winrt::Windows::Foundation::Uri(to_hstring(connectionPath.c_str()));
			HttpResponseMessage httpResponse;
			httpResponse = deleteHttpClient.DeleteAsync(requestUri).get();
			string returnMessage = to_string(httpResponse.Content().ReadAsStringAsync().get());
			deleteData.returnCode = (unsigned int)httpResponse.StatusCode();
			deleteData.returnMessage = returnMessage;
			json jsonValue;
			if (returnMessage != "") {
				try {
					jsonValue = jsonValue.parse(returnMessage);
				}
				catch (...) {
					DiscordCoreAPI::rethrowException("HttpRequestAgent::httpDELETEObjectData() Error: ");
				}
			}
			deleteData.data = jsonValue;
			if (httpResponse.Headers().HasKey(L"x-ratelimit-reset-after")) {
				pRateLimitData->msRemain = stof(httpResponse.Headers().TryLookup(L"x-ratelimit-reset-after").value().c_str()) * 1000;
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-remaining")) {
				pRateLimitData->getsRemaining = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-remaining").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-bucket")) {
				pRateLimitData->bucket = to_string(httpResponse.Headers().TryLookup(L"x-ratelimit-bucket").value().c_str());
			}
			if (httpResponse.Headers().HasKey(L"x-ratelimit-limit")) {
				pRateLimitData->totalGets = stoi(httpResponse.Headers().TryLookup(L"x-ratelimit-limit").value().c_str());
			}			
			pRateLimitData->timeStartedAt = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
			if (workloadType == HttpWorkloadType::DELETE_MESSAGE_OLD) {
				pRateLimitData->getsRemaining = 0;
				if (!pRateLimitData->isItMarked) {
					pRateLimitData->msRemainTotal = pRateLimitData->msRemain;
				}
				pRateLimitData->msRemain = pRateLimitData->msRemainTotal / pRateLimitData->totalGets;
				pRateLimitData->isItMarked = true;
			}
			else if (workloadType == HttpWorkloadType::DELETE_MESSAGE) {
				pRateLimitData->getsRemaining = pRateLimitData->getsRemaining - 2;
			}
			if ((int)httpResponse.StatusCode() == 429) {
				cout << "httpDELETEObjectDataAsync(), We've hit rate limit! Time Remaining: " << to_string(pRateLimitData->msRemain) << endl << endl;
				if (executeByRateLimitData(pRateLimitData)) {
					HttpData returnData;
					return returnData;
				}
				deleteData = httpDELETEObjectData(relativeURL, pRateLimitData, workloadType);
			}
			return deleteData;
		}

	};
	map<HttpWorkloadType, string> HttpRequestAgent::rateLimitDataBucketValues{};
	shared_ptr<ThreadContext> HttpRequestAgent::threadContext{ nullptr };
	map<string, RateLimitData> HttpRequestAgent::rateLimitData{};
	string HttpRequestAgent::botToken{ "" };
	string HttpRequestAgent::baseURL{ "" };
}
#endif
