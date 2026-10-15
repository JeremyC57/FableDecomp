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

struct CSpecialAbilitiesTurncoatSpellDef : CParentDefClassBase {
    CContainer StaminaCostPerSec;
    CContainer ResistanceDamagePerSec;
    CContainer TurncoatTime;
    CContainer TurncoatRange;
    CContainer TurncoatXPUpEveryNSecs;
    CSpecialAbilitiesTurncoatSpellDef& operator=(const CSpecialAbilitiesTurncoatSpellDef& other);
};

CSpecialAbilitiesTurncoatSpellDef& CSpecialAbilitiesTurncoatSpellDef::operator=(const CSpecialAbilitiesTurncoatSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    StaminaCostPerSec = other.StaminaCostPerSec;
    ResistanceDamagePerSec = other.ResistanceDamagePerSec;
    TurncoatTime = other.TurncoatTime;
    TurncoatRange = other.TurncoatRange;
    TurncoatXPUpEveryNSecs = other.TurncoatXPUpEveryNSecs;
    return *this;
}

int main()
{
    CSpecialAbilitiesTurncoatSpellDef a, b;
    a.m_marker = 7;
    a.StaminaCostPerSec.m_id = 201;
    a.ResistanceDamagePerSec.m_id = 202;
    a.TurncoatTime.m_id = 203;
    a.TurncoatRange.m_id = 204;
    a.TurncoatXPUpEveryNSecs.m_id = 205;
    CSpecialAbilitiesTurncoatSpellDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 5
        || g_stringAssigns != 0
        || b.StaminaCostPerSec.m_id != 201
        || b.ResistanceDamagePerSec.m_id != 202
        || b.TurncoatTime.m_id != 203
        || b.TurncoatRange.m_id != 204
        || b.TurncoatXPUpEveryNSecs.m_id != 205) {
        std::printf("004601ea_TEST FAIL\n");
        return 1;
    }
    std::printf("004601ea_TEST PASS\n");
    return 0;
}