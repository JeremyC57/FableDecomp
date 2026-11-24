#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test: CPlayerGui::AddHitByMelee -- the def's smack effect is started (not auto-started) at the hit position,
// oriented up (0,0,1) / forward (0,1,0); attacker and victim are unused.
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
class CPlayerGuiDef { public: long SmackEffect; };
static CPlayerGuiDef g_def = { 42 };
static int g_peeks = 0;
const CPlayerGuiDef* __fastcall PeekGuiDef() { ++g_peeks; return &g_def; }
namespace NParticleEngine {
class CParticleEmitter {};
class CParticleEmitterManager {
public:
    int Created; long Effect; float PX, PY, PZ; bool Start; float UpZ, FwdY, UpX, FwdZ;
    static CParticleEmitterManager* Get();
    CParticleEmitter* CreateEmitter(long effect, const C3DVector& pos, bool start, const CRightHandedSet& o)
    {
        ++Created; Effect = effect; PX = pos.X; PY = pos.Y; PZ = pos.Z; Start = start;
        UpX = o.Up.X; UpZ = o.Up.Z; FwdY = o.Forward.Y; FwdZ = o.Forward.Z;
        return 0;
    }
};
static CParticleEmitterManager g_mgr;
CParticleEmitterManager* CParticleEmitterManager::Get() { return &g_mgr; }
}
class CThing { int Unused; };
class CPlayerGui {
public:
    void AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos)
    {
        const CPlayerGuiDef* def = PeekGuiDef();
        NParticleEngine::CParticleEmitterManager::Get()->CreateEmitter(def->SmackEffect, pos, false,
            CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
    }
};
int main()
{
    NParticleEngine::g_mgr.Created = 0; NParticleEngine::g_mgr.Start = true;
    CPlayerGui gui; CThing a, v;
    C3DVector pos(1.0f, 2.0f, 3.0f);
    gui.AddHitByMelee(a, v, pos);
    NParticleEngine::CParticleEmitterManager& m = NParticleEngine::g_mgr;
    if (m.Created != 1 || g_peeks != 1 || m.Effect != 42 || m.Start) { std::printf("00643d8c_TEST FAIL\n"); return 1; }
    if (m.PX != 1.0f || m.PY != 2.0f || m.PZ != 3.0f) { std::printf("00643d8c_TEST FAIL\n"); return 1; }
    if (m.UpZ != 1.0f || m.UpX != 0.0f || m.FwdY != 1.0f || m.FwdZ != 0.0f) { std::printf("00643d8c_TEST FAIL\n"); return 1; }
    std::printf("00643d8c_TEST PASS\n");
    return 0;
}