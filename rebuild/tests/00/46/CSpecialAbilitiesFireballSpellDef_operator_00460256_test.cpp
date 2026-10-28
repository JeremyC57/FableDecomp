#include <cstdio>

static int g_baseAssigns = 0;
static int g_containerAssigns = 0;
static int g_stringAssigns = 0;

class CParentDefClassBase { public:
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

class CCharString { public:
    long m_id;
    CCharString() : m_id(0) { }
    const CCharString& operator=(const CCharString& other)
    {
        m_id = other.m_id;
        ++g_stringAssigns;
        return *this;
    }
};

class CDefString { public: long TablePos; };
class CRGBColour { public: unsigned long m_packed; };
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };

class CSpecialAbilitiesFireballSpellDef : public CParentDefClassBase { public:
    float ReleaseDelay;
    float Inclination;
    CContainer StaminaCost;
    float TargettingArcAngle;
    CSpecialAbilitiesFireballSpellDef& operator=(const CSpecialAbilitiesFireballSpellDef& other);
};

CSpecialAbilitiesFireballSpellDef& CSpecialAbilitiesFireballSpellDef::operator=(const CSpecialAbilitiesFireballSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    ReleaseDelay = other.ReleaseDelay;
    Inclination = other.Inclination;
    StaminaCost = other.StaminaCost;
    TargettingArcAngle = other.TargettingArcAngle;
    return *this;
}

int main()
{
    CSpecialAbilitiesFireballSpellDef a, b;
    a.m_marker = 7;
    a.ReleaseDelay = 1.5f;
    b.ReleaseDelay = 0.0f;
    a.Inclination = 2.5f;
    b.Inclination = 0.0f;
    a.StaminaCost.m_id = 203;
    a.TargettingArcAngle = 4.5f;
    b.TargettingArcAngle = 0.0f;
    CSpecialAbilitiesFireballSpellDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 1
        || g_stringAssigns != 0
        || b.ReleaseDelay != 1.5f
        || b.Inclination != 2.5f
        || b.StaminaCost.m_id != 203
        || b.TargettingArcAngle != 4.5f) {
        std::printf("00460256_TEST FAIL\n");
        return 1;
    }
    std::printf("00460256_TEST PASS\n");
    return 0;
}