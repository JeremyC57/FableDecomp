// behaviour test for 0x00460A61 CSoundDef::operator=
#include <cstdio>
static int g_copies = 0;
struct CParentDefClassBase {
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CVec { int v; CVec& operator=(const CVec& o) { v = o.v; ++g_copies; return *this; } };
struct CSoundDef : CParentDefClassBase {
    CVec SoundBankEntries, AtmosBankEntries, MusicEntries, MusicSetEntries, ReverbEnvironmentEntries;
    float AtmosListenerDampingMin, AtmosListenerDampingRange, DistanceModelCurve;
    CSoundDef& operator=(const CSoundDef& other)
    {
        CParentDefClassBase::operator=(other);
        SoundBankEntries = other.SoundBankEntries;
        AtmosBankEntries = other.AtmosBankEntries;
        MusicEntries = other.MusicEntries;
        MusicSetEntries = other.MusicSetEntries;
        ReverbEnvironmentEntries = other.ReverbEnvironmentEntries;
        AtmosListenerDampingMin = other.AtmosListenerDampingMin;
        AtmosListenerDampingRange = other.AtmosListenerDampingRange;
        DistanceModelCurve = other.DistanceModelCurve;
        return *this;
    }
};
int main()
{
    CSoundDef a, b;
    b.baseTag = 4; b.SoundBankEntries.v = 1; b.ReverbEnvironmentEntries.v = 5; b.AtmosListenerDampingMin = 0.5f; b.DistanceModelCurve = 2.0f;
    a.baseTag = 0; a.SoundBankEntries.v = 0; a.ReverbEnvironmentEntries.v = 0; a.AtmosListenerDampingMin = 0; a.DistanceModelCurve = 0;
    CSoundDef* r = &(a = b);
    if (r != &a || a.baseTag != 4 || a.SoundBankEntries.v != 1 || a.ReverbEnvironmentEntries.v != 5
        || a.AtmosListenerDampingMin != 0.5f || a.DistanceModelCurve != 2.0f || g_copies != 5)
    { std::printf("00460a61_TEST FAIL\n"); return 1; }
    std::printf("00460a61_TEST PASS\n");
    return 0;
}
