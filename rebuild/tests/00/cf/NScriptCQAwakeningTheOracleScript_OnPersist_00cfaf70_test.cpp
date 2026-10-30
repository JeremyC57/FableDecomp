// Behaviour test for NScript::CQ_AwakeningTheOracleScript::OnPersist (0x00CFAF70): the ten Transfer calls in retail
// order with their keys, members and defaults, through a recording persist context.
#include <cstdio>
#include <cstring>
class CPersistContext { public:
    const char* names[16]; void* values[16]; long defaults[16]; int count; bool loading;
    CPersistContext() : count(0), loading(false) {}
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue)
    {
        names[count] = name; values[count] = &value; defaults[count] = (long)defaultValue; ++count;
        if (loading) value = (T)(count * 3);
    }
};
struct Oracle {
    bool MissionSucceeded, PlayedOpeningCS, PlayedLostBayCS, PlayedNecroCS;
    long TabletsCollected; bool SpawnedFinalSpotsStuff; long PositionTaken; bool GivenDiggingSpotsRewards;
    long GraveNumbersGivenOut; long FinalSummonersAlive; bool DisplayedBadTabletMessage, PlayedOpeningNecroCS;
    long TotalSpawnedCreaturesAlive; bool AutosavedInNW3;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("PlayedLostBayCS", PlayedLostBayCS, false);
        context.Transfer("PlayedOpeningCS", PlayedOpeningCS, false);
        context.Transfer("PlayedNecroCS", PlayedNecroCS, false);
        context.Transfer("TabletsCollected", TabletsCollected, 0L);
        context.Transfer("SpawnedFinalSpotsStuff", SpawnedFinalSpotsStuff, false);
        context.Transfer("GivenDiggingSpotsRewards", GivenDiggingSpotsRewards, false);
        context.Transfer("GraveNumbersGivenOut", GraveNumbersGivenOut, 0L);
        context.Transfer("DisplayedBadTabletMessage", DisplayedBadTabletMessage, false);
        context.Transfer("PlayedOpeningNecroCS", PlayedOpeningNecroCS, false);
        context.Transfer("AutosavedInNW3", AutosavedInNW3, false);
    }
};
int main()
{
    Oracle q; std::memset(&q, 0, sizeof(q));
    CPersistContext c; c.loading = true;
    q.OnPersist(c);
    const char* want[] = { "PlayedLostBayCS", "PlayedOpeningCS", "PlayedNecroCS", "TabletsCollected", "SpawnedFinalSpotsStuff",
                           "GivenDiggingSpotsRewards", "GraveNumbersGivenOut", "DisplayedBadTabletMessage",
                           "PlayedOpeningNecroCS", "AutosavedInNW3" };
    void* where[] = { &q.PlayedLostBayCS, &q.PlayedOpeningCS, &q.PlayedNecroCS, &q.TabletsCollected, &q.SpawnedFinalSpotsStuff,
                      &q.GivenDiggingSpotsRewards, &q.GraveNumbersGivenOut, &q.DisplayedBadTabletMessage,
                      &q.PlayedOpeningNecroCS, &q.AutosavedInNW3 };
    bool ok = c.count == 10;
    for (int i = 0; ok && i < 10; ++i)
        ok = std::strcmp(c.names[i], want[i]) == 0 && c.values[i] == where[i] && c.defaults[i] == 0;
    ok = ok && q.TabletsCollected == 12 && q.GraveNumbersGivenOut == 21 && q.MissionSucceeded == false && q.PositionTaken == 0;
    if (!ok) { std::printf("00cfaf70_TEST FAIL\n"); return 1; }
    std::printf("00cfaf70_TEST PASS\n");
    return 0;
}