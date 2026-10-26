// behaviour test for 0x0046F4F0 CSkyDef::operator=: base part, 14 scalars and the flare vector are copied, *this returned
#include <cstdio>
static int g_baseCopies = 0, g_vecCopies = 0;
class CParentDefClassBase { public:
    unsigned char body[0x35];
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { ++g_baseCopies; for (int i = 0; i < 0x35; ++i) body[i] = o.body[i]; return *this; }
};
struct CVec { int* f; int* l; int* e; CVec& operator=(const CVec& o) { ++g_vecCopies; f = o.f; l = o.l; e = o.e; return *this; } };
class CSkyDef : public CParentDefClassBase { public:
    long SunRadius, SunTexture, MoonRadius, MoonTexture, SunFlareRadius, SunFlareTexture, StarTexture, StarChartTextureSize;
    float StarSize; long StarChartFilter, TwinkleInterval; float TwinkleSpeed, TwinkleMin, TwinkleMax;
    CVec FlareElements;
    CSkyDef& operator=(const CSkyDef& other)
    {
        CParentDefClassBase::operator=(other);
        SunRadius = other.SunRadius; SunTexture = other.SunTexture; MoonRadius = other.MoonRadius; MoonTexture = other.MoonTexture;
        SunFlareRadius = other.SunFlareRadius; SunFlareTexture = other.SunFlareTexture; StarTexture = other.StarTexture;
        StarChartTextureSize = other.StarChartTextureSize; StarSize = other.StarSize; StarChartFilter = other.StarChartFilter;
        TwinkleInterval = other.TwinkleInterval; TwinkleSpeed = other.TwinkleSpeed; TwinkleMin = other.TwinkleMin;
        TwinkleMax = other.TwinkleMax; FlareElements = other.FlareElements;
        return *this;
    }
};
int main()
{
    CSkyDef a, b; int x[3];
    for (int i = 0; i < 0x35; ++i) { a.body[i] = (unsigned char)i; b.body[i] = 0; }
    a.SunRadius = 1; a.SunTexture = 2; a.MoonRadius = 3; a.MoonTexture = 4; a.SunFlareRadius = 5; a.SunFlareTexture = 6;
    a.StarTexture = 7; a.StarChartTextureSize = 8; a.StarSize = 9.5f; a.StarChartFilter = 10; a.TwinkleInterval = 11;
    a.TwinkleSpeed = 12.5f; a.TwinkleMin = 13.5f; a.TwinkleMax = 14.5f; a.FlareElements.f = x; a.FlareElements.l = x + 1; a.FlareElements.e = x + 3;
    CSkyDef* r = &(b = a);
    if (r != &b || g_baseCopies != 1 || g_vecCopies != 1 || b.body[7] != 7 || b.SunRadius != 1 || b.StarTexture != 7 ||
        b.StarSize != 9.5f || b.TwinkleMax != 14.5f || b.TwinkleInterval != 11 || b.FlareElements.l != x + 1) { std::printf("0046f4f0_TEST FAIL\n"); return 1; }
    std::printf("0046f4f0_TEST PASS\n");
    return 0;
}