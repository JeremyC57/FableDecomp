// Behaviour test for NCameraModes::CProjectileWeapon's constructor (0x00879620): the base gets the manager and def,
// ShotFiredTimer = -1, not finished, no attractors, the two zoom-in FOV limits stored.
#include <cstdio>
struct CGameCameraManager { int id; };
struct CDefPointer { const void* m_pDef; };
static CGameCameraManager* g_baseManager = 0; static const CDefPointer* g_baseDef = 0;
struct CBase { CBase(CGameCameraManager& m, const CDefPointer& d) { g_baseManager = &m; g_baseDef = &d; } virtual ~CBase() { } };
struct CVec { void* m_pFirst; void* m_pLast; void* m_pEnd; CVec() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CProjectileWeapon : CBase {
    long ShotFiredTimer; bool Finished; CVec Attractors; float MaxZoomInFOV; float MinZoomInFOV;
    CProjectileWeapon(CGameCameraManager& m, const CDefPointer& d, float maxFov, float minFov)
        : CBase(m, d), ShotFiredTimer(-1), Finished(false), MaxZoomInFOV(maxFov), MinZoomInFOV(minFov) { }
};
int main()
{
    CGameCameraManager mgr; mgr.id = 1; CDefPointer def; def.m_pDef = &mgr;
    CProjectileWeapon w(mgr, def, 0.9f, 0.4f);
    bool ok = g_baseManager == &mgr && g_baseDef == &def && w.ShotFiredTimer == -1 && !w.Finished && w.Attractors.m_pFirst == 0
           && w.Attractors.m_pLast == 0 && w.Attractors.m_pEnd == 0 && w.MaxZoomInFOV == 0.9f && w.MinZoomInFOV == 0.4f;
    if (!ok) { std::printf("00879620_TEST FAIL\n"); return 1; }
    std::printf("00879620_TEST PASS\n");
    return 0;
}
