// Behaviour test for CTCElectrocutedEffect::~CTCElectrocutedEffect (0x007CE730): fade or kill the effect first, then the tint
// vector (elements in order, then the block), the effect pointer, the def release, then ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CPtrModel { const char* Tag; CPtrModel() : Tag("") { } virtual ~CPtrModel() { Log(Tag); } };
struct CDefPointee { long RefCount; int Deleted; };
struct CDefModel { CDefPointee* Object; ~CDefModel() { if (Object) { if (--Object->RefCount == 0) { Object->Deleted++; Log("D"); } Object = 0; } } };
struct CPtrVectorModel {
    CPtrModel* Start; CPtrModel* Finish;
    ~CPtrVectorModel() { for (CPtrModel* p = Start; p != Finish; ++p) Log(p->Tag); if (Start) { Log("F"); } }
};
static CPtrModel* g_faded = 0;
static void FadeOrKillEffect(CPtrModel& effect) { g_faded = &effect; Log("K"); }
struct CTCBaseModel { virtual ~CTCBaseModel() { Log("B"); } };
struct CTCElectrocutedEffectModel : CTCBaseModel {
    CDefModel PDef;
    long CurTime, FadeInTime, FadeOutTimeStart, FadeOutTime;
    CPtrModel PEffect;
    CPtrVectorModel TintList;
    virtual ~CTCElectrocutedEffectModel() { FadeOrKillEffect(PEffect); }
};
int main()
{
    CDefPointee def = { 3, 0 };
    CPtrModel tints[1];
    tints[0].Tag = "x";
    CPtrModel* expected = 0;
    {
        CTCElectrocutedEffectModel e;
        expected = &e.PEffect;
        e.PDef.Object = &def; e.PEffect.Tag = "e";
        e.TintList.Start = tints; e.TintList.Finish = tints + 1;
    }
    if (g_faded != expected || std::strcmp(g_log, "KxFeB") != 0 || def.RefCount != 2 || def.Deleted) { std::printf("007ce730_TEST FAIL\n"); return 1; }
    std::printf("007ce730_TEST PASS\n");
    return 0;
}