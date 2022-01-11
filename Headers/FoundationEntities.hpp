// FoundationEntities.hpp - Header for all of the Discord/Support API data structures.
// May 12, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include "IndexInitial.hpp"

namespace DiscordCoreInternal {

    using namespace std::chrono;
    using namespace nlohmann;
    using namespace std;

    class BaseSocketAgent;
    class HttpClient;
    class DataParser;
}

namespace DiscordCoreAPI {

    using namespace std::chrono;
    using namespace nlohmann;
    using namespace std;

    class Test;
    struct GetGuildMemberRolesData;
    template<typename returnType>
    class CoRoutine;
    template<>
    class CoRoutine<void>;
    class DiscordCoreClient;
    class VoiceConnection;
    struct GetRolesData;
    class GuildMember;
    class ChannelData;
    class BotUser;

    /// Permission values, for a given Channel, by Role or GuildMember. \brief Permission values, for a given Channel, by Role or GuildMember.
    enum class Permission : int64_t {
        CREATE_INSTANT_INVITE = 0x0000000000000001,///< Create Instant Invite.
        KICK_MEMBERS = 0x0000000000000002,///< Kick Members.
        BAN_MEMBERS = 0x0000000000000004,///< Ban Members.
        ADMINISTRATOR = 0x0000000000000008,///< Administrator.
        MANAGE_CHANNELS = 0x0000000000000010,///< Manage Channels.
        MANAGE_GUILD = 0x0000000000000020,///< Manage Guild.
        ADD_REACTIONS = 0x0000000000000040,///< Add Reactions.
        VIEW_AUDIT_LOG = 0x0000000000000080,///< View Audit Log.
        PRIORITY_SPEAKER = 0x0000000000000100,///< Priority Speaker.
        STREAM = 0x0000000000000200,///< Stream.
        VIEW_CHANNEL = 0x0000000000000400,///< View Channel.
        SEND_MESSAGES = 0x0000000000000800,///< Send Messages.
        SEND_TTS_MESSAGES = 0x0000000000001000,///< Send TTS Messages.
        MANAGE_MESSAGES = 0x0000000000002000,///< Manage Messages.
        EMBED_LINKS = 0x0000000000004000,///< Embed Links.
        ATTACH_FILES = 0x0000000000008000,///< Attach Files.
        READ_MESSAGE_HISTORY = 0x0000000000010000,///< Read Message History.
        MENTION_EVERYONE = 0x0000000000020000,///< Mention Everyone.
        USE_EXTERNAL_EMOJIS = 0x0000000000040000,///< Use External Emoji.
        VIEW_GUILD_INSIGHTS = 0x0000000000080000,///< View Guild Insights.
        CONNECT = 0x0000000000100000,///< Connect.
        SPEAK = 0x0000000000200000,///< Speak.
        MUTE_MEMBERS = 0x0000000000400000,///< Mute Members.
        DEAFEN_MEMBERS = 0x0000000000800000,///< Deafen Members.
        MOVE_MEMBERS = 0x0000000001000000,///< Move Members.
        USE_VAD = 0x0000000002000000,///< Use VAD.
        CHANGE_NICKNAME = 0x0000000004000000,///< Change Nickname.
        MANAGE_NICKNAMES = 0x0000000008000000,///< Manage Nicknames.
        MANAGE_ROLES = 0x0000000010000000,///< Manage Roles.
        MANAGE_WEBHOOKS = 0x0000000020000000,///< Manage Webhooks.
        MANAGE_EMOJIS_AND_STICKERS = 0x0000000040000000,///< Manage Emojis And Stickers.
        USE_APPLICATION_COMMANDS = 0x0000000080000000,///< Use Application Commands.
        REQUEST_TO_SPEAK = 0x0000000100000000,///< Request To Speak.
        MANAGE_EVENTS = 0x0000000200000000,///< Manage Events.
        MANAGE_THREADS = 0x0000000400000000,///< Manage Threads.
        CREATE_PUBLIC_THREADS = 0x0000000800000000,///< Create Public Threads.
        CREATE_PRIVATE_THREADS = 0x0000001000000000,///< Create Private Threads.
        USE_EXTERNAL_STICKERS = 0x0000002000000000,///< Use External Stickers.
        SEND_MESSAGES_IN_THREADS = 0x0000004000000000,///< Send Messages In Threads.
        START_EMBEDDED_ACTIVITIES = 0x0000008000000000,///< Start Embedded Activities.
        MODERATE_MEMBERS = 0x0000010000000000///< Moderate Members.
    };

    struct DiscordCoreAPI_Dll CURLWrapper {

        struct CURLDeleter {
            void operator()(CURL* other) {
                if (other != nullptr) {
                    curl_easy_cleanup(other);
                    other = nullptr;
                }
            }
        };

        CURLWrapper& operator=(CURL* other) {
            this->thePtr.reset(other);
            return *this;
        }

        operator CURL* () {
            return this->thePtr.get();
        }

        CURLWrapper(nullptr_t) {};

        CURLWrapper(CURL* other) {
            this->thePtr.reset(other);
        }

    protected:
        unique_ptr<CURL, CURLDeleter> thePtr{ nullptr , CURLDeleter{} };
    };

    struct DiscordCoreAPI_Dll CURLUWrapper {

        struct CURLUDeleter {
            void operator()(CURLU* other) {
                if (other != nullptr) {
                    curl_url_cleanup(other);
                    other = nullptr;
                }
            }
        };

        CURLUWrapper& operator=(CURLU* other) {
            this->thePtr.reset(other);
            return *this;
        }

        operator CURLU* () {
            return this->thePtr.get();
        }

        CURLUWrapper(nullptr_t) {};

        CURLUWrapper(CURLU* other) {
            this->thePtr.reset(other);
        }

    protected:
        unique_ptr<CURLU, CURLUDeleter> thePtr{ nullptr , CURLUDeleter{} };
    };

    struct DiscordCoreAPI_Dll CURLCharWrapper {

        struct CURLCharDeleter {
            void operator()(char* other) {
                if (other != nullptr) {
                    curl_free(other);
                    other = nullptr;
                }
            }
        };

        CURLCharWrapper& operator=(char* other) {
            this->thePtr.reset(other);
            return *this;
        }

        operator string() {
            string returnValue = this->thePtr.get();
            return returnValue;
        }

        CURLCharWrapper(nullptr_t) {};

        CURLCharWrapper(char* other) {
            this->thePtr.reset(other);
        }

    protected:
        unique_ptr<char, CURLCharDeleter> thePtr{ nullptr, CURLCharDeleter{} };
    };

    bool operator==(CURLCharWrapper& lhs, const  string& rhs);

    template<typename T>
    concept Copyable = copyable<T>;

    /// A messaging block for data-structures. \brief A messaging block for data-structures.
   /// \param objectType The type of object that will be sent over the message block.
    template<Copyable objectType>
    class UnboundedMessageBlock {
    public:

        UnboundedMessageBlock<objectType>& operator=(UnboundedMessageBlock<objectType>&& other) {
            this->theArray = move(other.theArray);
            other.theArray = queue<objectType>{};
            return *this;
        }

        UnboundedMessageBlock(UnboundedMessageBlock<objectType>&& other) {
            *this = move(other);
        }

        UnboundedMessageBlock<objectType>& operator=(const UnboundedMessageBlock<objectType>&) = delete;

        UnboundedMessageBlock(const UnboundedMessageBlock<objectType>&) = delete;

        UnboundedMessageBlock<objectType>& operator=(UnboundedMessageBlock<objectType>&) = delete;

        UnboundedMessageBlock(UnboundedMessageBlock<objectType>&) = delete;

        UnboundedMessageBlock() = default;

        void send(objectType theObject) {
            this->theArray.push(theObject);
        }

        void clearContents() {
            this->theArray = queue<objectType>{};
        }

        bool tryReceive(objectType& theObject) {
            if (this->theArray.size() == 0) {
                return false;
            }
            else {
                theObject = this->theArray.front();
                this->theArray.pop();
                return true;
            }
        }

    protected:

        queue<objectType> theArray{};

    };

    /// A thread-safe messaging block for data-structures. \brief A thread-safe messaging block for data-structures.
    /// \param objectType The type of object that will be sent over the message block.
    template<Copyable objectType>
    class TSUnboundedMessageBlock {
    public:

        TSUnboundedMessageBlock<objectType>& operator=(TSUnboundedMessageBlock<objectType>&& other) noexcept {
            this->theArray = move(other.theArray);
            other.theArray = queue<objectType>{};
            return *this;
        }

        TSUnboundedMessageBlock(TSUnboundedMessageBlock<objectType>&& other) noexcept {
            *this = move(other);
        }

        TSUnboundedMessageBlock<objectType>& operator=(const TSUnboundedMessageBlock<objectType>&) = delete;

        TSUnboundedMessageBlock(const TSUnboundedMessageBlock<objectType>&) = delete;

        TSUnboundedMessageBlock<objectType>& operator=(TSUnboundedMessageBlock<objectType>&) = delete;

        TSUnboundedMessageBlock(TSUnboundedMessageBlock<objectType>&) = delete;

        TSUnboundedMessageBlock() = default;

        void send(objectType theObject) {
            lock_guard<mutex> accessLock{ this->accessMutex };
            this->theArray.push(theObject);
        }

        void clearContents() {
            lock_guard<mutex> accessLock{ this->accessMutex };
            this->theArray = queue<objectType>{};
        }

        bool tryReceive(objectType& theObject) {
            lock_guard<mutex> accessLock{ this->accessMutex };
            if (this->theArray.size() == 0) {
                return false;
            }
            else {
                theObject = this->theArray.front();
                this->theArray.pop();
                return true;
            }
        }

    protected:

        mutex accessMutex{ mutex() };
        queue<objectType> theArray{};

    };

    class DiscordCoreAPI_Dll Time {
    public:

        Time(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t minute, int64_t second) {
            this->second = second;
            this->minute = minute;
            this->month = month;
            this->year = year;
            this->hour = hour;
            this->day = day;
        };

        int64_t getTime() {
            int64_t theValue{};
            for (int64_t x = 1970; x < this->year; x += 1) {
                theValue += this->secondsInJan;
                theValue += this->secondsInFeb;
                theValue += this->secondsInMar;
                theValue += this->secondsInApr;
                theValue += this->secondsInMay;
                theValue += this->secondsInJun;
                theValue += this->secondsInJul;
                theValue += this->secondsInAug;
                theValue += this->secondsInSep;
                theValue += this->secondsInOct;
                theValue += this->secondsInNov;
                theValue += this->secondsInDec;
                if (x % 4 == 0) {
                    theValue += this->secondsPerDay;
                }
            }
            if (this->month > 0) {
                theValue += (this->day - 1) * this->secondsPerDay;
                theValue += this->hour * this->secondsPerHour;
                theValue += this->minute * this->secondsPerMinute;
                theValue += this->second;
            }
            if (this->month > 1) {
                theValue += this->secondsInJan;
            }
            if (this->month > 2) {
                theValue += this->secondsInFeb;
            }
            if (this->month > 3) {
                theValue += this->secondsInMar;
            }
            if (this->month > 4) {
                theValue += this->secondsInApr;
            }
            if (this->month > 5) {
                theValue += this->secondsInMay;
            }
            if (this->month > 6) {
                theValue += this->secondsInJun;
            }
            if (this->month > 7) {
                theValue += this->secondsInJul;
            }
            if (this->month > 8) {
                theValue += this->secondsInAug;
            }
            if (this->month > 9) {
                theValue += this->secondsInSep;
            }
            if (this->month > 10) {
                theValue += this->secondsInOct;
            }
            if (this->month > 11) {
                theValue += this->secondsInNov;
            }
            return theValue;
        }

    protected:
        int64_t year{ 0 };
        int64_t month{ 0 };
        int64_t day{ 0 };
        int64_t hour{ 0 };
        int64_t minute{ 0 };
        int64_t second{ 0 };
        const int64_t secondsInJan{ 31 * 24 * 60 * 60 };
        const int64_t secondsInFeb{ 28 * 24 * 60 * 60 };
        const int64_t secondsInMar{ 31 * 24 * 60 * 60 };
        const int64_t secondsInApr{ 30 * 24 * 60 * 60 };
        const int64_t secondsInMay{ 31 * 24 * 60 * 60 };
        const int64_t secondsInJun{ 30 * 24 * 60 * 60 };
        const int64_t secondsInJul{ 31 * 24 * 60 * 60 };
        const int64_t secondsInAug{ 31 * 24 * 60 * 60 };
        const int64_t secondsInSep{ 30 * 24 * 60 * 60 };
        const int64_t secondsInOct{ 31 * 24 * 60 * 60 };
        const int64_t secondsInNov{ 30 * 24 * 60 * 60 };
        const int64_t secondsInDec{ 31 * 24 * 60 * 60 };
        const int64_t secondsPerMinute{ 60 };
        const int64_t secondsPerHour{ 60 * 60 };
        const int64_t secondsPerDay{ 60 * 60 * 24 };
    };

    template<typename timeType>
    class DiscordCoreAPI_Dll StopWatch {
    public:

        StopWatch() = delete;

        StopWatch(timeType maxNumberOfMsNew) {
            this->maxNumberOfMs = maxNumberOfMsNew.count();
            this->startTime = static_cast<int64_t>(duration_cast<timeType>(steady_clock::now().time_since_epoch()).count());
        }

        int64_t totalTimePassed() {
            int64_t currentTime = static_cast<int64_t>(duration_cast<timeType>(steady_clock::now().time_since_epoch()).count());
            int64_t elapsedTime = currentTime - this->startTime;
            return elapsedTime;
        }

        bool hasTimePassed() {
            int64_t currentTime = static_cast<int64_t>(duration_cast<timeType>(steady_clock::now().time_since_epoch()).count());
            int64_t elapsedTime = currentTime - this->startTime;
            if (elapsedTime >= this->maxNumberOfMs) {
                return true;
            }
            else {
                return false;
            }
        }

        void resetTimer() {
            this->startTime = static_cast<int64_t>(duration_cast<timeType>(steady_clock::now().time_since_epoch()).count());
        }

    protected:

        int64_t maxNumberOfMs{ 0 };
        int64_t startTime{ 0 };

    };

    template <typename T>
    bool waitForTimeToPass(UnboundedMessageBlock<T>& outBuffer, T& argOne, int32_t timeInMsNew) {
        StopWatch<milliseconds> stopWatch{ milliseconds(timeInMsNew) };
        bool doWeBreak{ false };
        while (!outBuffer.tryReceive(argOne)) {
            this_thread::sleep_for(milliseconds(1));
            if (stopWatch.hasTimePassed()) {
                doWeBreak = true;
                break;
            }
        };
        return doWeBreak;
    }

    template <typename T>
    bool waitForTimeToPass(TSUnboundedMessageBlock<T>& outBuffer, T& argOne, int32_t timeInMsNew) {
        StopWatch<milliseconds> stopWatch{ milliseconds(timeInMsNew) };
        bool doWeBreak{ false };
        while (!outBuffer.tryReceive(argOne)) {
            this_thread::sleep_for(milliseconds(1));
            if (stopWatch.hasTimePassed()) {
                doWeBreak = true;
                break;
            }
        };
        return doWeBreak;
    }

    /// Time formatting methods. \brief Time formatting methods.
    enum class TimeFormat {
        LongDate = 'D',		/// "20 April 2021" - Long Date
        LongDateTime = 'F',		/// "Tuesday, 20 April 2021 16:20" - Long Date/Time
        LongTime = 'T',		/// "16:20:30" - Long Time
        ShortDate = 'd',		/// "20/04/2021" - Short Date
        ShortDateTime = 'f',		/// "20 April 2021 16:20" - Short Date/Time
        ShortTime = 't',		/// "16:20" - Short Time
    };

    DiscordCoreAPI_Dll string convertMsToDurationString(int32_t durationInMs);

    DiscordCoreAPI_Dll void reportException(string stackTrace, UnboundedMessageBlock<exception>* sendBuffer = nullptr, bool rethrow = false);

    DiscordCoreAPI_Dll string convertToLowerCase(string stringToConvert);

    DiscordCoreAPI_Dll bool hasTimeElapsed(string timeStamp, int64_t days = 0, int64_t hours = 0, int64_t minutes = 0);

    DiscordCoreAPI_Dll string getTimeAndDate();

    DiscordCoreAPI_Dll string getISO8601TimeStamp(string year, string month, string day, string hour, string minute, string second);

    DiscordCoreAPI_Dll string convertTimeInMsToDateTimeString(int64_t timeInMs, TimeFormat timeFormat);

    DiscordCoreAPI_Dll int64_t convertTimestampToInteger(string timeStamp);

    DiscordCoreAPI_Dll bool nanoSleep(int64_t ns);

    DiscordCoreAPI_Dll void spinLock(int64_t timeInNsToSpinLockFor);

    DiscordCoreAPI_Dll string urlDecode(string inputString);

    DiscordCoreAPI_Dll string urlEncode(string inputString);

    DiscordCoreAPI_Dll string generateX64BaseEncodedKey();

    class DiscordCoreAPI_Dll TimeStamp {
    public:

        TimeStamp(string originalTimeStampNew) {
            this->originalTimeStamp = originalTimeStampNew;
        }

        string getDateTimeStamp(TimeFormat timeFormat) {
            this->timeStampInMs = convertTimestampToInteger(this->originalTimeStamp);
            string newString = convertTimeInMsToDateTimeString(this->timeStampInMs, timeFormat);
            return newString;
        }

        string getOriginalTimeStamp() {
            return this->originalTimeStamp;
        }

    protected:

        string originalTimeStamp{ "" };

        int64_t timeStampInMs{ 0 };

    };

    struct DiscordCoreAPI_Dll OpusEncoderWrapper {

        struct DiscordCoreAPI_Dll OpusEncoderDeleter {
            void operator()(OpusEncoder* other) {
                opus_encoder_destroy(other);
            }
        };

        OpusEncoderWrapper& operator=(OpusEncoder* other) {
            this->thePtr.reset(other);
            return *this;
        }

        operator OpusEncoder* () {
            return this->thePtr.get();
        }

        OpusEncoderWrapper(nullptr_t) {};
    protected:
        unique_ptr<OpusEncoder, OpusEncoderDeleter> thePtr{ nullptr , OpusEncoderDeleter{} };
    };

    /**
    * \addtogroup utilities
    * @{
    */

    /// Permissions class, for representing and manipulating Permission values. \brief Permissions class, for representing and manipulating Permission values.
    class DiscordCoreAPI_Dll Permissions : public string {
    public:

        Permissions& operator=(string& other) {
            if (other.size() == 0) {
                this->push_back('0');
            }
            else {
                for (auto& value : other) {
                    this->push_back(value);
                }
            }
            return *this;
        }

        Permissions(string permsNew) {
            if (permsNew.size() == 0) {
                this->push_back('0');
            }
            else {
                for (auto& value : permsNew) {
                    this->push_back(value);
                }
            }
        }

        operator const char* () {
            return this->data();
        }

        operator string() {
            return *this;
        }

        /// Adds one or more Permissions to the current Permissions value. \brief Adds one or more Permissions to the current Permissions value.
        /// \param permissionsToAdd A vector containing the Permissions you wish to add.
        void addPermissions(vector<Permission> permissionsToAdd);

        /// Removes one or more Permissions from the current Permissions value. \brief Removes one or more Permissions from the current Permissions value.
        /// \param permissionsToRemove A vector containing the Permissions you wish to remove.
        void removePermissions(vector<Permission> permissionsToRemove);

        /// Displays the currently present Permissions in a string, and returns a vector with each of them stored in string format. \brief Displays the currently present Permissions in a string, and returns a vector with each of them stored in string format.
        /// \returns A vector full of strings of the Permissions that are in the input string's value.
        vector<string> displayPermissions();

        /// Returns a string containing ALL of the possible Permissions. \brief Returns a string containing ALL of the possible Permissions.
        /// \returns A string containing all of the possible Permissions.
        static string getAllPermissions();

        /// Returns a string containing the currently held Permissions. \brief Returns a string containing the currently held Permissions.
        /// \returns A string containing the current Permissions.
        string getCurrentPermissionString();

        /// Returns a string containing the currently held Permissions in a given Guild. \brief Returns a string containing the currently held Permissions in a given Guild.
        /// \param guildMember The GuildMember who's Permissions are to be evaluated.
        /// \returns A string containing the current Permissions.
        static string getCurrentGuildPermissions(GuildMember guildMember);

        /// Returns a string containing all of a given User's Permissions for a given Channel. \brief Returns a string containing all of a given User's Permissions for a given Channel.
        /// \param guildMember The GuildMember who's Permissions to analyze.
        /// \param channel The Channel withint which to check for Permissions.
        /// \returns A string containing the final Permission's value for a given Channel.
        static string getCurrentChannelPermissions(GuildMember guildMember, ChannelData channel);

        /// Checks for a given Permission in a chosen Channel, for a specific User. \brief Checks for a given Permission in a chosen Channel, for a specific User.
        /// \param guildMember The GuildMember who to check the Permissions of.
        /// \param channel The Channel within which to check for the Permission's presence.
        /// \param permission A Permission to check the current Channel for.
        /// \returns A bool suggesting the presence of the chosen Permission.
        bool checkForPermission(GuildMember guildMember, ChannelData channel, Permission permission);

    protected:

        static string computeBasePermissions(GuildMember guildMember);

        static string computeOverwrites(string basePermissions, GuildMember guildMember, ChannelData channel);

        static string computePermissions(GuildMember guildMember, ChannelData channel);
    };

    /**@}*/

    /**
    * \addtogroup foundation_entities
    * @{
    */

    /// For ids of DiscordEntities. \brief For ids of DiscordEntities.
    typedef string Snowflake;

    /// Base class for all Discord entities. \brief Base class for all Discord entities.
    class DiscordCoreAPI_Dll DiscordEntity {
    public:
        Snowflake id{ "" };///< The identifier "snowflake" of the given entity.
        /// Converts the snowflake-id into a time and date stamp. \brief Converts the snowflake-id into a time and date stamp.
        /// \returns string A string containing the timestamp.
        string getCreatedAtTimestamp(TimeFormat timeFormat);

        virtual ~DiscordEntity() {};
    };

    /// Role tags data. \brief Role tags data.
    struct DiscordCoreAPI_Dll RoleTagsData {
        string premiumSubscriber{ "" };///< Are they a premium subscriber?
        string integrationId{ "" };///< What is the integration id?
        string botId{ "" };///< What is the bot id?
    };

    /// Data structure representing a single Role. \brief Data structure representing a single Role.
    class DiscordCoreAPI_Dll RoleData : public DiscordEntity {
    public:
        Permissions permissions{ "" }; ///< The Role's base Guild Permissions.
        bool mentionable{ false }; ///< Is ths Role mentionable?
        string unicodeEmoji{ "" };///< Emoji representing the Role.
        int32_t position{ 0 }; ///< Its position amongst the rest of the Guild's roles.
        bool managed{ false }; ///< is it a managed Role?
        RoleTagsData tags{}; ///< Role tags for the Role.
        bool hoist{ false };///< Is it hoisted above the other roles?
        int32_t color{ 0 };///< The Role's color.
        string name{ "" };///< The Role's name.
        string icon{ "" };///< Icon representing the Role.

        virtual ~RoleData() {};
    };

    /// Data structure representing a single User. \brief Data structure representing a single User.
    class DiscordCoreAPI_Dll UserData : public DiscordEntity {
    public:
        string discriminator{ "" }; ///< The # next to their User name.
        bool mfaEnabled{ false };///< MFA enabled?
        int32_t premiumType{ 0 };///< Their premium nitro status.
        int32_t publicFlags{ 0 };///< Public flags.
        bool verified{ false };///< Verified?
        string createdAt{ "" };///< When the User was created.
        string userName{ "" };///< Their username.
        bool system{ false };///< Are they a partof the system?        
        string avatar{ "" };///< Their avatar url.
        string locale{ "" };///< The region they are from/in.
        string email{ "" };///< Their email address.
        int32_t flags{ 0 };///< Flags.
        bool bot{ false };///< Are they a bot?

        virtual ~UserData() {};
    };

    /// Attachment data. \brief Attachment data.
    class DiscordCoreAPI_Dll AttachmentData : public DiscordEntity {
    public:
        string contentType{ "" }; ///< Type of content for the attachment.
        string filename{ "" };///< The file name of the attachment.
        string proxyUrl{ "" };///< The proxy url for the attachment.
        int32_t height{ 0 };///< The height of the attachment.
        int32_t width{ 0 };///< The width of the attachment.
        int32_t size{ 0 };///< The size of the attachment.
        string url{ "" };///< The url for the attachment.

        virtual ~AttachmentData() {};
    };

    /// Sticker format types. \brief Sticker format types.
    enum class StickerFormatType {
        PNG = 1,///< Png.
        APNG = 2,///< Apng.
        LOTTIE = 3///< Lottie
    };

    /// Embed footer data. \brief Embed footer data.
    struct DiscordCoreAPI_Dll EmbedFooterData {
        string proxyIconUrl{ "" };///< Proxy icon url.
        string iconUrl{ "" };///< Icon url.
        string text{ "" };///< Footer text.
    };

    /// Embed image data. \brief Embed image data.
    struct DiscordCoreAPI_Dll EmbedImageData {
        string proxyUrl{ "" };///< Proxy url.
        int32_t height{ 0 };///< Image height.
        int32_t width{ 0 };///< Image width.
        string url{ "" };///< Image url.        
    };

    /// Embed thumbnail data. \brief Embed thumbnail data.
    struct DiscordCoreAPI_Dll EmbedThumbnailData {
        string proxyUrl{ "" };///< Proxy url.
        int32_t height{ 0 };///< Image height.
        int32_t width{ 0 };///< Image width.
        string url{ "" };///< Image url.        
    };

    /// Embed video data. \brief Embed video data.
    struct DiscordCoreAPI_Dll EmbedVideoData {
        string proxyUrl{ "" };///< Proxy url.
        int32_t height{ 0 };///< Image height.
        int32_t width{ 0 };///< Image width.
        string url{ "" };///< Image url.        
    };

    /// Embed provider data. \brief Embed provider data.
    struct DiscordCoreAPI_Dll EmbedProviderData {
        string name{ "" }; ///< Name.
        string url{ "" };///< Url.
    };

    /// Embed author data. \brief Embed author data.
    struct DiscordCoreAPI_Dll EmbedAuthorData {
        string proxyIconUrl{ "" };///< Proxy icon url.
        string iconUrl{ "" };///< Icon url.
        string name{ "" }; ///< Name.
        string url{ "" };///< Url.
    };

    /// Embed field data. \brief Embed field data.
    struct DiscordCoreAPI_Dll EmbedFieldData {
        bool Inline{ false };///< Is the field inline with the rest of them?
        string value{ "" };///< The text on the field.
        string name{ "" };///< The title of the field.
    };

    /// Embed types. \brief Embed types.
    enum class EmbedType {
        Rich = 0,///< Rich.
        Image = 1,///< Image.
        Video = 2,///< Video.
        Gifv = 3,///< Gifv.
        Article = 4,///< Article.
        link = 5///< Link.
    };

    /// Embed data. \brief Embed data.
    class DiscordCoreAPI_Dll EmbedData {
    public:

        string hexColorValue{ "000000" };///< Hex color value of the embed.
        vector<EmbedFieldData> fields{};///< Array of embed fields.
        EmbedThumbnailData thumbnail{};///< Embed thumbnail data.
        EmbedProviderData provider{};///< Embed provider data.
        string description{ "" };///< Description of the embed.
        EmbedFooterData footer{};///< Embed footer data.
        EmbedAuthorData author{};///< Embed author data.
        EmbedImageData image{};///< Embed image data.
        EmbedVideoData video{};///< Embed video data.
        string timestamp{ "" };///< Timestamp to be placed on the embed.
        string title{ "" };///< Title of the embed.
        string type{ "" };///< Type of the embed.
        string url{ "" };///< Url for the embed.

        /// Sets the author's name and avatar for the embed. \brief Sets the author's name and avatar for the embed.
        /// \param authorName The author's name.
        /// \param authorAvatarUrl The url to their avatar.
        /// \returns A pointer to this embed.
        EmbedData* setAuthor(string authorName, string authorAvatarUrl = "") {
            this->author.name = authorName;
            this->author.iconUrl = authorAvatarUrl;
            return this;
        }

        /// Sets the footer's values for the embed. \brief Sets the footer's values for the embed.
        /// \param footerText The footer's text.
        /// \param footerIconUrlText Url to the footer's icon.
        /// \returns A pointer to this embed.
        EmbedData* setFooter(string footerText, string footerIconUrlText = "") {
            this->footer.text = footerText;
            this->footer.iconUrl = footerIconUrlText;
            return this;
        }

        /// Sets the timestamp on the embed. \brief Sets the timestamp on the embed.
        /// \param timeStamp The timestamp to be set.
        /// \returns A pointer to this embed.
        EmbedData* setTimeStamp(string timeStamp) {
            this->timestamp = timeStamp;
            return this;
        }

        /// Adds a field to the embed. \brief Adds a field to the embed.
        /// \param name The title of the embed field.
        /// \param value The contents of the embed field.
        /// \param Inline Is it inline with the rest of the fields on the embed?
        /// \returns A pointer to this embed.
        EmbedData* addField(string name, string value, bool Inline = true) {
            EmbedFieldData embedFieldData;
            embedFieldData.name = name;
            embedFieldData.Inline = Inline;
            embedFieldData.value = value;
            this->fields.push_back(embedFieldData);
            return this;
        }

        /// Sets the description (the main contents) of the embed. \brief Sets the description (the main contents) of the embed.
        /// \param descriptionNew The contents of the description to set.
        /// \returns A pointer to this embed.
        EmbedData* setDescription(string descriptionNew) {
            this->description = descriptionNew;
            return this;
        }

        /// Sets the color of the embed, by applying a hex-color value. \brief Sets the color of the embed, by applying a hex-color value.
        /// \param hexColorValueNew A string containing a hex-number value (Between 0x00 and 0xFFFFFF).
        /// \returns A pointer to this embed.
        EmbedData* setColor(string hexColorValueNew) {
            this->hexColorValue = hexColorValueNew;
            return this;
        }

        /// Sets the thumbnail of the embed. \brief Sets the thumbnail of the embed.
        /// \param thumbnailUrl The url to the thumbnail to be used.
        /// \returns A pointer to this embed.
        EmbedData* setThumbnail(string thumbnailUrl) {
            this->thumbnail.url = thumbnailUrl;
            return this;
        }

        /// Sets the title of the embed. \brief Sets the title of the embed.
        /// \param titleNew A string containing the desired title.
        /// \returns A pointer to this embed.
        EmbedData* setTitle(string titleNew) {
            this->title = titleNew;
            return this;
        }

        /// Sets the image of the embed. \brief Sets the image of the embed.
        /// \param imageUrl The url of the image to be set on the embed.
        /// \returns A pointer to this embed.
        EmbedData* setImage(string imageUrl) {
            this->image.url = imageUrl;
            return this;
        }
    };

    /// Message reference data.\brief Message reference data.
    struct DiscordCoreAPI_Dll MessageReferenceData {
        bool failIfNotExists{ false };///< Fail if the Message doesn't exist?
        string messageId{ "" };///< Id of the Message to reference.
        string channelId{ "" };///< Id of the Channel that the referenced Message was sent in.
        string guildId{ "" };///< Id of the Guild that the referenced Message was sent in.
    };

    /// Permission overwrites types. \brief Permission overwrites types.
    enum class PermissionOverwritesType {
        Role = 0,///< Role.
        User = 1///< User.
    };

    /// A Permission overwrite, for a given Channel. \brief A Permission overwrite, for a given Channel.
    class DiscordCoreAPI_Dll OverWriteData : public DiscordEntity {
    public:
        PermissionOverwritesType type{};   ///< Role or User type.
        string channelId{ "" };///< Channel id for which Channel this overwrite beint64_ts to.
        Permissions allow{ "" };///< Collection of Permissions to allow.
        Permissions deny{ "" };///< Collection of Permissions to deny.

        virtual ~OverWriteData() {};
    };

    /// Channel types. \brief Channel types.
    enum class ChannelType {
        GUILD_TEXT = 0,///< Guild text.
        DM = 1,///< Direct-Message.
        GUILD_VOICE = 2,/// Guild voice.
        GROUP_DM = 3,///< Group direct-Message.
        GUILD_CATEGORY = 4,///< Guild category.
        GUILD_NEWS = 5,///< Guild news.
        GUILD_STORE = 6,///< Guild store.
        GUILD_NEWS_THREAD = 10,///< Guild news Thread.
        GUILD_PUBLIC_THREAD = 11,///< Guild public Thread.
        GUILD_PRIVATE_THREAD = 12,///< Guild private Thread.
        GUILD_STAGE_VOICE = 13///< Guild stage-voice.
    };

    /// Meta data for a Thread type of Channel. \brief Meta data for a Thread type of Channel.
    struct DiscordCoreAPI_Dll ThreadMetadataData {
        int32_t autoArchiveDuration{ 0 }; ///< How int64_t before archiving this Thread.
        TimeStamp archiveTimestamp{ "" }; ///< (Where applicable) the time at which this Thread was archived.
        bool invitable{ false }; ///< The id of the individual who archived this Thread.
        bool archived{ false }; ///< Whether or not this Thread is currently archived.
        bool locked{ false }; ///< Whether or not this Thread is currently locked.
    };

    /// Data for a single member of a Thread. \brief Data for a single member of a Thread.
    class DiscordCoreAPI_Dll ThreadMemberData : public DiscordEntity {
    public:
        TimeStamp joinTimestamp{ "" }; ///< The time at which the member joined this Thread.
        string userId{ "" };    ///< The User's id.
        int32_t flags{ 0 }; ///< Flags.

        virtual ~ThreadMemberData() {};
    };

    /// Thread types. \brief Thread types.
    enum class ThreadType {
        GUILD_NEWS_THREAD = 10,///< Guild news Thread.
        GUILD_PUBLIC_THREAD = 11,///< Guild public Thread.
        GUILD_PRIVATE_THREAD = 12///< Guild private Thread.
    };

    /// Automatic Thread archiving durations. \brief Automatic Thread archiving durations.
    enum class ThreadAutoArchiveDuration :int32_t {
        SHORTEST = 60,///< Shortest.
        SHORT = 1440,///< Short.
        LONG = 4320,///< Long.
        LONGEST = 10080///< Longest.
    };

    /// Data structure representing a single Channel. \brief Data structure representing a single Channel.
    class DiscordCoreAPI_Dll ChannelData : public DiscordEntity {
    public:
        unordered_map<string, OverWriteData> permissionOverwrites{}; ///< Permission overwrites for the given Channel.
        unordered_map<string, UserData> recipients{};  ///< Recipients, in the case of a group DM or DM.
        int32_t defaultAutoArchiveDuration{ 0 };
        ThreadMetadataData threadMetadata{}; ///< Metadata in the case that this Channel is a Thread.
        ChannelType type{ ChannelType::DM };    ///< The type of the Channel.
        TimeStamp lastPinTimestamp{ "" };  ///< Timestamp of the last pinned Message.
        int32_t videoQualityMode{ 0 };  ///< Video quality mode.
        int32_t rateLimitPerUser{ 0 };  ///< Amount of seconds a User has to wait before sending another Message.
        string lastMessageId{ "" }; ///< Id of the last Message.
        string applicationId{ "" }; ///< Application id of the current application.
        ThreadMemberData member{}; ///< Thread member object for the current User, if they have joined the Thread.
        int32_t messageCount{ 0 }; ///< An approximate count of Messages in a Thread, stops counting at 50.
        int32_t memberCount{ 0 };   ///< Count of members active in the Channel.
        string rtcRegion{ "" }; ///< Real-time clock region.
        int32_t userLimit{ 0 }; ///< User limit, in the case of voice channels.
        int32_t position{ 0 };  ///< The position of the Channel, in the Guild's Channel list.
        string parentId{ "" }; ///< Id of the parent Channel, if applicable.
        string guildId{ "" };  ///< Id of the Channel's Guild, if applicable.
        string ownerId{ "" };   ///< Id of the Channel's owner.
        int32_t bitrate{ 0 };   ///< Bitrate of the Channel, if it is a voice Channel.
        string topic{ "" }; ///< The Channel's topic.
        bool nsfw{ false }; ///< Whether or not it is nsfw.
        string name{ "" };  ///< Name of the Channel.
        string icon{ "" };  ///< Icon for the Channel, if applicable.

        virtual ~ChannelData() {};
    };

    /// Voice data for a given GuildMember. \brief Voice data for a given GuildMember.
    struct DiscordCoreAPI_Dll VoiceData {
        TimeStamp requestToSpeakTimestamp{ "" };///< The time at which the User requested to speak.
        bool selfStream{ false };///< Whether this User is streaming using "Go Live".
        bool selfVideo{ false };///< Whether this User's camera is enabled.
        bool selfDeaf{ false };///< Whether this User is locally deafened.
        bool selfMute{ false };///< Whether this User is locally muted.
        string channelId{ "" };///< The Channel id this User is connected to.
        string sessionId{ "" };///< The session id for this voice state.
        bool suppress{ false };///< Whether this User is muted by the current User.
        string guildId{ "" };///< The Guild id this voice state is for.
        string userId{ "" };///< The User id this voice state is for.
        bool deaf{ false };///< Whether this User is deafened by the server.
        bool mute{ false };///< Whether this User is muted by the server.
    };

    /// Data structure representing a single GuildMember. \brief Data structure representing a single GuildMember.
    class DiscordCoreAPI_Dll GuildMemberData : public DiscordEntity {
    public:
        Permissions permissions{ "" };///< Their base-level Permissions in the Guild.
        string premiumSince{ "" };///< If applicable, when they first boosted the server.
        string userMention{ "" };///< What to enter to get them mentioned in a Message.
        vector<string> roles{}; ///< The Guild roles that they have.
        TimeStamp joinedAt{ "" };///< When they joined the Guild.
        bool pending{ false };///< Are they waiting at the entry screen?
        VoiceData voiceData{};///< Info about this GuildMember's voice state.
        string guildId{ "" };///< The current Guild's id.
        bool deaf{ false };///< Are they server deafened?
        bool mute{ false };///< Are they server muted?
        string nick{ "" };///< Their nick/display name.
        UserData user{};///< User data for the current GuildMember.

        virtual ~GuildMemberData() {};
    };

    /// Voice state data. \brief Voice state data.
    struct DiscordCoreAPI_Dll VoiceStateData {
        TimeStamp requestToSpeakTimestamp{ "" };///< The time at which the User requested to speak.
        GuildMemberData member{};///< The Guild member this voice state is for.
        bool selfStream{ false };///< Whether this User is streaming using "Go Live".
        bool selfVideo{ false };///< Whether this User's camera is enabled.
        bool selfDeaf{ false };///< Whether this User is locally deafened.
        bool selfMute{ false };///< Whether this User is locally muted.
        string channelId{ "" };///< The Channel id this User is connected to.
        string sessionId{ "" };///< The session id for this voice state.
        bool suppress{ false };///< Whether this User is muted by the current User.
        string guildId{ "" };///< The Guild id this voice state is for.
        string userId{ "" };///< The User id this voice state is for.
        bool deaf{ false };///< Whether this User is deafened by the server.
        bool mute{ false };///< Whether this User is muted by the server.
        operator VoiceData() {
            VoiceData newData{};
            newData.requestToSpeakTimestamp = this->requestToSpeakTimestamp;
            newData.selfStream = this->selfStream;
            newData.selfVideo = this->selfVideo;
            newData.sessionId = this->sessionId;
            newData.channelId = this->channelId;
            newData.suppress = this->suppress;
            newData.selfDeaf = this->selfDeaf;
            newData.selfMute = this->selfMute;
            newData.guildId = this->guildId;
            newData.userId = this->userId;
            newData.deaf = this->deaf;
            newData.mute = this->mute;
            return newData;
        }
    };

    /// Data representing an active Thread. \brief Data representing an active Thread.
    struct DiscordCoreAPI_Dll ActiveThreadsData {
        vector<ThreadMemberData> members{};
        vector<ChannelData> threads{};
        bool hasMore{ false };
    };

    /// Data representing an archived Thread. \brief Data representing an archived Thread.
    struct DiscordCoreAPI_Dll ArchivedThreadsData : public ActiveThreadsData {};

    /// Application command-option types. \brief Application command-option types.
    enum class ApplicationCommandOptionType {
        SUB_COMMAND = 1,///< Sub-command.
        SUB_COMMAND_GROUP = 2,///< Sub-command group.
        STRING = 3,///< String.
        INTEGER = 4,///< Integer.
        BOOLEAN = 5,///< Boolean.
        USER = 6,///< User.
        CHANNEL = 7,///< Channel.
        ROLE = 8,///< Role.
        MENTIONABLE = 9,///< Mentionable.
        NUMBER = 10///< Number.
    };

    /// Application command permission-types. \brief Application command permission-types.
    enum class ApplicationCommandPermissionType {
        Role = 1,///< Role.
        User = 2///< User.
    };

    /// Permissions data for an ApplicationCommand. \brief Permissions data for an ApplicationCommand.
    class DiscordCoreAPI_Dll ApplicationCommandPermissionData : public DiscordEntity {
    public:
        ApplicationCommandPermissionType type{ ApplicationCommandPermissionType::Role }; ///< The type of Permission.
        bool permission{ false }; ///< Whether the Permission is active or not.

        virtual ~ApplicationCommandPermissionData() {}
    };

    /// Represents the Permissions for accessing an ApplicationCommand from within a Guild. \brief Represents the Permissions for accessing an ApplicationCommand from within a Guild.
    class DiscordCoreAPI_Dll GuildApplicationCommandPermissionsData : public DiscordEntity {
    public:
        vector<ApplicationCommandPermissionData> permissions{};///< The Permissions.
        string applicationId{ "" };///< The application's id.
        string guildId{ "" };///< The Guild's id.

        virtual ~GuildApplicationCommandPermissionsData() {}
    };

    /// Data structure representing a single emoji. \brief Data structure representing a single emoji.
    class DiscordCoreAPI_Dll EmojiData : public DiscordEntity {
    public:
        bool requireColons{ false };///< Require colons to render it?
        vector<RoleData> roles{};///< Roles that are allowed to use this emoji.
        bool available{ true };///< Is it available to be used?
        bool animated{ false };///< Is it animated?
        bool managed{ false };///< Is it managed?
        string name{ "" };///< What is its name?
        UserData user{};///< User that created this emoji.

        virtual ~EmojiData() {};
    };

    /// For updating/modifying a given Channel's properties. \brief For updating/modifying a given Channel's properties.
    struct DiscordCoreAPI_Dll UpdateChannelData {
        unordered_map<string, OverWriteData> permissionOverwrites{};
        int32_t defaultAutoArchiveDuration{ 10080 };
        int32_t videoQualityMode{ 1 };
        int32_t rateLimitPerUser{ 0 };
        int32_t bitrate{ 48000 };
        int32_t userLimit{ 0 };
        string parentId{ "" };
        string rtcRgion{ "" };
        int32_t position{ 0 };
        ChannelType type{};
        string topic{ "" };
        bool nsfw{ false };
        string name{ "" };
    };

    /// Data structure representing a single reaction. \brief /// Data structure representing a single reaction.
    class DiscordCoreAPI_Dll ReactionData : public DiscordEntity {
    public:
        GuildMemberData member{}; ///< The GuildMember who placed the reaction.
        string channelId{ "" }; ///< The id of the Channel where it was placed.
        string messageId{ "" }; ///< The id of the Message upon which it was placed.
        string guildId{ "" };///< The id of the Guild where it was placed.
        string userId{ "" }; ///< The id of the User who placed the reaction.
        int32_t count{ 0 };///< The number of times this particular emoji was placed as a reaction to the given Message.
        EmojiData emoji{}; ///< The emoji that was placed as a reaction.
        bool me{ false }; ///< Whether or not I (The bot) placed it.

        virtual ~ReactionData() {};
    };

    /// Structure representing Voice Region Data. \brief Structure representing Voice Region Data.
    struct DiscordCoreAPI_Dll VoiceRegionData {
        bool deprecated{ false };///< Whether this is a deprecated voice region(avoid switching to these).
        bool optimal{ false };///< True for a single server that is closest to the current User's client.
        bool custom{ false };///< Whether this is a custom voice region(used for events / etc).
        string name{ "" };///< Name of the region.
        string id{ "" };///< Unique ID for the region.
    };

    /// Message activity types. \brief Message activity types.
    enum class MessageActivityType {
        JOIN = 1,///< Join.
        SPECTATE = 2,///< Spectate.
        LISTEN = 3,///< Listen.
        JOIN_REQUEST = 5///< Join-request.
    };

    /// Message activity data. \brief Message activity data.
    struct DiscordCoreAPI_Dll MessageActivityData {
        MessageActivityType type{ MessageActivityType::JOIN };///< Message activity type.
        string partyId{ "" };///< Party id.
    };

    /// Ban data. \brief Ban data.
    struct DiscordCoreAPI_Dll BanData {
        bool failedDueToPerms{ false };///< Failed due to perms?
        string reason{ "" };///< Reason for the ban.
        UserData user{};///< User that was banned.
    };

    /// Team members object data. \brief Team members object data.
    struct DiscordCoreAPI_Dll TeamMembersObjectData {
        vector<Permissions> permissions{};///< Permissions for the team.
        int32_t membershipState{ 0 };///< Current state.
        string teamId{ "" };///< Id of the current team.
        UserData user{};///< User data of the current User.
    };

    /// For updating the current voice state. \brief For updating the current voice state.
    struct DiscordCoreAPI_Dll UpdateVoiceStateData {
        string channelId{ "" };///< Id of the desired voice Channel. Leave blank to disconnect.
        bool selfMute{ false };///< Whether or not we self-mute ourselves.
        bool selfDeaf{ false };///< Whether or not we self-deafen ourselves.
        string guildId{ "" };///< The id of the Guild fo which we would like to establish a voice connection.
    };

    /// Team object data. \brief Team object data.
    class DiscordCoreAPI_Dll TeamObjectData : public DiscordEntity {
    public:
        vector<TeamMembersObjectData> members{};///< Array of team members object data.
        string ownerUserId{ "" };///< User id of the team owner.
        string icon{ "" };///<Icon for the team.

        virtual ~TeamObjectData() {};
    };

    /// Application data. \brief Application data.
    class DiscordCoreAPI_Dll ApplicationData : public DiscordEntity {
    public:
        bool botRequireCodeGrant{ false };///< Does the bot require a code grant?
        string termsOfServiceUrl{ "" };///< Terms of service Url.
        string privacyPolicyUrl{ "" };///< Privacy policy Url.
        vector<string> rpcOrigins{};///< Array of RPC origin strings.
        string primarySkuId{ "" };///< Primary SKU Id.
        string description{ "" };///< Description of the application.
        bool botPublic{ false };///< Is the bot public?
        string coverImage{ "" };///< The cover image.
        string verifyKey{ "" };///< The verification key.
        TeamObjectData team{};///< Team object data.
        string  summary{ "" };///< Summary of the application.
        string guildId{ "" };///< Guild id.
        int32_t flags{ 0 };///< Application flags.
        string slug{ "" };///< Sluhg.
        string name{ "" };///< Application's name.
        string icon{ "" };///< Application's icon.
        UserData owner{};///< Application's owner.

        virtual ~ApplicationData() {};
    };

    /// Authorization info structure. \brief Authorization info structure.
    struct DiscordCoreAPI_Dll AuthorizationInfoData {
        ApplicationData application{};///< Partial application object the current application.
        vector<string> scopes{};///< Array of strings - the scopes the User has authorized the application for.
        string expires{ "" };///< When the access token expires.
        UserData user{};/// The User who has authorized, if the User has authorized with the identify scope.
    };

    /// Account data. \brief Account data.
    class DiscordCoreAPI_Dll AccountData : public DiscordEntity {
    public:
        string name{ "" };///< Name of the account.
        virtual ~AccountData() {}
    };

    /// Guild Widget Data. \brief Guild Widget Data.
    struct DiscordCoreAPI_Dll GuildWidgetData {
        string channelId{ "" };///< The widget Channel id.
        bool enabled{ false };///< Whether the widget is enabled.
    };

    /// Widget style options. \brief Widget style options.
    enum class WidgetStyleOptions {
        Shield = 0,///< Shield
        Banner1 = 1,///< Banner1
        Banner2 = 2,///< Banner2
        Banner3 = 3,///< Banner3
        Banner4 = 4///< Banner4
    };

    /// Guild widget image data. \brief Guild widget image data.
    struct DiscordCoreAPI_Dll GuildWidgetImageData {
        string url{ "" };
    };

    /// Integration data. \brief Integration data.
    class DiscordCoreAPI_Dll IntegrationData : public DiscordEntity {
    public:
        int32_t expireGracePeriod{ 0 };///< How int64_t before the integration expires.
        int32_t subscriberCount{ 0 };///< Number of current subscribers.
        ApplicationData application{};///<Application data.
        bool enableEmoticons{ true };///<Emoticons enabled?
        int32_t expireBehavior{ 0 };///< What to do upon expiry.
        TimeStamp syncedAt{ "" };///< Time it was last synced at.
        bool enabled{ false };///< Enabled?
        bool syncing{ false };///< Is it syncing?
        AccountData account{};///< Account data.
        bool revoked{ false };///< Has it been revoked?
        string roleId{ "" };///< Role Id.
        string name{ "" };///< Name of the integration.
        string type{ "" };///< Type of integration.
        UserData user{};///< User data for the integration.

        virtual ~IntegrationData() {}
    };

    /// Audit log events. \brief Audit log events.
    enum class AuditLogEvent {
        GUILD_UPDATE = 1,
        CHANNEL_CREATE = 10,
        CHANNEL_UPDATE = 11,
        CHANNEL_DELETE = 12,
        CHANNEL_OVERWRITE_CREATE = 13,
        CHANNEL_OVERWRITE_UPDATE = 14,
        CHANNEL_OVERWRITE_DELETE = 15,
        MEMBER_KICK = 20,
        MEMBER_PRUNE = 21,
        MEMBER_BAN_ADD = 22,
        MEMBER_BAN_REMOVE = 23,
        MEMBER_UPDATE = 24,
        MEMBER_ROLE_UPDATE = 25,
        MEMBER_MOVE = 26,
        MEMBER_DISCONNECT = 27,
        BOT_ADD = 28,
        ROLE_CREATE = 30,
        ROLE_UPDATE = 31,
        ROLE_DELETE = 32,
        INVITE_CREATE = 40,
        INVITE_UPDATE = 41,
        INVITE_DELETE = 42,
        WEBHOOK_CREATE = 50,
        WEBHOOK_UPDATE = 51,
        WEBHOOK_DELETE = 52,
        EMOJI_CREATE = 60,
        EMOJI_UPDATE = 61,
        EMOJI_DELETE = 62,
        MESSAGE_DELETE = 72,
        MESSAGE_BULK_DELETE = 73,
        MESSAGE_PIN = 74,
        MESSAGE_UNPIN = 75,
        INTEGRATION_CREATE = 80,
        INTEGRATION_UPDATE = 81,
        INTEGRATION_DELETE = 82,
        STAGE_INSTANCE_CREATE = 83,
        STAGE_INSTANCE_UPDATE = 84,
        STAGE_INSTANCE_DELETE = 85,
        STICKER_CREATE = 90,
        STICKER_UPDATE = 91,
        STICKER_DELETE = 92,
        GUILD_SCHEDULED_EVENT_CREATE = 100,
        GUILD_SCHEDULED_EVENT_UPDATE = 101,
        GUILD_SCHEDULED_EVENT_DELETE = 102,
        THREAD_CREATE = 110,
        THREAD_UPDATE = 111,
        THREAD_DELETE = 112
    };

    /// Audit log entry info data \brief Audit log entry info data.
    class DiscordCoreAPI_Dll AuditLogEntryInfoData : public DiscordEntity {
    public:
        string deleteMemberDays{ "" };///<Number of days for which the member's Messages were deleted.
        string membersRemoved{ "" };///< Number of members that were removed upon a prune.
        string channelId{ "" };///< Channel Id.
        string messageId{ "" };///< Message Id.
        string roleName{ "" };///< Role name.
        string count{ "" };///<Count.
        string type{ "" };///< Type.

        virtual ~AuditLogEntryInfoData() {}
    };

    /// Audit log change data. \brief Audit log change data.
    struct DiscordCoreAPI_Dll AuditLogChangeData {
        string newValueString{ "" };///< New value, if it's a string.
        string oldValueString{ "" };///< Old value, if it's a string.
        bool newValueBool{ false };///< New value, if it's a bool.
        bool oldValueBool{ false };///< Old value, if it's a bool.
        int32_t newValueInt{ 0 };///< New value, if it's an int32_t.
        int32_t oldValueInt{ 0 };///< Old value, if it's an int32_t.
        string key{ "" };///< The key of the audit log change.
    };

    /// Guild prune count data. \brief Guild prune count data.
    struct DiscordCoreAPI_Dll GuildPruneCountData {
        int32_t count{ 0 };
    };

    /// Audit log entry data. \brief Audit log entry data.
    class DiscordCoreAPI_Dll AuditLogEntryData : public DiscordEntity {
    public:
        vector<AuditLogChangeData> changes{};///< Array of audit log change data.
        AuditLogEntryInfoData options{};///< Audit log entry info data.
        string createdTimeStamp{ "" };///< Time at which this entry was created.
        AuditLogEvent actionType{};///< Audit log action type.
        string targetId{ "" };///< Id of the target User.
        string userId{ "" };///< Id of the executing User.
        string reason{ "" };///< The reason that was entered for the given change.

        virtual ~AuditLogEntryData() {}
    };

    /// Party data. \brief Party data.
    class DiscordCoreAPI_Dll PartyData : public DiscordEntity {
    public:
        vector<int32_t> size{ 0, 0 };///< The size of the party.

        virtual ~PartyData() {}
    };

    /// Assets data. \brief Party data.
    struct DiscordCoreAPI_Dll AssetsData {
        string largeImage{ "" };    ///< Keyname of an asset to display.
        string smallImage{ "" };    ///< Keyname of an asset to display.
        string largeText{ "" };     ///< Hover text for the large image.
        string smallText{ "" };     ///< Hover text for the small image.
    };

    /// Secrets data. \brief Secrets data.
    struct DiscordCoreAPI_Dll SecretsData {
        string spectate{ "" }; ///< Unique hash for the given match context.
        string match{ "" };	///< Unique hash for Spectate button.
        string join{ "" };	///< Unique hash for chat invitesand Ask to Join.
    };

    /// Timestamp data. \brief Timestamp data.
    struct DiscordCoreAPI_Dll TimestampData {
        int64_t start{ 0 }; ///< Unix timestamp - Send this to have an "elapsed" timer.
        int64_t end{ 0 };   ///< Unix timestamp - send this to have a "remaining" timer.
    };

    /// Button data. \brief Button data.
    struct DiscordCoreAPI_Dll ButtonData {
        string label{ "" }; ///< Visible label of the button.
        string url{ "" };   ///< Url to display on the button.
    };

    /// Activity types. \brief Activity types.
    enum class ActivityType {
        Game = 0,///< Game.
        Streaming = 1,///< Streaming.
        Listening = 2,///< Listening.
        Watching = 3,///< Watching.
        Custom = 4,///< Custom.
        Competing = 5///< Competing.
    };

    /// Activity data. \brief Activity data.
    struct DiscordCoreAPI_Dll ActivityData {
        TimestampData timestamps{}; ///< Timestamp data.
        string applicationId{ "" }; ///< Application id for the current application.
        int32_t createdAt{ 0 };     ///< Timestamp of when the activity began.
        bool instance{ false }; ///< Whether this activity is an instanced context, like a match.
        SecretsData secrets{}; ///< Secrets data.
        string details{ "" };   ///< Details about the activity.
        ButtonData buttons{};   ///< Button Data.
        ActivityType type{};    ///< Activity data.
        AssetsData assets{};    ///< Assets data.
        int32_t flags{ 0 }; ///< Flags.
        string state{ "" };      ///< The player's current party status
        EmojiData emoji{};  ///< Emoji associated with the activity.
        PartyData party{};  ///< Party data.
        string name{ "" };  ///< Name of the activity.
        string url{ "" };   ///< Url associated with the activity.        
    };

    /// Client status data. \brief Client status data.
    struct DiscordCoreAPI_Dll ClientStatusData {
        string desktop{ "" }; ///< Desktop name.
        string mobile{ "" };///< Mobile name.
        string web{ "" };///< Web link.
    };

    /// Premium tier levels. \brief Premium tier levels.
    enum class PremiumTier {
        NONE = 0,///< None.
        TIER_1 = 1,///< Tier 1.
        TIER_2 = 2,///< Tier 2.
        TIER_3 = 3///< Tier 3.
    };

    /// Default Message notification levels. \brief Default Message notification levels.
    enum class DefaultMessageNotificationLevel {
        ALL_MESSAGES = 0,///< All messages.
        ONLY_MENTIONS = 1///< Only mentions.
    };

    /// Explicit content filter levels. \brief Explicit content filter levels.
    enum class ExplicitContentFilterLevel {
        DISABLED = 0,///< Disabled.
        MEMBERS_WITHOUT_ROLES = 1,///< Members without roles.
        ALL_MEMBERS = 2///< All members.
    };

    /// MFA levels. \brief MFA levels.
    enum class MFALevel {
        NONE = 0,///< None.
        ELEVATED = 1///< Elevated.
    };

    /// Verification levels. \brief /// Verification levels.
    enum class VerificationLevel {
        NONE = 0,///< None.
        LOW = 1,///< Low.
        MEDIUM = 2,///< Medium.
        HIGH = 3,///< High.
        VERY_HIGH = 4///< Very high.
    };

    /// Welcome screen Channel data. \brief Welcome screen Channel data.
    struct DiscordCoreAPI_Dll WelcomeScreenChannelData {
        string description{ "" }; ///< Description of the welcome Channel.
        string channelId{ "" };///< Id of the welcome Channel.
        string emojiName{ "" };///< Emoji name for the Channel.
        string emojiId{ "" };///< Emoji id for the Channel.
    };

    /// Welcome screen data. \brief Welcome screen data.
    struct DiscordCoreAPI_Dll WelcomeScreenData {
        vector<WelcomeScreenChannelData> welcomeChannels{}; ///< Welcome screen Channel data.
        string description{ "" };   ///< Description of the welcome screen.
    };

    /// Presence update data. \brief Presence update data.
    struct DiscordCoreAPI_Dll PresenceUpdateData {
        vector<ActivityData> activities{}; ///< Array of activities.
        ClientStatusData clientStatus{};    ///< Current client status.
        string guildId{ "" };   ///< Guild id for the current presence.
        string status{ "" };    ///< Status of the current presence.
        UserData user{};    ///< User data for the current presence.
    };

    /// Stage instance privacy levels. \brief Stage instance privacy levels.
    enum class StageInstancePrivacyLevel {
        PUBLIC = 1,///< Public.
        GUILD_ONLY = 2///< Guild only.
    };

    /// Stage instance data. \brief Stage instance data.
    class DiscordCoreAPI_Dll StageInstanceData : public DiscordEntity {
    public:
        StageInstancePrivacyLevel privacyLevel{ 0 };  ///< Privacy level of the Channel.
        bool discoverableDisabled{ false }; ///< Is it discoverable?
        string channelId{ "" }; ///< The Channel's id.
        string guildId{ "" };   ///< The Guild id for which the Channel exists in.
        string topic{ "" }; ///< The topic of the StageInstance.

        virtual ~StageInstanceData() {}
    };

    /// Sticker types. \brief Sticker types.
    enum class StickerType {
        STANDARD = 1,///< Standard.
        GUILD = 2///< Guild.
    };

    /// Data representing a single Sticker. \brief Data representing a single Sticker.
    class DiscordCoreAPI_Dll StickerData : public DiscordEntity {
    public:
        StickerFormatType formatType{}; ///< Format type.
        string description{ "" };   ///< Description of the Sticker.
        bool available{ false };    ///< Is it available for use?
        int32_t nsfwLevel{ 0 }; ///< NSFW warning level.
        int32_t sortValue{ 0 };///< Where in the stack of stickers it resides.
        string guildId{ "" };   ///< The Guild id for which the Sticker exists in.
        string packId{ "" };    ///< Pack id of the Sticker.    
        string asset{ "" }; ///< Asset value for the Sticker.
        StickerType type{}; ///< The type of Sticker.
        string name{ "" }; ///< The Sticker's name.
        string tags{ "" };  ///< Tags for the Sticker to use.
        UserData user{};    ///< The User that uploaded the Guild Sticker.

        virtual ~StickerData() {};
    };

    /// Data representing a single Guild preview. \brief Data representing a single Guild preview.
    struct DiscordCoreAPI_Dll GuildPreviewData {
        int32_t approximatePresenceCount{ 0 };
        int32_t approximateMemberCount{ 0 };
        string discoverySplash{ "" };
        vector<EmojiData> emojis{};
        vector<string> features{};
        string description{ "" };
        string splash{ "" };
        string name{ "" };
        string icon{ "" };
        string id{ "" };
    };

    /// Afk timeout durations. \brief Afk timeout durations.
    enum class AfkTimeOutDurations {
        SHORTEST = 60,///< Shortest.
        SHORT = 300,///< Short.
        MEDIUM = 900,///< Medium.
        LONG = 1800,///< Long.
        LONGEST = 3600///< Longest.
    };

    /// Data structure representing a single Guild. \brief Data structure representing a single Guild.
    class DiscordCoreAPI_Dll GuildData : public DiscordEntity {
    public:
        DefaultMessageNotificationLevel defaultMessageNotifications{};///<Default Message notification level.
        unordered_map<string, PresenceUpdateData> presences{}; ///< Array of presences for each GuildMember.
        unordered_map<string, VoiceStateData> voiceStates{};///< Array of Guild-member voice-states.
        ExplicitContentFilterLevel explicitContentFilter{}; ///< Explicit content filtering level, by default.
        unordered_map<string, GuildMemberData> members{};  ///< Array of GuildMembers.
        unordered_map<string, StickerData> stickers{}; ///< Array of Guild stickers.
        unordered_map<string, ChannelData> channels{}; ///< Array of Guild channels.
        unordered_map<string, ChannelData> threads{};  ///< Array of Guild threads.
        vector<StageInstanceData> stageInstances{}; ///< Array of stage instances.
        unordered_map<string, EmojiData> emoji{};  ///< Array of Guild emojis.
        unordered_map<string, RoleData> roles{};   ///< Array of Guild roles.
        int32_t premiumSubscriptionCount{ 0 }; ///< Premium subscription count.
        int32_t approximatePresenceCount{ 0 }; ///< Approximate quantity of presences.
        VerificationLevel verificationLevel{};  ///< Verification level required.
        string publicUpdatesChannelId{ "" }; ///< Id of the public updates Channel.
        int32_t approximateMemberCount{ 0 };    ///< Approximate member count.
        WelcomeScreenData welcomeScreen{};  ///< Welcome screen for the Guild.
        int32_t maxVideoChannelUsers{ 0 };  ///< Maximum quantity of users per video Channel.
        AfkTimeOutDurations afkTimeOut{};    ///< Time for an individual to time out as afk.
        int32_t systemChannelFlags{ 0 };    ///< System Channel flags.
        Permissions permissions{ "" };   ///< Current Permissions for the bot in the Guild.
        string discoverySplash{ "" };   ///< Link to the discovery image's splash.
        string preferredLocale{ "" };   ///< Preferred locale, for voice chat servers.
        string widgetChannelId{ "" };   ///< Channel id for the Guild's widget.
        string systemChannelId{ "" };   ///< Channel id for the Guild's system Channel.
        string rulesChannelId{ "" };    ///< Channel id for the Guild's rules Channel.
        bool widgetEnabled{ false };    ///< Is the widget enabled?        
        string vanityUrlCode{ "" }; ///< Vanity Url code, if applicable.
        string applicationId{ "" }; ///< The current application id.
        int32_t maxPresences{ 0 };  ///< Max number of presences allowed.
        string afkChannelId{ "" };  ///< Channel if of the "afk" Channel,
        vector<string> features{};  ///< List of Guild features.
        bool unavailable{ false };  ///< Is the Guild currently available to the bot?
        PremiumTier premiumTier{};  ///< What is the premium tier?
        TimeStamp joinedAt{ "" };  ///< When the bot joined this Guild.
        string description{ "" };   ///< Description of the Guild.
        int32_t memberCount{ 0 };   ///< Member count.
        int32_t maxMembers{ 0 };    ///< Max quantity of members.
        int32_t nsfwLevel{ 0 }; ///< NSFW warning level.
        string createdAt{ "" }; ///< When was the Guild created?
        string iconHash{ "" };      ///< Url to the Guild's icon.
        string ownerId{ "" };   ///< User id of the Guild's owner.
        string region{ "" };    ///< Region of the world where the Guild's servers are.
        bool owner{ false };    ///< Is the bot the owner?        
        MFALevel mfaLevel{};    ///< MFA level.
        bool large{ false };    ///< Is this Guild "large"?        
        string splash{ "" };    ///< Url to the Guild's splash.
        string banner{ "" };    ///< Url to the Guild's banner.       
        string icon{ "" };  ///< Url to the Guild's icon.
        string name{ "" };  ///< The Guild's name.

        GuildData() {};

        virtual ~GuildData() {};
    };

    /// Guild scheduled event privacy levels. \brief Guild scheduled event privacy levels.
    enum class GuildScheduledEventPrivacyLevel {
        PUBLIC = 1,///< Public.
        GUILD_ONLY = 2///< Guild only.
    };

    /// GuildScheduledEventStatus. \brief GuildScheduledEventStatus.
    enum class GuildScheduledEventStatus {
        SCHEDULED = 1,///< Scheduled.
        ACTIVE = 2,///< Active.
        COMPLETED = 3,///< Completed.
        CANCELED = 4///< Cancelled.
    };

    /// Guild scheduled event entity types. /brief Guild scheduled event entity types.
    enum class GuildScheduledEventEntityType {
        NONE = 0,///< None.
        STAGE_INSTANCE = 1,///< Stage instance.
        VOICE = 2,///< Voice.
        EXTERNAL = 3///< External.
    };

    /// Guild scheduled event entity metadata. \brief Guild scheduled event entity metadata.
    struct DiscordCoreAPI_Dll GuildScheduledEventMetadata {
        string location{ "" };
    };

    /// Data representing a Guild Scheduled Event. \brief Data representing a Guild Scheduled Event.
    class DiscordCoreAPI_Dll GuildScheduledEventData : public DiscordEntity {
    public:
        GuildScheduledEventPrivacyLevel privacyLevel{};///< The privacy level of the scheduled event.
        GuildScheduledEventMetadata entityMetadata{};///< Additional metadata for the Guild scheduled event.
        GuildScheduledEventEntityType entityType{};///< The type of the scheduled event.
        GuildScheduledEventStatus status{};///< The status of the scheduled event.
        string scheduledStartTime{ "" };///< The time the scheduled event will start.
        string scheduledEndTime{ "" };///< The time the scheduled event will end, required if entity_type is EXTERNAL.
        string description{ "" };///< The description of the scheduled event(1 - 1000 characters).
        uint32_t userCount{ 0 };///< The number of users subscribed to the scheduled event.
        string channelId{ "" };///< The Channel id in which the scheduled event will be hosted, or null if scheduled entity type is EXTERNAL.
        string creatorId{ "" };///< The id of the User that created the scheduled event *.
        string entityId{ "" };///< The id of an entity associated with a Guild scheduled event.
        string guildId{ "" };///< The Guild id which the scheduled event belongs to.
        UserData creator{};///< The User that created the scheduled event.
        string name{ "" };///< The name of the scheduled event(1 - 100 characters).

        virtual ~GuildScheduledEventData() {};
    };

    /// Data representing a single GuildScheduledEventUser. \brief Data representing a single GuildScheduledEventUser.
    struct DiscordCoreAPI_Dll GuildScheduledEventUserData {
        string guildScheduledEventId{ "" };///< The scheduled event id which the User subscribed to/
        GuildMemberData member{};///< Guild member data for this User for the Guild which this event belongs to, if any.
        UserData user{};///< User which subscribed to an event.
    };

    /// Invite data. \brief Invite data.
    struct DiscordCoreAPI_Dll InviteData {
        GuildScheduledEventData guildScheduledEvent{};///< Scheduled Guild event.
        int32_t approximatePresenceCount{ 0 };///< Approximate presence count.
        int32_t approximateMemberCount{ 0 };///< Approximate member count.
        ApplicationData targetApplication{};///< Application data.
        StageInstanceData stageInstance{};///< Stage instance data.
        TimeStamp createdAt{ "" };///< Time it was created at.
        TimeStamp expiresAt{ "" };///< When the invite expires.
        int32_t targetType{ 0 };///< Target type.
        bool temporary{ false };///< Is it temporary?
        UserData targetUser{};///< Target User of the invite.
        ChannelData channel{};///< Channel data of the Channel that the invite is for.
        string guildId{ "" };///< The Guild this invite is for.
        int32_t maxUses{ 0 };///< Max number of uses.
        int32_t maxAge{ 0 };///< Maximum age of the invite.
        UserData inviter{};///< The User who created the invite.
        string code{ "" };///< Unique invite code.
        GuildData guild{};///< Guild data of the Channel that the invite is for.
        int32_t uses{ 0 };///< Maximum nunmber of uses.
    };

    /// Represents a Guild Template. \brief Represents a Guild Template.
    struct DiscordCoreAPI_Dll GuildTemplateData {
        GuildData serializedSourceGuild{};///< The Guild snapshot this template contains.
        string sourceGuildId{ "" };///< The ID of the Guild this template is based on.
        string description{ "" };///< The description for the template.
        uint32_t usageCount{ 0 };///< Number of times this template has been used.
        string creatorId{ "" };///< The ID of the User who created the template.
        string createdAt{ "" };///< When this template was created.
        string updatedAt{ "" };///< When this template was last synced to the source Guild.
        bool isDirty{ false };///< Whether the template has unsynced changes.
        UserData creator{};///< The User who created the template.
        string code{ "" };///< The template code(unique ID).
        string name{ "" };///< Template name.
    };

    /// Invite target types. \brief Invite target types.
    enum class InviteTargetTypes {
        STREAM = 1,///< Stream.
        EMBEDDED_APPLICATION = 2///< Embedded application.
    };

    /// WebHook types. \brief WebHook types.
    enum class WebHookType {
        Incoming = 1,///< Incoming.
        Channel_Follower = 2,///< Channel follower.
        Application = 3///< Application.
    };

    /// WebHook data. \brief WebHook data.
    class DiscordCoreAPI_Dll WebHookData : public DiscordEntity {
    public:
        ChannelData sourceChannel{};///< Channel for which the WebHook was issued.
        string applicationId{ "" };///< Application id.
        GuildData sourceGuild{};///< Source Guild id.
        string channelId{ "" };///< Channel id for which the WebHook was issued.
        WebHookType type{ 0 };///< Type of WebHook.
        string guildId{ "" };///< Guild id for which the WebHook was issued.
        string avatar{ "" };///< Avatar of the WebHook.
        string token{ "" };///< Token of the WebHook.
        string name{ "" };///< Name of the WebHook.
        string url{ "" };///< Url of the WebHook.
        UserData user{};///< User which create the WebHook.

        virtual ~WebHookData() {};
    };

    /// Audit log data. \brief Audit log data.
    class DiscordCoreAPI_Dll AuditLogData {
    public:
        auto getAuditLogData(string userIdOfChanger, AuditLogEvent auditLogType) {
            for (auto& value : this->auditLogEntries) {
                if (value.id == userIdOfChanger && value.actionType == auditLogType) {
                    return value;
                }
            }
            return AuditLogEntryData();
        }
        auto getAuditLogData(AuditLogEvent auditLogType, string userIdOfTarget) {
            for (auto& value : this->auditLogEntries) {
                if (value.targetId == userIdOfTarget && value.actionType == auditLogType) {
                    return value;
                }
            }
            return AuditLogEntryData();
        }
        vector<AuditLogEntryData> auditLogEntries{};///< Array of audit log entries.
        vector<IntegrationData> integrations{};///< Integration data.
        vector<WebHookData> webhooks{};///<Array of WebHook data.
        vector<ChannelData> threads{};///< Array of Channel data.
        vector<UserData> users{};///< Array of u
    };

    /// For removing a reaction. \brief For removing a reaction.
    struct DiscordCoreAPI_Dll ReactionRemoveData {
        string channelId{ "" };
        string messageId{ "" };
        string guildId{ "" };
        string userId{ "" };
        EmojiData emoji{};
    };

    /// For storing key-Interaction-related values. \brief For storing key-Interaction-related values.
    struct DiscordCoreAPI_Dll InteractionPackageData {
        string interactionToken{ "" };
        string applicationId{ "" };
        string interactionId{ "" };
    };

    /// For storing key-Message-related values. \brief For storing key-Message-related values.
    struct DiscordCoreAPI_Dll MessagePackageData {
        string channelId{ "" };
        string messageId{ "" };
    };

    /// Data structure representing an ApplicationCommand's option choice. \brief Data structure representing an ApplicationCommand's option choice.
    struct DiscordCoreAPI_Dll ApplicationCommandOptionChoiceData {
        string valueString{ "" }; ///< The value, if the given choice is a string.
        float valueFloat{ 0.0f }; ///< The value, if the given choice is a float.
        int32_t	valueInt{ 0 }; ///< The value, if the given choice is an int32_t.
        string name{ "" };  ///< The name of the current choice.        
    };

    /// Data structure representing an ApplicationCommand's option. \brief Data structure representing an ApplicationCommand's option.
    struct DiscordCoreAPI_Dll ApplicationCommandOptionData {
        vector<ApplicationCommandOptionChoiceData>	choices{}; ///< A vector of possible choices for the current ApplicationCommand option.
        vector<ApplicationCommandOptionData> options{};///< A vector of possible options for the current ApplicationCommand option.
        ApplicationCommandOptionType type{};///< The type of command option.
        vector<ChannelType> channelTypes{};///< Set when the ApplicationCommand option type is set to Channel.
        string description{ "" };///< A description of the current ApplicationCommand option.
        bool required{ false };///< Whether this option is required to enter the command or not.
        string name{ "" };///< Name of the current ApplicationCommand option.
    };

    /// Representing "TypingStart" data. \brief Representing "TypingStart" data.
    struct DiscordCoreAPI_Dll TypingStartData {
        GuildMemberData member{};
        int32_t timestamp{ 0 };
        string channelId{ "" };
        string guildId{ "" };
        string userId{ "" };
    };

    /// YouTube format data. \brief YouTube format data.
    struct DiscordCoreAPI_Dll YouTubeFormat {
        string signatureCipher{ "" };
        string audioSampleRate{ "" };
        int32_t averageBitrate{ 0 };
        int64_t contentLength{ 0 };
        bool doWeGetSaved{ false };
        string audioQuality{ "" };
        string downloadUrl{ "" };
        string signature{ "" };
        string mimeType{ "" };
        int32_t bitrate{ 0 };
        string quality{ "" };
        string codecs{ "" };
        string aitags{ "" };
        int32_t height{ 0 };
        int32_t width{ 0 };
        int32_t itag{ 0 };
        int32_t fps{ 0 };
    };

    /// Application command types. \brief Application command types.
    enum class ApplicationCommandType {
        CHAT_INPUT = 1,///< Chat input.
        USER = 2,///< User.
        MESSAGE = 3///< Message.
    };

    /// User command Interaction data. \brief User command Interaction data.
    struct DiscordCoreAPI_Dll UserCommandInteractionData {
        string targetId{ "" }; ///< The target User's id.
    };

    /// Message command interacction data. \brief Message command interacction data.
    struct DiscordCoreAPI_Dll MessageCommandInteractionData {
        string targetId{ "" }; ///< The target Message's id.
    };

    enum class ComponentType;

    /// Component Interaction data. \brief Component Interaction data.
    struct DiscordCoreAPI_Dll ComponentInteractionData {
        ComponentType componentType{}; ///< The type of component.
        vector<string> values{};///< The values of the components.
        string customId{ "" };///< The custom id of the Interaction entity.
    };

    /// Allowable mentions for a Message. \brief Allowable mentions for a Message.
    struct DiscordCoreAPI_Dll AllowedMentionsData {
        bool repliedUser{ false };///< For replies, whether to mention the author of the Message being replied to (default false).
        vector<string> parse{}; ///< A vector of allowed mention types to parse from the content.
        vector<string> roles{}; ///< Array of role_ids to mention (Max size of 100)
        vector<string> users{}; ///<Array of user_ids to mention (Max size of 100)
    };

    /// Interaction types. \brief Interaction types.
    enum class InteractionType {
        PING = 1,///< Ping.
        APPLICATION_COMMAND = 2,///< Application command.
        MESSAGE_COMPONENT = 3///< Message component.
    };

    /// Represents a single selection from a select-menu. \brief Represents a single selection from a select-menu.
    struct DiscordCoreAPI_Dll SelectOptionData {
        string description{ "" };///< Description of the select-menu-option.
        bool _default{ false };///<Is it the default option?
        string label{ "" };///< A visible label for the select-menu-option.
        string value{ "" };///< A value for identifying the option.
        EmojiData emoji{};///< An optional emoji to put on it.
    };

    /// Component types. \brief Component types.
    enum class ComponentType {
        ActionRow = 1,///< Action row.
        Button = 2,///< Button.
        SelectMenu = 3///< Select-menu.
    };

    /// Button styles. \brief Button styles.
    enum class ButtonStyle {
        Primary = 1,///< Primary.
        Success = 3,///< Success.
        Secondary = 2,///< Secondary.
        Danger = 4,///< Danger.
        Link = 5///< Link.
    };

    /// Represents a single Message-component. \brief Represents a single Message-component.
    struct DiscordCoreAPI_Dll ComponentData {
        vector<SelectOptionData> options{}; ///< A vector of select-options, in the case of this being a select-menu.
        string placeholder{ "" };   ///< Custom placeholder text if nothing is selected, max 100 characters.
        bool disabled{ false }; ///< Whether this component is active.
        string customId{ "" };  ///< A custom id for identifying the component.
        ComponentType type{};   ///< Which type of component?
        ButtonStyle style{};    ///< Which style of button?
        string label{ "" }; ///< What is the visible label?
        int32_t maxValues{ 0 }; ///< Maximum number of selectable options?
        int32_t minValues{ 0 }; ///< Minimum number of required selectable options?
        EmojiData emoji{}; ///< An emoji to put on the component.
        string url{ "" }; ///< An optional Url to place.
    };

    /// Action row data of Message components. \brief Action row data of Message components.
    struct DiscordCoreAPI_Dll ActionRowData {
        vector<ComponentData> components{}; ///< Array of components to make up the action-row.
    };

    /// Interaction callback types. \brief Interaction callback types.
    enum class InteractionCallbackType {
        Pong = 1,///< Pong.
        ChannelMessageWithSource = 4,///< Channel Message with source.
        DeferredChannelMessageWithSource = 5,///< Deferred Channel Message with source.
        DeferredUpdateMessage = 6,///< Deferred update Message.
        UpdateMessage = 7///< Update Message.
    };

    /// Interaction ApplicationCommand callback data. \brief Interaction ApplicationCommand callback data.
    struct DiscordCoreAPI_Dll InteractionApplicationCommandCallbackData {
        AllowedMentionsData allowedMentions{};///< Allowed mentions data.
        vector<ActionRowData> components{};///< Message components.
        vector<EmbedData> embeds{};///< Message embeds.
        string content{ "" };///< Message content.
        bool tts{ false };///< Is it TTS?
        int32_t flags{ 0 };///< Flags.
    };

    /// Interaction response data. \brief Interaction response data.
    struct DiscordCoreAPI_Dll InteractionResponseData {
        InteractionApplicationCommandCallbackData data{}; ///< Interaction ApplicationCommand callback data.
        InteractionCallbackType type{};///< Interaction callback type.
    };

    /// Data structure representing an ApplicationCommand. \brief Data structure representing an ApplicationCommand.
    class DiscordCoreAPI_Dll ApplicationCommandData : public DiscordEntity {
    public:
        vector<ApplicationCommandOptionData> options{};///< A vector of possible options for the current ApplicationCommand.
        bool defaultPermission{ false };///< Whether or not the default Permission in the Guild is to have access to this command or not.
        ApplicationCommandType type{};///< The type of ApplicationCommand.
        string applicationId{ "" };///< The current application id.
        string description{ "" };///< A description of the current ApplicationCommand.
        string guildId{ "" };///< (Where applicable) a Guild id for which to assign this ApplicationCommand to.
        string version{ "" };///< An autoincremented version.
        string name{ "" };///< Name of the current ApplicationCommand.

        virtual ~ApplicationCommandData() {}
    };

    /// Function data for repeated functions to be loaded. \brief Function data for repeated functions to be loaded.
    struct DiscordCoreAPI_Dll RepeatedFunctionData {
        function<void()> function{ nullptr };///< The function pointer to be loaded.
        int32_t intervalInMs{ 0 };  ///< The time interval at which to call the function.
        bool repeated{ false }; ///< Whether or not the function is repeating.
    };

    /// Channel mention data. \brief Channel mention data.
    class DiscordCoreAPI_Dll ChannelMentionData : public DiscordEntity {
    public:
        string guildId{ "" }; ///< The id of the Guild where it took place.
        string name{ "" }; ///< The name of the Channel that was mentioned.
        ChannelType type{}; ///< The type of Channel that was mentioned.

        virtual ~ChannelMentionData() {}
    };

    /// Data for when some Channel pins are updated. \brief Data for when some Channel pins are updated.
    struct DiscordCoreAPI_Dll ChannelPinsUpdateEventData {
        TimeStamp lastPinTimestamp{ "" }; ///< The time of the last pinned Message.
        string channelId{ "" };     ///< The id of the Channel within which the Message was pinned.
        string guildId{ "" };   ///< The id of the Guild within which the Message was pinned.
    };

    /// Data for when threads are synced. \brief Data for when threads are synced.
    struct DiscordCoreAPI_Dll ThreadListSyncData {
        vector<ThreadMemberData> members{}; ///< Array of members that are a part of the Thread.
        vector<ChannelData> threads{};  ///< All active threads in the given channels that the current User can access.
        vector<string> channelIds{};    ///< The parent Channel ids whose threads are being synced. If omitted, then threads were synced for the entire Guild. This array may contain channel_ids that have no active threads as well, so you know to clear that data.
        string guildId{ "" };   ///< The id of the Guild for which the threads are being synced.
    };

    /// Represents a Thread-members-update. \brief Represents a Thread-members-update.
    class DiscordCoreAPI_Dll ThreadMembersUpdateData : public DiscordEntity {
    public:
        vector<ThreadMemberData> addedMembers{}; ///< New members added to the Thread.
        vector<string> removedMemberIds{}; ///< Members who have been removed.
        string guildId{ "" };   ///< Guild id of the Thread.
        int32_t memberCount{ 0 };///< Number of Guild-members in the Thread.

        virtual ~ThreadMembersUpdateData() {}
    };

    /// Message Interaction data. \brief Message Interaction data.
    class DiscordCoreAPI_Dll MessageInteractionData : public DiscordEntity {
    public:
        InteractionType type{};
        string name{ "" };
        UserData user{};

        virtual ~MessageInteractionData() {}
    };

    /// Message types. \brief Message types.
    enum class MessageType {
        DEFAULT = 0,///< Default.
        RECIPIENT_ADD = 1,///< Recipient add.
        RECIPIENT_REMOVE = 2,///< Recipient remove.
        CALL = 3,///< Call.
        CHANNEL_NAME_CHANGE = 4,///< Channel name change.
        CHANNEL_ICON_CHANGE = 5,///< Channel icon change.
        CHANNEL_PINNED_MESSAGE = 6,///< Channel pinned Message.
        GUILD_MEMBER_JOIN = 7,///< Guild memeber join.
        USER_PREMIUM_GUILD_SUBSCRIPTION = 8,///< User premium Guild subscription.
        USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_1 = 9,///< User premium Guild subscription tier 1.
        USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_2 = 10,///< User premium Guild subscription tier 2.
        USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_3 = 11,///< User premium Guild subscription tier 3.
        CHANNEL_FOLLOW_ADD = 12,///< Channel follow add.
        GUILD_DISCOVERY_DISQUALIFIED = 14,///< Guild discovery disqualified.
        GUILD_DISCOVERY_REQUALIFIED = 15,///< Guild discovery requalified.
        GUILD_DISCOVERY_GRACE_PERIOD_INITIAL_WARNING = 16,///< Guild discovery grade period initial warning.
        GUILD_DISCOVERY_GRACE_PERIOD_FINAL_WARNING = 17,///< Guild discovery grade period final warning.
        THREAD_CREATED = 18,///< Thread created.
        REPLY = 19,///< Reply.
        CHAT_INPUT_COMMAND = 20,///< Chat input command.
        THREAD_STARTER_MESSAGE = 21,///< Thread starter Message.
        GUILD_INVITE_REMINDER = 22,///< Guild invite reminder.
        CONTEXT_MENU_COMMAND = 23///< Context menu command.
    };

    /// Message flags. \brief Message flags.
    enum class MessageFlags {
        CROSSPOSTED = 1ull << 0,///< Crossposted.
        IS_CROSSPOST = 1ull << 1,///< Is crosspost.
        SUPPRESS_EMBEDS = 1ull << 2,///< Supress embeds.
        SOURCE_MESSAGE_DELETED = 1ull << 3,///< Source Message deleted.
        URGENT = 1ull << 4,///< Urgent.
        HAS_THREAD = 1ull << 5,///< Has Thread.
        EPHEMERAL = 1ull << 6,///< Ephemeral.
        LOADING = 1ull << 7///< Loading.
    };

    /// Sticker item types. \brief Sticker item types.
    enum class StickerItemType {
        PNG = 1,///< Png.
        APNG = 2,///< Apng.
        LOTTIE = 3///< Lottie.
    };

    /// Message Sticker item data. \brief Message Sticker item data.
    class DiscordCoreAPI_Dll StickerItemData : public DiscordEntity {
    public:
        StickerItemType formatType{}; ///< Message Sticker item type.
        string name{ "" }; ///< The name of the Sticker.

        virtual ~StickerItemData() {}
    };

    /// The core of a Message's data structure. \brief The core of a Message's data structure.
    class DiscordCoreAPI_Dll MessageDataOld : public DiscordEntity {
    public:
        vector<ChannelMentionData> mentionChannels{};///< array of Channel mention data.
        MessageReferenceData messageReference{};///< Message reference data.
        vector<StickerItemData> stickerItems{};///< Array of Message Sticker item data.
        vector<AttachmentData> attachments{};///< Array of attachment data.
        MessageInteractionData interaction{};///< Message Interaction data.
        vector<ActionRowData> components{};///< Array of action row data.
        vector<ReactionData> reactions{};//< Array of reaction data.
        TimeStamp editedTimestamp{ "" };///< The time at which it was edited.
        vector<StickerData> stickers{};///< Array of Message Sticker data.
        MessageActivityData activity{};///< Message activity data.
        ApplicationData application{};///< Application data.
        vector<string> mentionRoles{};///< Vector of "mention roles" ids.
        bool mentionEveryone{ false };///< Does the Message mention everyone?
        vector<UserData> mentions{};///< Array of User data, for individual's that were mentioned.
        vector<EmbedData> embeds{};///< Array of Message embeds.
        string applicationId{ "" };///< Application id.
        TimeStamp timestamp{ "" };///< The timestamp of when the Message was created.
        GuildMemberData member{};///< The author's Guild member data.
        string channelId{ "" };///< The Channel it was sent in.
        string webhookId{ "" };///< WebHook id of the Message, if applicable.
        string guildId{ "" };///< The id of the Guild the Message was sent in.
        bool pinned{ false };///< Is it pinned?
        string content{ "" };///< The Message's content.
        ChannelData thread{};///< The Thread that the Message was sent in, if applicable.
        string nonce{ "" };///< Nonce.
        MessageType type{};///< Message type.
        UserData author{};///< The author's User data.
        bool tts{ false };///< Is it a text-to-speech Message?
        int32_t flags{ 0 };///< Flags.

        virtual ~MessageDataOld() {};

    };

    /// Data structure representing a single Message. \brief Data structure representing a single Message.
    class DiscordCoreAPI_Dll MessageData : public MessageDataOld {
    public:
        MessageDataOld referencedMessage{}; ///< The referenced Message, to reply to.
        string requesterId{ "" }; ///< Requester's id, of who sent this Message.

        virtual ~MessageData() {};
    };

    /// Resolved data. \brief Resolved data.
    struct DiscordCoreAPI_Dll ResolvedData {
        unordered_map<string, GuildMemberData> members{}; ///< Map full of GuildMemeberData.
        unordered_map<string, MessageData> messages{};///< Map full of MessageData.
        unordered_map<string, ChannelData> channels{};///< Map full of ChannelData.
        unordered_map<string, UserData> users{};///< Map full of UserData.
        unordered_map<string, RoleData> roles{};///< Map full of RoleData.
    };

    struct DiscordCoreAPI_Dll ApplicationCommandInteractionDataOption;

    DiscordCoreAPI_Dll vector<ApplicationCommandInteractionDataOption> convertAppCommandInteractionDataOptions(vector<ApplicationCommandInteractionDataOption> originalOptions);

    /// Represents a Sticker pack. \brief Represents a Sticker pack.
    struct DiscordCoreAPI_Dll StickerPackData {
        vector<StickerData> stickers{};///< Array of Sticker objects	the stickers in the pack.
        string coverStickerId{ "" };///< Id of a Sticker in the pack which is shown as the pack's icon.
        string bannerAssetId{ "" };///< Id of the Sticker pack's banner image.
        string description{ "" };///< Description of the Sticker pack.
        string skuId{ "" };///< Id of the pack's SKU.
        string name{ "" };///< Name of the Sticker pack.
        string Id{ "" };///< Id of the Sticker pack.
    };

    /// Connection visibility types. \brief Connection visibility types.
    enum class ConnectionVisibilityTypes {
        None = 0,///< None.
        Everyone = 1///< Everyone.
    };

    /// Represents a single User Connection. \brief Represents a single User Connection.
    struct DiscordCoreAPI_Dll ConnectionData {
        vector<IntegrationData> integrations{};///< An array of partial server integrations.
        ConnectionVisibilityTypes visibility{};///< Visibility of this connection.
        bool showActivity{ false };///< Whether activities related to this connection will be shown in presence updates.
        bool friendSync{ false };///< Whether friend sync is enabled for this connection.
        bool verified{ false };///< Whether the connection is verified.
        bool revoked{ false };///< Whether the connection is revoked.
        string name{ "" };///< The username of the connection account.
        string type{ "" };///< The service of the connection(twitch, youtube)
        string id{ "" };///< Id of the connection account.
    };

    /// ApplicationCommand Interaction data option. \brief ApplicationCommand Interaction data option.
    struct DiscordCoreAPI_Dll ApplicationCommandInteractionDataOption {
        vector<ApplicationCommandInteractionDataOption> options{};///< ApplicationCommand Interaction data options.
        ApplicationCommandOptionType type{};///< The type of ApplicationCommand options.
        string valueString{ "" };///< The value if it's a string.
        bool valueBool{ false };///< the value if it's a bool.
        string name{ "" };///< The name of the current option.
        int32_t valueInt{ 0 };///< The value if it's an int32_t.
        bool focused{ false };///< 	True if this option is the currently focused option for autocomplete
    };

    /// ApplicationCommand Interaction data.
    class DiscordCoreAPI_Dll ApplicationCommandInteractionData : public DiscordEntity {
    public:
        vector<ApplicationCommandInteractionDataOption> options{};///< ApplicationCommand Interaction data options.
        ApplicationCommandType type{}; ///< The type of ApplicationCommand.
        ResolvedData resolved{};///< Resolved data.
        string name{ "" };///< The name of the command.

        virtual ~ApplicationCommandInteractionData() {}
    };

    /// Interaction data data. \brief Interaction data data.
    struct DiscordCoreAPI_Dll InteractionDataData {
        ApplicationCommandInteractionData applicationCommanddata{};///< ApplicationCommand Interaction data.
        MessageCommandInteractionData messageInteractionData{};///< Message command Interaction data.
        UserCommandInteractionData userInteractionData{};///< User command Interaction data.
        ComponentInteractionData componentData{};///< Component Interaction data.
    };

    /// Interaction data. \brief Interaction data.
    class DiscordCoreAPI_Dll InteractionData : public DiscordEntity {
    public:
        InteractionDataData data{};///< The Interaction's data.
        string applicationId{ "" };///< The application's id.
        GuildMemberData member{};///< The data of the Guild member who sent the Interaction, if applicable.
        string requesterId{ "" };///< The id of the sender of the Interaction.
        InteractionType type{};///< The type of Interaction.
        string channelId{ "" };///< The Channel the Interaction was sent in.
        MessageData message{};///< The Message that the Interaction came through on, if applicable.
        int32_t version{ 0 };///< The Interaction version.
        string guildId{ "" };///< The Guild id of the Guild it was sent in.
        string token{ "" };///< The Interaction token.
        UserData user{};///< The User data of the sender of the Interaction.
        json rawData{};///< The Interaction's raw data.
        InteractionData() {};
        InteractionData(string requesterId) {
            this->requesterId = requesterId;
        }

        virtual ~InteractionData() {}
    };

    /// Data from the SessionStart info. \brief Data from the SessionStart info.
    struct DiscordCoreAPI_Dll SessionStartData {
        uint32_t maxConcurrency{ 0 };///< The number of identify requests allowed per 5 seconds.
        uint32_t resetAfter{ 0 };///< The number of milliseconds after which the limit resets.
        uint32_t remaining{ 0 };///< The remaining number of session starts the current User is allowed.
        uint32_t total{ 0 };///< The total number of session starts the current User is allowed.
    };

    /// Data from the GetGatewatBot endpoint. \brief Data from the GetGatewatBot endpoint.
    struct DiscordCoreAPI_Dll GatewayBotData {
        SessionStartData sessionStartLimit{};///< Information on the current session start limit.
        uint32_t shards{ 0 };///< The recommended number of shards to use when connecting.
        string url{ "" };///< The WSS Url that can be used for connecting to the gateway.       
    };

    /// Input event response types. \brief Input event response types.
    enum class InputEventResponseType {
        Unset = 0,///< Unset.
        DeferredResponse = 1,///< Deferred response.
        InteractionResponse = 2,///< Interaction response.
        InteractionResponseEdit = 3,///< Interaction response edit.
        EphemeralInteractionResponse = 4,///< Ephemeral Interaction response.
        RegularMessage = 5,///< Regular Message.
        RegularMessageEdit = 6,///< Regular Message edit.
        FollowUpMessage = 7,///< Follow-up Message.
        FollowUpMessageEdit = 8,///< Follow-up Message edit.
        EphemeralFollowUpMessage = 9,///< Ephemeral follow-up Message.
        SendDM = 10///< Send Dm.
    };

    /// Input event types. \brief Input event types.
    enum class InputEventType {
        UNSET = 0,///< Unset.
        SLASH_COMMAND_INTERACTION = 1,///< Slash-command Interaction.
        BUTTON_INTERACTION = 2,///< Button Interaction.
        REGULAR_MESSAGE = 3,///< Regular Message.
        SELECT_MENU_INTERACTION = 4,///< Select-menu Interaction.
        MESSAGE_COMMAND_INTERACTION = 5,///< Message-command Interaction.
        USER_COMMAND_INTERACTION = 6///< User-command Interaction.
    };

    /// Data representing a Guild Emoji Update event. \brief Data representing a Guild Emoji Update event.
    struct DiscordCoreAPI_Dll GuildEmojisUpdateEventData {
        vector<EmojiData> emojis{};
        string guildId{ "" };
    };

    /// Data representing a Guild Sticker Update event. \brief Data representing a Guild Stickers Update event.
    struct DiscordCoreAPI_Dll GuildStickersUpdateEventData {
        vector<StickerData> stickers{};
        string guildId{ "" };
    };

    struct DiscordCoreAPI_Dll GuildMembersChunkEventData {
        vector<PresenceUpdateData> presences{};
        vector<GuildMemberData> members{};
        vector<string> notFound{};
        int32_t chunkIndex{ 0 };
        int32_t chunkCount{ 0 };
        string guildId{ "" };
        string nonce{ "" };
    };

    /// Data representing an input-event, which is any Message or Interaction that is coming into the bot as an input. \brief Data representing an input-event, which is any Message or Interaction that is coming into the bot as an input.
    class DiscordCoreAPI_Dll InputEventData {
    public:

        friend class  DiscordCoreInternal::BaseSocketAgent;
        friend struct RecurseThroughMessagePagesData;
        friend struct OnInteractionCreationData;
        friend class  RespondToInputEventData;
        friend struct BaseFunctionArguments;
        friend class  DiscordCoreClient;
        friend class  EventHandler;
        friend struct CommandData;
        friend class  InputEvents;

        InputEventResponseType inputEventResponseType{}; ///< The type of event response that is represented by this structure.
        InputEventType eventType{}; ///< The type of input-event that is represented by this structure.

        InputEventData() {};

        InputEventData(MessageData messageData, InteractionData interactionData, InputEventType eventType) {
            this->interactionData = interactionData;
            this->messageData = messageData;
            this->eventType = eventType;
            if (this->messageData.channelId == "") {
                this->messageData.channelId = this->interactionData.channelId;
            }
            else {
                this->interactionData.channelId = this->messageData.channelId;
            }
            if (this->interactionData.id == "") {
                this->interactionData.id = this->messageData.interaction.id;
            }
            if (this->messageData.guildId == "") {
                this->messageData.guildId = this->interactionData.guildId;
            }
            else {
                this->interactionData.guildId = this->messageData.guildId;
            }
            if (this->messageData.id == "") {
                this->messageData.id = this->interactionData.message.id;
            }
            else {
                this->interactionData.message.id = this->messageData.id;
            }
            if (this->messageData.member.user.id == "") {
                this->messageData.member = this->interactionData.member;
            }
            else {
                this->interactionData.member = this->messageData.member;
            }
            if (this->interactionData.user.id != "") {
                this->messageData.author = this->interactionData.user;
            }
            else {
                this->interactionData.user = this->messageData.author;
            }
            if (this->messageData.requesterId != "") {
                this->requesterId = this->messageData.requesterId;
            }
            else {
                this->requesterId = this->interactionData.requesterId;
            }
        }

        /// Returns the username of the last User to trigger this input-event. \brief Returns the username of the last User to trigger this input-event.
        /// \returns A string containing the User name.
        string getUserName() {
            if (this->messageData.author.userName == "" && this->interactionData.member.user.userName != "") {
                return this->interactionData.member.user.userName;
            }
            else if (this->interactionData.member.user.userName == "" && this->interactionData.user.userName != "") {
                return this->interactionData.user.userName;
            }
            else if (this->messageData.author.userName != "") {
                return this->messageData.author.userName;
            }
            else {
                return string();
            }
        }

        /// Gets the avatar Url of the last User to trigger this input-event. \brief Gets the avatar Url of the last User to trigger this input-event.
        /// \returns A string containing the avatar Url.
        string getAvatarUrl() {
            if (this->messageData.author.avatar == "" && this->interactionData.member.user.avatar != "") {
                return this->interactionData.member.user.avatar;
            }
            else if (this->interactionData.member.user.avatar == "" && this->interactionData.user.avatar != "") {
                return this->interactionData.user.avatar;
            }
            else if (this->messageData.author.avatar != "") {
                return this->messageData.author.avatar;
            }
            else {
                return string();
            }
        }

        /// Returns the Message embeds that are on the Message, if applicable. \brief Returns the Message embeds that are on the Message, if applicable.
        /// \returns A vector containing the EmbedData.
        vector<EmbedData> getEmbeds() {
            if (this->interactionData.message.embeds.size() > 0) {
                if (this->interactionData.message.embeds.at(0).description != "") {
                    return this->interactionData.message.embeds;
                }
                else if (this->interactionData.message.embeds.at(0).fields.size() > 0) {
                    if (this->interactionData.message.embeds.at(0).fields.at(0).value != "") {
                        return this->interactionData.message.embeds;
                    }
                }
            }
            else {
                return this->messageData.embeds;
            }
            return vector<EmbedData>();
        }

        /// Returns the Message components that are on the Message, if applicable. \brief Returns the Message components that are on the Message, if applicable.
        /// \returns A vector containing ActionRowData.
        vector<ActionRowData> getComponents() {
            if (this->interactionData.message.components.size() > 0) {
                if (this->interactionData.message.components.at(0).components.at(0).customId != "") {
                    return this->interactionData.message.components;
                }
            }
            else {
                return this->messageData.components;
            }
            return vector<ActionRowData>();
        }

        /// Returns the User id of the last requester of this input-event. \brief Returns the User id of the last requester of this input-event.
        /// \returns A string containing the author's id.
        string getAuthorId() {
            if (this->messageData.author.id == "") {
                return this->interactionData.user.id;
            }
            else if (this->messageData.author.id != "") {
                return this->messageData.author.id;
            }
            else {
                return string();
            }
        }

        /// Returns the Interaction id, if appplicable, of this input-event. \brief Returns the Interaction id, if appplicable, of this input-event.
        /// \returns A string containing the Interaction id.
        string getInteractionId() {
            if (this->interactionData.id == "") {
                return this->messageData.interaction.id;
            }
            else if (this->interactionData.id != "") {
                return this->interactionData.id;
            }
            else if (this->interactionData.message.interaction.id != "") {
                return this->interactionData.message.interaction.id;
            }
            else {
                return string();
            }
        }

        /// Returns the application id. \brief Returns the application id.
        /// \returns A string containing the application id.
        string getApplicationId() {
            if (this->interactionData.applicationId == "") {
                return this->messageData.application.id;
            }
            else if (this->interactionData.applicationId != "") {
                return this->interactionData.applicationId;
            }
            else {
                return string();
            }
        }

        /// Returns the Channel id of this input-event. \brief Returns the Channel id of this input-event.
        /// \returns A string containing the Channel id.
        string getChannelId() {
            if (this->interactionData.channelId == "") {
                return this->messageData.channelId;
            }
            else if (this->interactionData.channelId != "") {
                return this->interactionData.channelId;
            }
            else {
                return string();
            }
        }

        /// Returns the Interaction token, if applicable, of this input-event. \brief Returns the Interaction token, if applicable, of this input-event.
        /// \returns A string containing the Interaction token.
        string getInteractionToken() {
            if (this->interactionData.token != "") {
                return this->interactionData.token;
            }
            else {
                return string();
            }
        }

        /// Returns the Guild id, of this input-event. \brief Returns the Guild id, of this input-event.
        /// \returns A string containing the Guild id.
        string getGuildId() {
            if (this->messageData.guildId == "") {
                return this->interactionData.guildId;
            }
            else if (this->messageData.guildId != "") {
                return this->messageData.guildId;
            }
            else {
                return string();
            }
        }

        /// Returns the Message content, if applicable, of this input-event. \brief Returns the Message content, if applicable, of this input-event.
        /// \returns A string containing the Message's content.
        string getMessageContent() {
            if (this->interactionData.message.content != "") {
                return this->interactionData.message.content;
            }
            else {
                return this->messageData.content;
            }
        }

        /// Returns the Message id, if applicable, of this input-event. \brief Returns the Message id, if applicable, of this input-event.
        /// \returns A string containing the Message id.
        string getMessageId() {
            if (this->messageData.id != "") {
                return this->messageData.id;

            }
            else if (this->interactionData.message.id != "") {
                return this->interactionData.message.id;
            }
            else {
                return string();
            }
        }

        /// Returns the Interaction data, if applicable, of this input-event. \brief Returns the InteractionData, if applicable, of this input-event.
        /// \returns An InteractionData structure.
        InteractionData getInteractionData() {
            return this->interactionData;
        }

        /// Returns the Message data, if applicable, of this input-event. \brief Returns the Message data, if applicable, of this input-event.
        /// \returns A MessageData structure.
        MessageData getMessageData() {
            if (this->messageData.id != "") {
                return this->messageData;

            }
            else {
                return this->interactionData.message;
            }
        }

        /// Returns the requester id, of this input-event. \brief Returns the requester id, of this input-event.
        /// \returns A string containing the requester's id.
        string getRequesterId() {
            return this->requesterId;
        };

        virtual ~InputEventData() {};

    protected:
        InteractionData interactionData{};
        MessageData messageData{};
        string requesterId{ "" };
    };

    /// \brief Data for responding to an input-event.
    class DiscordCoreAPI_Dll RespondToInputEventData {
    public:

        friend class  CreateEphemeralInteractionResponseData;
        friend class  DiscordCoreInternal::BaseSocketAgent;
        friend class  CreateDeferredInteractionResponseData;
        friend class  CreateEphemeralFollowUpMessageData;
        friend class  CreateInteractionResponseData;
        friend class  EditInteractionResponseData;
        friend class  DeferComponentResponseData;
        friend class  CreateFollowUpMessageData;
        friend class  EditFollowUpMessageData;
        friend class  CreateMessageData;
        friend class  EditMessageData;
        friend class  InputEvents;
        friend class  SendDMData;

        InputEventResponseType type{}; ///< The type of response to make.

        RespondToInputEventData(string channelIdNew) {
            this->channelId = channelIdNew;
            this->type = InputEventResponseType::RegularMessage;
        }

        RespondToInputEventData(InteractionData dataPackage) {
            this->type = InputEventResponseType::DeferredResponse;
            this->applicationId = dataPackage.applicationId;
            this->requesterId = dataPackage.requesterId;
            this->interactionToken = dataPackage.token;
            this->messageId = dataPackage.message.id;
            this->channelId = dataPackage.channelId;
            this->interactionId = dataPackage.id;
        };

        RespondToInputEventData(InputEventData dataPackage) {
            this->interactionToken = dataPackage.getInteractionToken();
            this->applicationId = dataPackage.getApplicationId();
            this->interactionId = dataPackage.getInteractionId();
            this->requesterId = dataPackage.getRequesterId();
            this->channelId = dataPackage.getChannelId();
            this->messageId = dataPackage.getMessageId();
        }

        /// Adds a button to the response Message. \brief Adds a button to the response Message.
        /// \param disabled Whether the button is active or not.
        /// \param customId A custom id to give for identifying the button.
        /// \param buttonLabel A visible label for the button.
        /// \param buttonStyle The style of the button.
        /// \param emojiName An emoji name, if desired.        
        /// \param emojiId An emoji id, if desired.
        /// \param url A url, if applicable.
        void addButton(bool disabled, string customId, string buttonLabel, ButtonStyle buttonStyle, string emojiName = "", string emojiId = "", string url = "") {
            if (this->components.size() == 0) {
                ActionRowData actionRowData;
                this->components.push_back(actionRowData);
            }
            if (this->components.size() < 5) {
                if (this->components.at(this->components.size() - 1).components.size() < 5) {
                    ComponentData component;
                    component.type = ComponentType::Button;
                    component.emoji.name = emojiName;
                    component.label = buttonLabel;
                    component.style = buttonStyle;
                    component.customId = customId;
                    component.disabled = disabled;
                    component.emoji.id = emojiId;
                    component.url = url;
                    this->components.at(this->components.size() - 1).components.push_back(component);
                }
                else if (this->components.at(this->components.size() - 1).components.size() == 5) {
                    ActionRowData actionRowData;
                    this->components.push_back(actionRowData);
                }
            }
        }

        /// Adds a select-menu to the response Message. \brief Adds a select-menu to the response Message.
        /// \param disabled Whether the select-menu is active or not.
        /// \param customId A custom id to give for identifying the select-menu.
        /// \param options A vector of select-menu-options to offer.
        /// \param placeholder Custom placeholder text if nothing is selected, max 100 characters.
        /// \param maxValues Maximum number of selections that are possible.
        /// \param minValues Minimum required number of selections that are required.
        void addSelectMenu(bool disabled, string customId, vector<SelectOptionData> options, string placeholder, int32_t maxValues, int32_t minValues) {
            if (this->components.size() == 0) {
                ActionRowData actionRowData;
                this->components.push_back(actionRowData);
            }
            if (this->components.size() < 5) {
                if (this->components.at(this->components.size() - 1).components.size() < 5) {
                    ComponentData componentData;
                    componentData.type = ComponentType::SelectMenu;
                    componentData.placeholder = placeholder;
                    componentData.maxValues = maxValues;
                    componentData.minValues = minValues;
                    componentData.disabled = disabled;
                    componentData.customId = customId;
                    componentData.options = options;
                    this->components.at(this->components.size() - 1).components.push_back(componentData);
                }
                else if (this->components.at(this->components.size() - 1).components.size() == 5) {
                    ActionRowData actionRowData;
                    this->components.push_back(actionRowData);
                }

            }
        }

        /// For setting the allowable mentions in a response. \brief For setting the allowable mentions in a response.
        /// \param dataPackage An AllowedMentionsData structure.
        void addAllowedMentions(AllowedMentionsData dataPackage) {
            this->allowedMentions = dataPackage;
        }

        /// For setting the components in a response. \brief For setting the components in a response. 
        /// \param dataPackage An ActionRowData structure.
        void addComponentRow(ActionRowData dataPackage) {
            this->components.push_back(dataPackage);
        }

        /// For setting the embeds in a response. \brief For setting the embeds in a response.
        /// \param dataPackage An EmbedData structure.
        void addMessageEmbed(EmbedData dataPackage) {
            this->embeds.push_back(dataPackage);
        }

        /// For setting the Message content in a response. \brief For setting the Message content in a response.
        /// \param dataPackage A string, containing the content.
        void addContent(string dataPackage) {
            this->content = dataPackage;
        }

        /// For setting the tts status of a response. \brief For setting the tts status of a response.
        /// \param enabledTTs A bool.
        void setTTSStatus(bool enabledTTs) {
            this->tts = enabledTTs;
        }

        /// For setting the direct-Message User target of a response. \brief For setting the direct-Message User target of a response.
        /// \param targetUserIdNew A string, containging the target User's id.
        void setTargetUserID(string targetUserIdNew) {
            this->targetUserId = targetUserIdNew;
        }

    protected:

        AllowedMentionsData allowedMentions{};
        vector<ActionRowData> components{};
        string interactionToken{ "" };
        vector<EmbedData> embeds{};
        string interactionId{ "" };
        string applicationId{ "" };
        string targetUserId{ "" };
        string requesterId{ "" };
        string channelId{ "" };
        string messageId{ "" };
        string content{ "" };
        int32_t flags{ 0 };
        bool tts{ false };
    };

    struct DiscordCoreAPI_Dll CommandData {
    public:

        vector<string> optionsArgs{};
        InputEventData eventData{};
        string commandName{ "" };

        CommandData() = default;

        CommandData(InputEventData inputEventData) {
            this->eventData = inputEventData;
            if (inputEventData.interactionData.data.applicationCommanddata.name != "") {
                this->commandName = inputEventData.interactionData.data.applicationCommanddata.name;
            }
            if (inputEventData.interactionData.data.messageInteractionData.targetId != "") {
                this->optionsArgs.push_back(inputEventData.interactionData.data.messageInteractionData.targetId);
            }
            else if (inputEventData.interactionData.data.userInteractionData.targetId != "") {
                this->optionsArgs.push_back(inputEventData.interactionData.data.userInteractionData.targetId);
            }
        }
    };

    /// For selecting the caching style of the library. \brief For selecting the caching style of the library.
    struct DiscordCoreAPI_Dll CacheOptions {
        bool cacheGuildMembers{ false };///< Do we cache GuildMembers?
        bool cacheChannels{ false };///< Do we cache Channels?
        bool cacheGuilds{ false };///< Do we cache Guilds?
        bool cacheRoles{ false };///< Do we cache Roles?
        bool cacheUsers{ false };///< Do we cache Users?
    };

    /// Guild application command permissions data. \brief /// Guild application command permissions data.
    class DiscordCoreAPI_Dll GuildApplicationCommandPermissionData : public DiscordEntity {
    public:
        vector<ApplicationCommandPermissionData> permissions{};
        string applicationId{ "" };
        string guildId{ "" };

        virtual ~GuildApplicationCommandPermissionData() {}
    };

    /**@}*/


    /// Song types. \brief Song types.
    enum class SongType {
        YouTube = 0,///< YouTube.
        SoundCloud = 1///< SoundCloud.
    };

    /// Represents a download Url. \brief Represents a download Url.
    struct DiscordCoreAPI_Dll DownloadUrl {
        int32_t contentSize{ 0 };
        string urlPath{ "" };
    };

    /**
    * \addtogroup voice_connection
    * @{
    */

    /// Represents a single frame of raw audio data. \brief Represents a single frame of raw audio data.
    struct DiscordCoreAPI_Dll RawFrameData {
        vector<uint8_t> data{};///< The audio data.
        int32_t sampleCount{ -1 };///< The number of samples per this frame.
        RawFrameData& operator=(RawFrameData&& other) noexcept {
            if (this != &other) {
                this->sampleCount = other.sampleCount;
                other.sampleCount = -1;
                this->data = move(other.data);
                other.data = vector<uint8_t>{};
            }
            return *this;
        }
        RawFrameData(RawFrameData&& other) noexcept {
            *this = move(other);
        }
        RawFrameData& operator=(const RawFrameData& other) {
            this->sampleCount = other.sampleCount;
            this->data = other.data;
            return *this;
        }
        RawFrameData(const RawFrameData& other) {
            *this = other;
        }
        RawFrameData() {};
    };

    /// Represents a single frame of encoded audio data. \brief Represents a single frame of encoded audio data.
    struct DiscordCoreAPI_Dll EncodedFrameData {
        vector<uint8_t> data{};///< The audio data.
        int32_t sampleCount{ -1 };///< The number of samples per this frame.
        EncodedFrameData& operator=(EncodedFrameData&& other) noexcept {
            if (this != &other) {
                this->sampleCount = other.sampleCount;
                other.sampleCount = -1;
                this->data = move(other.data);
                other.data = vector<uint8_t>{};
            }
            return *this;
        }
        EncodedFrameData(EncodedFrameData&& other) noexcept {
            *this = move(other);
        }
        EncodedFrameData& operator=(const EncodedFrameData& other) {
            this->sampleCount = other.sampleCount;
            this->data = other.data;
            return *this;
        }
        EncodedFrameData(const EncodedFrameData& other) {
            *this = other;
        }
        EncodedFrameData() {}
    };

    /// Audio frame types. \brief Audio frame types.
    enum class AudioFrameType {
        Unset = 0,///< Unset.
        Encoded = 1,///< Encoded.
        RawPCM = 2,///< Raw PCM.
        Cancel = 3///< Cancel.
    };

    /// Represents a single frame of audio data. \brief Represents a single frame of audio data.
    struct DiscordCoreAPI_Dll AudioFrameData {
        AudioFrameType type{ AudioFrameType::Unset };///< The type of audio frame.
        EncodedFrameData encodedFrameData{};///< To be filled if it's already encoded.
        RawFrameData rawFrameData{};///< To be filled if it's raw audio data.
        AudioFrameData& operator=(AudioFrameData&& other) noexcept {
            if (this != &other) {
                this->encodedFrameData = move(other.encodedFrameData);
                other.encodedFrameData = EncodedFrameData{};
                this->rawFrameData = move(other.rawFrameData);
                other.rawFrameData = RawFrameData{};
                this->type = other.type;
                other.type = AudioFrameType::Unset;
            }
            return *this;
        }
        AudioFrameData(AudioFrameData&& other) noexcept {
            *this = move(other);
        }
        AudioFrameData& operator=(const AudioFrameData& other) {
            this->encodedFrameData = other.encodedFrameData;
            this->rawFrameData = other.rawFrameData;
            this->type = other.type;
            return *this;
        }
        AudioFrameData(const AudioFrameData& other) {
            *this = other;
        }
        AudioFrameData() {};
    };

    class DiscordCoreAPI_Dll SoundCloudSong;
    class DiscordCoreAPI_Dll YouTubeSong;

    /// A song from the various platforms. \brief A song from the various platforms.
    struct DiscordCoreAPI_Dll Song {
    public:

        friend class  DiscordCoreInternal::DataParser;
        friend class  SoundCloudRequestBuilder;
        friend class  YouTubeRequestBuilder;
        friend class  DatabaseManagerAgent;
        friend class  SoundCloudSong;
        friend class  SoundCloudAPI;
        friend class  YouTubeSong;
        friend class  YouTubeAPI;
        friend class  SongAPI;

        SongType type{ SongType::SoundCloud };///< The type of song.

        string secondDownloadUrl{ "" };
        string firstDownloadUrl{ "" };
        string html5PlayerFile{ "" };
        string addedByUserName{ "" };///< The User name of the individual who added this Song to the playlist.
        string addedByUserId{ "" };///< The User id of the individual who added this Song to the playlist.
        string thumbnailUrl{ "" };///< The Url of the thumbnail image of this Song.
        string html5Player{ "" };
        string description{ "" };///< A description of the Song.
        string songTitle{ "" };///< The title of the Song.
        string duration{ "" };///< The duration of the Song.
        string viewUrl{ "" };///< The url for listening to this Song through a browser.

        virtual ~Song() {};

        operator SoundCloudSong();

        operator YouTubeSong();

    protected:

        vector<DownloadUrl> finalDownloadUrls{};
        string trackAuthorization{ "" };
        string playerResponse{ "" };
        bool doWeGetSaved{ false };
        int32_t contentLength{ 0 };
        YouTubeFormat format{};
        string songId{ "" };

    };

    /// Song completion event data. \brief Song completion event data.
    struct DiscordCoreAPI_Dll SongCompletionEventData {
        VoiceConnection* voiceConnection{}; ///< A pointer to the current VoiceConnection.
        bool wasItAFail{ false };///< Is this a replay? (Did a track recently fail to play?)
        Song previousSong{};///< The previously played Song.
    };

    /// Playlist of songs and other variables. \brief Playlist of songs and other variables.
    struct DiscordCoreAPI_Dll Playlist {
    public:
        bool isLoopSongEnabled{ false };///< Is looping of Songs currently enabled?
        bool isLoopAllEnabled{ false };///< Is looping of the entire Playlist currently enabled?
        vector<Song> songQueue{};///< The list of Songs that are stored to be played.
        Song currentSong{};///< The current Song that is playing.
    };

    /**@}*/

    class DiscordCoreAPI_Dll YouTubeSong : public Song {
    public:

        operator Song();

    };

    class DiscordCoreAPI_Dll SoundCloudSong : public Song {
    public:

        operator Song();

    };

    /**
    * \addtogroup utilities
    * @{
    */

    /// A cache for storing objects of any kind. \brief A cache for storing objects of any kind.
    /// \param keyType The type to use for the storage keys.
    /// \param storageType The type of item to be stored.
    template<typename keyType, typename storageType>
    class ObjectCache {
    public:

        friend class  Guilds;

        ObjectCache<keyType, storageType>& operator=(ObjectCache<keyType, storageType>&& other) {
            if (this != &other) {
                this->cache = move(other.cache);
                other.cache = ObjectCache<keyType, storageType>{};
            }
            return *this;
        }

        ObjectCache(ObjectCache<keyType, storageType>&& other) {
            *this = move(other);
        }

        ObjectCache<keyType, storageType>& operator=(ObjectCache<keyType, storageType>&) = delete;

        ObjectCache(ObjectCache<keyType, storageType>&) = delete;

        ObjectCache() = default;

        auto end() {
            return this->cache.end();
        }

        auto begin() {
            return this->cache.begin();
        }

        /// Returns a value at a chosen value-id. \brief Returns a value at a chosen value-id.
        /// \param valueId The chosen item's key.
        /// \returns storageType The typed item that is stored.
        storageType& at(keyType valueId) {
            return ref(this->cache.at(valueId));
        }

        /// Checks if an item exists at a chosen item-id. \brief Checks if an item exists at a chosen item-id.
        /// \param valueId The chosen item's key.
        /// \returns bool Whether or not the item is present at the given key.
        bool contains(keyType valueId) {
            return this->cache.contains(valueId);
        }

        /// Erases an item at a chosen item-id. \brief Erases an item at a chosen item-id.
        /// \param valueId The chosen item's key.
        void erase(keyType valueId) {
            if (this->cache.contains(valueId)) {
                this->cache.erase(valueId);
            }
        }

        /// Stores an item in the cache. \brief Stores an item in the cache.
        /// \param valueId The item's id to store it at.
        /// \param storageValue The item to store in the object-cache.
        void insert_or_assign(keyType valueId, storageType storageValue) {
            this->cache.insert_or_assign(move(valueId), move(storageValue));
        }

    protected:

        ObjectCache<keyType, storageType> cache{};
    };

    /// A Thread-safe cache for storing objects of any kind. \brief A Thread-safe cache for storing objects of any kind.
    /// \param keyType The type to use for the storage keys.
    /// \param storageType The type of item to be stored.
    template<typename keyType, typename storageType>
    class TSObjectCache {
    public:

        friend class  Guilds;

        TSObjectCache<keyType, storageType>& operator=(TSObjectCache<keyType, storageType>&& other) {
            if (this != &other) {
                this->cache = move(other.cache);
                other.cache = unordered_map<keyType, storageType>{};
            }
            return *this;
        }

        TSObjectCache(TSObjectCache<keyType, storageType>&& other) {
            *this = move(other);
        }

        TSObjectCache<keyType, storageType>& operator=(TSObjectCache<keyType, storageType>&) = delete;

        TSObjectCache(TSObjectCache<keyType, storageType>&) = delete;

        TSObjectCache() = default;

        auto end() {
            lock_guard<mutex> accessLock{ this->accessMutex };
            return this->cache.end();
        }

        auto begin() {
            lock_guard<mutex> accessLock{ this->accessMutex };
            return this->cache.begin();
        }

        /// Returns a value at a chosen value-id. \brief Returns a value at a chosen value-id.
        /// \param valueId The chosen item's key.
        /// \returns storageType The typed item that is stored.
        storageType& at(keyType valueId) {
            lock_guard<mutex> accessLock{ this->accessMutex };
            return ref(this->cache.at(valueId));
        }

        /// Checks if an item exists at a chosen item-id. \brief Checks if an item exists at a chosen item-id.
        /// \param valueId The chosen item's key.
        /// \returns bool Whether or not the item is present at the given key.
        bool contains(keyType valueId) {
            lock_guard<mutex> accessLock{ this->accessMutex };
            return this->cache.contains(valueId);
        }

        /// Erases an item at a chosen item-id. \brief Erases an item at a chosen item-id.
        /// \param valueId The chosen item's key.
        void erase(keyType valueId) {
            lock_guard<mutex> accessLock{ this->accessMutex };
            if (this->cache.contains(valueId)) {
                this->cache.erase(valueId);
            }
        }

        /// Stores an item in the cache. \brief Stores an item in the cache.
        /// \param valueId The item's id to store it at.
        /// \param storageValue The item to store in the object-cache.
        void insert_or_assign(keyType valueId, storageType storageValue) {
            lock_guard<mutex> accessLock{ *this->accessMutex };
            this->cache.insert_or_assign(move(valueId), move(storageValue));
        }

    protected:

        unordered_map<keyType, storageType> cache{};

        mutex accessMutex{};
    };

    /// A Thread-safe cache for storing objects of any kind - it can have multiple items stored using the same key. \brief A Thread-safe cache for storing objects of any kind - it can have multiple items stored using the same key.
    /// \param storageType The type of item to be stored.
    template<typename keyType, typename storageType>
    class ObjectMultiCache {
    public:

        friend class  Guilds;

        ObjectMultiCache<keyType, storageType>& operator=(ObjectMultiCache<keyType, storageType>&& other) {
            if (this != &other) {
                this->accessMutex = other.accessMutex;
                other.accessMutex = unique_ptr<mutex>{};
                this->cache = move(other.cache);
                other.cache = multimap<keyType, storageType>{};
            }
            return *this;
        }

        ObjectMultiCache(ObjectMultiCache<keyType, storageType>&& other) {
            *this = move(other);
        }

        ObjectMultiCache<keyType, storageType>& operator=(ObjectMultiCache<keyType, storageType>&) = delete;

        ObjectMultiCache(ObjectMultiCache<keyType, storageType>&) = delete;

        ObjectMultiCache() = default;

        auto end() {
            lock_guard<mutex> endLock{ this->accessMutex };
            return this->cache.end();
        }

        auto begin() {
            lock_guard<mutex> beginLock{ this->accessMutex };
            return this->cache.begin();
        }

        /// Returns a value at a chosen value-id. \brief Returns a value at a chosen value-id.
        /// \param valueId The chosen item's key.
        /// \returns storageType The typed item that is stored.
        storageType& at(keyType valueId) {
            lock_guard<mutex> returnLock{ this->accessMutex, defer_lock };
            return ref(this->cache.extract(valueId));
        }

        /// Checks if an item exists at a chosen item-id. \brief Checks if an item exists at a chosen item-id.
        /// \param valueId The chosen item's key.
        /// \returns bool Whether or not the item is present at the given key.
        bool contains(keyType valueId) {
            lock_guard<mutex> containsLock{ this->accessMutex };
            return this->cache.contains(valueId);
        }

        /// Erases an item at a chosen item-id. \brief Erases an item at a chosen item-id.
        /// \param valueId The chosen item's key.
        void erase(keyType valueId) {
            lock_guard<mutex> eraseLock{ this->accessMutex };
            if (this->cache.contains(valueId)) {
                this->cache.erase(valueId);
            }
        }

        /// Stores an item in the cache. \brief Stores an item in the cache.
        /// \param valueId The item's id to store it at.
        /// \param storageValue The item to store in the object-cache.
        void insert_or_assign(keyType valueId, storageType storageValue) {
            lock_guard<mutex> storeLock{ *this->accessMutex };
            this->cache.insert_or_assign(move(valueId), move(storageValue));
        }

    protected:

        mutex accessMutex{};

        multimap<keyType, storageType> cache{};
    };

    struct DiscordCoreAPI_Dll RecurseThroughMessagePagesData {
        InputEventData inputEventData{};
        uint32_t currentPageIndex{};
        string buttonId{};
    };

    DiscordCoreAPI_Dll RecurseThroughMessagePagesData recurseThroughMessagePages(string userID, unique_ptr<InputEventData> originalEvent, uint32_t currentPageIndex, vector<EmbedData> messageEmbeds, bool deleteAfter, uint32_t waitForMaxMs, bool returnResult = false);
    /**@}*/
};

namespace  DiscordCoreInternal {

    enum class HttpWorkloadClass {
        GET = 0,
        PUT = 1,
        POST = 2,
        PATCH = 3,
        DELETE = 4
    };

    enum class HttpWorkloadType {
        UNSET = 0,
        GET_GLOBAL_APPLICATION_COMMANDS = 1,
        POST_GLOBAL_APPLICATION_COMMAND = 2,
        GET_GLOBAL_APPLICATION_COMMAND = 3,
        PATCH_GLOBAL_APPLICATION_COMMAND = 4,
        DELETE_GLOBAL_APPLICATION_COMMAND = 5,
        BULK_PUT_GLOBAL_APPLICATION_COMMANDS = 6,
        GET_GUILD_APPLICATION_COMMANDS = 7,
        POST_GUILD_APPLICATION_COMMAND = 8,
        GET_GUILD_APPLICATION_COMMAND = 9,
        PATCH_GUILD_APPLICATION_COMMAND = 10,
        DELETE_GUILD_APPLICATION_COMMAND = 11,
        BULK_PUT_GUILD_APPLICATION_COMMANDS = 12,
        GET_GUILD_APPLICATION_COMMANDS_PERMISSIONS = 13,
        GET_GUILD_APPLICATION_COMMAND_PERMISSIONS = 14,
        PUT_GUILD_APPLICATION_COMMAND_PERMISSIONS = 15,
        BATCH_PUT_GUILD_APPLICATION_COMMAND_PERMISSIONS = 16,
        POST_INTERACTION_RESPONSE = 17,
        GET_INTERACTION_RESPONSE = 18,
        PATCH_INTERACTION_RESPONSE = 19,
        DELETE_INTERACTION_RESPONSE = 20,
        POST_FOLLOWUP_MESSAGE = 21,
        GET_FOLLOWUP_MESSAGE = 22,
        PATCH_FOLLOWUP_MESSAGE = 23,
        DELETE_FOLLOWUP_MESSAGE = 24,
        GET_GUILD_AUDIT_LOGS = 25,
        GET_CHANNEL = 26,
        PATCH_CHANNEL = 27,
        DELETE_CHANNEL = 28,
        GET_MESSAGES = 29,
        GET_MESSAGE = 30,
        POST_MESSAGE = 31,
        CROSSPOST_MESSAGE = 32,
        PUT_REACTION = 33,
        DELETE_OWN_REACTION = 34,
        DELETE_USER_REACTION = 35,
        GET_REACTIONS = 36,
        DELETE_ALL_REACTIONS = 37,
        DELETE_REACTIONS_BY_EMOJI = 38,
        PATCH_MESSAGE = 39,
        DELETE_MESSAGE = 40,
        DELETE_MESSAGE_OLD = 41,
        BULK_DELETE_MESSAGES = 42,
        PUT_CHANNEL_PERMISSION_OVERWRITES = 43,
        GET_CHANNEL_INVITES = 44,
        POST_CHANNEL_INVITE = 45,
        DELETE_CHANNEL_PERMISSION_OVERWRITES = 46,
        POST_FOLLOW_NEWS_CHANNEL = 47,
        POST_TRIGGER_TYPING_INDICATOR = 48,
        GET_PINNED_MESSAGES = 49,
        PUT_PIN_MESSAGE = 50,
        DELETE_PIN_MESSAGE = 51,
        PUT_RECIPIENT_TO_GROUP_DM = 52,
        DELETE_RECIPIENT_FROM_GROUP_DM = 53,
        POST_THREAD_WITH_MESSAGE = 54,
        POST_THREAD_WITHOUT_MESSAGE = 55,
        PUT_SELF_IN_THREAD = 56,
        PUT_THREAD_MEMBER = 57,
        DELETE_SELF_FROM_THREAD = 58,
        DELETE_THREAD_MEMBER = 59,
        GET_THREAD_MEMBER = 60,
        GET_THREAD_MEMBERS = 61,
        GET_ACTIVE_THREADS = 62,
        GET_PUBLIC_ARCHIVED_THREADS = 63,
        GET_PRIVATE_ARCHIVED_THREADS = 64,
        GET_JOINED_PRIVATE_ARCHIVED_THREADS = 65,
        GET_EMOJI_LIST = 66,
        GET_GUILD_EMOJI = 67,
        POST_GUILD_EMOJI = 68,
        PATCH_GUILD_EMOJI = 69,
        DELETE_GUILD_EMOJI = 70,
        POST_GUILD = 71,
        GET_GUILD = 72,
        GET_GUILD_PREVIEW = 73,
        PATCH_GUILD = 74,
        DELETE_GUILD = 75,
        GET_GUILD_CHANNELS = 76,
        POST_GUILD_CHANNEL = 77,
        PATCH_GUILD_CHANNEL_POSITIONS = 78,
        GET_GUILD_ACTIVE_THREADS = 79,
        GET_GUILD_MEMBER = 80,
        GET_GUILD_MEMBERS = 81,
        GET_SEARCH_GUILD_MEMBERS = 82,
        PUT_GUILD_MEMBER = 83,
        PATCH_GUILD_MEMBER = 84,
        PATCH_CURRENT_GUILD_MEMBER = 85,
        PUT_GUILD_MEMBER_ROLE = 86,
        DELETE_GUILD_MEMBER_ROLE = 87,
        DELETE_GUILD_MEMBER = 88,
        GET_GUILD_BANS = 89,
        GET_GUILD_BAN = 90,
        PUT_GUILD_BAN = 91,
        DELETE_GUILD_BAN = 92,
        GET_GUILD_ROLES = 93,
        POST_GUILD_ROLE = 94,
        PATCH_GUILD_ROLE_POSITIONS = 95,
        PATCH_GUILD_ROLE = 96,
        DELETE_GUILD_ROLE = 97,
        GET_GUILD_PRUNE_COUNT = 98,
        POST_GUILD_PRUNE = 99,
        GET_GUILD_VOICE_REGIONS = 100,
        GET_GUILD_INVITES = 101,
        GET_GUILD_INTEGRATIONS = 102,
        DELETE_GUILD_INTEGRATION = 103,
        GET_GUILD_WIDGET_SETTINGS = 104,
        MODIFY_GUILD_WIDGET = 105,
        GET_GUILD_WIDGET = 106,
        GET_VANITY_INVITE = 107,
        GET_GUILD_WIDGET_IMAGE = 108,
        GET_GUILD_WELCOME_SCREEN = 109,
        PATCH_GUILD_WELCOME_SCREEN = 110,
        PATCH_CURRENT_USER_VOICE_STATE = 111,
        PATCH_USER_VOICE_STATE = 112,
        GET_GUILD_SCHEDULED_EVENTS = 113,
        POST_GUILD_SCHEDULED_EVENT = 114,
        GET_GUILD_SCHEDULED_EVENT = 115,
        PATCH_GUILD_SCHEDULED_EVENT = 116,
        DELETE_GUILD_SCHEDULED_EVENT = 117,
        GET_GUILD_SCHEDULED_EVENT_USERS = 118,
        GET_GUILD_TEMPLATE = 119,
        POST_GUILD_FROM_GUILD_TEMPLATE = 120,
        GET_GUILD_TEMPLATES = 121,
        POST_GUILD_TEMPLATE = 122,
        PUT_GUILD_TEMPLATE = 123,
        PATCH_GUILD_TEMPLATE = 124,
        DELETE_GUILD_TEMPLATE = 125,
        GET_INVITE = 126,
        DELETE_INVITE = 127,
        POST_STAGE_INSTANCE = 128,
        GET_STAGE_INSTANCE = 129,
        PATCH_STAGE_INSTANCE = 130,
        DELETE_STAGE_INSTANCE = 131,
        GET_STICKER = 132,
        GET_NITRO_STICKER_PACKS = 133,
        GET_GUILD_STICKERS = 134,
        POST_GUILD_STICKER = 135,
        PATCH_GUILD_STICKER = 136,
        DELETE_GUILD_STICKER = 137,
        GET_CURRENT_USER = 138,
        GET_USER = 139,
        PATCH_CURRENT_USER = 140,
        GET_CURRENT_USER_GUILDS = 141,
        DELETE_LEAVE_GUILD = 142,
        POST_CREATE_USER_DM = 143,
        GET_USER_CONNECTIONS = 144,
        GET_VOICE_REGIONS = 145,
        POST_WEBHOOK = 146,
        GET_CHANNEL_WEBHOOKS = 147,
        GET_GUILD_WEBHOOKS = 148,
        GET_WEBHOOK = 149,
        GET_WEBHOOK_WITH_TOKEN = 150,
        PATCH_WEBHOOK = 151,
        PATCH_WEBHOOK_WITH_TOKEN = 152,
        DELETE_WEBHOOK = 153,
        DELETE_WEBHOOK_WITH_TOKEN = 154,
        POST_EXECUTE_WEBHOOK = 155,
        GET_WEBHOOK_MESSAGE = 156,
        PATCH_WEBHOOK_MESSAGE = 157,
        DELETE_WEBHOOK_MESSAGE = 158,
        GET_APPLICATION_INFO = 159,
        GET_AUTHORIZATION_INFO = 160,
        GET_GATEWAY_BOT = 161
    };

    struct DiscordCoreAPI_Dll HttpWorkloadData {
        unordered_map<string, string> headersToInsert{};
        HttpWorkloadClass workloadClass{};
        HttpWorkloadType workloadType{};
        string relativePath{ "" };
        string callStack{ "" };
        string content{ "" };
        string baseUrl{ "" };
    };

    struct DiscordCoreAPI_Dll RateLimitData {
        HttpWorkloadType workloadType{ HttpWorkloadType::UNSET };
        int64_t nextExecutionTime{ 0 };
        int64_t msRemainTotal{ 0 };
        int64_t timeStartedAt{ 0 };
        int32_t getsRemaining{ 0 };
        string tempBucket{ "" };
        int32_t totalGets{ 0 };
        int64_t msRemain{ 0 };
        string bucket{ "" };
        mutex accessMutex{};
    };

    struct DiscordCoreAPI_Dll VoiceConnectInitData {
        string channelId{ "" };
        string guildId{ "" };
        string userId{ "" };
    };

    struct DiscordCoreAPI_Dll VoiceConnectionData {
        string voiceEncryptionMode{ "" };
        vector<uint8_t> secretKey{};
        uint32_t audioSSRC{ 0 };
        string externalIp{ "" };
        string voicePort{ "" };
        string sessionId{ "" };
        string endPoint{ "" };
        string voiceIp{ "" };
        string token{ "" };
        string keys{ "" };
    };

};