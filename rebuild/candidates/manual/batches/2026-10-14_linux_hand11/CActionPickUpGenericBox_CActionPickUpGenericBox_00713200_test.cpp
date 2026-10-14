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

struct CActionPickUpGenericBox : CAction { CScriptThing m_box; long m_frame; bool m_pickedUp;
    virtual void v00() { }
    CActionPickUpGenericBox(CThing* box) : CAction(), m_box(box->GetScriptThing()), m_frame(-1), m_pickedUp(false) { } };
int main()
{
    CThing t; CActionPickUpGenericBox a(&t);
    if (g_base != 1 || g_scriptThings != 1 || a.m_pThing != 0 || a.m_active || a.m_finished || a.m_box.PImp_Data != &t || a.m_frame != -1 || a.m_pickedUp) { std::printf("00713200_TEST FAIL\n"); return 1; }
    std::printf("00713200_TEST PASS\n"); return 0;
}
