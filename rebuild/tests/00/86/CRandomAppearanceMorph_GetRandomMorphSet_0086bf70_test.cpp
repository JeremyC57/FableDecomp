// Behaviour test for CRandomAppearanceMorph::GetRandomMorphSet @ 0x0086BF70: the three rolls run in order with the right arguments.
#include <cstdio>

template<class T> class CArray { public: T* m_first; T* m_last; T* m_end; };
struct CKeyPairCompareLess {};
template<class K, class V> class CVectorMap { public: CArray<V> m_vector; CKeyPairCompareLess m_compare; bool m_sorted; };
struct CTextureMorph { unsigned long NewTexture, GroupID; };
struct CMorphSetTextureMorph { long OriginalTexture, NewTexture; };
struct CMorphSet { long BodyParts; long SkeletalMorphs; CArray<CMorphSetTextureMorph> GeneralTextureMorphs; };

static int g_log[8]; static int g_n = 0;
class CRandomAppearanceMorph { public:
    CVectorMap<unsigned long, CTextureMorph> GeneralTextureMorphs;
    const void* m_lastMap; const void* m_lastResult; unsigned long m_lastGroup; bool m_lastFlag;
    void GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& set) { g_log[g_n++] = 1; seed += 1; set.SkeletalMorphs = (long)seed; }
    void GetRandomBodyParts(unsigned long& seed, CMorphSet& set, bool flag) { g_log[g_n++] = 2; seed += 10; set.BodyParts = (long)seed; m_lastFlag = flag; }
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long groupID, CVectorMap<unsigned long, CTextureMorph>& morphs, CArray<CMorphSetTextureMorph>& result)
    { g_log[g_n++] = 3; seed += 100; m_lastGroup = groupID; m_lastMap = &morphs; m_lastResult = &result; }
    void GetRandomMorphSet(unsigned long& seed, CMorphSet& morphSet, bool flag)
    {
        GetRandomSkeletalMorph(seed, morphSet);
        GetRandomBodyParts(seed, morphSet, flag);
        GetRandomTextureMorphs(seed, 0, GeneralTextureMorphs, morphSet.GeneralTextureMorphs);
    }
};

int main()
{
    CRandomAppearanceMorph m; m.m_lastGroup = 99; m.m_lastFlag = false;
    CMorphSet set; unsigned long seed = 5;
    m.GetRandomMorphSet(seed, set, true);
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || seed != 116 || set.SkeletalMorphs != 6 || set.BodyParts != 16 ||
        !m.m_lastFlag || m.m_lastGroup != 0 || m.m_lastMap != &m.GeneralTextureMorphs || m.m_lastResult != &set.GeneralTextureMorphs) {
        std::printf("0086bf70_TEST FAIL\n"); return 1;
    }
    std::printf("0086bf70_TEST PASS\n");
    return 0;
}