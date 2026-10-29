// Behaviour test for 0x00E96C30 NScript::CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString, CCharString):
// gather the "BodyGuard" things; one bodyguard goes to the first marker, two go to the first and second markers, any other
// count does nothing; a terminating thread returns before teleporting. Checked through a logging game interface, with a
// fixed-size stand-in for the STLport vector; things balance.
#include <cstdio>
#include <cstring>
static char gLog[2048];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text) : PStringData(text) { }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { }
};
static int gThingsAlive = 0;
class CScriptThing { public:
    int Id;
    CScriptThing(int id = 0) : Id(id) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other) : Id(other.Id) { ++gThingsAlive; }
    CScriptThing& operator=(const CScriptThing& other) { Id = other.Id; return *this; }
    ~CScriptThing() { --gThingsAlive; }
};
class CThingVector { public:                               // the test's stand-in for std::vector<CScriptThing>
    CScriptThing Items[4];
    unsigned int Count;
    CThingVector() : Count(0) { }
    unsigned int size() const { return Count; }
    CScriptThing& operator[](unsigned int n) { return Items[n]; }
};
static unsigned int gBodyguards;
class CGameScriptInterfaceBase { public:
    long GetAllThingsWithScriptName(const CCharString& name, CThingVector& things) const
    {
        char t[64]; std::sprintf(t, "GetAll %s", name.PStringData); Log(t);
        for (unsigned int i = 0; i < gBodyguards; ++i) things.Items[i].Id = 100 + (int)i;
        things.Count = gBodyguards;
        return (long)gBodyguards;
    }
    CScriptThing GetThingWithScriptName(const CCharString& name) const
    {
        char t[64]; std::sprintf(t, "Get %s", name.PStringData); Log(t);
        return CScriptThing((int)std::strlen(name.PStringData));
    }
    void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const
    {
        char t[64]; std::sprintf(t, "Teleport %d>%d %d", thing.Id, target.Id, (int)a); Log(t);
    }
};
static bool gTerminating;
namespace NScript {
class CV_HauntedHouseScript { public:
    CGameScriptInterfaceBase* Game;
    bool IsActiveThreadTerminating() const { Log("Term?"); return gTerminating; }
    void SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2);
};

void CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString marker1, CCharString marker2)
{
    CThingVector bodyguards;
    Game->GetAllThingsWithScriptName("BodyGuard", bodyguards);
    if (bodyguards.size() == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(bodyguards[0], Game->GetThingWithScriptName(marker1), false);
    }
    if (bodyguards.size() == 2) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityTeleportToThing(bodyguards[0], Game->GetThingWithScriptName(marker1), false);
        Game->EntityTeleportToThing(bodyguards[1], Game->GetThingWithScriptName(marker2), false);
    }
}
}   // namespace NScript

static int Check(unsigned int count, bool terminating, const char* want)
{
    gLog[0] = 0; gBodyguards = count; gTerminating = terminating;
    CGameScriptInterfaceBase game;
    NScript::CV_HauntedHouseScript script;
    script.Game = &game;
    script.SendBodyguardsToTheRightPlace(CCharString("MarkA"), CCharString("MarkerBB"));
    int ok = std::strcmp(gLog, want) == 0 && gThingsAlive == 0;
    if (!ok) std::printf("%u %d: %s (%d)\n", count, (int)terminating, gLog, gThingsAlive);
    return ok;
}

int main()
{
    int ok = Check(0, false, "GetAll BodyGuard;")
          && Check(1, false, "GetAll BodyGuard;Term?;Get MarkA;Teleport 100>5 0;")
          && Check(2, false, "GetAll BodyGuard;Term?;Get MarkA;Teleport 100>5 0;Get MarkerBB;Teleport 101>8 0;")
          && Check(3, false, "GetAll BodyGuard;")
          && Check(1, true, "GetAll BodyGuard;Term?;")
          && Check(2, true, "GetAll BodyGuard;Term?;");
    if (!ok) { std::printf("00e96c30_TEST FAIL\n"); return 1; }
    std::printf("00e96c30_TEST PASS\n");
    return 0;
}