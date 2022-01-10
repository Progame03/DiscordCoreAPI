/// EventEntities.hpp - Header for the "Event-related stuff".
/// Dec 12, 2021
/// Chris M.
/// https://github.com/RealTimeChris

#pragma once

#include "IndexInitial.hpp"
#include "FoundationEntities.hpp"
#include "CoRoutine.hpp"

namespace DiscordCoreAPI {

    struct DiscordCoreAPI_Dll EventDelegateToken {

        template<typename R, typename ...Args>
        friend class  Event;

        template<typename R, typename ...Args>
        friend class  EventDelegate;

        friend inline bool operator==(const EventDelegateToken& lhs, const EventDelegateToken& rhs);

        friend inline bool operator<(const EventDelegateToken& lhs, const EventDelegateToken& rhs);

        EventDelegateToken() = default;

    protected:

        string handlerId{ "" };
        string eventId{ "" };

    };

    bool operator==(const EventDelegateToken& lhs, const EventDelegateToken& rhs) {
        if (lhs.eventId == rhs.eventId && lhs.handlerId == rhs.handlerId) {
            return true;
        }
        else {
            return false;
        }
    }

    bool operator<(const EventDelegateToken& lhs, const EventDelegateToken& rhs) {
        if (lhs.handlerId < rhs.handlerId) {
            return true;
        }
        else {
            return false;
        }
    }

    template<typename R, typename ...Args>
    class EventDelegate {
    public:

        template<typename R, typename... A>
        friend class  Event;

        EventDelegate<R, Args...>& operator=(EventDelegate<R, Args...>&& other) noexcept {
            if (this != &other) {
                this->theFunction = move(other.theFunction);
                other.theFunction = function<R(Args...)>{};
            }
            return *this;
        }

        EventDelegate(EventDelegate<R, Args...>&& other) noexcept {
            *this = move(other);
        }

        EventDelegate<R, Args...>& operator=(const EventDelegate<R, Args...>& other) = delete;

        EventDelegate(const EventDelegate<R, Args...>& other) = delete;

        EventDelegate<R, Args...>& operator=(EventDelegate<R, Args...>& other) = delete;

        EventDelegate(EventDelegate<R, Args...>& other) = delete;

        EventDelegate(function<R(Args...)> theFunctionNew) {
            this->theFunction = theFunctionNew;
        }

        EventDelegate(R(*theFunctionNew)(Args...)) {
            this->theFunction = theFunctionNew;
        }

    protected:
        function<R(Args...)>theFunction{};
    };

    template<typename R, typename  ...Args>
    class Event {
    public:

        Event<R, Args...>& operator=(Event<R, Args...>&& other) noexcept {
            if (this != &other) {
                this->theFunctions = move(other.theFunctions);
                other.theFunctions = map<EventDelegateToken, EventDelegate<R, Args...>>{};
                this->eventId = move(other.eventId);
                other.eventId = string{};
            }
            return *this;
        }

        Event(Event<R, Args...>&& other) noexcept {
            *this = move(other);
        }

        Event<R, Args...>& operator=(const Event<R, Args...>&) = delete;

        Event(const Event<R, Args...>&) = delete;

        Event<R, Args...>& operator=(Event<R, Args...>&) = delete;

        Event(Event<R, Args...>&) = delete;

        Event() {
            this->eventId = to_string(duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count());
        }

        EventDelegateToken add(EventDelegate<R, Args...> eventDelegate) {
            EventDelegateToken eventToken{};
            eventToken.handlerId = to_string(duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count());
            eventToken.eventId = this->eventId;
            this->theFunctions.insert_or_assign(eventToken, move(eventDelegate));
            return eventToken;
        }

        void remove(EventDelegateToken  eventToken) {
            if (this->theFunctions.contains(eventToken)) {
                this->theFunctions.erase(eventToken);
            }
        }

        void operator()(Args... args) {
            for (auto& [key, value] : this->theFunctions) {
                value.theFunction(args...);
            }
        }

    protected:
        map<EventDelegateToken, EventDelegate<R, Args...>> theFunctions{};
        string eventId{ "" };
    };

    template<typename ...Args>
    class EventDelegate<void, Args...> {
    public:

        friend class  Event<void, Args...>;

        EventDelegate<void, Args...>& operator=(EventDelegate<void, Args...>&& other) noexcept {
            if (this != &other) {
                this->theFunction = move(other.theFunction);
                other.theFunction = function<void(Args...)>{};
            }
            return *this;
        }

        EventDelegate(EventDelegate<void, Args...>&& other) noexcept {
            *this = move(other);
        }

        EventDelegate<void, Args...>& operator=(const EventDelegate<void, Args...>& other) = delete;

        EventDelegate(const EventDelegate<void, Args...>& other) = delete;

        EventDelegate<void, Args...>& operator=(EventDelegate<void, Args...>& other) = delete;

        EventDelegate(EventDelegate<void, Args...>& other) = delete;

        EventDelegate(function<void(Args...)> theFunctionNew) {
            this->theFunction = theFunctionNew;
        }

        EventDelegate(void(*theFunctionNew)(Args...)) {
            this->theFunction = theFunctionNew;
        }

    protected:
        function<void(Args...)>theFunction{};
    };

    template<typename ...Args>
    class Event<void, Args...> {
    public:

        Event<void, Args...>& operator=(Event<void, Args...>&& other) noexcept {
            if (this != &other) {
                this->theFunctions = move(other.theFunctions);
                other.theFunctions = map<EventDelegateToken, EventDelegate<void, Args...>>{};
                this->eventId = move(other.eventId);
                other.eventId = string{};
            }
            return *this;
        }

        Event(Event<void, Args...>&& other) noexcept {
            *this = move(other);
        }

        Event<void, Args...>& operator=(const Event<void, Args...>&) = delete;

        Event(const Event<void, Args...>&) = delete;

        Event<void, Args...>& operator=(Event<void, Args...>&) = delete;

        Event(Event<void, Args...>&) = delete;

        Event() {
            this->eventId = to_string(duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count());
        }

        EventDelegateToken add(EventDelegate<void, Args...> eventDelegate) {
            EventDelegateToken eventToken{};
            eventToken.handlerId = to_string(duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count());
            eventToken.eventId = this->eventId;
            this->theFunctions.insert_or_assign(eventToken, move(eventDelegate));
            return eventToken;
        }

        void remove(EventDelegateToken  eventToken) {
            if (this->theFunctions.contains(eventToken)) {
                this->theFunctions.erase(eventToken);
            }
        }

        void operator()(Args... args) {
            for (auto& [key, value] : this->theFunctions) {
                value.theFunction(args...);
            }
        }

    protected:

        map<EventDelegateToken, EventDelegate<void, Args...>> theFunctions{};
        string eventId{ "" };

    };

    struct DiscordCoreAPI_Dll EventCore {

        friend class  Event<void, void>;
        friend struct EventCoreDeleter;

        EventCore& operator=(const EventCore&) = delete;

        EventCore(const EventCore&) = delete;

        EventCore& operator=(EventCore&) = delete;

        EventCore(EventCore&) = delete;

        EventCore() = default;

    protected:

        shared_ptr<bool> theEventState{ make_shared<bool>() };

    };

    template<>
    class DiscordCoreAPI_Dll Event<void, void> {
    public:
        
        atomic<shared_ptr<bool>> theEventState{ nullptr };
        shared_ptr<EventCore> theEventCore{ nullptr };

        Event<void, void>& operator=(const Event<void, void>& other) {
            this->theEventState.store(other.theEventState.load());
            this->theEventCore = other.theEventCore;
            return *this;
        }

        Event(const Event<void, void>& other) {
            *this = other;
        } 

        Event<void, void>& operator=(Event<void, void>& other) {
            this->theEventState.store(other.theEventState.load());
            this->theEventCore = other.theEventCore;
            return *this;
        }

        Event(Event<void, void>& other) {
            *this = other;
        }

        Event() {
            this->theEventCore = make_shared<EventCore>();
            this->theEventState.store(this->theEventCore->theEventState);
        }

        uint32_t wait(int64_t millisecondsMaxToWait = UINT64_MAX, string testString = "") {
            int64_t millisecondsWaited{ 0 };
            while (true) {
                cout << "WERE HERE THIS IS IT!" << endl;
                if (*this->theEventState.load()) {
                    return 0;
                }
                else {
                    int64_t startTime = duration_cast<milliseconds, int64_t>(steady_clock::now().time_since_epoch()).count();
                    this_thread::sleep_for(milliseconds(1));
                    int64_t endTime = duration_cast<milliseconds, int64_t>(steady_clock::now().time_since_epoch()).count();
                    millisecondsWaited += endTime - startTime;
                }
                if (millisecondsWaited >= millisecondsMaxToWait) {
                    return 1;
                }
            }
        }

        void set(string testValue = "") {
            auto thePtr = this->theEventState.load();
            *thePtr = true;
            this->theEventState.store(thePtr);
        }

        void reset() {
            auto thePtr = this->theEventState.load();
            *thePtr = false;
            this->theEventState.store(thePtr);
        }
    };


}