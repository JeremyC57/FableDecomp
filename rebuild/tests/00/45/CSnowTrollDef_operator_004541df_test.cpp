// behaviour test for CSnowTrollDef::operator= (0x004541DF): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };

struct CSnowTrollDef : CDefClassBase {
    long IcicleAttackIndex;
    float SecondsIcicleAttackTargetPositionPredictionTime;
    float DistancePoundGroundRange;
    float SecondsForIcicleAssault;
    float SecondsLeadIntoIcicles;
    float SecondsBetweenIcicles;
    float SecondsBetweenWarningFXAndTrigger;
    float SecondsForTrapToRemain;
    float DistanceRange;
    float DistanceAroundTarget;
    float DistanceExclusionAroundIcicles;
    long IcicleAssaultWarningFXIndex;
    long IcicleAssaultTrapIndex;
    float IceTrailDistanceLimit;
    CSnowTrollDef& operator=(const CSnowTrollDef& other);
};
CSnowTrollDef& CSnowTrollDef::operator=(const CSnowTrollDef& other)
{
    CDefClassBase::operator=(other);
    IcicleAttackIndex = other.IcicleAttackIndex;
    SecondsIcicleAttackTargetPositionPredictionTime = other.SecondsIcicleAttackTargetPositionPredictionTime;
    DistancePoundGroundRange = other.DistancePoundGroundRange;
    SecondsForIcicleAssault = other.SecondsForIcicleAssault;
    SecondsLeadIntoIcicles = other.SecondsLeadIntoIcicles;
    SecondsBetweenIcicles = other.SecondsBetweenIcicles;
    SecondsBetweenWarningFXAndTrigger = other.SecondsBetweenWarningFXAndTrigger;
    SecondsForTrapToRemain = other.SecondsForTrapToRemain;
    DistanceRange = other.DistanceRange;
    DistanceAroundTarget = other.DistanceAroundTarget;
    DistanceExclusionAroundIcicles = other.DistanceExclusionAroundIcicles;
    IcicleAssaultWarningFXIndex = other.IcicleAssaultWarningFXIndex;
    IcicleAssaultTrapIndex = other.IcicleAssaultTrapIndex;
    IceTrailDistanceLimit = other.IceTrailDistanceLimit;
    return *this;
}
int main()
{
    CSnowTrollDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    CSnowTrollDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.IcicleAttackIndex; const unsigned char* y = (const unsigned char*)&b.IcicleAttackIndex; for (unsigned int i = 0; i < sizeof(a.IcicleAttackIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsIcicleAttackTargetPositionPredictionTime; const unsigned char* y = (const unsigned char*)&b.SecondsIcicleAttackTargetPositionPredictionTime; for (unsigned int i = 0; i < sizeof(a.SecondsIcicleAttackTargetPositionPredictionTime); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DistancePoundGroundRange; const unsigned char* y = (const unsigned char*)&b.DistancePoundGroundRange; for (unsigned int i = 0; i < sizeof(a.DistancePoundGroundRange); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsForIcicleAssault; const unsigned char* y = (const unsigned char*)&b.SecondsForIcicleAssault; for (unsigned int i = 0; i < sizeof(a.SecondsForIcicleAssault); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsLeadIntoIcicles; const unsigned char* y = (const unsigned char*)&b.SecondsLeadIntoIcicles; for (unsigned int i = 0; i < sizeof(a.SecondsLeadIntoIcicles); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsBetweenIcicles; const unsigned char* y = (const unsigned char*)&b.SecondsBetweenIcicles; for (unsigned int i = 0; i < sizeof(a.SecondsBetweenIcicles); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsBetweenWarningFXAndTrigger; const unsigned char* y = (const unsigned char*)&b.SecondsBetweenWarningFXAndTrigger; for (unsigned int i = 0; i < sizeof(a.SecondsBetweenWarningFXAndTrigger); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsForTrapToRemain; const unsigned char* y = (const unsigned char*)&b.SecondsForTrapToRemain; for (unsigned int i = 0; i < sizeof(a.SecondsForTrapToRemain); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DistanceRange; const unsigned char* y = (const unsigned char*)&b.DistanceRange; for (unsigned int i = 0; i < sizeof(a.DistanceRange); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DistanceAroundTarget; const unsigned char* y = (const unsigned char*)&b.DistanceAroundTarget; for (unsigned int i = 0; i < sizeof(a.DistanceAroundTarget); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DistanceExclusionAroundIcicles; const unsigned char* y = (const unsigned char*)&b.DistanceExclusionAroundIcicles; for (unsigned int i = 0; i < sizeof(a.DistanceExclusionAroundIcicles); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IcicleAssaultWarningFXIndex; const unsigned char* y = (const unsigned char*)&b.IcicleAssaultWarningFXIndex; for (unsigned int i = 0; i < sizeof(a.IcicleAssaultWarningFXIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IcicleAssaultTrapIndex; const unsigned char* y = (const unsigned char*)&b.IcicleAssaultTrapIndex; for (unsigned int i = 0; i < sizeof(a.IcicleAssaultTrapIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IceTrailDistanceLimit; const unsigned char* y = (const unsigned char*)&b.IceTrailDistanceLimit; for (unsigned int i = 0; i < sizeof(a.IceTrailDistanceLimit); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004541df_TEST FAIL\n"); return 1; }
    std::printf("004541df_TEST PASS\n");
    return 0;
}