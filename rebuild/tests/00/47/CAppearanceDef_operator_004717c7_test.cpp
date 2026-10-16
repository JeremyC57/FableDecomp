// behaviour test for CAppearanceDef::operator= (0x004717C7): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };

#pragma pack(push, 1)
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
struct CAnimationSet { unsigned long m_words[8]; CAnimationSet& operator=(const CAnimationSet& o) { ++g_ops; for (int i = 0; i < 8; ++i) m_words[i] = o.m_words[i]; return *this; } };
struct CAppearanceDef : CDefClassBase {
    CEngineGraphic Graphic;
    CAnimationSet Animation;
    unsigned char OutlineEffectAlphaRef;
    unsigned char OutlineEffectAlphaBias;
    bool OverrideLightingChannel;
    long LightingChannel;
    CEngineGraphic FadeOutWhenCloseGraphic;
    bool HasFadeOutWhenCloseGraphic;
    float FadeOutGraphicRenderFadeDistance;
    float FadeOutGraphicNearRenderFadeDistance;
    float FadeOutGraphicNearRenderClipDistance;
    CAppearanceDef& operator=(const CAppearanceDef& other);
};
CAppearanceDef& CAppearanceDef::operator=(const CAppearanceDef& other)
{
    CDefClassBase::operator=(other);
    Graphic = other.Graphic;
    Animation = other.Animation;
    OutlineEffectAlphaRef = other.OutlineEffectAlphaRef;
    OutlineEffectAlphaBias = other.OutlineEffectAlphaBias;
    OverrideLightingChannel = other.OverrideLightingChannel;
    LightingChannel = other.LightingChannel;
    FadeOutWhenCloseGraphic = other.FadeOutWhenCloseGraphic;
    HasFadeOutWhenCloseGraphic = other.HasFadeOutWhenCloseGraphic;
    FadeOutGraphicRenderFadeDistance = other.FadeOutGraphicRenderFadeDistance;
    FadeOutGraphicNearRenderFadeDistance = other.FadeOutGraphicNearRenderFadeDistance;
    FadeOutGraphicNearRenderClipDistance = other.FadeOutGraphicNearRenderClipDistance;
    return *this;
}
int main()
{
    CAppearanceDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.OverrideLightingChannel = true;
    a.HasFadeOutWhenCloseGraphic = true;
    CAppearanceDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.Graphic; const unsigned char* y = (const unsigned char*)&b.Graphic; for (unsigned int i = 0; i < sizeof(a.Graphic); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.Animation; const unsigned char* y = (const unsigned char*)&b.Animation; for (unsigned int i = 0; i < sizeof(a.Animation); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.OutlineEffectAlphaRef; const unsigned char* y = (const unsigned char*)&b.OutlineEffectAlphaRef; for (unsigned int i = 0; i < sizeof(a.OutlineEffectAlphaRef); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.OutlineEffectAlphaBias; const unsigned char* y = (const unsigned char*)&b.OutlineEffectAlphaBias; for (unsigned int i = 0; i < sizeof(a.OutlineEffectAlphaBias); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.OverrideLightingChannel; const unsigned char* y = (const unsigned char*)&b.OverrideLightingChannel; for (unsigned int i = 0; i < sizeof(a.OverrideLightingChannel); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.LightingChannel; const unsigned char* y = (const unsigned char*)&b.LightingChannel; for (unsigned int i = 0; i < sizeof(a.LightingChannel); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FadeOutWhenCloseGraphic; const unsigned char* y = (const unsigned char*)&b.FadeOutWhenCloseGraphic; for (unsigned int i = 0; i < sizeof(a.FadeOutWhenCloseGraphic); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HasFadeOutWhenCloseGraphic; const unsigned char* y = (const unsigned char*)&b.HasFadeOutWhenCloseGraphic; for (unsigned int i = 0; i < sizeof(a.HasFadeOutWhenCloseGraphic); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FadeOutGraphicRenderFadeDistance; const unsigned char* y = (const unsigned char*)&b.FadeOutGraphicRenderFadeDistance; for (unsigned int i = 0; i < sizeof(a.FadeOutGraphicRenderFadeDistance); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FadeOutGraphicNearRenderFadeDistance; const unsigned char* y = (const unsigned char*)&b.FadeOutGraphicNearRenderFadeDistance; for (unsigned int i = 0; i < sizeof(a.FadeOutGraphicNearRenderFadeDistance); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FadeOutGraphicNearRenderClipDistance; const unsigned char* y = (const unsigned char*)&b.FadeOutGraphicNearRenderClipDistance; for (unsigned int i = 0; i < sizeof(a.FadeOutGraphicNearRenderClipDistance); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004717c7_TEST FAIL\n"); return 1; }
    std::printf("004717c7_TEST PASS\n");
    return 0;
}