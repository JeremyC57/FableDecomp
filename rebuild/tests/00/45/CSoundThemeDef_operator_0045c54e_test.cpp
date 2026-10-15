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

struct CSoundThemeDef : CParentDefClassBase {
    CRGBColour EditColour;
    long AtmosIndex;
    long MusicSet;
    float Gain;
    float Range;
    float Granularity;
    CContainer AtmosGroupMap;
    bool PlayOnlyWhenInside;
    bool PlayQuietlyWhenInside;
    bool PlayOnlyWhenOutside;
    bool PlayQuietlyWhenOutside;
    CSoundThemeDef& operator=(const CSoundThemeDef& other);
};

CSoundThemeDef& CSoundThemeDef::operator=(const CSoundThemeDef& other)
{
    CParentDefClassBase::operator=(other);
    EditColour = other.EditColour;
    AtmosIndex = other.AtmosIndex;
    MusicSet = other.MusicSet;
    Gain = other.Gain;
    Range = other.Range;
    Granularity = other.Granularity;
    AtmosGroupMap = other.AtmosGroupMap;
    PlayOnlyWhenInside = other.PlayOnlyWhenInside;
    PlayQuietlyWhenInside = other.PlayQuietlyWhenInside;
    PlayOnlyWhenOutside = other.PlayOnlyWhenOutside;
    PlayQuietlyWhenOutside = other.PlayQuietlyWhenOutside;
    return *this;
}

int main()
{
    CSoundThemeDef a, b;
    a.m_marker = 7;
    a.EditColour.m_packed = 0x11223301UL;
    b.EditColour.m_packed = 0;
    a.AtmosIndex = 102;
    b.AtmosIndex = 0;
    a.MusicSet = 103;
    b.MusicSet = 0;
    a.Gain = 4.5f;
    b.Gain = 0.0f;
    a.Range = 5.5f;
    b.Range = 0.0f;
    a.Granularity = 6.5f;
    b.Granularity = 0.0f;
    a.AtmosGroupMap.m_id = 207;
    a.PlayOnlyWhenInside = true;
    b.PlayOnlyWhenInside = false;
    a.PlayQuietlyWhenInside = true;
    b.PlayQuietlyWhenInside = false;
    a.PlayOnlyWhenOutside = true;
    b.PlayOnlyWhenOutside = false;
    a.PlayQuietlyWhenOutside = true;
    b.PlayQuietlyWhenOutside = false;
    CSoundThemeDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 1
        || g_stringAssigns != 0
        || b.EditColour.m_packed != 0x11223301UL
        || b.AtmosIndex != 102
        || b.MusicSet != 103
        || b.Gain != 4.5f
        || b.Range != 5.5f
        || b.Granularity != 6.5f
        || b.AtmosGroupMap.m_id != 207
        || b.PlayOnlyWhenInside != true
        || b.PlayQuietlyWhenInside != true
        || b.PlayOnlyWhenOutside != true
        || b.PlayQuietlyWhenOutside != true) {
        std::printf("0045c54e_TEST FAIL\n");
        return 1;
    }
    std::printf("0045c54e_TEST PASS\n");
    return 0;
}