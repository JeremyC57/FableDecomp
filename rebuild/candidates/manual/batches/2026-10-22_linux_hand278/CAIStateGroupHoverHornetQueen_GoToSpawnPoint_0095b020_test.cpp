// Behaviour test for CAIStateGroup_HoverHornetQueen::GoToSpawnPoint (0x0095B020): model copy with scripted stubs.
#include <cstdio>
#include <cstring>

struct V { float X, Y, Z; };
static unsigned int g_seed;
static int g_navType, g_terminated, g_lookCalls, g_moveCalls, g_runnableLeft, g_lookResult, g_moveRunning;
static char g_searched[32];
static int g_rect[4];
static bool g_markerExists;
static V g_marker, g_start, g_creaturePos, g_moveTarget;
static unsigned int rotr13(unsigned int v) { return (v >> 13) | (v << 19); }
static bool Runnable() { if (g_runnableLeft <= 0) return false; --g_runnableLeft; return true; }
struct Queen {
    bool FirstSpawn;
    void GoToSpawnPoint()
    {
        g_navType = 1;
        const V* found = 0;
        int index;
        if (FirstSpawn) { index = 1; FirstSpawn = false; }
        else { g_seed = g_seed * 0x24A1u + 0x24DFu; g_seed = rotr13(g_seed); index = (int)(g_seed % 4u) + 1; }
        char text[32];
        std::sprintf(text, "QueenDepositPos%d", index);
        std::strcpy(g_searched, text);
        int cx = (int)g_creaturePos.X, cy = (int)g_creaturePos.Y;
        g_rect[0] = cx - 60; g_rect[1] = cy - 60; g_rect[2] = cx + 60; g_rect[3] = cy + 60;
        if (g_markerExists) found = &g_marker;
        V target = found ? *found : g_start;
        if (Runnable()) {
            ++g_lookCalls;
            if (g_lookResult != 2) { ++g_terminated; return; }
            if (Runnable()) {
                int result;
                do {
                    ++g_moveCalls; g_moveTarget = target;
                    result = (g_moveRunning-- > 0) ? 4 : 2;
                    if (!Runnable()) { ++g_terminated; return; }
                } while (result == 4);
                g_navType = 0;
                return;
            }
        }
        ++g_terminated;
    }
};
static int fail(int n) { std::printf("0095b020_TEST FAIL %d\n", n); return 1; }
static void reset() { g_terminated = g_lookCalls = g_moveCalls = 0; g_navType = -1; }

int main()
{
    Queen q; q.FirstSpawn = true;
    g_start.X = 7; g_start.Y = 8; g_start.Z = 9; g_marker.X = 1; g_marker.Y = 2; g_marker.Z = 3;
    g_creaturePos.X = 100.7f; g_creaturePos.Y = -20.2f; g_creaturePos.Z = 0;
    // first spawn: deposit 1, marker found, fly there over three ticks, navigator restored
    reset(); g_markerExists = true; g_runnableLeft = 100; g_lookResult = 2; g_moveRunning = 2;
    q.GoToSpawnPoint();
    if (std::strcmp(g_searched, "QueenDepositPos1") != 0 || q.FirstSpawn) return fail(1);
    if (g_rect[0] != 40 || g_rect[1] != -80 || g_rect[2] != 160 || g_rect[3] != 40) return fail(2);
    if (g_moveCalls != 3 || g_navType != 0 || g_terminated != 0 || g_moveTarget.X != 1.0f) return fail(3);
    // later spawns: random 1..4, no marker -> start position
    int seen[5] = { 0, 0, 0, 0, 0 };
    for (unsigned int s = 0; s < 64; ++s) {
        reset(); g_seed = s * 7919u; g_markerExists = false; g_runnableLeft = 100; g_moveRunning = 0;
        q.GoToSpawnPoint();
        int n = g_searched[15] - '0';
        if (n < 1 || n > 4) return fail(4);
        ++seen[n];
        if (g_moveTarget.Z != 9.0f || g_navType != 0) return fail(5);
    }
    if (!seen[1] || !seen[2] || !seen[3] || !seen[4]) return fail(6);
    // look-at fails: terminate, navigator left on hover
    reset(); g_runnableLeft = 100; g_lookResult = 3;
    q.GoToSpawnPoint();
    if (g_terminated != 1 || g_moveCalls != 0 || g_navType != 1) return fail(7);
    // not runnable at all: terminate without looking
    reset(); g_runnableLeft = 0; g_lookResult = 2;
    q.GoToSpawnPoint();
    if (g_terminated != 1 || g_lookCalls != 0) return fail(8);
    // stops being runnable during the move: terminate
    reset(); g_runnableLeft = 3; g_moveRunning = 5;
    q.GoToSpawnPoint();
    if (g_terminated != 1 || g_moveCalls != 2 || g_navType != 1) return fail(9);
    std::printf("0095b020_TEST PASS\n");
    return 0;
}
