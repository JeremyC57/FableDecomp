// Behaviour test for CPlayer::InitModeLightning @ 0x00636480: attach the mode's 12 input processes, flag Z-targeting, drop a carried
// thing when the creature is in mode 0x22 with no pending carry actions, enter creature mode 25 (re-adding mode 12 when it
// was current), then the camera Z-target push and the targeting lock pick (player mode 2 only).
#include <cstdio>
#include <vector>
struct Modes {
    int Current;
    std::vector<int> Log;
    void AddMode(int m) { Log.push_back(m); }
    void RemoveMode(int m) { Log.push_back(-m); }
};
struct Carrying { bool Pending; int Dropped; };
struct Player {
    int Attached;
    bool ZTargeting, Aggressive;
    int CameraPushes, LockPicks;
    void Init(int processes, bool lightning, int mode, Modes& modes, Carrying& carrying, bool hasCamera)
    {
        for (int i = 0; i < processes; ++i)
            ++Attached;
        ZTargeting = true;
        if (modes.Current == 0x22 && !carrying.Pending)
            ++carrying.Dropped;
        int current = modes.Current;
        if (current != 25)
            modes.AddMode(25);
        if (current == 12) {
            modes.RemoveMode(current);
            modes.AddMode(current);
        }
        if (lightning) {
            if (mode == 2)
                ++LockPicks;
            if (hasCamera)
                ++CameraPushes;
        } else {
            Aggressive = true;
            if (hasCamera)
                ++CameraPushes;
            ++LockPicks;
        }
    }
};
int main()
{
    Player p = { 0, false, false, 0, 0 };
    Modes m;
    m.Current = 12;
    Carrying c = { false, 0 };
    p.Init(12, 1 != 0, 2, m, c, true);
    bool ok = p.Attached == 12 && p.ZTargeting && m.Log.size() == 3 && m.Log[0] == 25 && m.Log[1] == -12 && m.Log[2] == 12 &&
              c.Dropped == 0 && p.CameraPushes == 1 && p.LockPicks == 1;
    Modes m2;
    m2.Current = 0x22;
    p.Init(12, 1 != 0, 1, m2, c, false);
    ok = ok && c.Dropped == 1 && m2.Log.size() == 1 && p.CameraPushes == 1 && p.LockPicks == (1 ? 1 : 2);
    Modes m3;
    m3.Current = 25;
    c.Pending = true;
    m3.Current = 25;
    p.Init(12, 1 != 0, 2, m3, c, true);
    ok = ok && m3.Log.empty() && c.Dropped == 1;
    if (!ok) { std::printf("00636480_TEST FAIL\n"); return 1; }
    std::printf("00636480_TEST PASS\n");
    return 0;
}