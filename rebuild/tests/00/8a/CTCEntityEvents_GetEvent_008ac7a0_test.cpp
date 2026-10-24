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

namespace NEntityEvents { struct CEventFilter_None { bool operator()(const CEventBase*) const { return true; } }; }

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
};
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

static bool Expected(const CTestEvent& e)
{
    return e.Type == 0x27 && (true);
}

int main()
{
    static const long types[4] = { 0x27, 0x27, 0x2, 0x27 };
    int i = 0;
    g_named.Tag = 7;
    g_attacker.Tag = 7;
    g_weapon.DefGlobalIndex = 3; g_weapon.StateBits = 0;
    g_deadweapon.DefGlobalIndex = 3; g_deadweapon.StateBits = 1;
    CEventList::SNode* head = &g_nodes[N];
    head->_M_next = head; head->_M_prev = head;
    for (i = 0; i < N; ++i) {
        CTestEvent& e = g_events[i];
        e.Type = types[i % 4];
        e.WorldFrame = i;
        e.Hitting = (i % 3 == 0) ? &g_attacker : ((i % 3 == 1) ? &g_named : 0);
        e.Weapon = (i % 5 == 0) ? 0 : ((i % 5 == 1) ? &g_deadweapon : &g_weapon);
        e.KnockDown = (i % 2 == 0);
        e.Decapitate = (i % 7 < 3);
        e.Blocked = (i % 6 == 1);
        e.Shot = (i % 4 < 2);
        e.Melee = (i % 8 > 2);
        e.FlourishHit = (i % 9 < 5);
        e.Flourish = (i % 10 > 3);
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
    NEntityEvents::CEventFilter_None f0 = {};
    const NEntityEvents::CEventFilter_None& filter = f0;
    const NEntityEvents::CEventShotStruck* found = 0;
    bool ok = list.GetEvent(&found, filter);
    const CTestEvent* want = 0;
    for (i = N - 1; i >= 0; --i)
        if (Expected(g_events[i])) { want = &g_events[i]; break; }
    int bad = 0;
    if (!want || !ok || (const void*)found != (const void*)want) bad = 1;
    for (i = 0; i < N; ++i)
        g_events[i].Type = 0x2;
    const NEntityEvents::CEventShotStruck* untouched = (const NEntityEvents::CEventShotStruck*)&g_events[3];
    found = untouched;
    bool ok2 = list.GetEvent(&found, filter);
    if (ok2 || found != untouched) bad = 1;
    for (i = 0; i < N; ++i)
        if (g_infos[i]->RefCount != 1) bad = 1;
    if (g_deleted != 0) bad = 1;
    if (bad) { std::printf("008ac7a0_TEST FAIL\n"); return 1; }
    std::printf("008ac7a0_TEST PASS\n");
    return 0;
}