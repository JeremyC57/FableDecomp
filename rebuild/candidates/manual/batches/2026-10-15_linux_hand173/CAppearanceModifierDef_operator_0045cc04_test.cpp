// behaviour test for CAppearanceModifierDef::operator= (0x0045CC04): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CCharStringVector { unsigned long m_words[3]; CCharStringVector& operator=(const CCharStringVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };struct CAppearanceModifierGraphics { unsigned long m_words[3]; CAppearanceModifierGraphics& operator=(const CAppearanceModifierGraphics& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };
struct CAppearanceModifierDef : CDefClassBase {
    CCharStringVector HideMaterials;
    CAppearanceModifierGraphics MorphGraphics;
    long CoversBodyAreaFlags;
    bool IsRemoveable;
    long HeroSuit;
    long SuitPart;
    long AppearanceType;
    float SoundRadiusMultiplier;
    float VisibilityMultiplier;
    long ArmourDefIndex;
    float OveralProtectionWeighting;
    float Scariness;
    float Attractiveness;
    float Goodstrength;
    CAppearanceModifierDef& operator=(const CAppearanceModifierDef& other);
};
CAppearanceModifierDef& CAppearanceModifierDef::operator=(const CAppearanceModifierDef& other)
{
    CDefClassBase::operator=(other);
    HideMaterials = other.HideMaterials;
    MorphGraphics = other.MorphGraphics;
    CoversBodyAreaFlags = other.CoversBodyAreaFlags;
    IsRemoveable = other.IsRemoveable;
    HeroSuit = other.HeroSuit;
    SuitPart = other.SuitPart;
    AppearanceType = other.AppearanceType;
    SoundRadiusMultiplier = other.SoundRadiusMultiplier;
    VisibilityMultiplier = other.VisibilityMultiplier;
    ArmourDefIndex = other.ArmourDefIndex;
    OveralProtectionWeighting = other.OveralProtectionWeighting;
    Scariness = other.Scariness;
    Attractiveness = other.Attractiveness;
    Goodstrength = other.Goodstrength;
    return *this;
}
int main()
{
    CAppearanceModifierDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.IsRemoveable = true;
    CAppearanceModifierDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.HideMaterials; const unsigned char* y = (const unsigned char*)&b.HideMaterials; for (unsigned int i = 0; i < sizeof(a.HideMaterials); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.MorphGraphics; const unsigned char* y = (const unsigned char*)&b.MorphGraphics; for (unsigned int i = 0; i < sizeof(a.MorphGraphics); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.CoversBodyAreaFlags; const unsigned char* y = (const unsigned char*)&b.CoversBodyAreaFlags; for (unsigned int i = 0; i < sizeof(a.CoversBodyAreaFlags); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.IsRemoveable; const unsigned char* y = (const unsigned char*)&b.IsRemoveable; for (unsigned int i = 0; i < sizeof(a.IsRemoveable); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HeroSuit; const unsigned char* y = (const unsigned char*)&b.HeroSuit; for (unsigned int i = 0; i < sizeof(a.HeroSuit); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SuitPart; const unsigned char* y = (const unsigned char*)&b.SuitPart; for (unsigned int i = 0; i < sizeof(a.SuitPart); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.AppearanceType; const unsigned char* y = (const unsigned char*)&b.AppearanceType; for (unsigned int i = 0; i < sizeof(a.AppearanceType); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundRadiusMultiplier; const unsigned char* y = (const unsigned char*)&b.SoundRadiusMultiplier; for (unsigned int i = 0; i < sizeof(a.SoundRadiusMultiplier); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.VisibilityMultiplier; const unsigned char* y = (const unsigned char*)&b.VisibilityMultiplier; for (unsigned int i = 0; i < sizeof(a.VisibilityMultiplier); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ArmourDefIndex; const unsigned char* y = (const unsigned char*)&b.ArmourDefIndex; for (unsigned int i = 0; i < sizeof(a.ArmourDefIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.OveralProtectionWeighting; const unsigned char* y = (const unsigned char*)&b.OveralProtectionWeighting; for (unsigned int i = 0; i < sizeof(a.OveralProtectionWeighting); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Scariness; const unsigned char* y = (const unsigned char*)&b.Scariness; for (unsigned int i = 0; i < sizeof(a.Scariness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Attractiveness; const unsigned char* y = (const unsigned char*)&b.Attractiveness; for (unsigned int i = 0; i < sizeof(a.Attractiveness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Goodstrength; const unsigned char* y = (const unsigned char*)&b.Goodstrength; for (unsigned int i = 0; i < sizeof(a.Goodstrength); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("0045cc04_TEST FAIL\n"); return 1; }
    std::printf("0045cc04_TEST PASS\n");
    return 0;
}
