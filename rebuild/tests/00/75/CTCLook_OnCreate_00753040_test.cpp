#include <cstdio>
struct CLookDefPointer { const void* m_pDef; };
struct CAppearanceDefPointer { const void* m_pDef; };
static int g_lookDef, g_appDef;
class CDefClassBase { public:
    int calls;
    bool GetSubDef(CLookDefPointer& out) { ++calls; out.m_pDef = &g_lookDef; return true; }
    bool GetSubDef(CAppearanceDefPointer& out) { ++calls; out.m_pDef = &g_appDef; return true; }
};
class CThing { public: CDefClassBase* PBaseDef; };
class CTCLook { public:
    CThing* m_pThing; CLookDefPointer PDef; CAppearanceDefPointer PAppearanceDef; int centred; float ex, ey;
    void CentreHead() { ++centred; }
    void ChangeEyeAngles(float a, float b) { ex = a; ey = b; }
    void OnCreate()
    {
        m_pThing->PBaseDef->GetSubDef(PDef);
        m_pThing->PBaseDef->GetSubDef(PAppearanceDef);
        CentreHead();
        ChangeEyeAngles(0.0f, 0.0f);
    }
};
int main()
{
    CDefClassBase def; def.calls = 0; CThing thing; thing.PBaseDef = &def;
    CTCLook l; l.m_pThing = &thing; l.PDef.m_pDef = 0; l.PAppearanceDef.m_pDef = 0; l.centred = 0; l.ex = l.ey = 3.0f;
    l.OnCreate();
    if (def.calls != 2 || l.PDef.m_pDef != &g_lookDef || l.PAppearanceDef.m_pDef != &g_appDef || l.centred != 1 || l.ex != 0.0f || l.ey != 0.0f) { std::printf("00753040_TEST FAIL\n"); return 1; }
    std::printf("00753040_TEST PASS\n");
    return 0;
}