#include <cstdio>
// Behaviour test for COpinionPersonalityDef::COpinionPersonalityDef @ 0x00456F71
static int g_traitsBuilt = 0, g_mapsBuilt = 0, g_basesBuilt = 0;
class CParentDefClassBase { public: CParentDefClassBase() { ++g_basesBuilt; } virtual ~CParentDefClassBase() {} };
class COpinionPersonalityTraits { public: long m_value; COpinionPersonalityTraits() : m_value(42) { ++g_traitsBuilt; } };
class CAttitudeEnabledMap { public: void* m_pHeader; CAttitudeEnabledMap() : m_pHeader(0) { ++g_mapsBuilt; } };
class COpinionPersonalityDef : public CParentDefClassBase {
public:
    COpinionPersonalityTraits* PersonalityTraits;
    bool AttitudeEnabledDefaultForNotNone;
    CAttitudeEnabledMap AttitudeEnabled;
    float ToleranceToBeingHit;
    COpinionPersonalityDef()
    {
        PersonalityTraits = new COpinionPersonalityTraits[5];
        AttitudeEnabledDefaultForNotNone = true;
    }
};
int main()
{
    COpinionPersonalityDef d;
    if (!d.PersonalityTraits || d.PersonalityTraits[4].m_value != 42 || g_traitsBuilt != 5 || g_mapsBuilt != 1 || g_basesBuilt != 1 ||
        !d.AttitudeEnabledDefaultForNotNone) { std::printf("00456f71_TEST FAIL\n"); return 1; }
    delete[] d.PersonalityTraits;
    std::printf("00456f71_TEST PASS\n");
    return 0;
}
