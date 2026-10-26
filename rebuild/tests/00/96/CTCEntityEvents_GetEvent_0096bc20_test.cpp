// Behaviour test for 0x0096bc20 CTCEntityEvents::GetEvent<CEventHitBySpecialAbility, ...>: the NEWEST event of type 0x1b that passes the filter is returned
// (walking the STLport list in reverse); nothing when no event of that type passes; the counted-pointer copies leave the counts unchanged.
#include <cstdio>
#include <cstdlib>
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
static int g_deleted = 0;
void __fastcall KillEvent(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); } }
    T* operator->() const { return Data; }
};
struct CEventBase { long Type; virtual ~CEventBase() {} virtual long GetType() const { return Type; } long WorldFrame; };
struct CEventList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CCountedPointer<CEventBase> _M_data; };
    SNode* _M_node;
    struct const_iterator {
        SNode* _M_node;
        const_iterator(SNode* node) : _M_node(node) {}
        const CCountedPointer<CEventBase>& operator*() const { return _M_node->_M_data; }
        const_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    struct reverse_iterator {
        const_iterator current;
        reverse_iterator(const_iterator it) : current(it) {}
        const CCountedPointer<CEventBase>& operator*() const { const_iterator tmp = current; return *--tmp; }
        reverse_iterator& operator++() { --current; return *this; }
        const_iterator base() const { return current; }
        bool operator!=(const reverse_iterator& o) const { return base() != o.base(); }
    };
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }
};
namespace NEntityEvents {
class CIsEventCreatedAfter { public: long WorldFrame; bool operator()(const CEventBase& event) const { return event.WorldFrame > WorldFrame; } };
class CEventHitBySpecialAbility : public CEventBase { public: enum { EventType = 0x1b }; bool Explosion; };

}
template <class A, class B> class CFilter_Bind2 { public:
    const A& First; const B& Second;
    template <class T> bool operator()(const T& event) const { return First(event) && Second(event); }
};
class CTCEntityEvents { public: unsigned char pad[0xc]; void* Events; };
struct CTCEntityEventsMethods : CTCEntityEvents {
    const CEventList& GetEvents() const { return *(const CEventList*)Events; }
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter& filter) const
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
};
using namespace NEntityEvents;
static CEventHitBySpecialAbility g_ev[5];
static CEventList::SNode g_nodes[6];      // g_nodes[5] = list header
static CEventList g_list;
static void Link(int i, long type, long frame, bool expl)
{
    g_ev[i].Type = type; g_ev[i].WorldFrame = frame; g_ev[i].Explosion = expl;
    CCPPointerInfo* info = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo));
    info->RefCount = 1; info->DeleteFunc = KillEvent; info->Data = &g_ev[i];
    g_nodes[i]._M_data.Data = &g_ev[i]; g_nodes[i]._M_data.Info = info;
}
int main()
{
    int i = 0;
    for (; i < 6; ++i) { g_nodes[i]._M_next = &g_nodes[(i + 1) % 6]; g_nodes[i]._M_prev = &g_nodes[(i + 5) % 6]; }
    // oldest .. newest: [0] right type frame 5 (explosion), [1] other type frame 50, [2] right type frame 20 (no explosion), [3] right type frame 30 (explosion), [4] other type frame 99
    Link(0, 0x1b, 5, true); Link(1, 0x1b + 1, 50, false); Link(2, 0x1b, 20, false); Link(3, 0x1b, 30, true); Link(4, 0x1b + 2, 99, true);
    g_list._M_node = &g_nodes[5];
    CTCEntityEventsMethods tc; tc.Events = &g_list;
    CIsEventCreatedAfter filter; filter.WorldFrame = 10;
    CEventHitBySpecialAbility* out = 0;
    bool ok = true;
    ok = ok && tc.GetEvent(&out, filter) && out == &g_ev[3];              // newest right-typed event after frame 10
    filter.WorldFrame = 25; out = 0; ok = ok && tc.GetEvent(&out, filter) && out == &g_ev[3];
    filter.WorldFrame = 30; out = 0; ok = ok && !tc.GetEvent(&out, filter) && out == 0;   // 30 is not after 30; older ones fail too
    filter.WorldFrame = 0;  out = 0; ok = ok && tc.GetEvent(&out, filter) && out == &g_ev[3];
    ok = ok && g_deleted == 0 && g_nodes[3]._M_data.Info->RefCount == 1 && g_nodes[0]._M_data.Info->RefCount == 1;
    CEventList emptyList; CEventList::SNode lone; lone._M_next = lone._M_prev = &lone; emptyList._M_node = &lone;
    tc.Events = &emptyList; out = 0;
    ok = ok && !tc.GetEvent(&out, filter) && out == 0;
    if (!ok) { std::printf("0096bc20_TEST FAIL\n"); return 1; }
    std::printf("0096bc20_TEST PASS\n");
    return 0;
}