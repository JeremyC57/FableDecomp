// Behaviour test for CTCMultiStrike::~CTCMultiStrike (0x007CCA90): destroy every particle-effect pointer in order and free the
// block, then the target and weapon pointers, the def release, then ~CTCBase.
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
struct CTCBaseModel { virtual ~CTCBaseModel() { Log("B"); } };
struct CTCMultiStrikeModel : CTCBaseModel {
    CDefModel PDef;
    CPtrModel PWeapon;
    CPtrModel PTarget;
    CPtrVectorModel ParticleEffects;
    virtual ~CTCMultiStrikeModel() { }
};
int main()
{
    CDefPointee def = { 1, 0 };
    CPtrModel effects[2];
    effects[0].Tag = "a"; effects[1].Tag = "b";
    {
        CTCMultiStrikeModel m;
        m.PDef.Object = &def; m.PWeapon.Tag = "w"; m.PTarget.Tag = "t";
        m.ParticleEffects.Start = effects; m.ParticleEffects.Finish = effects + 2;
    }
    if (std::strcmp(g_log, "abFtwDB") != 0 || def.Deleted != 1) { std::printf("007cca90_TEST FAIL\n"); return 1; }
    std::printf("007cca90_TEST PASS\n");
    return 0;
}