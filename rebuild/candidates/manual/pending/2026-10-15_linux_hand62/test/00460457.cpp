#include <cstdio>

static int g_baseAssigns = 0;
static int g_containerAssigns = 0;
static int g_stringAssigns = 0;

struct CParentDefClassBase {
    virtual void v00() { }
    long m_marker;                              // stands in for the out-of-line base part
    CParentDefClassBase() : m_marker(0) { }
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        m_marker = other.m_marker;
        ++g_baseAssigns;
        return *this;
    }
};

struct CContainer {                             // a retail vector / map: assignment is out of line
    long m_id;
    CContainer() : m_id(0) { }
    CContainer& operator=(const CContainer& other)
    {
        m_id = other.m_id;
        ++g_containerAssigns;
        return *this;
    }
};

struct CCharString {
    long m_id;
    CCharString() : m_id(0) { }
    CCharString& operator=(const CCharString& other)
    {
        m_id = other.m_id;
        ++g_stringAssigns;
        return *this;
    }
};

struct CDefString { long TablePos; };
struct CRGBColour { unsigned long m_packed; };
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };

struct CSpecialAbilitiesSummonSpellDef : CParentDefClassBase {
    CContainer TimeOfSummon;
    CDefString BaseSummonCreatureName;
    float FadeInTime;
    float FadeOutTime;
    long ResetCreatureManaCost;
    CContainer SummonTintColour;
    CCharString EyeLeftDummyPointName;
    CCharString EyeRightDummyPointName;
    CSpecialAbilitiesSummonSpellDef& operator=(const CSpecialAbilitiesSummonSpellDef& other);
};

CSpecialAbilitiesSummonSpellDef& CSpecialAbilitiesSummonSpellDef::operator=(const CSpecialAbilitiesSummonSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    TimeOfSummon = other.TimeOfSummon;
    BaseSummonCreatureName = other.BaseSummonCreatureName;
    FadeInTime = other.FadeInTime;
    FadeOutTime = other.FadeOutTime;
    ResetCreatureManaCost = other.ResetCreatureManaCost;
    SummonTintColour = other.SummonTintColour;
    EyeLeftDummyPointName = other.EyeLeftDummyPointName;
    EyeRightDummyPointName = other.EyeRightDummyPointName;
    return *this;
}

int main()
{
    CSpecialAbilitiesSummonSpellDef a, b;
    a.m_marker = 7;
    a.TimeOfSummon.m_id = 201;
    a.BaseSummonCreatureName.TablePos = 302;
    b.BaseSummonCreatureName.TablePos = -1;
    a.FadeInTime = 3.5f;
    b.FadeInTime = 0.0f;
    a.FadeOutTime = 4.5f;
    b.FadeOutTime = 0.0f;
    a.ResetCreatureManaCost = 105;
    b.ResetCreatureManaCost = 0;
    a.SummonTintColour.m_id = 206;
    a.EyeLeftDummyPointName.m_id = 207;
    a.EyeRightDummyPointName.m_id = 208;
    CSpecialAbilitiesSummonSpellDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 2
        || g_stringAssigns != 2
        || b.TimeOfSummon.m_id != 201
        || b.BaseSummonCreatureName.TablePos != 302
        || b.FadeInTime != 3.5f
        || b.FadeOutTime != 4.5f
        || b.ResetCreatureManaCost != 105
        || b.SummonTintColour.m_id != 206
        || b.EyeLeftDummyPointName.m_id != 207
        || b.EyeRightDummyPointName.m_id != 208) {
        std::printf("00460457_TEST FAIL\n");
        return 1;
    }
    std::printf("00460457_TEST PASS\n");
    return 0;
}
