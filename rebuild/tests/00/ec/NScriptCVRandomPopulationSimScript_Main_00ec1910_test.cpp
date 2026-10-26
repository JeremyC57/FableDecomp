#include <cstdio>
#include <cstring>
// Behaviour test for CV_RandomPopulationSimScript::Main (0x00ec1910): the bindings are added in order with their names, parent and
// flags, then PostAddScriptedEntities runs locally and on the interface, then the threads spawn in order.
class CCharString { public:
    char Text[64];
    CCharString(const char* t) { std::strcpy(Text, t); }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); }
};
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CBinding {
    CCharString Name; void* Parent; int AllocId; bool Valid; unsigned long Flags;
    CBinding(const CCharString& n, void* p, int a, unsigned long f) : Name(n), Parent(p), AllocId(a), Valid(true), Flags(f) {}
};
struct CSpawned {
    CCharString Name; unsigned long Id; void* Owner;
    CSpawned(const CCharString& n, unsigned long id, void* o) : Name(n), Id(id), Owner(o) {}
};
struct CGSI {
    void PostAddScriptedEntities() const { Log("GSI.Post"); }
    void ActivateQuest(const CCharString& n) const { char b[96]; std::sprintf(b, "Activate:%s", n.Text); Log(b); }
};
struct CScript {
    CGSI* GSI; int Errors;
    void AddEntityScriptBinding(CBinding* b) {
        char t[96]; std::sprintf(t, "Bind:%s:%d:%lu", b->Name.Text, b->AllocId, b->Flags); Log(t);
        if (b->Parent != this || !b->Valid) ++Errors;
        delete b;
    }
    void PostAddScriptedEntities() { Log("Post"); }
    void AddSpawnedFunction(CSpawned* f, const CCharString& region) {
        char t[96]; std::sprintf(t, "Spawn:%s:%s", f->Name.Text, region.Text); Log(t);
        if (f->Owner != this || f->Id != 0) ++Errors;
        delete f;
    }
    void DeclareGossipCategories() { Log("Gossip"); }
    void Main()
    {
        AddEntityScriptBinding(new CBinding("RandomPopulationVillager", this, 0, 0));
        AddEntityScriptBinding(new CBinding("RandomPopulationGuard", this, 1, 0));
        AddEntityScriptBinding(new CBinding("RandomPopulationBandit", this, 2, 0));
        PostAddScriptedEntities();
        GSI->PostAddScriptedEntities();
        AddSpawnedFunction(new CSpawned("ProcessAlbionPopulationSim", 0, this), CCharString(""));
        AddSpawnedFunction(new CSpawned("HighDetailPopulationSim", 0, this), CCharString(""));
    }
};
int main()
{
    CGSI gsi; CScript s; s.GSI = &gsi; s.Errors = 0; g_log[0] = 0;
    s.Main();
    if (s.Errors != 0 || std::strcmp(g_log, "Bind:RandomPopulationVillager:0:0;Bind:RandomPopulationGuard:1:0;Bind:RandomPopulationBandit:2:0;Post;GSI.Post;Spawn:ProcessAlbionPopulationSim:;Spawn:HighDetailPopulationSim:;") != 0) { std::printf("00ec1910_TEST FAIL\n"); return 1; }
    std::printf("00ec1910_TEST PASS\n");
    return 0;
}