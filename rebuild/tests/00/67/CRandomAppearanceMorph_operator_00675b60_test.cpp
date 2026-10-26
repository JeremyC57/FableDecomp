// Behaviour test for CRandomAppearanceMorph::operator= @ 0x00675B60: every container and the max group id copied.
#include <cstdio>

static int g_assigns = 0;
template<class T> class CArray { public:
    long m_items[4]; int m_count;
    CArray() : m_count(0) {}
    CArray& operator=(const CArray& other)
    {
        ++g_assigns; m_count = other.m_count;
        for (int i = 0; i < m_count; ++i) m_items[i] = other.m_items[i];
        return *this;
    }
};
struct CKeyPairCompareLess {};
template<class K, class V> class CVectorMap { public: CArray<V> m_vector; CKeyPairCompareLess m_compare; bool m_sorted; CVectorMap() : m_sorted(false) {} };
struct CBodyPartMesh {}; struct CTextureMorph {}; struct CSkeletalMorph {};

class CRandomAppearanceMorph { public:
    CArray<CArray<CBodyPartMesh> > BodyParts;
    CArray<CVectorMap<unsigned long, CTextureMorph> > BodyPartTextureMorphs;
    CVectorMap<long, CSkeletalMorph> SkeletalMorphs;
    CVectorMap<unsigned long, CTextureMorph> GeneralTextureMorphs;
    unsigned long MaxTextureGroupID;
    CRandomAppearanceMorph& operator=(const CRandomAppearanceMorph& other)
    {
        BodyParts = other.BodyParts;
        BodyPartTextureMorphs = other.BodyPartTextureMorphs;
        SkeletalMorphs = other.SkeletalMorphs;
        GeneralTextureMorphs = other.GeneralTextureMorphs;
        MaxTextureGroupID = other.MaxTextureGroupID;
        return *this;
    }
};

int main()
{
    CRandomAppearanceMorph a, b;
    a.BodyParts.m_count = 1; a.BodyParts.m_items[0] = 11;
    a.BodyPartTextureMorphs.m_count = 2; a.BodyPartTextureMorphs.m_items[1] = 22;
    a.SkeletalMorphs.m_vector.m_count = 1; a.SkeletalMorphs.m_vector.m_items[0] = 33; a.SkeletalMorphs.m_sorted = true;
    a.GeneralTextureMorphs.m_vector.m_count = 3; a.GeneralTextureMorphs.m_vector.m_items[2] = 44; a.GeneralTextureMorphs.m_sorted = false;
    a.MaxTextureGroupID = 7; b.MaxTextureGroupID = 0; b.GeneralTextureMorphs.m_sorted = true;
    CRandomAppearanceMorph& r = (b = a);
    if (&r != &b || g_assigns != 4 || b.BodyParts.m_items[0] != 11 || b.BodyPartTextureMorphs.m_items[1] != 22 ||
        b.SkeletalMorphs.m_vector.m_items[0] != 33 || !b.SkeletalMorphs.m_sorted || b.GeneralTextureMorphs.m_vector.m_items[2] != 44 ||
        b.GeneralTextureMorphs.m_sorted || b.MaxTextureGroupID != 7) {
        std::printf("00675b60_TEST FAIL\n"); return 1;
    }
    std::printf("00675b60_TEST PASS\n");
    return 0;
}