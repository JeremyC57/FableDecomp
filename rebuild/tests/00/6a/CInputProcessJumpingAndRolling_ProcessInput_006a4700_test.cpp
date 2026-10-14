#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static const void* g_e[2]; static const void* g_p[2];
struct CInputEvent { long k; }; struct CProcessedInput { long t; };
struct CInputProcessJumpingAndRolling {
    void ProcessRolling(const CInputEvent& event, CProcessedInput& processed) { std::strcat(g_seq, "R"); g_e[0] = &event; g_p[0] = &processed; }
    void ProcessJumping(const CInputEvent& event, CProcessedInput& processed) { std::strcat(g_seq, "J"); g_e[1] = &event; g_p[1] = &processed; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed) { ProcessRolling(event, processed); ProcessJumping(event, processed); } };
int main()
{
    CInputProcessJumpingAndRolling j; CInputEvent e; CProcessedInput p; j.ProcessInput(e, p);
    if (std::strcmp(g_seq, "RJ") || g_e[0] != &e || g_e[1] != &e || g_p[0] != &p || g_p[1] != &p) { std::printf("006a4700_TEST FAIL\n"); return 1; }
    std::printf("006a4700_TEST PASS\n");
    return 0;
}