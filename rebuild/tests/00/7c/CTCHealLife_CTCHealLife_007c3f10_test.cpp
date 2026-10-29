// Behaviour test for CTCHealLife::~CTCHealLife @ 0x007C3F10: an empty user body, so the members go in reverse order
// (healing target, the float vector, the effect vector, the heal list cleared and its node freed, the removed item, the
// def pointer released and nulled) before the CTCBase part.
#include <stdio.h>
#include <string.h>
static char g_log[64];
static void Log(const char* s) { strcat(g_log, s); }
struct CIntelligentPointer { const char* Tag; CIntelligentPointer(const char* t) : Tag(t) { } ~CIntelligentPointer() { Log(Tag); } };
struct CPointee { int RefCount; int Deleted; };
struct CDefPointer { CPointee* Object; ~CDefPointer() { CPointee* o = Object; if (o) { if (--o->RefCount == 0) o->Deleted++; Object = 0; } Log("D"); } };
struct CHealList { int Count; ~CHealList() { Count = 0; Log("L"); } };
struct CVec { const char* Tag; CVec(const char* t) : Tag(t) { } ~CVec() { Log(Tag); } };
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
class CTCHealLife : public CTCBase { public:
    CDefPointer PDef; float TotalHealthAccum; unsigned long LoopingSound; long SpellLevel; CIntelligentPointer PItemRemoved;
    CHealList ThingsToHeal; CVec HealingEffects; CVec HealthIncreaseAccum; CIntelligentPointer PHealingTarget;
    CTCHealLife(CPointee* def) : PItemRemoved("i"), HealingEffects("E"), HealthIncreaseAccum("F"), PHealingTarget("t")
    { PDef.Object = def; ThingsToHeal.Count = 3; }
    ~CTCHealLife() { }
};
int main()
{
    CPointee def = { 1, 0 };
    CTCHealLife* h = new CTCHealLife(&def);
    delete h;
    if (strcmp(g_log, "tFELiDB") != 0 || def.Deleted != 1 || def.RefCount != 0) { printf("007c3f10_TEST FAIL\n"); return 1; }
    printf("007c3f10_TEST PASS\n");
    return 0;
}