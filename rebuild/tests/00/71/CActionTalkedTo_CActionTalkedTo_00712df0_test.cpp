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

struct CBaseObject { int x; };
static int g_ip = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* PItem; CBaseIntelligentPointer() : PItem(0) { ++g_ip; } void SetPItem(CBaseObject* item) { PItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } };
struct CActionTalkedTo : CAction { CIntelligentPointer m_target; CScriptThing m_talker;
    virtual void v00() { }
    CActionTalkedTo(CThing* talker, CBaseObject* target) : CAction(), m_target(target), m_talker(talker->GetScriptThing()) { } };
int main()
{
    CThing t; CBaseObject o; CActionTalkedTo a(&t, &o);
    if (g_base != 1 || g_ip != 1 || g_scriptThings != 1 || a.m_pThing != 0 || a.m_active || a.m_finished || a.m_target.PItem != &o || a.m_talker.PImp_Data != &t || sizeof(a) != 0x20) { std::printf("00712df0_TEST FAIL\n"); return 1; }
    std::printf("00712df0_TEST PASS\n"); return 0;
}