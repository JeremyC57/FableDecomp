#include <cstdio>
#include <cstring>
// Behaviour test for CV_ExposeMayorScript::Main (0x00e75ba0): the bindings are added in order with their names, parent and
// flags, then PostAddScriptedEntities runs locally and on the interface, the "V_ExposeMayorRhodri" quest is activated, the gossip categories are declared last, then the threads spawn in order.
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
        AddEntityScriptBinding(new CBinding("EM_SuspiciousVillager", this, 0, 1));
        AddEntityScriptBinding(new CBinding("EM_Sheriff", this, 1, 1));
        AddEntityScriptBinding(new CBinding("EM_FlashLightPoint", this, 2, 0));
        AddEntityScriptBinding(new CBinding("EM_Rhodri", this, 3, 0));
        AddEntityScriptBinding(new CBinding("HH_CellarDoor", this, 4, 0));
        PostAddScriptedEntities();
        GSI->PostAddScriptedEntities();
        GSI->ActivateQuest("V_ExposeMayorRhodri");
        AddSpawnedFunction(new CSpawned("DoMission", 0, this), CCharString(""));
        AddSpawnedFunction(new CSpawned("PreventConflictWithTCE", 0, this), CCharString(""));
        DeclareGossipCategories();
    }
};
int main()
{
    CGSI gsi; CScript s; s.GSI = &gsi; s.Errors = 0; g_log[0] = 0;
    s.Main();
    if (s.Errors != 0 || std::strcmp(g_log, "Bind:EM_SuspiciousVillager:0:1;Bind:EM_Sheriff:1:1;Bind:EM_FlashLightPoint:2:0;Bind:EM_Rhodri:3:0;Bind:HH_CellarDoor:4:0;Post;GSI.Post;Activate:V_ExposeMayorRhodri;Spawn:DoMission:;Spawn:PreventConflictWithTCE:;Gossip;") != 0) { std::printf("00e75ba0_TEST FAIL\n"); return 1; }
    std::printf("00e75ba0_TEST PASS\n");
    return 0;
}