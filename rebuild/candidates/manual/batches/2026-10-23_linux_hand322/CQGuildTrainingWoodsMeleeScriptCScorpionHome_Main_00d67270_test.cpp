#include <cstdio>
// behaviour test: CQ_GuildTrainingWoodsMeleeScript::CScorpionHome::Main -- nag about the stick when the timer runs out
// (re-armed for 8), keep at most three scorpions alive, and finish when all are gone
static int g_frame = 0, g_alive = 0, g_spawned = 0, g_nags = 0;
struct Home {
    bool ScorpionsAlive; bool ScorpionsDestroyed; long ScorpionsLeft; long timerEnds; bool carrying;
    bool IsDone() { return g_frame > 500; }
    void Frame() { ++g_frame; if (g_frame == 20) carrying = true; if (g_frame % 4 == 0 && g_alive > 0) --g_alive; }
    void Main()
    {
        timerEnds = g_frame + 5;
        while (ScorpionsAlive) {
            Frame();
            if (IsDone()) return;
            if (!carrying && timerEnds - g_frame <= 0) { ++g_nags; timerEnds = g_frame + 8; }
            unsigned int size = (unsigned int)g_alive;
            if (size < 3) {
                if (size == 0 && ScorpionsLeft == 0) { ScorpionsAlive = false; ScorpionsDestroyed = true; }
                else if (ScorpionsLeft > 0) { ++g_alive; ++g_spawned; --ScorpionsLeft; }
            }
        }
    }
};
int main()
{
    Home h; h.ScorpionsAlive = true; h.ScorpionsDestroyed = false; h.ScorpionsLeft = 5; h.carrying = false;
    h.Main();
    if (h.ScorpionsAlive || !h.ScorpionsDestroyed || g_spawned != 5 || g_nags != 2) {
        std::printf("00d67270_TEST FAIL %d %d\n", g_spawned, g_nags); return 1;
    }
    std::printf("00d67270_TEST PASS\n");
    return 0;
}
