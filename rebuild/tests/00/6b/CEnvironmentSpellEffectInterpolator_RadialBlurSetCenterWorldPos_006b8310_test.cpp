#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0; static void* g_pos[4]; static void* g_arg[4]; static long g_flag[4];
struct CScreenEffectPriorityStack { void SetAreaCenter(void* p, void* pos) { g_pos[g_n] = p; g_arg[g_n] = pos; g_seq[g_n++] = 'A'; } void SetAreaCenterB(void* p, void* pos) { g_pos[g_n] = p; g_arg[g_n] = pos; g_seq[g_n++] = 'B'; }
    void SetAreaTrackPos(void* p, long f) { g_pos[g_n] = p; g_flag[g_n] = f; g_seq[g_n++] = 'C'; } void SetAreaTrackPosB(void* p, long f) { g_pos[g_n] = p; g_flag[g_n] = f; g_seq[g_n++] = 'D'; } };
struct CEnvironmentSpellEffectInterpolator { unsigned char _pad[0x90]; CScreenEffectPriorityStack m_radialBlurStack;
    void RadialBlurSetCenterWorldPos(void* position, void* pos) { m_radialBlurStack.SetAreaCenter(position, pos); m_radialBlurStack.SetAreaCenterB(position, pos); m_radialBlurStack.SetAreaTrackPos(position, 1); m_radialBlurStack.SetAreaTrackPosB(position, 1); } };
int main() {
    CEnvironmentSpellEffectInterpolator e; int p, q; e.RadialBlurSetCenterWorldPos(&p, &q); if (std::strcmp(g_seq, "ABCD") || g_pos[0] != &p || g_pos[3] != &p || g_arg[1] != &q || g_flag[2] != 1 || g_flag[3] != 1) { std::printf("006b8310_TEST FAIL\n"); return 1; }
    std::printf("006b8310_TEST PASS\n"); return 0;
}