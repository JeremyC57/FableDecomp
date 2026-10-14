#include <cstdio>

struct CThing;
static int g_base = 0, g_scriptThings = 0;
struct CScriptThing { void* __vftable; CThing* PImp_Data; void* PImp_Info;
    CScriptThing(CThing* t) : __vftable(0), PImp_Data(t), PImp_Info(0) { }
    CScriptThing(const CScriptThing& o) : __vftable(o.__vftable), PImp_Data(o.PImp_Data), PImp_Info(o.PImp_Info) { } };
struct CThing { CScriptThing GetScriptThing() { ++g_scriptThings; return CScriptThing(this); } };
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CAction : CBase { CThing* m_pThing; bool m_active; bool m_finished;
    CAction() : CBase(), m_pThing(0), m_active(false), m_finished(false) { } };

struct CActionFollowCreature : CAction { CScriptThing m_creature; long m_frameCount; bool m_arrived; long m_distance; bool m_moving; bool m_run; bool m_unused26; bool m_lost;
    virtual void v00() { }
    CActionFollowCreature(CThing* creature, long distance, bool run) : CAction(), m_creature(creature->GetScriptThing()), m_frameCount(0), m_arrived(false), m_distance(distance), m_moving(false), m_run(run), m_lost(false) { } };
int main()
{
    CThing t; CActionFollowCreature a(&t, 42, true);
    if (g_base != 1 || g_scriptThings != 1 || a.m_pThing != 0 || a.m_active || a.m_finished || a.m_creature.PImp_Data != &t || a.m_frameCount != 0 || a.m_arrived || a.m_distance != 42 || a.m_moving || !a.m_run || a.m_lost || sizeof(a) != 0x28) { std::printf("00905180_TEST FAIL\n"); return 1; }
    std::printf("00905180_TEST PASS\n"); return 0;
}
