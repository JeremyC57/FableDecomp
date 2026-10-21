// Behaviour test for the AI-library CInstance check (0x00C388A0): the two assert messages and the streamer result.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* file, long line, const char* msg)
{
    char buf[96];
    std::sprintf(buf, "%s(%ld) : %s\n", file, line, msg);
    std::strcat(g_log, buf);
}
struct Streamer { long ready, result; long IsReady() { return ready; } long Stream() { return result; } };
struct Instance {
    long m_state; Streamer* m_pFrameStreamer;
    long CheckFrameStreamer()
    {
        if (!m_pFrameStreamer) Log(".\\CInstance.cpp", 8, "Null frame streamer");
        if (m_state != 2) Log(".\\CInstance.cpp", 9, "ESTATE");
        long ready = m_pFrameStreamer->IsReady();
        if (!ready) return ready;
        return m_pFrameStreamer->Stream();
    }
};
int main()
{
    Streamer s = { 1, 42 }; Instance a = { 2, &s };
    g_log[0] = 0;
    if (a.CheckFrameStreamer() != 42 || g_log[0] != 0) { std::printf("00c388a0_TEST FAIL\n"); return 1; }
    Instance b = { 3, &s }; s.ready = 0;
    if (b.CheckFrameStreamer() != 0 || std::strcmp(g_log, ".\\CInstance.cpp(9) : ESTATE\n") != 0) { std::printf("00c388a0_TEST FAIL\n"); return 1; }
    std::printf("00c388a0_TEST PASS\n");
    return 0;
}