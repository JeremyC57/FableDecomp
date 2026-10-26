#include <cstdio>

static int g_baseCopies = 0, g_vecCopies = 0;
class CParentDefClassBase { public:
    virtual void v00() { }
    unsigned char _body[0x38];
    CParentDefClassBase() { }
    CParentDefClassBase(const CParentDefClassBase& other) { ++g_baseCopies; }
};
struct CAbilityTrackLevelDefVector {
    void* m_first; void* m_last; void* m_end;
    CAbilityTrackLevelDefVector() : m_first(0), m_last(0), m_end(0) { }
    CAbilityTrackLevelDefVector(const CAbilityTrackLevelDefVector& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end) { ++g_vecCopies; }
};
class CAbilityTrackDef : public CParentDefClassBase { public:
    long m_abilityType; CAbilityTrackLevelDefVector m_levels; long m_maxLevel;
    virtual void v00() { }
    CAbilityTrackDef() : m_abilityType(7), m_maxLevel(9) { m_levels.m_first = (void*)0x1234; }
    CAbilityTrackDef(const CAbilityTrackDef& o) : CParentDefClassBase(o), m_abilityType(o.m_abilityType), m_levels(o.m_levels), m_maxLevel(o.m_maxLevel) { }
};
int main()
{
    CAbilityTrackDef a; CAbilityTrackDef b(a);
    if (g_baseCopies != 1 || g_vecCopies != 1 || b.m_abilityType != 7 || b.m_maxLevel != 9 || b.m_levels.m_first != (void*)0x1234) { std::printf("004786cf_TEST FAIL\n"); return 1; }
    std::printf("004786cf_TEST PASS\n"); return 0;
}