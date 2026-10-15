#include <cstdio>
// Behaviour model of 0x00CD2757: CScreenFilterSThingByPass(thing, bypassSet).
static int g_copies = 0;
struct CScriptThing { virtual ~CScriptThing() {} int m_id; CScriptThing(int id) : m_id(id) {} CScriptThing(const CScriptThing& o) : m_id(o.m_id) { ++g_copies; } };
struct CScreenFilterSThingByPass {
    CScriptThing thing; bool bypassSet;
    CScreenFilterSThingByPass(const CScriptThing& t, bool b) : thing(t), bypassSet(b) {}
};
int main()
{
    CScriptThing hero(42);
    CScreenFilterSThingByPass f(hero, true);
    if (f.thing.m_id != 42 || !f.bypassSet || g_copies != 1) { std::printf("00cd2757_TEST FAIL\n"); return 1; }
    CScreenFilterSThingByPass g(hero, false);
    if (g.bypassSet || g_copies != 2) { std::printf("00cd2757_TEST FAIL\n"); return 1; }
    std::printf("00cd2757_TEST PASS\n");
    return 0;
}
