// CEngineThemeDef::Transfer(CPersistContext&) -- persist the engine theme definition: the texture indices and sizes, illumination floats, group/height/cover/material, minimap type, destruction defs, friction, water height/type/no-water def, then the "Passable" bit (default true)
// Leaf: ?Transfer@CEngineThemeDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045979B, length 368 bytes (virtual, `ret 4`).
// Pragma: #pragma optimize("s", on) (unaligned entry, def module; ebp frame).
// Callees: CPersistContext::Transfer(long&) @ 0x00431102, Transfer(float&) @ 0x00431061 (the codex-verified typed
//   transfers), the enum instantiations Transfer(EMinimapThemeType&) @ 0x0045990B and Transfer(EWaterType&) @ 0x004599AF
//   (both CheckCRC + switch bodies right after this function), Transfer<bool>(name, value, default) @ 0x004045C0.
// Layout: Ego_r.pdb CEngineThemeDef (168 bytes) minus 4 (retail CDefClassBase is 0x3c bytes): BaseTexture +0x3c ..
//   CliffBumpMapMaxSize +0x60, BaseTextureSelfIllumination +0x64, CliffTextureSelfIllumination +0x68, WaterHeight +0x6c,
//   WaterType +0x70, NoWaterThemeDef +0x74, LocalDetailGeneratorDef +0x78, GroupDef +0x7c, Height +0x80,
//   MinCameraHeight +0x84, CoverValue +0x88, MaterialDef +0x8c, MinimapTheme +0x90, DestructionEffectDef +0x94,
//   DestructionThemeDef +0x98, Friction +0x9c, Passable bit 0 of +0xa0 (no generated engine header for the class).
// Shape: one unnamed transfer per member in the persisted (not declaration) order; the bitfield goes through a bool
//   local homed in the dead context-argument slot ([ebp+0Bh]) and is merged back with the xor/and/xor bit store; the
//   `true` default is a one-byte temporary at [ebp-1].
#pragma optimize("s", on)

enum EWaterType { WATER_TYPE_NONE };
enum EMinimapThemeType { MINIMAP_THEME_NONE };

class CPersistContext { public:
    template <class T> void Transfer(T& value);                                              // 0x00431102
    void Transfer(EMinimapThemeType& value);                                 // 0x0045990B
    void Transfer(EWaterType& value);                                        // 0x004599AF
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0
};

class CDefClassBase { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

class CEngineThemeDef : public CDefClassBase { public:
    long              BaseTexture;                    // +0x3c
    long              BackgroundTexture;              // +0x40
    long              CliffBaseTexture;               // +0x44
    long              CliffBackgroundTexture;         // +0x48
    long              BaseBumpMap;                    // +0x4c
    long              CliffBumpMap;                   // +0x50
    long              BaseTextureMaxSize;             // +0x54
    long              CliffTextureMaxSize;            // +0x58
    long              BaseBumpMapMaxSize;             // +0x5c
    long              CliffBumpMapMaxSize;            // +0x60
    float             BaseTextureSelfIllumination;    // +0x64
    float             CliffTextureSelfIllumination;   // +0x68
    float             WaterHeight;                    // +0x6c
    EWaterType        WaterType;                      // +0x70
    long              NoWaterThemeDef;                // +0x74
    long              LocalDetailGeneratorDef;        // +0x78
    long              GroupDef;                       // +0x7c
    float             Height;                         // +0x80
    float             MinCameraHeight;                // +0x84
    float             CoverValue;                     // +0x88
    long              MaterialDef;                    // +0x8c
    EMinimapThemeType MinimapTheme;                   // +0x90
    long              DestructionEffectDef;           // +0x94
    long              DestructionThemeDef;            // +0x98
    float             Friction;                       // +0x9c
    bool              Passable : 1;                   // +0xa0 bit 0
    virtual void Transfer(CPersistContext& context);
};

void CEngineThemeDef::Transfer(CPersistContext& context)
{
    context.Transfer(BaseTexture);
    context.Transfer(BackgroundTexture);
    context.Transfer(CliffBaseTexture);
    context.Transfer(CliffBackgroundTexture);
    context.Transfer(BaseBumpMap);
    context.Transfer(CliffBumpMap);
    context.Transfer(BaseTextureMaxSize);
    context.Transfer(CliffTextureMaxSize);
    context.Transfer(BaseBumpMapMaxSize);
    context.Transfer(CliffBumpMapMaxSize);
    context.Transfer(BaseTextureSelfIllumination);
    context.Transfer(CliffTextureSelfIllumination);
    context.Transfer(GroupDef);
    context.Transfer(Height);
    context.Transfer(MinCameraHeight);
    context.Transfer(CoverValue);
    context.Transfer(MaterialDef);
    context.Transfer(MinimapTheme);
    context.Transfer(LocalDetailGeneratorDef);
    context.Transfer(DestructionEffectDef);
    context.Transfer(DestructionThemeDef);
    context.Transfer(Friction);
    context.Transfer(WaterHeight);
    context.Transfer(WaterType);
    context.Transfer(NoWaterThemeDef);
    bool passable = Passable;
    context.Transfer("Passable", passable, true);
    Passable = passable;
}