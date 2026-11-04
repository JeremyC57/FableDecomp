// Behaviour test for 0x0095D0E0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Group {
    bool FirePointAlive, EnemyAlive, Stopping; long TimeOutCounter; std::vector<long> Shots; long WorldFrame; long Fps;
    bool IsStillRunnable() {
        if (FirePointAlive && EnemyAlive && !Stopping) {
            long lastShot = TimeOutCounter;
            if (lastShot > 0) {
                for (size_t i = 0; i < Shots.size(); ++i) if (Shots[i] > lastShot) { TimeOutCounter = Shots[i]; lastShot = Shots[i]; }
                if (WorldFrame - TimeOutCounter > Fps * 5) return false;
            }
            return true;
        }
        return false;
    }
};
int main() {
    Group g; g.FirePointAlive = g.EnemyAlive = true; g.Stopping = false; g.TimeOutCounter = 0; g.WorldFrame = 1000; g.Fps = 30;
    bool ok = g.IsStillRunnable();
    g.TimeOutCounter = 100; ok = ok && !g.IsStillRunnable();
    g.Shots.push_back(50); g.Shots.push_back(900); ok = ok && g.IsStillRunnable() && g.TimeOutCounter == 900;
    g.Stopping = true; ok = ok && !g.IsStillRunnable();
    std::printf(ok ? "0095d0e0_TEST PASS\n" : "0095d0e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
