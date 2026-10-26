#include <cstdio>
// Behaviour test for 00dc3df0: CSecretPassageSkeleton's ctor copies the parent's interface and Me, keeps the parent and the
// master data, and default-constructs the four skeleton_object strings.
static int g_strings = 0;
class CScriptThing { public: int id; CScriptThing(int i) : id(i) {} CScriptThing(const CScriptThing& o) : id(o.id) {} };
class CCharString { public: const char* p; CCharString() : p("") { ++g_strings; } };
struct Interface { int unused; };
struct Parent { Interface* Game; };
struct Base { Interface* Game; CScriptThing Me; Base(const CScriptThing& t, Interface* g) : Game(g), Me(t) {} };
struct Skeleton : Base {
    Parent* ParentClass; void* PMasterData; bool returned[4]; CCharString skeleton_object[4]; bool collected_all;
    Skeleton(Parent& parent, void* data, const CScriptThing& me) : Base(me, parent.Game), ParentClass(&parent) { PMasterData = data; }
};
int main()
{
    Interface gi; Parent parent; parent.Game = &gi; int data = 0; CScriptThing me(42);
    Skeleton s(parent, &data, me);
    int ok = s.Game == &gi && s.Me.id == 42 && s.ParentClass == &parent && s.PMasterData == &data && g_strings == 4
          && s.skeleton_object[3].p[0] == 0;
    if (!ok) { std::printf("00dc3df0_TEST FAIL\n"); return 1; }
    std::printf("00dc3df0_TEST PASS\n");
    return 0;
}