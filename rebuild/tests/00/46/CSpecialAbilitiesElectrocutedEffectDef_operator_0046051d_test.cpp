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

class CSpecialAbilitiesElectrocutedEffectDef : public CParentDefClassBase { public:
    float FadeInTimeSecs;
    float FadeOutTimeStartSecs;
    float FadeOutTimeSecs;
    CContainer Color;
    CContainer HighlightColor;
    float HighlightWidth;
    float HeightSmall;
    float HeightMedium;
    CSpecialAbilitiesElectrocutedEffectDef& operator=(const CSpecialAbilitiesElectrocutedEffectDef& other);
};

CSpecialAbilitiesElectrocutedEffectDef& CSpecialAbilitiesElectrocutedEffectDef::operator=(const CSpecialAbilitiesElectrocutedEffectDef& other)
{
    CParentDefClassBase::operator=(other);
    FadeInTimeSecs = other.FadeInTimeSecs;
    FadeOutTimeStartSecs = other.FadeOutTimeStartSecs;
    FadeOutTimeSecs = other.FadeOutTimeSecs;
    Color = other.Color;
    HighlightColor = other.HighlightColor;
    HighlightWidth = other.HighlightWidth;
    HeightSmall = other.HeightSmall;
    HeightMedium = other.HeightMedium;
    return *this;
}

int main()
{
    CSpecialAbilitiesElectrocutedEffectDef a, b;
    a.m_marker = 7;
    a.FadeInTimeSecs = 1.5f;
    b.FadeInTimeSecs = 0.0f;
    a.FadeOutTimeStartSecs = 2.5f;
    b.FadeOutTimeStartSecs = 0.0f;
    a.FadeOutTimeSecs = 3.5f;
    b.FadeOutTimeSecs = 0.0f;
    a.Color.m_id = 204;
    a.HighlightColor.m_id = 205;
    a.HighlightWidth = 6.5f;
    b.HighlightWidth = 0.0f;
    a.HeightSmall = 7.5f;
    b.HeightSmall = 0.0f;
    a.HeightMedium = 8.5f;
    b.HeightMedium = 0.0f;
    CSpecialAbilitiesElectrocutedEffectDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 2
        || g_stringAssigns != 0
        || b.FadeInTimeSecs != 1.5f
        || b.FadeOutTimeStartSecs != 2.5f
        || b.FadeOutTimeSecs != 3.5f
        || b.Color.m_id != 204
        || b.HighlightColor.m_id != 205
        || b.HighlightWidth != 6.5f
        || b.HeightSmall != 7.5f
        || b.HeightMedium != 8.5f) {
        std::printf("0046051d_TEST FAIL\n");
        return 1;
    }
    std::printf("0046051d_TEST PASS\n");
    return 0;
}