#include <cstdio>
// Behaviour test for 00e83920: CFisticuffsMember's ctor copies the parent's interface and Me, keeps the parent and the master
// data, takes a fresh ShoutTimer and default-constructs the cheer/boo strings.
static int g_strings = 0;
struct Interface { long nextTimer; long RegisterTimer() { return nextTimer++; } };
static Interface g_interface;
class CTimer { public: long Index; CTimer() : Index(g_interface.RegisterTimer()) {} };
class CScriptThing { public: int id; CScriptThing(int i) : id(i) {} CScriptThing(const CScriptThing& o) : id(o.id) {} };
class CCharString { public: const char* p; CCharString() : p("") { ++g_strings; } };
struct Parent { Interface* Game; };
struct Base { Interface* Game; CScriptThing Me; Base(const CScriptThing& t, Interface* g) : Game(g), Me(t) {} };
struct Member : Base {
    Parent* ParentClass; void* PMasterData; bool PerformingCheerOrBoo; long FC_Status; CTimer ShoutTimer;
    CCharString CheerName, BooName;
    Member(Parent& parent, void* data, const CScriptThing& me) : Base(me, parent.Game), ParentClass(&parent) { PMasterData = data; }
};
int main()
{
    g_interface.nextTimer = 9;
    Parent parent; parent.Game = &g_interface; int data = 0; CScriptThing me(7);
    Member m(parent, &data, me);
    Member n(parent, 0, me);
    int ok = m.Game == &g_interface && m.Me.id == 7 && m.ParentClass == &parent && m.PMasterData == &data
          && m.ShoutTimer.Index == 9 && n.ShoutTimer.Index == 10 && n.PMasterData == 0 && g_strings == 4;
    if (!ok) { std::printf("00e83920_TEST FAIL\n"); return 1; }
    std::printf("00e83920_TEST PASS\n");
    return 0;
}