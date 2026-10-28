// Test: FindPlayerInteractionEvent -- the most recent message of type 0x14 targetted at the creature
#include <cstdio>
class CMessageEvent { public: long Type; int Target; bool OriginatorAlive; };
class CMessageEventManager { public:
    const CMessageEvent* events; int count;
    template <class F> const CMessageEvent* FindMostRecentMessage(const F& filter) const
    {
        for (int i = count - 1; i >= 0; --i) if (filter(events[i])) return &events[i];
        return 0;
    }
};
struct IsOfType { long Type; bool operator()(const CMessageEvent& e) const { return e.Type == Type; } };
struct IsOriginatorAlive { bool operator()(const CMessageEvent& e) const { return e.OriginatorAlive; } };
struct IsTargettedAtThing { int Thing; bool operator()(const CMessageEvent& e) const { return e.Target == Thing; } };
template <class F1, class F2> struct Bind2 { F1 a; F2 b; bool operator()(const CMessageEvent& e) const { return a(e) && b(e); } };
template <class F1, class F2, class F3> struct Bind3 { F1 a; F2 b; F3 c; bool operator()(const CMessageEvent& e) const { return a(e) && b(e) && c(e); } };
static const CMessageEvent* Find(const CMessageEventManager* manager, int creature)
{
    IsOfType t = { 0x14 }; IsTargettedAtThing g = { creature };
    Bind2<IsOfType, IsTargettedAtThing> filter = { t, g };
    return manager->FindMostRecentMessage(filter);
}
int main()
{
    CMessageEvent ev[5] = { { 0x14, 7, true }, { 0x14, 7, false }, { 0x14, 8, true }, { 0x01, 7, true }, { 0x14 + 1, 7, true } };
    CMessageEventManager m = { ev, 5 };
    const CMessageEvent* r = Find(&m, 7);
    if (r != &ev[1] || Find(&m, 9) != 0) { std::printf("0092edc0_TEST FAIL\n"); return 1; }
    std::printf("0092edc0_TEST PASS\n");
    return 0;
}