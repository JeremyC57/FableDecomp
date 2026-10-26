// Behaviour test for CQ_BanditCampScript::CheckAnyBanditsKilled @ 0x00D032F0: count the hero's kills flagged as bandits
// (bit 2 of each killed entry); the first kill clears the kill-none boast and reaching the def's quota sets kill-many.
#include <cstdio>
#include <vector>
struct Data { bool KillMany, KillNone; };
struct Script {
    Data* D;
    bool BoastBegun;
    float Quota;
    int Frames, MaxFrames, CounterUpdates;
    std::vector<std::vector<int> > KillsPerFrame;
    bool IsActiveThreadTerminating() { return Frames >= MaxFrames; }
    void Run()
    {
        long killedCount = 0;
        BoastBegun = true;
        do {
            const std::vector<int>& killed = KillsPerFrame[Frames];
            for (unsigned int i = 0; i < killed.size(); ++i)
                if (killed[i] & 4)
                    ++killedCount;
            if (D->KillNone && killedCount != 0)
                D->KillNone = false;
            if (!D->KillMany) {
                float killedF = (float)killedCount;
                if (killedF >= Quota) {
                    D->KillMany = true;
                    ++CounterUpdates;
                }
            }
            ++Frames;
        } while (!IsActiveThreadTerminating());
    }
};
int main()
{
    Data d = { false, true };
    Script s;
    s.D = &d; s.BoastBegun = false; s.Quota = 3.0f; s.Frames = 0; s.MaxFrames = 4; s.CounterUpdates = 0;
    s.KillsPerFrame.resize(4);
    s.KillsPerFrame[1].push_back(1);
    s.KillsPerFrame[2].push_back(4); s.KillsPerFrame[2].push_back(5);
    s.KillsPerFrame[3].push_back(6);
    s.Run();
    bool ok = s.BoastBegun && !d.KillNone && d.KillMany && s.CounterUpdates == 1;
    Data d2 = { false, true };
    Script t = s;
    t.D = &d2; t.Frames = 0; t.MaxFrames = 3; t.CounterUpdates = 0;
    t.Run();
    ok = ok && !d2.KillNone && !d2.KillMany && t.CounterUpdates == 0;
    if (!ok) { std::printf("00d032f0_TEST FAIL\n"); return 1; }
    std::printf("00d032f0_TEST PASS\n");
    return 0;
}