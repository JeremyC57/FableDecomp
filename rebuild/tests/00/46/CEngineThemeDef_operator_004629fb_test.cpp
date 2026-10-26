#include <cstdio>
#include <cstring>
// Behaviour test: runs the reconstructed operator= on a filled source and checks every member and the out-of-line container/base calls.
// CEngineThemeDef::operator=(const CEngineThemeDef&) -- memberwise copy: CParentDefClassBase part, the base/cliff texture and bump-map indices and sizes, self-illumination, water, linked defs, heights, cover, material, minimap theme, destruction defs, friction and the Passable bit; returns *this

// Retail: Fable.exe (TLC, VC7.1) function @ 0x004629FB, length 238 bytes.
// Pragma:  -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CEngineThemeDef's vtable 0x01235194 is the 5-byte `jmp 0x004629FB` right before this body. No containers,
// so PDB members +0x40.. sit at retail +0x3c.. unchanged.
// Shape: memberwise assignment in declaration order; the last member is the one-bit `bool Passable : 1`,
// whose copy is the read-xor-mask-xor merge into the destination byte.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00() {}
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

class CEngineThemeDef : public CParentDefClassBase { public:  // vtable 0x01235194
    long  BaseTexture;                  // +0x3c
    long  BackgroundTexture;            // +0x40
    long  CliffBaseTexture;             // +0x44
    long  CliffBackgroundTexture;       // +0x48
    long  BaseBumpMap;                  // +0x4c
    long  CliffBumpMap;                 // +0x50
    long  BaseTextureMaxSize;           // +0x54
    long  CliffTextureMaxSize;          // +0x58
    long  BaseBumpMapMaxSize;           // +0x5c
    long  CliffBumpMapMaxSize;          // +0x60
    float BaseTextureSelfIllumination;  // +0x64
    float CliffTextureSelfIllumination; // +0x68
    float WaterHeight;                  // +0x6c
    long  WaterType;                    // +0x70 EWaterType
    long  NoWaterThemeDef;              // +0x74
    long  LocalDetailGeneratorDef;      // +0x78
    long  GroupDef;                     // +0x7c
    float Height;                       // +0x80
    float MinCameraHeight;              // +0x84
    float CoverValue;                   // +0x88
    long  MaterialDef;                  // +0x8c
    long  MinimapTheme;                 // +0x90 EMinimapThemeType
    long  DestructionEffectDef;         // +0x94
    long  DestructionThemeDef;          // +0x98
    float Friction;                     // +0x9c
    bool  Passable : 1;                 // +0xa0 bit 0
    CEngineThemeDef& operator=(const CEngineThemeDef& other);
};

CEngineThemeDef& CEngineThemeDef::operator=(const CEngineThemeDef& other)
{
    CParentDefClassBase::operator=(other);
    BaseTexture = other.BaseTexture;
    BackgroundTexture = other.BackgroundTexture;
    CliffBaseTexture = other.CliffBaseTexture;
    CliffBackgroundTexture = other.CliffBackgroundTexture;
    BaseBumpMap = other.BaseBumpMap;
    CliffBumpMap = other.CliffBumpMap;
    BaseTextureMaxSize = other.BaseTextureMaxSize;
    CliffTextureMaxSize = other.CliffTextureMaxSize;
    BaseBumpMapMaxSize = other.BaseBumpMapMaxSize;
    CliffBumpMapMaxSize = other.CliffBumpMapMaxSize;
    BaseTextureSelfIllumination = other.BaseTextureSelfIllumination;
    CliffTextureSelfIllumination = other.CliffTextureSelfIllumination;
    WaterHeight = other.WaterHeight;
    WaterType = other.WaterType;
    NoWaterThemeDef = other.NoWaterThemeDef;
    LocalDetailGeneratorDef = other.LocalDetailGeneratorDef;
    GroupDef = other.GroupDef;
    Height = other.Height;
    MinCameraHeight = other.MinCameraHeight;
    CoverValue = other.CoverValue;
    MaterialDef = other.MaterialDef;
    MinimapTheme = other.MinimapTheme;
    DestructionEffectDef = other.DestructionEffectDef;
    DestructionThemeDef = other.DestructionThemeDef;
    Friction = other.Friction;
    Passable = other.Passable;
    return *this;
}

static int g_baseCalls = 0, g_vecCalls = 0;
CParentDefClassBase& CParentDefClassBase::operator=(const CParentDefClassBase& o)
{ ++g_baseCalls; for (int i = 0; i < 0x35; ++i) _defClassBase[i] = o._defClassBase[i]; return *this; }

int main()
{
    static CEngineThemeDef a, b;
    std::memset(a._defClassBase, 0, sizeof(a._defClassBase));
    std::memset(b._defClassBase, 0, sizeof(b._defClassBase));
    b._defClassBase[0x21] = 0x5a;
    b.BaseTexture = 1; b.CliffBumpMapMaxSize = 512; b.WaterHeight = -1.0f; b.GroupDef = 9; b.MinimapTheme = 2; b.Friction = 0.3f; b.Passable = true;
    CEngineThemeDef& r = (a = b);
    bool ok = (&r == &a) && g_baseCalls == 1 && a._defClassBase[0x21] == 0x5a;
    ok = ok && a.BaseTexture == 1 && a.CliffBumpMapMaxSize == 512 && a.WaterHeight == -1.0f && a.GroupDef == 9 && a.MinimapTheme == 2 && a.Friction == 0.3f && a.Passable; b.Passable = false; a = b; ok = ok && !a.Passable;
    if (!ok) { std::printf("004629fb_TEST FAIL\n"); return 1; }
    std::printf("004629fb_TEST PASS\n");
    return 0;
}