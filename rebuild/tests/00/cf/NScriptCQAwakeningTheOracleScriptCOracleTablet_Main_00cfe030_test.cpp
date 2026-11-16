// Behaviour test for CQ_AwakeningTheOracleScript::COracleTablet::Main (0x00CFE030) (src/00cfe030.cpp, RELOCATION_MATCH)
// Models the function's decisions: the minimap marker (green orb only for the BAD_ONE /
// GOOD_ONE data strings), the sparkle keep-alive loop (re-create when the 4 s timer ran out and the effect died; retire the spot once
// three tablets are collected: marker removed once, spot erased from DiggingPoints), the dig reward (first of Y / M / C that is not
// yet given and that the hero carries: expression, flag, ++TabletsCollected), the cursed tablet (message shown once, creatures
// spawned at the spawn points until TotalSpawnedCreaturesAlive exceeds ATO_MaxSpawnedCreatures, named creature unless the data
// string is "NULL" or empty), AlreadyDugHere, the final erase from DiggingPoints, and termination at every wait.
#include <cstdio>
#include <cstring>

struct Parent {
    long TabletsCollected; bool GotY, GotM, GotC, DisplayedBadTabletMessage; long TotalSpawnedCreaturesAlive;
    long DiggingPoints[4]; long NumDiggingPoints;
    void Erase(long id)
    {
        for (long i = 0; i < NumDiggingPoints; ++i) {
            if (DiggingPoints[i] == id) {
                for (long j = i; j + 1 < NumDiggingPoints; ++j) DiggingPoints[j] = DiggingPoints[j + 1];
                --NumDiggingPoints;
                break;
            }
        }
    }
};

struct Tablet {
    Parent* P; long Id; const char* Data; bool AlreadyDugHere;
    long Frame, TerminateAt, EnabledUntil;      // script clock: the dig happens when the spot stops being enabled
    bool HeroHas[3];                             // Y, M, C tablets in the hero's possession
    const char* SpawnData[3]; long NumSpawn; long MaxSpawned;
    // observations
    const char* Marker; long MarkerRemoved, SparklesCreated, Named, Dreadwings, Messages, Expression; bool Faded, CursedRemoved;
    long TimerLeft; bool SparklesAlive;

    bool T() const { return Frame >= TerminateAt; }
    void Main()
    {
        if (T()) return;
        if (!strcmp(Data, "BAD_ONE") || !strcmp(Data, "GOOD_ONE")) Marker = "HUD_ORB_GREEN_SMALL";
        ++SparklesCreated;
        bool removed = false;
        while (Frame < EnabledUntil) {
            ++Frame; if (TimerLeft > 0) --TimerLeft;
            if (T()) return;
            if (TimerLeft == 0 && !SparklesAlive) { ++SparklesCreated; TimerLeft = 4; }
            if (!removed && P->TabletsCollected > 2) {
                ++MarkerRemoved; removed = true;
                P->Erase(Id);
                Faded = true;
            }
        }
        if (T()) return;
        ++MarkerRemoved;
        if (!AlreadyDugHere && strcmp(Data, "BAD_ONE")) {
            bool* got[3] = { &P->GotY, &P->GotM, &P->GotC };
            for (long i = 0; i < 3; ++i) {
                if (!*got[i] && HeroHas[i]) {
                    Expression = i; *got[i] = true; HeroHas[i] = false; ++Messages;
                    ++Frame; if (T()) return;        // wait for the info text to be clicked past
                    ++P->TabletsCollected;
                    break;
                }
            }
        } else if (!AlreadyDugHere && !strcmp(Data, "BAD_ONE")) {
            if (!P->DisplayedBadTabletMessage) {
                ++Messages;
                ++Frame; if (T()) return;
                P->DisplayedBadTabletMessage = true;
            }
            for (long i = 0; i < NumSpawn; ++i) {
                if (P->TotalSpawnedCreaturesAlive > MaxSpawned) break;
                if (strcmp(SpawnData[i], "NULL") && strcmp(SpawnData[i], "")) ++Named; else ++Dreadwings;
            }
            CursedRemoved = true;
        }
        AlreadyDugHere = true;
        P->Erase(Id);
        Faded = true;
    }
};

static Tablet Make(Parent* p, long id, const char* data)
{
    Tablet t; memset(&t, 0, sizeof t);
    t.P = p; t.Id = id; t.Data = data; t.TerminateAt = 1000; t.EnabledUntil = 6; t.Expression = -1; t.MaxSpawned = 5;
    t.SpawnData[0] = "CREATURE_HOBBE"; t.SpawnData[1] = "NULL"; t.SpawnData[2] = ""; t.NumSpawn = 3;
    return t;
}

int main()
{
    Parent p = { 0, false, false, false, false, 0, { 7, 8, 9, 0 }, 3 };
    // a good tablet: the hero carries M and C, Y is not given yet -> M is taken first (Y skipped: not carried)
    Tablet a = Make(&p, 8, "GOOD_ONE"); a.HeroHas[1] = a.HeroHas[2] = true; a.Main();
    bool ok = a.Marker && !strcmp(a.Marker, "HUD_ORB_GREEN_SMALL") && a.Expression == 1 && p.GotM && !p.GotC && p.TabletsCollected == 1 && a.AlreadyDugHere
        && p.NumDiggingPoints == 2 && p.DiggingPoints[0] == 7 && p.DiggingPoints[1] == 9 && a.Faded && a.MarkerRemoved == 1
        && a.SparklesCreated == 3 && a.Named == 0 && !a.CursedRemoved;
    // an unmarked spot: no marker, nothing carried -> no reward, still dug and erased
    Tablet b = Make(&p, 7, "PLAIN"); b.SparklesAlive = true; b.Main();
    ok = ok && b.Marker == 0 && b.Expression == -1 && p.TabletsCollected == 1 && p.NumDiggingPoints == 1 && b.SparklesCreated == 1;
    // the cursed tablet: message once, one named creature and two dreadwings, cursed tablets removed
    Tablet c = Make(&p, 9, "BAD_ONE"); c.Main();
    ok = ok && c.Messages == 1 && p.DisplayedBadTabletMessage && c.Named == 1 && c.Dreadwings == 2 && c.CursedRemoved && c.Expression == -1 && p.NumDiggingPoints == 0;
    // a second cursed tablet: no second message, and no spawn once the alive count exceeds the maximum
    p.TotalSpawnedCreaturesAlive = 6; p.DiggingPoints[0] = 4; p.NumDiggingPoints = 1;
    Tablet d = Make(&p, 4, "BAD_ONE"); d.Main();
    ok = ok && d.Messages == 0 && d.Named == 0 && d.Dreadwings == 0 && d.CursedRemoved;
    // three tablets collected while the spot is enabled: marker removed in the loop (once) and again after it, spot erased early
    p.TabletsCollected = 3; p.DiggingPoints[0] = 5; p.NumDiggingPoints = 1;
    Tablet e = Make(&p, 5, "GOOD_ONE"); e.AlreadyDugHere = true; e.HeroHas[0] = true; e.Main();
    ok = ok && e.MarkerRemoved == 2 && e.Expression == -1 && p.TabletsCollected == 3 && p.NumDiggingPoints == 0;
    // termination inside the sparkle loop: nothing after it runs
    p.DiggingPoints[0] = 6; p.NumDiggingPoints = 1; p.TabletsCollected = 0;
    Tablet f = Make(&p, 6, "GOOD_ONE"); f.TerminateAt = 3; f.HeroHas[0] = true; f.Main();
    ok = ok && f.Expression == -1 && !f.AlreadyDugHere && p.NumDiggingPoints == 1 && f.MarkerRemoved == 0;
    if (!ok) { std::printf("00cfe030_TEST FAIL\n"); return 1; }
    std::printf("00cfe030_TEST PASS\n");
    return 0;
}