// Behaviour test for the CRegionDef constructor @ 0x004564B5: base constructed, both sound vector maps empty, script name unset.
#include <cstdio>

static int g_baseCtors = 0;
class CParentDefClassBase { public:
    long m_globalIndex;
    CParentDefClassBase() : m_globalIndex(-1) { ++g_baseCtors; }
    virtual ~CParentDefClassBase() {}
};
template<class K, class V> class CVectorMap { public:
    V* m_first; V* m_last; V* m_end; unsigned char m_compare; bool m_sorted;
    CVectorMap() : m_first(0), m_last(0), m_end(0), m_sorted(false) {}
};
class CSoundPair { public: long a; };
class CDefString { public: long TablePos; CDefString() : TablePos(-1) {} };
class CSoundMap { public: CVectorMap<unsigned long, CSoundPair> SoundMap; CVectorMap<unsigned long, CDefString> CriteriaMap; };
class CRegionDef : public CParentDefClassBase { public:
    long EnvironmentTheme, InternalEnvironmentTheme, InternalLightingChannelCount, CameraManagerSetIndex, CameraManagerSetBuildingIndex;
    CSoundMap SoundMap; bool PreventHeroWeaponUse, PreventHeroWillUse; CDefString WanderingPopulationScriptDefName;
    CRegionDef() {}
};

int main()
{
    CRegionDef d;
    if (g_baseCtors != 1 || d.SoundMap.SoundMap.m_first != 0 || d.SoundMap.SoundMap.m_last != 0 || d.SoundMap.SoundMap.m_end != 0 ||
        d.SoundMap.SoundMap.m_sorted || d.SoundMap.CriteriaMap.m_first != 0 || d.SoundMap.CriteriaMap.m_end != 0 || d.SoundMap.CriteriaMap.m_sorted ||
        d.WanderingPopulationScriptDefName.TablePos != -1) {
        std::printf("004564b5_TEST FAIL\n"); return 1;
    }
    std::printf("004564b5_TEST PASS\n");
    return 0;
}