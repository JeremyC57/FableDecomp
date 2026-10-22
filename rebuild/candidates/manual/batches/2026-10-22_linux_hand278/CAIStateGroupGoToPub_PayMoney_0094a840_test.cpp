// Behaviour test for CAIStateGroup_GoToPub::PayMoney (0x0094A840): model copy with scripted stubs.
#include <cstdio>

static int g_frame, g_fps, g_replyAt, g_stopAt, g_freed, g_played, g_removed;
static int g_msgType[8], g_msgIndex[8], g_msgCount;
static bool g_barHasBarman;
struct Pub {
    bool barman;
    int barIndex;
    int AddMessage(int type, int index) { g_msgType[g_msgCount] = type; g_msgIndex[g_msgCount] = index; return ++g_msgCount; }
    bool Reply() { ++g_frame; return g_replyAt >= 0 && g_frame >= g_replyAt; }
    bool IsStopping() { return g_stopAt >= 0 && g_frame >= g_stopAt; }
    bool PayMoney()
    {
        if (!barman) {
            barman = g_barHasBarman;
            if (!barman) { ++g_freed; return false; }
        }
        int request = AddMessage(0x30, barIndex);
        long timeout = g_frame + g_fps * 20;
        while (!Reply()) {
            if (g_frame > timeout || IsStopping()) {
                if (request == 1) ++g_removed;
                AddMessage(0x31, barIndex);
                ++g_freed;
                return false;
            }
        }
        ++g_played;
        AddMessage(0x30, 0);
        ++g_freed;
        return true;
    }
};
static int fail(int n) { std::printf("0094a840_TEST FAIL %d\n", n); return 1; }
static void reset(Pub& p) { p.barman = true; p.barIndex = 4; g_frame = 0; g_fps = 30; g_replyAt = 10; g_stopAt = -1; g_freed = g_played = g_removed = g_msgCount = 0; g_barHasBarman = true; }
int main()
{
    Pub p;
    reset(p);
    if (!p.PayMoney() || g_msgCount != 2 || g_msgType[0] != 0x30 || g_msgIndex[0] != 4 || g_msgIndex[1] != 0 || g_played != 1 || g_freed != 1) return fail(1);
    reset(p); g_replyAt = -1;
    if (p.PayMoney() || g_msgCount != 2 || g_msgType[1] != 0x31 || g_msgIndex[1] != 4 || g_removed != 1 || g_frame != 601 || g_played != 0) return fail(2);
    reset(p); g_replyAt = -1; g_stopAt = 5;
    if (p.PayMoney() || g_frame != 5 || g_freed != 1) return fail(3);
    reset(p); p.barman = false; g_barHasBarman = false;
    if (p.PayMoney() || g_msgCount != 0 || g_freed != 1) return fail(4);
    reset(p); p.barman = false;
    if (!p.PayMoney() || !p.barman) return fail(5);
    std::printf("0094a840_TEST PASS\n");
    return 0;
}
