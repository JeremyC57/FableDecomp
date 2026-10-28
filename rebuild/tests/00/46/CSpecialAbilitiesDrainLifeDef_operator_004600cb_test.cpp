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

class CSpecialAbilitiesDrainLifeDef : public CParentDefClassBase { public:
    CContainer AmountOfHealthSucked;
    CContainer NumEffects;
    CContainer SpellRange;
    CContainer ShotName;
    float SpellDelaySecs;
    CSpecialAbilitiesDrainLifeDef& operator=(const CSpecialAbilitiesDrainLifeDef& other);
};

CSpecialAbilitiesDrainLifeDef& CSpecialAbilitiesDrainLifeDef::operator=(const CSpecialAbilitiesDrainLifeDef& other)
{
    CParentDefClassBase::operator=(other);
    AmountOfHealthSucked = other.AmountOfHealthSucked;
    NumEffects = other.NumEffects;
    SpellRange = other.SpellRange;
    ShotName = other.ShotName;
    SpellDelaySecs = other.SpellDelaySecs;
    return *this;
}

int main()
{
    CSpecialAbilitiesDrainLifeDef a, b;
    a.m_marker = 7;
    a.AmountOfHealthSucked.m_id = 201;
    a.NumEffects.m_id = 202;
    a.SpellRange.m_id = 203;
    a.ShotName.m_id = 204;
    a.SpellDelaySecs = 5.5f;
    b.SpellDelaySecs = 0.0f;
    CSpecialAbilitiesDrainLifeDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 4
        || g_stringAssigns != 0
        || b.AmountOfHealthSucked.m_id != 201
        || b.NumEffects.m_id != 202
        || b.SpellRange.m_id != 203
        || b.ShotName.m_id != 204
        || b.SpellDelaySecs != 5.5f) {
        std::printf("004600cb_TEST FAIL\n");
        return 1;
    }
    std::printf("004600cb_TEST PASS\n");
    return 0;
}