// Behaviour test for CThingAICreature::~CThingAICreature @ 0x008320C0: drop the brain (counted pointer reset to null),
// leave a creature interaction if one is set, then the members go in reverse order before ~CThingCreatureBase.
#include <stdio.h>
#include <string.h>
static char g_log[64];
static void Log(const char* s) { strcat(g_log, s); }
struct CInfo { int RefCount; };
struct CCountedPointer { void* Data; CInfo* Info; const char* Tag;
    CCountedPointer(const char* t) : Data(0), Info(0), Tag(t) { }
    void ReleaseData() { if (Info && --Info->RefCount == 0) Log("x"); }
    void Reset(void* p) { ReleaseData(); Info = 0; Data = p; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; Log(Tag); } };
struct CIntelligentPointer { void* P; bool IsNull() const { return P == 0; } };
struct CMember { const char* Tag; CMember(const char* t) : Tag(t) { } ~CMember() { Log(Tag); } };
class CThingCreatureBase { public: CIntelligentPointer PCreatureInteraction; int Removed;
    CThingCreatureBase() : Removed(0) { PCreatureInteraction.P = 0; }
    void RemoveFromCreatureInteraction() { Removed++; PCreatureInteraction.P = 0; Log("I"); }
    virtual ~CThingCreatureBase() { Log("B"); } };
class CThingAICreature : public CThingCreatureBase { public:
    CCountedPointer RelationshipsInit; CMember OverridingBrainName; CCountedPointer PCurrentBrainOutput, PBrain; CMember Suspenders;
    CThingAICreature() : RelationshipsInit("r"), OverridingBrainName("n"), PCurrentBrainOutput("o"), PBrain("b"), Suspenders("s") { }
    ~CThingAICreature() { PBrain.Reset(0); if (!PCreatureInteraction.IsNull()) RemoveFromCreatureInteraction(); }
};
int main()
{
    static int dummy;
    CInfo brain = { 1 };
    CThingAICreature* c = new CThingAICreature;
    c->PBrain.Data = &dummy; c->PBrain.Info = &brain; c->PCreatureInteraction.P = &dummy;
    delete c;
    if (strcmp(g_log, "xIsbonrB") != 0 || brain.RefCount != 0) { printf("008320c0_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    c = new CThingAICreature; delete c;
    if (strcmp(g_log, "sbonrB") != 0) { printf("008320c0_TEST FAIL\n"); return 1; }
    printf("008320c0_TEST PASS\n");
    return 0;
}