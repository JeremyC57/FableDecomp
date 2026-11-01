// Behaviour test for CTCTrophy::~CTCTrophy (0x006DDB80): leave the global TC list, then the two witness lists (last declared
// first), the def pointer release, then the CTCBase destructor.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }

struct CDefPointee { long RefCount; bool Deleted; void Delete() { Deleted = true; Log("D"); } };
struct CDefPointerModel {
    CDefPointee* Object;
    ~CDefPointerModel() { if (Object) { CDefPointee* o = Object; if (--o->RefCount == 0) o->Delete(); Object = 0; } }
};
struct CWitnessListModel {                      // one-word list: count of nodes, tag
    int Nodes; const char* Tag;
    ~CWitnessListModel() { Log(Tag); Nodes = 0; }
};
struct CTCBaseModel {
    virtual ~CTCBaseModel() { Log("B"); }
    void RemoveFromGlobalTCs() { Log("R"); }
};
struct CTCTrophyModel : CTCBaseModel {
    CDefPointerModel PDef;
    CWitnessListModel AvailableWitnesses;
    CWitnessListModel WitnessesAchieved;
    virtual ~CTCTrophyModel() { RemoveFromGlobalTCs(); }
};

int main()
{
    CDefPointee def = { 1, false };
    {
        CTCTrophyModel t;
        t.PDef.Object = &def;
        t.AvailableWitnesses.Nodes = 2; t.AvailableWitnesses.Tag = "A";
        t.WitnessesAchieved.Nodes = 1; t.WitnessesAchieved.Tag = "W";
    }
    if (std::strcmp(g_log, "RWADB") != 0 || !def.Deleted || def.RefCount != 0) { std::printf("006ddb80_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    CDefPointee shared = { 2, false };
    {
        CTCTrophyModel t;
        t.PDef.Object = &shared;
        t.AvailableWitnesses.Nodes = 0; t.AvailableWitnesses.Tag = "A";
        t.WitnessesAchieved.Nodes = 0; t.WitnessesAchieved.Tag = "W";
    }
    if (std::strcmp(g_log, "RWAB") != 0 || shared.Deleted || shared.RefCount != 1) { std::printf("006ddb80_TEST FAIL\n"); return 1; }
    std::printf("006ddb80_TEST PASS\n");
    return 0;
}