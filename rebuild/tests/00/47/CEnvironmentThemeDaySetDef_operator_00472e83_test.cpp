// Behaviour test for CEnvironmentThemeDaySetDef::operator= (0x00472E83).
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x38];
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        for (int i = 0; i < 0x38; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CEnvironmentThemeDefArray {
    void* m_pData; long m_size; long m_capacity;
    CEnvironmentThemeDefArray& operator=(const CEnvironmentThemeDefArray& o)
    { m_pData = o.m_pData; m_size = o.m_size; m_capacity = o.m_capacity; g_order[g_calls++] = 2; return *this; }
};

struct CRGBColour {
    unsigned char B, G, R, A;
};

struct CEnvironmentThemeDaySetDef : CParentDefClassBase {
    CEnvironmentThemeDefArray TimeTheme;
    float                     SunTilt;
    float                     SunRotate;
    float                     SunHeight;
    float                     MoonTilt;
    float                     MoonRotate;
    float                     MoonHeight;
    long                      ColourLookupColumn;
    CRGBColour                EditorColour;
    float                     FishWeightMult;
    CEnvironmentThemeDaySetDef& operator=(const CEnvironmentThemeDaySetDef& other);
};

CEnvironmentThemeDaySetDef& CEnvironmentThemeDaySetDef::operator=(const CEnvironmentThemeDaySetDef& other)
{
    CParentDefClassBase::operator=(other);
    TimeTheme = other.TimeTheme;
    SunTilt = other.SunTilt;
    SunRotate = other.SunRotate;
    SunHeight = other.SunHeight;
    MoonTilt = other.MoonTilt;
    MoonRotate = other.MoonRotate;
    MoonHeight = other.MoonHeight;
    ColourLookupColumn = other.ColourLookupColumn;
    EditorColour = other.EditorColour;
    FishWeightMult = other.FishWeightMult;
    return *this;
}

int main()
{
    static int themes[2];
    CEnvironmentThemeDaySetDef a, b;
    for (int i = 0; i < 0x38; ++i) { a._body[i] = 0; b._body[i] = 2; }
    a.TimeTheme.m_pData = 0; a.TimeTheme.m_size = 0; a.TimeTheme.m_capacity = 0;
    b.TimeTheme.m_pData = themes; b.TimeTheme.m_size = 2; b.TimeTheme.m_capacity = 2;
    a.SunTilt = a.SunRotate = a.SunHeight = a.MoonTilt = a.MoonRotate = a.MoonHeight = 0.0f;
    b.SunTilt = 1.0f; b.SunRotate = 2.0f; b.SunHeight = 3.0f; b.MoonTilt = 4.0f; b.MoonRotate = 5.0f; b.MoonHeight = 6.0f;
    a.ColourLookupColumn = 0; b.ColourLookupColumn = 7;
    a.EditorColour.B = a.EditorColour.G = a.EditorColour.R = a.EditorColour.A = 0;
    b.EditorColour.B = 10; b.EditorColour.G = 20; b.EditorColour.R = 30; b.EditorColour.A = 40;
    a.FishWeightMult = 0.0f; b.FishWeightMult = 0.25f;
    CEnvironmentThemeDaySetDef& r = (a = b);
    if (&r != &a || g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || a._body[1] != 2
        || a.TimeTheme.m_pData != themes || a.TimeTheme.m_size != 2 || a.SunTilt != 1.0f || a.SunRotate != 2.0f
        || a.SunHeight != 3.0f || a.MoonTilt != 4.0f || a.MoonRotate != 5.0f || a.MoonHeight != 6.0f
        || a.ColourLookupColumn != 7 || a.EditorColour.R != 30 || a.EditorColour.A != 40 || a.FishWeightMult != 0.25f) {
        std::printf("00472e83_TEST FAIL\n");
        return 1;
    }
    std::printf("00472e83_TEST PASS\n");
    return 0;
}