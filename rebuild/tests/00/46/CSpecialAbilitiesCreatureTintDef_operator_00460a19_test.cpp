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

class CSpecialAbilitiesCreatureTintDef : public CParentDefClassBase { public:
    float FadeInTimeSecs;
    float FadeOutTimeSecs;
    CContainer Color;
    CSpecialAbilitiesCreatureTintDef& operator=(const CSpecialAbilitiesCreatureTintDef& other);
};

CSpecialAbilitiesCreatureTintDef& CSpecialAbilitiesCreatureTintDef::operator=(const CSpecialAbilitiesCreatureTintDef& other)
{
    CParentDefClassBase::operator=(other);
    FadeInTimeSecs = other.FadeInTimeSecs;
    FadeOutTimeSecs = other.FadeOutTimeSecs;
    Color = other.Color;
    return *this;
}

int main()
{
    CSpecialAbilitiesCreatureTintDef a, b;
    a.m_marker = 7;
    a.FadeInTimeSecs = 1.5f;
    b.FadeInTimeSecs = 0.0f;
    a.FadeOutTimeSecs = 2.5f;
    b.FadeOutTimeSecs = 0.0f;
    a.Color.m_id = 203;
    CSpecialAbilitiesCreatureTintDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 1
        || g_stringAssigns != 0
        || b.FadeInTimeSecs != 1.5f
        || b.FadeOutTimeSecs != 2.5f
        || b.Color.m_id != 203) {
        std::printf("00460a19_TEST FAIL\n");
        return 1;
    }
    std::printf("00460a19_TEST PASS\n");
    return 0;
}