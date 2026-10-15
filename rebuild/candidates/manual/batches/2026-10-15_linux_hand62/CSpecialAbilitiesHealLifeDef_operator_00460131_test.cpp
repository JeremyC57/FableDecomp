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

struct CSpecialAbilitiesHealLifeDef : CParentDefClassBase {
    CContainer HealthIncreasePerFrame;
    CContainer TotalHealthIncrease;
    CContainer RadiusOfEffect;
    CSpecialAbilitiesHealLifeDef& operator=(const CSpecialAbilitiesHealLifeDef& other);
};

CSpecialAbilitiesHealLifeDef& CSpecialAbilitiesHealLifeDef::operator=(const CSpecialAbilitiesHealLifeDef& other)
{
    CParentDefClassBase::operator=(other);
    HealthIncreasePerFrame = other.HealthIncreasePerFrame;
    TotalHealthIncrease = other.TotalHealthIncrease;
    RadiusOfEffect = other.RadiusOfEffect;
    return *this;
}

int main()
{
    CSpecialAbilitiesHealLifeDef a, b;
    a.m_marker = 7;
    a.HealthIncreasePerFrame.m_id = 201;
    a.TotalHealthIncrease.m_id = 202;
    a.RadiusOfEffect.m_id = 203;
    CSpecialAbilitiesHealLifeDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 3
        || g_stringAssigns != 0
        || b.HealthIncreasePerFrame.m_id != 201
        || b.TotalHealthIncrease.m_id != 202
        || b.RadiusOfEffect.m_id != 203) {
        std::printf("00460131_TEST FAIL\n");
        return 1;
    }
    std::printf("00460131_TEST PASS\n");
    return 0;
}
