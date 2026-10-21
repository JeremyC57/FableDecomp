#include <cstdio>
// behaviour test: UpdateCreatureTrigger -- a triggerer stepping inside the self-trigger radius fires once the reset interval
// has elapsed (and the generator is active); standing still inside does not refire (LastSpaceTest); no triggers left stops it
struct P { float X, Y; };
struct CGen {
    P Me; P Creatures[2]; int NumCreatures; float Radius; int Interval; bool Active, ExternalTrigger, Wait, LastSpaceTest, Undead;
    int NumTriggersLeft, ActivationFrame, AliveCount, Created;
    void UpdateCreatureTrigger(int frame)
    {
        if (Wait && !Undead && AliveCount != 0) ActivationFrame = frame;
        if (!Undead) {
            if (Wait && AliveCount != 0) return;
            if (NumTriggersLeft == 0) return;
        }
        for (int i = 0; i < NumCreatures; ++i) {
            bool spaceTrigger = true, timeTrigger = true;
            if ((frame - ActivationFrame) / 30 < Interval) timeTrigger = false;
            if (!Undead) {
                float dx = Me.X - Creatures[i].X, dy = Me.Y - Creatures[i].Y;
                if (dx * dx + dy * dy < Radius * Radius) {
                    if (LastSpaceTest) spaceTrigger = false;
                    ActivationFrame = frame; LastSpaceTest = true;
                } else { LastSpaceTest = false; spaceTrigger = false; }
            }
            if ((spaceTrigger && timeTrigger && Active) || (ExternalTrigger && timeTrigger)) {
                ActivationFrame = frame;
                if (NumTriggersLeft > 0) NumTriggersLeft--;
                Created++;
                ExternalTrigger = Undead;
                return;
            }
        }
    }
};
int main()
{
    CGen g = { { 0, 0 }, { { 10, 0 }, { 50, 50 } }, 2, 5.0f, 1, true, false, false, false, false, 2, 0, 0, 0 };
    g.UpdateCreatureTrigger(60);                       // outside the radius: nothing
    bool ok = g.Created == 0 && !g.LastSpaceTest;
    g.Creatures[0].X = 3.0f;
    g.UpdateCreatureTrigger(90);                       // steps inside, interval elapsed: fires
    ok = ok && g.Created == 1 && g.NumTriggersLeft == 1 && g.ActivationFrame == 90 && g.LastSpaceTest;
    g.UpdateCreatureTrigger(200);                      // still inside: no refire
    ok = ok && g.Created == 1 && g.ActivationFrame == 200;
    g.ExternalTrigger = true; g.Creatures[0].X = 10.0f;
    g.UpdateCreatureTrigger(260);                      // external trigger on the interval alone
    ok = ok && g.Created == 2 && g.NumTriggersLeft == 0 && !g.ExternalTrigger;
    g.ExternalTrigger = true;
    g.UpdateCreatureTrigger(400);                      // no triggers left
    ok = ok && g.Created == 2;
    if (!ok) { std::printf("0074baa0_TEST FAIL\n"); return 1; }
    std::printf("0074baa0_TEST PASS\n");
    return 0;
}