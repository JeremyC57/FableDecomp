#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CCPPointerInfo {
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
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
struct CThing {
    int            Tag;                               // test: stands for the script name
    unsigned short DefGlobalIndex;
    unsigned char  StateBits;
};
namespace NEntityEvents {
struct CEventBase {
    virtual ~CEventBase() {}
    virtual long GetType() const = 0;
    long WorldFrame;
};
// test event: one record holding every attribute the filters read
struct CEventHitBase : CEventBase {
    CThing* Hitting;
    CThing* Weapon;
    bool KnockDown, Decapitate, Blocked, Shot, Melee, FlourishHit, Flourish;
    virtual bool IsFlourish() const { return FlourishHit; }
    CThing* GetPThingHitting() const { return Hitting; }
    CThing* GetPWeapon() const { return Weapon; }
    bool IsKnockDown() const { return KnockDown; }
    bool IsDecapitate() const { return Decapitate; }
};
struct CEventHitBy : CEventHitBase { enum { EventType = 0x1 }; };
struct CEventHitBySpecialAbility : CEventHitBase { enum { EventType = 0x1b }; };
struct CEventHitThing : CEventHitBase { enum { EventType = 0x1d }; };
struct CEventShotStruck : CEventHitBase { enum { EventType = 0x27 }; };
}

namespace NEntityEvents { struct CIsEventOfType { long Type; bool operator()(const CEventBase* e) const { return e->GetType() == Type; } }; }
namespace NEntityEvents { struct CIsEventCreatedAfter { long WorldFrame; bool operator()(const CEventBase* e) const { return e->WorldFrame > WorldFrame; } }; }
namespace NEntityEvents { struct CIsEventCreatedBeforeOrOn { long WorldFrame; bool operator()(const CEventBase* e) const { return e->WorldFrame <= WorldFrame; } }; }
template <class A, class B, class C> struct CFilter_Bind3 {     // every filter, held by reference
    const A& First;
    const B& Second;
    const C& Third;
    template <class T> bool operator()(T event) const { return First(event) && Second(event) && Third(event); }
};

struct CTestEvent : NEntityEvents::CEventHitBase {
    long Type;
    long GetType() const { return Type; }
};
static int g_deleted = 0;
static void __fastcall DeleteEvent(void* data) { ++g_deleted; (void)data; }
struct CEventList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CCountedPointer<NEntityEvents::CEventBase> _M_data; };
    SNode* _M_node;
    struct const_iterator {                           // STLport _List_iterator
        SNode* _M_node;
        const_iterator(SNode* node) : _M_node(node) {}
        const CCountedPointer<NEntityEvents::CEventBase>& operator*() const { return _M_node->_M_data; }
        const_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    struct reverse_iterator {                         // STLport reverse_iterator: *it is *--(copy of base)
        const_iterator current;
        reverse_iterator(const_iterator it) : current(it) {}
        const CCountedPointer<NEntityEvents::CEventBase>& operator*() const { const_iterator tmp = current; return *--tmp; }
        reverse_iterator& operator++() { --current; return *this; }
        const_iterator base() const { return current; }
        bool operator!=(const reverse_iterator& o) const { return base() != o.base(); }
    };
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }
};
struct CTCEntityEventsMethods {
    CEventList List;
    const CEventList& GetEvents() const { return List; }
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter& filter) const;
    template <class TFilter> bool HasEvent(const TFilter& filter) const;
};
template <class TFilter>
bool CTCEntityEventsMethods::HasEvent(const TFilter& filter) const
{
    for (CEventList::reverse_iterator it = GetEvents().rbegin(); it != GetEvents().rend(); ++it)
    {
        if (filter((*it).Data))
            return true;
    }
    return false;
}
template <class TEvent, class TFilter>
bool CTCEntityEventsMethods::GetEvent(TEvent** out, const TFilter& filter) const
{
    for (CEventList::reverse_iterator it = GetEvents().rbegin(); it != GetEvents().rend(); ++it)
    {
        CCountedPointer<NEntityEvents::CEventBase> event = *it;
        if (event->GetType() == TEvent::EventType && filter((TEvent*)event.Data))
        {
            *out = (TEvent*)event.Data;
            return true;
        }
    }
    return false;
}

CThing g_attacker;
CThing g_named;
CThing g_weapon;
CThing g_deadweapon;
static const int N = 64;
CTestEvent g_events[N];
CEventList::SNode g_nodes[N + 1];
CCPPointerInfo* g_infos[N];

int main()
{
    static const long types[4] = { 0x1b, 0x1, 0x2, 0x1b };
    int i = 0;
    CEventList::SNode* head = &g_nodes[N];
    head->_M_next = head; head->_M_prev = head;
    for (i = 0; i < N; ++i) {
        CTestEvent& e = g_events[i];
        e.Type = types[i % 4];
        e.WorldFrame = i;
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
    NEntityEvents::CIsEventOfType t = { 0x1b };
    NEntityEvents::CIsEventCreatedAfter a = { 10 };
    NEntityEvents::CIsEventCreatedBeforeOrOn b = { 20 };
    CFilter_Bind3<NEntityEvents::CIsEventOfType, NEntityEvents::CIsEventCreatedAfter, NEntityEvents::CIsEventCreatedBeforeOrOn> yes = { t, a, b };
    NEntityEvents::CIsEventCreatedAfter a2 = { 20 };
    NEntityEvents::CIsEventCreatedBeforeOrOn b2 = { 21 };
    CFilter_Bind3<NEntityEvents::CIsEventOfType, NEntityEvents::CIsEventCreatedAfter, NEntityEvents::CIsEventCreatedBeforeOrOn> no = { t, a2, b2 };
    int any_yes = 0, any_no = 0;
    for (i = 0; i < N; ++i) {
        const CTestEvent& e = g_events[i];
        if (e.Type == 0x1b && e.WorldFrame > 10 && e.WorldFrame <= 20) any_yes = 1;
        if (e.Type == 0x1b && e.WorldFrame > 20 && e.WorldFrame <= 21) any_no = 1;
    }
    int bad = 0;
    if (!any_yes || any_no) bad = 1;
    if (!list.HasEvent(yes) || list.HasEvent(no)) bad = 1;
    CEventList::SNode empty;
    empty._M_next = &empty; empty._M_prev = &empty;
    CTCEntityEventsMethods none;
    none.List._M_node = &empty;
    if (none.HasEvent(yes)) bad = 1;
    for (i = 0; i < N; ++i)
        if (g_infos[i]->RefCount != 1) bad = 1;
    if (g_deleted != 0) bad = 1;
    if (bad) { std::printf("008d4f80_TEST FAIL\n"); return 1; }
    std::printf("008d4f80_TEST PASS\n");
    return 0;
}