#include <cstdio>
#include <cstdlib>
#include <cstring>
class CGameCameraManager;
class CCameraModeDef { public: unsigned long m_flags; float m_turnDegrees; unsigned long m_style; };
struct CDefPointer { const CCameraModeDef* m_pDef; };
struct CBase { float m_turnFraction; unsigned long m_style; unsigned long m_flags; };
static CBase g_mode;
struct CBaseInit {
    virtual CBase* Alloc(CGameCameraManager*, CDefPointer&) { return &g_mode; }
    CBase* AllocMain(CGameCameraManager* manager, CDefPointer& def)
    {
        CBase* mode = Alloc(manager, def);
        mode->m_flags = def.m_pDef->m_flags;
        mode->m_turnFraction = def.m_pDef->m_turnDegrees * (1.0f / 360.0f);
        mode->m_style = def.m_pDef->m_style;
        return mode;
    }
};
static bool Check()
{
    CCameraModeDef d; d.m_flags = 0x11; d.m_turnDegrees = 180.0f; d.m_style = 3;
    CDefPointer dp; dp.m_pDef = &d;
    CBaseInit init; CBase* m = init.AllocMain(0, dp);
    if (m != &g_mode || m->m_flags != 0x11 || m->m_style != 3) return false;
    return m->m_turnFraction > 0.49f && m->m_turnFraction < 0.51f;
}
int main()
{
    if (!Check()) { std::printf("0084d270_TEST FAIL\n"); return 1; }
    std::printf("0084d270_TEST PASS\n");
    return 0;
}