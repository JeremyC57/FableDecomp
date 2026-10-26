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
    CCharString& operator=(const CCharString& other)
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

class CSpecialAbilitiesGhostSwordDef : public CParentDefClassBase { public:
    float TimeOfSummon[4];
    float FadeInTime;
    float FadeOutTime;
    float CreationDistance;
    CSpecialAbilitiesGhostSwordDef& operator=(const CSpecialAbilitiesGhostSwordDef& other);
};

CSpecialAbilitiesGhostSwordDef& CSpecialAbilitiesGhostSwordDef::operator=(const CSpecialAbilitiesGhostSwordDef& other)
{
    CParentDefClassBase::operator=(other);
    for (int i = 0; i < 4; ++i)
        TimeOfSummon[i] = other.TimeOfSummon[i];
    FadeInTime = other.FadeInTime;
    FadeOutTime = other.FadeOutTime;
    CreationDistance = other.CreationDistance;
    return *this;
}

int main()
{
    CSpecialAbilitiesGhostSwordDef a, b;
    a.m_marker = 7;
    { for (int k = 0; k < 4; ++k) a.TimeOfSummon[k] = 1.0f + k; }
    { for (int k = 0; k < 4; ++k) b.TimeOfSummon[k] = 0.0f; }
    a.FadeInTime = 2.5f;
    b.FadeInTime = 0.0f;
    a.FadeOutTime = 3.5f;
    b.FadeOutTime = 0.0f;
    a.CreationDistance = 4.5f;
    b.CreationDistance = 0.0f;
    CSpecialAbilitiesGhostSwordDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 0
        || g_stringAssigns != 0
        || b.TimeOfSummon[0] != 1.0f || b.TimeOfSummon[3] != 4.0f
        || b.FadeInTime != 2.5f
        || b.FadeOutTime != 3.5f
        || b.CreationDistance != 4.5f) {
        std::printf("00460185_TEST FAIL\n");
        return 1;
    }
    std::printf("00460185_TEST PASS\n");
    return 0;
}