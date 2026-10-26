#include <cstdio>
// behaviour test: CQ_GuildTrainingWoodsDepartureScript::CScorpionHome::Main -- wait for point 1, then keep at most
// three scorpions alive, spawning from ScorpionsLeft; when none are alive and none are left, move to point 2
static int g_frame = 0, g_alive = 0, g_spawned = 0, g_lastCounter = -99, g_objective = 0;
struct Home {
    long point; long ScorpionsLeft; float beetles;
    bool IsDone() { return g_frame > 500; }
    void Frame() { ++g_frame; if (g_frame == 3) point = 1; if (g_frame % 4 == 0 && g_alive > 0) --g_alive; }
    void Main()
    {
        while (point != 1) { Frame(); if (IsDone()) return; }
        if (IsDone()) return;
        while (point == 1) {
            Frame();
            if (IsDone()) return;
            unsigned int size = (unsigned int)g_alive;
            g_lastCounter = (long)(beetles - ScorpionsLeft - size);
            if (size < 3) {
                if (size == 0 && ScorpionsLeft == 0) { point = 2; ++g_objective; }
                else if (ScorpionsLeft > 0) { ++g_alive; ++g_spawned; --ScorpionsLeft; }
            }
        }
    }
};
int main()
{
    Home h; h.point = 0; h.ScorpionsLeft = 6; h.beetles = 6.0f;
    h.Main();
    if (h.point != 2 || g_spawned != 6 || g_objective != 1 || g_lastCounter != 6 || g_frame > 500) {
        std::printf("00d643a0_TEST FAIL %ld %d %d %d\n", h.point, g_spawned, g_lastCounter, g_frame); return 1;
    }
    std::printf("00d643a0_TEST PASS\n");
    return 0;
}