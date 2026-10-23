#include <cstdio>
#include <cstring>
// behaviour test: CQ_WaspBossScript::DoMission -- gossip setup, five drones at QueenDepositPos1..5, wait for all wasps
// dead, queen hornet created, QueenHornetAttacks set, guildmaster thread spawned, WaspMenace unless failed
static int g_frame = 0, g_drones = 0, g_queen = 0, g_spawned = 0, g_menace = 0, g_gossip = 0, g_lastPos = 0;
static int g_alive = 0;
struct Wasp {
    bool MissionFailed, QueenHornetAttacks;
    bool IsActiveThreadTerminating() { return g_frame > 100; }
    void CreateDrone(int pos) { ++g_drones; ++g_alive; g_lastPos = pos; }
    void Frame() { ++g_frame; if (g_alive > 0) --g_alive; }
    void DoMission()
    {
        g_gossip += 3;
        for (long i = 1; i < 6; ++i) {
            if (IsActiveThreadTerminating()) return;
            CreateDrone((int)i);
        }
        if (IsActiveThreadTerminating()) return;
        while (g_alive != 0) { Frame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        ++g_queen;
        QueenHornetAttacks = true;
        ++g_spawned;
        Frame();
        if (IsActiveThreadTerminating()) return;
        if (!MissionFailed) ++g_menace;
    }
};
int main()
{
    Wasp w; w.MissionFailed = false; w.QueenHornetAttacks = false;
    w.DoMission();
    Wasp f; f.MissionFailed = true; f.QueenHornetAttacks = false;
    f.DoMission();
    if (g_drones != 10 || g_lastPos != 5 || g_queen != 2 || !w.QueenHornetAttacks || g_spawned != 2 || g_menace != 1 || g_gossip != 6) {
        std::printf("00e12580_TEST FAIL %d %d %d %d\n", g_drones, g_queen, g_spawned, g_menace); return 1;
    }
    std::printf("00e12580_TEST PASS\n");
    return 0;
}
