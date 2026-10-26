#include <cstdio>
// Behaviour test for 00ed7d50: CTB_Bard's ctor copies the parent's interface and Me, keeps the parent and the master data and
// default-constructs the three lastRandomSpeech strings.
static int g_strings = 0;
class CScriptThing { public: int id; CScriptThing(int i) : id(i) {} CScriptThing(const CScriptThing& o) : id(o.id) {} };
class CCharString { public: const char* p; CCharString() : p("") { ++g_strings; } };
struct Interface { int unused; };
struct Parent { Interface* Game; };
struct Base { Interface* Game; CScriptThing Me; Base(const CScriptThing& t, Interface* g) : Game(g), Me(t) {} };
struct Bard : Base {
    Parent* ParentClass; void* PMasterData; bool first_time_talked_to, given_gold;
    CCharString lastRandomSpeech1, lastRandomSpeech2, lastRandomSpeech3;
    Bard(Parent& parent, void* data, const CScriptThing& me) : Base(me, parent.Game), ParentClass(&parent) { PMasterData = data; }
};
int main()
{
    Interface gi; Parent parent; parent.Game = &gi; int data = 0; CScriptThing me(3);
    Bard b(parent, &data, me);
    int ok = b.Game == &gi && b.Me.id == 3 && b.ParentClass == &parent && b.PMasterData == &data && g_strings == 3
          && b.lastRandomSpeech3.p[0] == 0;
    if (!ok) { std::printf("00ed7d50_TEST FAIL\n"); return 1; }
    std::printf("00ed7d50_TEST PASS\n");
    return 0;
}