#include <cstdio>
#include <cstddef>
// Behaviour test for CConfigOptionsDefaultsDef::Transfer @ 0x00434290: fourteen transfers in persist-list order, each
// through the helper of the member's type (recording specialisations), at the retail member offsets.
static const void* g_order[32]; static int g_kind[32]; static int g_n = 0;
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};

class CConfigOptionsDefaultsDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x38];   // CParentDefClassBase body
    unsigned long Antialiasing;    // +0x3c
    long  ResolutionWidth;         // +0x40
    long  ResolutionHeight;        // +0x44
    long  BitDepth;                // +0x48
    long  MinResolutionWidth;      // +0x4c
    long  MinResolutionHeight;     // +0x50
    float TextureDetail;           // +0x54
    float MaxTextureDetail;        // +0x58
    float ShadowDetail;            // +0x5c
    float MaxShadowDetail;         // +0x60
    float MeshDetail;              // +0x64
    float MaxMeshDetail;           // +0x68
    float EffectsDetail;           // +0x6c
    float MaxEffectsDetail;        // +0x70
};

template <> void CPersistContext::Transfer<long>(long& value);
template <> void CPersistContext::Transfer<float>(float& value);
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value);
void CConfigOptionsDefaultsDef::Transfer(CPersistContext& context)
{
    context.Transfer(Antialiasing);
    context.Transfer(ResolutionWidth);
    context.Transfer(ResolutionHeight);
    context.Transfer(BitDepth);
    context.Transfer(TextureDetail);
    context.Transfer(MaxTextureDetail);
    context.Transfer(ShadowDetail);
    context.Transfer(MaxShadowDetail);
    context.Transfer(MeshDetail);
    context.Transfer(MaxMeshDetail);
    context.Transfer(EffectsDetail);
    context.Transfer(MaxEffectsDetail);
    context.Transfer(MinResolutionWidth);
    context.Transfer(MinResolutionHeight);
}
template <> void CPersistContext::Transfer<long>(long& p) { g_kind[g_n] = 0; g_order[g_n++] = &p; }
template <> void CPersistContext::Transfer<float>(float& p) { g_kind[g_n] = 1; g_order[g_n++] = &p; }
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& p) { g_kind[g_n] = 2; g_order[g_n++] = &p; }
static CConfigOptionsDefaultsDef d;
int main()
{
    CPersistContext c;
    d.Transfer(c);
    const void* exp[14] = {&d.Antialiasing,&d.ResolutionWidth,&d.ResolutionHeight,&d.BitDepth,&d.TextureDetail,
        &d.MaxTextureDetail,&d.ShadowDetail,&d.MaxShadowDetail,&d.MeshDetail,&d.MaxMeshDetail,&d.EffectsDetail,
        &d.MaxEffectsDetail,&d.MinResolutionWidth,&d.MinResolutionHeight};
    static const int kind[14] = {2,0,0,0,1,1,1,1,1,1,1,1,0,0};
    if (g_n != 14 || offsetof(CConfigOptionsDefaultsDef, Antialiasing) != 0x3c
        || offsetof(CConfigOptionsDefaultsDef, MaxEffectsDetail) != 0x70) { std::printf("00434290_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 14; ++i)
        if (g_kind[i] != kind[i] || g_order[i] != exp[i]) { std::printf("00434290_TEST FAIL\n"); return 1; }
    std::printf("00434290_TEST PASS\n");
    return 0;
}
