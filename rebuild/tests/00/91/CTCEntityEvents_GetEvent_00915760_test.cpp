#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCPPointerInfo { public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}           // test only: lets the test build list nodes
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    T* operator->() const { return Data; }
};
struct CEventBase {
    virtual ~CEventBase() {}
    virtual long GetType() const = 0;
    long WorldFrame;
};
struct CEventList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CCountedPointer<CEventBase> _M_data; };
    SNode* _M_node;
    struct reverse_iterator {
        SNode* current;
        reverse_iterator(SNode* node) : current(node) {}
        const CCountedPointer<CEventBase>& operator*() const { SNode* tmp = current->_M_prev; return tmp->_M_data; }
        reverse_iterator& operator++() { current = current->_M_prev; return *this; }
        bool operator!=(const reverse_iterator& o) const { return current != o.current; }
    };
    reverse_iterator rbegin() const { return reverse_iterator(_M_node); }
    reverse_iterator rend() const { return reverse_iterator(_M_node->_M_next); }
};
namespace NEntityEvents {
class CIsEventCreatedAfter { public:
    long WorldFrame;
    bool operator()(const CEventBase& event) const { return event.WorldFrame > WorldFrame; }
};
class CIsEventCreatedBeforeOrOn { public:
    long WorldFrame;
    bool operator()(const CEventBase& event) const { return event.WorldFrame <= WorldFrame; }
};
class CIsEventOfType { public:
    long Type;
    bool operator()(const CEventBase& event) const { return event.GetType() == Type; }
};
struct CThing { int Tag; };
// test event: type, frame, attacker, knocked-down flag
struct CEventHitByMethods : CEventBase {
    CThing* Attacker;
    bool    KnockedDown;
    CThing* GetAttacker() const { return Attacker; }
    bool WasKnockedDown() const { return KnockedDown; }
};
class CIsHitEventHitAndKnockedDownBy { public:
    CThing* Attacker;
    bool operator()(const CEventBase& event) const
    {
        const CEventHitByMethods& hit = (const CEventHitByMethods&)event;
        return hit.GetAttacker() == Attacker && hit.WasKnockedDown();
    }
};
}
template <class A, class B> class CFilter_Bind2 { public:
    const A& First;
    const B& Second;
    template <class T> bool operator()(const T& event) const { return First(event) && Second(event); }
};
struct CTestEvent : NEntityEvents::CEventHitByMethods {
    long Type;
    long GetType() const { return Type; }
};
static int g_deleted = 0;
static void __fastcall DeleteEvent(void* data) { ++g_deleted; (void)data; }
namespace NEntityEvents {
class CEventChildHurtGoToMother : public CEventBase { public: enum { EventType = 0x18 }; };
}

struct CTCEntityEventsMethods {
    CEventList List;
    const CEventList& GetEvents() const { return List; }
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter& filter) const;
};
// The newest event of TEvent's type that passes the filter: stored to *out, true; false when none.
template <class TEvent, class TFilter>
bool CTCEntityEventsMethods::GetEvent(TEvent** out, const TFilter& filter) const
{
    for (CEventList::reverse_iterator it = GetEvents().rbegin(); it != GetEvents().rend(); ++it)
    {
        CCountedPointer<CEventBase> event = *it;
        if (event->GetType() == TEvent::EventType && filter(*(TEvent*)event.Data))
        {
            *out = (TEvent*)event.Data;
            return true;
        }
    }
    return false;
}

NEntityEvents::CThing g_attacker;
NEntityEvents::CThing g_nobody;
static const int N = 24;
CTestEvent g_events[N];
CEventList::SNode g_nodes[N + 1];
CCPPointerInfo* g_infos[N];

static bool Expected(const CTestEvent& e)
{
    return e.Type == 0x18;
}

int main()
{
    static const long types[4] = { 0x18, 0x18, 0x19, 0x18 };
    int i = 0;
    CEventList::SNode* head = &g_nodes[N];
    head->_M_next = head; head->_M_prev = head;
    for (i = 0; i < N; ++i) {
        CTestEvent& e = g_events[i];
        e.Type = types[i % 4];
        e.WorldFrame = i;
        e.Attacker = (i % 3 == 0) ? &g_attacker : 0;
        e.KnockedDown = (i % 2 == 0);
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 1; info->DeleteFunc = DeleteEvent; info->Data = &e;
        g_infos[i] = info;
        CEventList::SNode* n = &g_nodes[i];
        n->_M_data.Data = &e; n->_M_data.Info = info;
        n->_M_next = head; n->_M_prev = head->_M_prev;
        head->_M_prev->_M_next = n; head->_M_prev = n;
    }
    CTCEntityEventsMethods list;
    list.List._M_node = head;
    NEntityEvents::CIsEventOfType filter = { 0x18 };
    NEntityEvents::CIsEventOfType other = { 0x19 };

    NEntityEvents::CEventChildHurtGoToMother* found = 0;
    bool ok = list.GetEvent(&found, filter);
    const CTestEvent* want = 0;
    for (i = N - 1; i >= 0; --i)
        if (Expected(g_events[i])) { want = &g_events[i]; break; }
    int bad = 0;
    if (!want || !ok || (const void*)found != (const void*)want) bad = 1;
    NEntityEvents::CEventChildHurtGoToMother* untouched = (NEntityEvents::CEventChildHurtGoToMother*)&g_attacker;
    found = untouched;
    bool ok2 = list.GetEvent(&found, other);
    if (ok2 || found != untouched) bad = 1;
    for (i = 0; i < N; ++i)
        if (g_infos[i]->RefCount != 1) bad = 1;
    if (g_deleted != 0) bad = 1;
    if (bad) { std::printf("00915760_TEST FAIL\n"); return 1; }
    std::printf("00915760_TEST PASS\n");
    return 0;
}