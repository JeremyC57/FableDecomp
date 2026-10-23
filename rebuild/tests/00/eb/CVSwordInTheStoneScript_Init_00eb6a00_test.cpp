#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_thingCtor = 0, g_thingDtor = 0, g_objCtor = 0, g_objDtor = 0, g_resizeA = 0, g_resizeB = 0; static unsigned g_nA = 0, g_nB = 0;
namespace N_006e7b40 { struct CScriptThing { unsigned char _body[0xc]; CScriptThing() { ++g_thingCtor; } ~CScriptThing() { ++g_thingDtor; } }; } using N_006e7b40::CScriptThing;
struct CScriptGameResourceObjectScriptedThingBase { unsigned char _body[0x10]; CScriptGameResourceObjectScriptedThingBase() { ++g_objCtor; } ~CScriptGameResourceObjectScriptedThingBase() { ++g_objDtor; } };
struct CScriptThingVector { CScriptThing* m_pFirst; CScriptThing* m_pLast; CScriptThing* m_pEnd; void resize(unsigned int n, const CScriptThing&) { ++g_resizeA; g_nA = n; if (g_thingDtor != 0) g_resizeA = -1; } };
struct CScriptedThingVector { CScriptGameResourceObjectScriptedThingBase* m_pFirst; CScriptGameResourceObjectScriptedThingBase* m_pLast; CScriptGameResourceObjectScriptedThingBase* m_pEnd; void resize(unsigned int n, const CScriptGameResourceObjectScriptedThingBase&) { ++g_resizeB; g_nB = n; if (g_objDtor != 0 || g_thingDtor != 1) g_resizeB = -1; } };
struct CV_SwordInTheStoneScript {
    virtual void v00() { }
    unsigned char _pad04[0x44]; long m_swordState; long m_crowdState; unsigned char m_swordTaken; unsigned char m_crowdGathered; unsigned char _pad52[0xe]; CScriptThingVector m_things; unsigned char _pad6c[0x10]; CScriptedThingVector m_scriptedThings;
    void Init() { m_swordState = 0; m_crowdState = 0; m_swordTaken = 0; m_crowdGathered = 0; m_things.resize(2, CScriptThing()); m_scriptedThings.resize(2, CScriptGameResourceObjectScriptedThingBase()); }
};
int main()
{
    CV_SwordInTheStoneScript s; s.m_swordState = 7; s.m_crowdState = 9; s.m_swordTaken = 1; s.m_crowdGathered = 1;
    s.Init();
    if (s.m_swordState != 0 || s.m_crowdState != 0 || s.m_swordTaken != 0 || s.m_crowdGathered != 0 || g_thingCtor != 1 || g_thingDtor != 1 || g_objCtor != 1 || g_objDtor != 1 || g_resizeA != 1 || g_resizeB != 1 || g_nA != 2 || g_nB != 2) { std::printf("00eb6a00_TEST FAIL\n"); return 1; }
    std::printf("00eb6a00_TEST PASS\n"); return 0;
}