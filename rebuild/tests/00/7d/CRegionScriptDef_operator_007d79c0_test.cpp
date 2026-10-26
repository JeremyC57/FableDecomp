// Behaviour test for CRegionScriptDef::operator= @ 0x007D79C0: base, name, sub-def map (vector + sorted flag) and both limits copied.
#include <cstdio>

static int g_baseAssigns = 0, g_vectorAssigns = 0;
class CDefClassBase { public:
    long m_globalIndex;
    CDefClassBase& operator=(const CDefClassBase& other) { ++g_baseAssigns; m_globalIndex = other.m_globalIndex; return *this; }
    virtual ~CDefClassBase() {}
};
template<class T> struct CRetailVector {
    long m_items[4]; int m_count;
    CRetailVector& operator=(const CRetailVector& other)
    {
        ++g_vectorAssigns; m_count = other.m_count;
        for (int i = 0; i < m_count; ++i) m_items[i] = other.m_items[i];
        return *this;
    }
};
struct CKeyPairCompareLess {};
template<class K, class V> class CVectorMap { public: CRetailVector<V> m_vector; CKeyPairCompareLess m_compare; bool m_sorted; };
class CDefString { public: long TablePos; };
struct CSubDefInfo { long a; };
class CParentDefClassBase : public CDefClassBase { public: CDefString InstantiationName; CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap; };
class CRegionScriptDef : public CParentDefClassBase { public:
    long RandomVillagerMax, RandomGuardMax;
    CRegionScriptDef& operator=(const CRegionScriptDef& other)
    {
        CParentDefClassBase::operator=(other);
        RandomVillagerMax = other.RandomVillagerMax;
        RandomGuardMax = other.RandomGuardMax;
        return *this;
    }
};

int main()
{
    CRegionScriptDef a, b;
    a.m_globalIndex = 9; a.InstantiationName.TablePos = 12; a.SubDefInfoMap.m_vector.m_count = 2;
    a.SubDefInfoMap.m_vector.m_items[0] = 5; a.SubDefInfoMap.m_vector.m_items[1] = 6; a.SubDefInfoMap.m_sorted = true;
    a.RandomVillagerMax = 8; a.RandomGuardMax = 3;
    b.m_globalIndex = 0; b.InstantiationName.TablePos = -1; b.SubDefInfoMap.m_vector.m_count = 0; b.SubDefInfoMap.m_sorted = false;
    b.RandomVillagerMax = 0; b.RandomGuardMax = 0;
    CRegionScriptDef& r = (b = a);
    if (&r != &b || g_baseAssigns != 1 || g_vectorAssigns != 1 || b.m_globalIndex != 9 || b.InstantiationName.TablePos != 12 ||
        b.SubDefInfoMap.m_vector.m_count != 2 || b.SubDefInfoMap.m_vector.m_items[1] != 6 || !b.SubDefInfoMap.m_sorted ||
        b.RandomVillagerMax != 8 || b.RandomGuardMax != 3) {
        std::printf("007d79c0_TEST FAIL\n"); return 1;
    }
    std::printf("007d79c0_TEST PASS\n");
    return 0;
}