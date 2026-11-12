// Behaviour test for NCreatureMode::CModeDragonFireBeam::UpdateBeam (0x008DB810): a copy of the logic over a small model.
#include <cstdio>
#include <cstring>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V operator*(float s) const { return V(X * s, Y * s, Z * s); }
    V operator+(const V& o) const { return V(X + o.X, Y + o.Y, Z + o.Z); }
    V& operator+=(const V& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; } };
struct Def { long ref; long FramesBetweenFireBeamUpdate; };
struct Thing { bool hasExplosionTC; int immuneSet; };
static long g_frame; static Def g_def; static int g_released; static bool g_hasCamMgr, g_hit; static V g_hitPos;
static V g_lastFrom, g_lastTo; static int g_creates; static long g_idx[2]; static V g_pos[2]; static Thing g_things[2]; static bool g_null[2];
static long GetIndex(const char* n) { return std::strcmp(n, "DRAGON_FIREBEAM_EXPLOSION") == 0 ? 11 : (std::strcmp(n, "DRAGON_FIREBEAM_EXPLOSION_INVISIBLE") == 0 ? 22 : -1); }
static Thing* Create(long idx, const V& p) { int i = g_creates++; g_idx[i] = idx; g_pos[i] = p; return g_null[i] ? 0 : &g_things[i]; }
static bool Ray(const V& f, const V& t, V& hit, V& n) { g_lastFrom = f; g_lastTo = t; if (g_hit) { hit = g_hitPos; n = V(0, 0, 1); } return g_hit; }
struct Mode { long LastExplodeFrame; bool BeamActive;
    void UpdateBeam() {
        if (!BeamActive) return;
        Def* def = &g_def; ++def->ref;
        if (g_frame - LastExplodeFrame > def->FramesBetweenFireBeamUpdate) {
            V mouth(1, 2, 3); V up(0, 0, 1);
            V end = up * -1.0f * 1000.0f; end += mouth;
            V hitPos, hitNormal;
            if (g_hasCamMgr && Ray(mouth, end, hitPos, hitNormal)) {
                long index = GetIndex("DRAGON_FIREBEAM_EXPLOSION");
                Thing* e = Create(index, hitPos);
                if (e) { if (e->hasExplosionTC) e->immuneSet++; LastExplodeFrame = g_frame; }
                V middle = (hitPos + mouth) * 0.5f;
                index = GetIndex("DRAGON_FIREBEAM_EXPLOSION_INVISIBLE");
                Thing* inv = Create(index, middle);
                if (inv && inv->hasExplosionTC) inv->immuneSet++;
            }
        }
        if (--def->ref == 0) g_released++;
    } };
static void Reset() { g_creates = 0; g_released = 0; std::memset(g_things, 0, sizeof g_things); g_null[0] = g_null[1] = false; g_def.ref = 0; }
int main()
{
    Mode m; m.BeamActive = false; m.LastExplodeFrame = 0; g_frame = 100; g_def.FramesBetweenFireBeamUpdate = 10;
    Reset(); m.UpdateBeam(); if (g_released != 0 || g_creates != 0) { std::printf("008db810_TEST FAIL\n"); return 1; }
    m.BeamActive = true; g_hasCamMgr = true; g_hit = true; g_hitPos = V(1, 2, -97);
    g_things[0].hasExplosionTC = true; g_things[1].hasExplosionTC = true;
    m.UpdateBeam();
    if (g_creates != 2 || g_idx[0] != 11 || g_idx[1] != 22 || m.LastExplodeFrame != 100 || g_released != 1
        || g_lastTo.Z != -997.0f || g_lastTo.X != 1.0f || g_pos[1].Z != -47.0f || g_pos[1].Y != 2.0f
        || g_things[0].immuneSet != 1 || g_things[1].immuneSet != 1) { std::printf("008db810_TEST FAIL\n"); return 1; }
    Reset(); g_frame = 105; m.UpdateBeam();   // too soon: no ray
    if (g_creates != 0 || g_released != 1) { std::printf("008db810_TEST FAIL\n"); return 1; }
    Reset(); g_frame = 200; g_null[0] = true; g_things[1].hasExplosionTC = true; m.UpdateBeam();   // first explosion fails: no frame stamp, invisible still made
    if (g_creates != 2 || m.LastExplodeFrame != 100 || g_things[1].immuneSet != 1) { std::printf("008db810_TEST FAIL\n"); return 1; }
    Reset(); g_hit = false; m.UpdateBeam();
    if (g_creates != 0 || g_released != 1) { std::printf("008db810_TEST FAIL\n"); return 1; }
    std::printf("008db810_TEST PASS\n");
    return 0;
}