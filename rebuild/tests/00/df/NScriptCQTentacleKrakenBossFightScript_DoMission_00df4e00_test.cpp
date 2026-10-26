// Behaviour test for 00df4e00 (CQ_TentacleKrakenBossFightScript::DoMission): waits for the KrakenChamber level, takes
// the auto-save only once, spawns WatchForLeaving, runs phase one, and succeeds unless the mission already failed.
#include <cstdio>
#include <cstring>

struct Model {
    int frames, loadedAt, termAt, saves, spawns, phaseOne, musicStops; bool Succeeded, Failed, AutoSaved;
    char spawnedName[32];
    bool Term() { return frames >= termAt; }
    bool IsLevelLoaded(const char* name) { return std::strcmp(name, "KrakenChamber") == 0 && frames >= loadedAt; }
    void Frame() { ++frames; }
    void Spawn(const char* name) { ++spawns; std::strcpy(spawnedName, name); }
    void DoMission()
    {
        while (!IsLevelLoaded("KrakenChamber")) { Frame(); if (Term()) return; }
        if (Term()) return;
        if (!AutoSaved) { if (Term()) return; AutoSaved = true; ++saves; }
        Spawn("WatchForLeaving");
        ++phaseOne;
        Frame();
        if (Term()) return;
        if (!Failed) { ++musicStops; Succeeded = true; }
    }
};
int main()
{
    Model m; std::memset(&m, 0, sizeof m); m.loadedAt = 3; m.termAt = 100;
    m.DoMission();
    int ok = m.saves == 1 && m.spawns == 1 && std::strcmp(m.spawnedName, "WatchForLeaving") == 0 && m.phaseOne == 1 && m.Succeeded && m.musicStops == 1;
    m.Succeeded = false; m.Failed = true; m.DoMission();
    if (m.saves != 1 || m.Succeeded || m.spawns != 2) ok = 0;
    Model n; std::memset(&n, 0, sizeof n); n.loadedAt = 50; n.termAt = 5; n.DoMission();
    if (n.spawns != 0 || n.saves != 0) ok = 0;
    if (!ok) { std::printf("00df4e00_TEST FAIL\n"); return 1; }
    std::printf("00df4e00_TEST PASS\n");
    return 0;
}