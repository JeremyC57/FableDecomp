// Behaviour test for CTCTurncoatSpell::~CTCTurncoatSpell (0x007C66F0): members die in reverse order -- three effect pointers,
// the removed item, the target, the def release, the timer (its vector, then its creature pointer) -- then ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CPtrModel { const char* Tag; virtual ~CPtrModel() { Log(Tag); } };
struct CDefPointee { long RefCount; int Deleted; };
struct CDefModel { CDefPointee* Object; ~CDefModel() { if (Object) { if (--Object->RefCount == 0) { Object->Deleted++; Log("D"); } Object = 0; } } };
struct CFloatVectorModel { float* Start; ~CFloatVectorModel() { if (Start) { delete[] Start; Log("V"); } } };
struct CTimerModel { long Held; long Level; CPtrModel PCreature; long Ability; long State; CFloatVectorModel Timings; };
struct CTCBaseModel { virtual ~CTCBaseModel() { Log("B"); } };
struct CTCTurncoatSpellModel : CTCBaseModel {
    CTimerModel Timer;
    CDefModel PDef;
    CPtrModel PTarget;
    CPtrModel PItemRemoved;
    CPtrModel OnMe, OnTarget, OnMeOutOfRange;
    virtual ~CTCTurncoatSpellModel() { }
};
int main()
{
    CDefPointee def = { 1, 0 };
    {
        CTCTurncoatSpellModel t;
        t.Timer.PCreature.Tag = "c"; t.Timer.Timings.Start = new float[2];
        t.PDef.Object = &def;
        t.PTarget.Tag = "t"; t.PItemRemoved.Tag = "i"; t.OnMe.Tag = "1"; t.OnTarget.Tag = "2"; t.OnMeOutOfRange.Tag = "3";
    }
    if (std::strcmp(g_log, "321itDVcB") != 0 || def.Deleted != 1) { std::printf("007c66f0_TEST FAIL\n"); return 1; }
    std::printf("007c66f0_TEST PASS\n");
    return 0;
}