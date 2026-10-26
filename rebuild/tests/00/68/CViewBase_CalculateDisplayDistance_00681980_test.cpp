#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CThing { public: int x; };
static CThing* g_asked = 0;
class COpinionReactionManager { public: float GetToleranceToBeingHit(CThing* thing) { g_asked = thing; return 10.0f; } };
class CDisplayEngine { public: unsigned char _pad00[0x74]; COpinionReactionManager* m_pOpinionManager; };
struct CViewTarget { unsigned char _pad00[0x50]; float m_displayOffset; };
static CDisplayEngine g_engine;
struct CViewBase {
    virtual void v00() { }
    CThing* m_pThing; unsigned char _pad08[8]; float m_nearOffset; float m_distance; unsigned char _pad18[8]; CViewTarget* m_pTarget;
    CDisplayEngine* GetDisplayEngine() { return &g_engine; }
    float CalculateDisplayDistance()
    {
        if (m_distance >= 0.0f) return m_distance - m_nearOffset;
        CThing* thing = m_pThing;
        COpinionReactionManager* manager = GetDisplayEngine()->m_pOpinionManager;
        float distance = manager->GetToleranceToBeingHit(thing);
        if (m_pTarget) distance += m_pTarget->m_displayOffset;
        return distance - m_nearOffset;
    }
};
int main()
{
    COpinionReactionManager mgr; g_engine.m_pOpinionManager = &mgr; CThing thing; CViewTarget target; target.m_displayOffset = 2.5f;
    CViewBase v; v.m_pThing = &thing; v.m_nearOffset = 1.0f; v.m_distance = 5.0f; v.m_pTarget = &target;
    if (v.CalculateDisplayDistance() != 4.0f || g_asked != 0) { std::printf("00681980_TEST FAIL\n"); return 1; }
    v.m_distance = -1.0f;
    if (v.CalculateDisplayDistance() != 11.5f || g_asked != &thing) { std::printf("00681980_TEST FAIL\n"); return 1; }
    v.m_pTarget = 0;
    if (v.CalculateDisplayDistance() != 9.0f) { std::printf("00681980_TEST FAIL\n"); return 1; }
    std::printf("00681980_TEST PASS\n"); return 0;
}