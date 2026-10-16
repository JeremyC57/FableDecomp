// behaviour test for CTattooDef::operator= (0x00453D46): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CCharString { void* m_pStringData; CCharString& operator=(const CCharString& o) { ++g_ops; m_pStringData = o.m_pStringData; return *this; } };
struct CTattooDef : CDefClassBase {
    CCharString TattooName;
    long TextureLayer;
    long ReplacingTextureIndex;
    long BankIndex;
    long Blend;
    bool RemovesAllTattoos;
    bool Permanent;
    long CoversBodyAreaFlags;
    float VisibilityMultiplier;
    long SpecificCoversBodyAreaFlags;
    CCharString CustomFileName;
    CCharString CustomFileNameBase;
    float Attractiveness;
    float Scariness;
    float Goodstrength;
    CTattooDef& operator=(const CTattooDef& other);
};
CTattooDef& CTattooDef::operator=(const CTattooDef& other)
{
    CDefClassBase::operator=(other);
    TattooName = other.TattooName;
    TextureLayer = other.TextureLayer;
    ReplacingTextureIndex = other.ReplacingTextureIndex;
    BankIndex = other.BankIndex;
    Blend = other.Blend;
    RemovesAllTattoos = other.RemovesAllTattoos;
    Permanent = other.Permanent;
    CoversBodyAreaFlags = other.CoversBodyAreaFlags;
    VisibilityMultiplier = other.VisibilityMultiplier;
    SpecificCoversBodyAreaFlags = other.SpecificCoversBodyAreaFlags;
    CustomFileName = other.CustomFileName;
    CustomFileNameBase = other.CustomFileNameBase;
    Attractiveness = other.Attractiveness;
    Scariness = other.Scariness;
    Goodstrength = other.Goodstrength;
    return *this;
}
int main()
{
    CTattooDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.RemovesAllTattoos = true;
    a.Permanent = true;
    CTattooDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.TattooName; const unsigned char* y = (const unsigned char*)&b.TattooName; for (unsigned int i = 0; i < sizeof(a.TattooName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TextureLayer; const unsigned char* y = (const unsigned char*)&b.TextureLayer; for (unsigned int i = 0; i < sizeof(a.TextureLayer); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ReplacingTextureIndex; const unsigned char* y = (const unsigned char*)&b.ReplacingTextureIndex; for (unsigned int i = 0; i < sizeof(a.ReplacingTextureIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BankIndex; const unsigned char* y = (const unsigned char*)&b.BankIndex; for (unsigned int i = 0; i < sizeof(a.BankIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Blend; const unsigned char* y = (const unsigned char*)&b.Blend; for (unsigned int i = 0; i < sizeof(a.Blend); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RemovesAllTattoos; const unsigned char* y = (const unsigned char*)&b.RemovesAllTattoos; for (unsigned int i = 0; i < sizeof(a.RemovesAllTattoos); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Permanent; const unsigned char* y = (const unsigned char*)&b.Permanent; for (unsigned int i = 0; i < sizeof(a.Permanent); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CoversBodyAreaFlags; const unsigned char* y = (const unsigned char*)&b.CoversBodyAreaFlags; for (unsigned int i = 0; i < sizeof(a.CoversBodyAreaFlags); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.VisibilityMultiplier; const unsigned char* y = (const unsigned char*)&b.VisibilityMultiplier; for (unsigned int i = 0; i < sizeof(a.VisibilityMultiplier); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SpecificCoversBodyAreaFlags; const unsigned char* y = (const unsigned char*)&b.SpecificCoversBodyAreaFlags; for (unsigned int i = 0; i < sizeof(a.SpecificCoversBodyAreaFlags); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CustomFileName; const unsigned char* y = (const unsigned char*)&b.CustomFileName; for (unsigned int i = 0; i < sizeof(a.CustomFileName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CustomFileNameBase; const unsigned char* y = (const unsigned char*)&b.CustomFileNameBase; for (unsigned int i = 0; i < sizeof(a.CustomFileNameBase); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Attractiveness; const unsigned char* y = (const unsigned char*)&b.Attractiveness; for (unsigned int i = 0; i < sizeof(a.Attractiveness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Scariness; const unsigned char* y = (const unsigned char*)&b.Scariness; for (unsigned int i = 0; i < sizeof(a.Scariness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Goodstrength; const unsigned char* y = (const unsigned char*)&b.Goodstrength; for (unsigned int i = 0; i < sizeof(a.Goodstrength); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("00453d46_TEST FAIL\n"); return 1; }
    std::printf("00453d46_TEST PASS\n");
    return 0;
}