#include <cstdio>

static int g_baseAssigns = 0, g_vecAssigns = 0;
class CParentDefClassBase { public:
    virtual void v00() { }
    unsigned char _body[0x38];
    CParentDefClassBase& operator=(const CParentDefClassBase& other) { ++g_baseAssigns; return *this; }
};
struct CAbilityTrackLevelDefVector {
    void* m_first; void* m_last; void* m_end;
    CAbilityTrackLevelDefVector& operator=(const CAbilityTrackLevelDefVector& o) { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; ++g_vecAssigns; return *this; }
};
class CAbilityTrackDef : public CParentDefClassBase { public:
    long m_abilityType; CAbilityTrackLevelDefVector m_levels; long m_maxLevel;
    virtual void v00() { }
    CAbilityTrackDef& operator=(const CAbilityTrackDef& other)
    {
        CParentDefClassBase::operator=(other);
        m_abilityType = other.m_abilityType;
        m_levels = other.m_levels;
        m_maxLevel = other.m_maxLevel;
        return *this;
    }
};
int main()
{
    CAbilityTrackDef a, b; a.m_abilityType = 3; a.m_maxLevel = 5; a.m_levels.m_first = (void*)0x10; a.m_levels.m_last = (void*)0x20; a.m_levels.m_end = (void*)0x30;
    b.m_abilityType = 0; b.m_maxLevel = 0; b.m_levels.m_first = 0; b.m_levels.m_last = 0; b.m_levels.m_end = 0;
    CAbilityTrackDef& r = (b = a);
    if (&r != &b || g_baseAssigns != 1 || g_vecAssigns != 1 || b.m_abilityType != 3 || b.m_maxLevel != 5 || b.m_levels.m_end != (void*)0x30) { std::printf("0047889d_TEST FAIL\n"); return 1; }
    std::printf("0047889d_TEST PASS\n"); return 0;
}