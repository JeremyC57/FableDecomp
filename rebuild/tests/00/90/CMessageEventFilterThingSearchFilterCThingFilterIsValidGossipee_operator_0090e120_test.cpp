// Behaviour test for 0x0090E120 CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>::operator(): false
// unless the message's originator is a live creature (all of type-flag bits 0..4 set), else the thing filter's verdict.
#include <cstdio>

class Thing { public: unsigned long ThingTypeFlags; bool gossipee; };
static bool IsCreature(const Thing* t) { unsigned long mask = 0x1f; return !(~t->ThingTypeFlags & mask); }

class Message { public: Thing* originator; };
class IsValidGossipee { public: mutable int calls; bool operator()(Thing* c) const { ++calls; return c->gossipee; } };
class ThingSearchFilter {
public:
    IsValidGossipee* Filter;
    bool operator()(const Message& m) const
    {
        Thing* creature = m.originator && IsCreature(m.originator) ? m.originator : 0;
        if (!creature)
            return false;
        return (*Filter)(creature);
    }
};

int main()
{
    IsValidGossipee g; g.calls = 0;
    ThingSearchFilter f; f.Filter = &g;
    Message m; m.originator = 0;
    if (f(m) || g.calls != 0) { std::printf("0090e120_TEST FAIL\n"); return 1; }
    Thing object = { 0x0e, true };
    m.originator = &object;
    if (f(m) || g.calls != 0) { std::printf("0090e120_TEST FAIL\n"); return 1; }
    Thing villager = { 0x3f, true };
    m.originator = &villager;
    if (!f(m) || g.calls != 1) { std::printf("0090e120_TEST FAIL\n"); return 1; }
    villager.gossipee = false;
    if (f(m) || g.calls != 2) { std::printf("0090e120_TEST FAIL\n"); return 1; }
    std::printf("0090e120_TEST PASS\n");
    return 0;
}