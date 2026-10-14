#include <cstdio>
struct C2DVector { float x, y; };
static const C2DVector* g_pos = 0; static float g_t = 0; static bool g_imm = false; static int g_notified = 0;
namespace NUISystem {
struct CComponent { void ChangePosition(const C2DVector& p, float t, bool i) { g_pos = &p; g_t = t; g_imm = i; } };
struct CDraggable : CComponent { virtual void OnPositionChanged() { ++g_notified; }
    void ChangePosition(const C2DVector& p, float t, bool i) { CComponent::ChangePosition(p, t, i); OnPositionChanged(); } }; }
int main() { C2DVector v = { 1, 2 }; NUISystem::CDraggable d; d.ChangePosition(v, 0.5f, true); if (g_pos != &v || g_t != 0.5f || !g_imm || g_notified != 1) { std::printf("0055b9f0_TEST FAIL\n"); return 1; } std::printf("0055b9f0_TEST PASS\n"); return 0; }