#include <cstdio>

static int g_baseCopies = 0;
class CParentDefClassBase { public:
    virtual void v00() { }
    unsigned char _body[0x38];
    CParentDefClassBase() { }
    CParentDefClassBase(const CParentDefClassBase& other) { ++g_baseCopies; }
};
class CAbilityTrackLevelDef : public CParentDefClassBase { public:
    long m_level, m_experienceCost, m_value;
    virtual void v00() { }
    CAbilityTrackLevelDef() : m_level(10), m_experienceCost(11), m_value(12) { }
    CAbilityTrackLevelDef(const CAbilityTrackLevelDef& o) : CParentDefClassBase(o), m_level(o.m_level), m_experienceCost(o.m_experienceCost), m_value(o.m_value) { }
};
int main()
{
    CAbilityTrackLevelDef a; CAbilityTrackLevelDef b(a);
    if (g_baseCopies != 1 || b.m_level != 10 || b.m_experienceCost != 11 || b.m_value != 12) { std::printf("004787f6_TEST FAIL\n"); return 1; }
    std::printf("004787f6_TEST PASS\n"); return 0;
}