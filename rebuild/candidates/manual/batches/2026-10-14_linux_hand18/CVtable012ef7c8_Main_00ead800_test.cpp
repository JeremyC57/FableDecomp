#include <cstdio>
static int g_pumps = 0; static int g_polls = 0; static int g_limit = 3;
struct CGameScriptInterface_Calls { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void Pump() { ++g_pumps; } };
static int g_copies = 0; static int g_dtors = 0; static void* g_waited = 0; static int g_waitPumps = -1;
struct CScriptThing_Calls { void* __vftable; void* PImp_Data; void* PImp_Count; CScriptThing_Calls() : __vftable(0), PImp_Data(0), PImp_Count(0) { } CScriptThing_Calls(const CScriptThing_Calls& o) : __vftable(o.__vftable), PImp_Data(o.PImp_Data), PImp_Count(o.PImp_Count) { ++g_copies; } };
struct CIsThingAlive { virtual void v00() { } CScriptThing_Calls Thing; CIsThingAlive(const CScriptThing_Calls& t) : Thing(t) { } ~CIsThingAlive() { ++g_dtors; } };
struct CVtable_012ef7c8 {
    virtual void v00() { } CGameScriptInterface_Calls* m_pInterface; CScriptThing_Calls m_thing;
    void WaitFor(CIsThingAlive* c) { g_waited = c->Thing.PImp_Data; g_waitPumps = g_pumps; }
    unsigned char IsFinished() { ++g_polls; return g_pumps >= g_limit ? 1 : 0; }
    void Main() { { CIsThingAlive condition(m_thing); WaitFor(&condition); } m_pInterface->Pump(); while (!IsFinished()) m_pInterface->Pump(); }
};
int main()
{
    CGameScriptInterface_Calls gi; CVtable_012ef7c8 a; a.m_pInterface = &gi; a.m_thing.PImp_Data = (void*)0x5150;
    a.Main();
    if (g_copies != 1 || g_dtors != 1 || g_waited != (void*)0x5150 || g_waitPumps != 0 || g_pumps != 3 || g_polls != 3 || (char*)&a.m_thing - (char*)&a != 8 || sizeof(CIsThingAlive) != 16) { std::printf("00ead800_TEST FAIL\n"); return 1; }
    g_pumps = 0; g_polls = 0; g_limit = 0; a.Main(); if (g_pumps != 1 || g_polls != 1 || g_dtors != 2) { std::printf("00ead800_TEST FAIL\n"); return 1; }
    std::printf("00ead800_TEST PASS\n"); return 0;
}
