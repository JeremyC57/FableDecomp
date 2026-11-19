// NParticleEngine::CPSCSingleSprite::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- persist the single-sprite settings by name: the spline base first, then the sprite/trail graphics by symbol name (EEngineGraphic symbol lookup when saving, SymbolExists/GetSymbolSLONG when loading) and the position param (CRC of its name when loading), then every size, colour, blend, bit-flag, fade and trail field
// Grade: DIFFER(2034v2032) 44/580 -- every non-relocation line matches except one: the StayWithEmitter (+0x94 bit 0) write-back loads the bool into al before `xor cl,al`; retail folds it as `xor cl,byte ptr [esp+24h]` (2 bytes). Same single-instruction gap as the parked twin CPSCLight::DatabaseSerialise 0x00B1A870; 9 write-back levers inert (`!= 0`, ternary, reference, if/else, named default, unsigned char field, shared function-scope flag)
// Leaf: ?DatabaseSerialise@CPSCSingleSprite@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B079B0, 2032 bytes, `ret 8` (extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb ?DatabaseSerialise@CPSCSingleSprite@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z + the manifest row.
// Callees: CPSCSplineBase::DatabaseSerialise 0x00B22670 (non-virtual base call), CPersistContext::Transfer<bool> 0x004045C0,
//   <unsigned long> 0x004106F0, <CCharString> 0x004109A0, <float> 0x00410620, <long> 0x00410BE0, <CRGBColour> 0x006420F0;
//   CSymbolMap::FindFirstSymbolWithVal(const CCharString&, long) const 0x009E50C0, GetSymbolSLONG 0x009E5120, SymbolExists
//   0x009E51C0; CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0, copy 0x0099EC30, ~ 0x0099EAE0,
//   operator= 0x0099EFB0; CCRC::Calc 0x004014A0; NParticleEngine::GetParticleEmitterManager 0x00AEAAF0 (inline const
//   GetDefaultBlendMode 2 / GetDefaultBlendOp 0, the getter call is kept).
// Layout: PDB (Ego_r) CPSCSingleSprite, every retail offset the PDB one minus 4 (as the landed WriteBinary 0x00B06060);
//   CEmitterDatabaseSerialiseInfo { CSymbolMap* GraphicSymbols; CSymbolMap* MeshSymbols; }; CSymbolInfo { GroupName, EntryName, Val }.
// Shape: the "EEngineGraphic" group string is a block-scoped named local (a temporary in the call pushes the value first:
//   565 -> 51 diff lines); the loading CRC helper is two ternaries, length first (the if/else form colours eax/ecx the other way);
//   each bit flag through its own bool block; TrailBlendMode's default is a literal 4, SpriteAlignment's a literal 0.
class C3DVector { public: float X, Y, Z; };
class CRGBColour { public:
    union { struct { unsigned char B, G, R, A; }; unsigned long Colour; };
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCharStringData { public:
    const char* Data;                                   // +0
    long Length;                                        // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
inline unsigned long GetCRC(const CCharString& s)
{
    unsigned long length = s.PStringData == 0 ? 0 : s.PStringData->Length;
    const char* data = s.PStringData == 0 ? "" : s.PStringData->Data;
    return CCRC::Calc(0, data, length);
}
class CSymbolMap { public:
    class CSymbolInfo { public:
        CCharString GroupName;
        CCharString EntryName;
        long Val;
    };
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& group, long value) const;   // 0x009E50C0
    long GetSymbolSLONG(const CCharString& name) const;      // 0x009E5120
    bool SymbolExists(const CCharString& name) const;        // 0x009E51C0
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CEnginePrimitiveHandle { public: long Index; long Generation; };
namespace NParticleEngine {
class CEmitterDatabaseSerialiseInfo { public:
    CSymbolMap* GraphicSymbols;
    CSymbolMap* MeshSymbols;
};
class CParticleEmitterManager { public:
    const long GetDefaultBlendMode() const { return 2; }
    const long GetDefaultBlendOp() const { return 0; }
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0
class CPSCSplineBase { public:
    unsigned char _base[0x30];                          // CPSCBase + spline data after the vptr (retail size 0x34)
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);   // 0x00B22670
};
class CPSCSingleSprite : public CPSCSplineBase { public:
    long SpriteBankIndex;                               // +0x34
    CRGBColour StartColour;                             // +0x38
    CRGBColour MidColour;                               // +0x3c
    CRGBColour EndColour;                               // +0x40
    float AnimationTimeSecs;                            // +0x44
    float StartRenderSize;
    float EndRenderSize;
    long SpriteAlignment;                               // +0x50
    long NoCrossedSprites;
    float InitialAngle;                                 // +0x58
    long FadeInEndInteger;                              // +0x5c
    long FadeOutBeginInteger;
    float AlphaFadeMinimum;                             // +0x64
    float SizeFadeMinimum;
    unsigned long PositionParam;                        // +0x6c
    long BlendMode;                                     // +0x70
    long BlendOp;
    long TrailBlendMode;
    long TrailBlendOp;
    long TrailBankIndex;                                // +0x80
    float TrailWidth;                                   // +0x84
    long MaxTrailLength;                                // +0x88
    long StayWithEmitterIntFactor;                      // +0x8c
    bool UseStartColour;                                // +0x90
    bool UseMidColour;
    bool UseEndColour;
    bool SelfIlluminating : 1;                          // +0x93
    bool AlphaFadeEnable : 1;
    bool RotateAroundCentre : 1;
    bool FaceMe2D : 1;
    bool FaceMe3D : 1;
    bool CrossedSprites : 1;
    bool SizeFadeEnable : 1;
    bool ForceAnimationTime : 1;
    bool StayWithEmitter : 1;                           // +0x94
    bool UsePosition;                                   // +0x95
    bool UseSplinePoints;                               // +0x96
    bool EnteringTimelineEnd;                           // +0x97
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};
void CPSCSingleSprite::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    CPSCSplineBase::DatabaseSerialise(context, info);
    if (context.IsSaving()) {
        CCharString spriteName;
        const CSymbolMap::CSymbolInfo* sprite;
        {
            CCharString group("EEngineGraphic");
            sprite = info.GraphicSymbols->FindFirstSymbolWithVal(group, SpriteBankIndex);
        }
        if (sprite)
            spriteName = sprite->EntryName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        CCharString trailName;
        const CSymbolMap::CSymbolInfo* trail;
        {
            CCharString group("EEngineGraphic");
            trail = info.GraphicSymbols->FindFirstSymbolWithVal(group, TrailBankIndex);
        }
        if (trail)
            trailName = trail->EntryName;
        context.Transfer("TrailName", trailName, EmptyCCharString());
        context.Transfer("PositionParam", PositionParam, 0UL);
    } else {
        CCharString spriteName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        if (info.GraphicSymbols->SymbolExists(spriteName))
            SpriteBankIndex = info.GraphicSymbols->GetSymbolSLONG(spriteName);
        else
            SpriteBankIndex = 0;
        CCharString trailName;
        context.Transfer("TrailName", trailName, EmptyCCharString());
        if (info.GraphicSymbols->SymbolExists(trailName))
            TrailBankIndex = info.GraphicSymbols->GetSymbolSLONG(trailName);
        else
            TrailBankIndex = 0;
        CCharString paramName;
        context.Transfer("PositionParam", paramName, EmptyCCharString());
        PositionParam = GetCRC(paramName);
    }
    {
        bool force = ForceAnimationTime;
        context.Transfer("ForceAnimationTime", force, false);
        ForceAnimationTime = force;
    }
    context.Transfer("AnimationTimeSecs", AnimationTimeSecs, 0.0f);
    context.Transfer("StartRenderSize", StartRenderSize, 0.0f);
    context.Transfer("EndRenderSize", EndRenderSize, 0.0f);
    context.Transfer("StartColour", StartColour, CRGBColour(0xff, 0xff, 0xff, 0xff));
    context.Transfer("MidColour", MidColour, CRGBColour(0xff, 0xff, 0xff, 0xff));
    context.Transfer("EndColour", EndColour, CRGBColour(0xff, 0xff, 0xff, 0xff));
    context.Transfer("UseStartColour", UseStartColour, true);
    context.Transfer("UseMidColour", UseMidColour, false);
    context.Transfer("UseEndColour", UseEndColour, true);
    context.Transfer("BlendMode", BlendMode, GetParticleEmitterManager().GetDefaultBlendMode());
    context.Transfer("BlendOp", BlendOp, GetParticleEmitterManager().GetDefaultBlendOp());
    context.Transfer("TrailBlendMode", TrailBlendMode, 4L);
    context.Transfer("TrailBlendOp", TrailBlendOp, GetParticleEmitterManager().GetDefaultBlendOp());
    {
        bool v = FaceMe2D;
        context.Transfer("FaceMe2D", v, false);
        FaceMe2D = v;
    }
    {
        bool v = FaceMe3D;
        context.Transfer("FaceMe3D", v, false);
        FaceMe3D = v;
    }
    {
        bool v = CrossedSprites;
        context.Transfer("CrossedSprites", v, false);
        CrossedSprites = v;
    }
    context.Transfer("InitialAngle", InitialAngle, 0.0f);
    context.Transfer("SpriteAlignment", SpriteAlignment, 0L);
    {
        bool v = RotateAroundCentre;
        context.Transfer("RotateAroundCentre", v, false);
        RotateAroundCentre = v;
    }
    context.Transfer("NoCrossedSprites", NoCrossedSprites, 0L);
    context.Transfer("FadeInEndInteger", FadeInEndInteger, 0L);
    context.Transfer("FadeOutBeginInteger", FadeOutBeginInteger, 0L);
    {
        bool v = AlphaFadeEnable;
        context.Transfer("AlphaFadeEnable", v, false);
        AlphaFadeEnable = v;
    }
    context.Transfer("AlphaFadeMinimum", AlphaFadeMinimum, 0.0f);
    {
        bool v = SizeFadeEnable;
        context.Transfer("SizeFadeEnable", v, false);
        SizeFadeEnable = v;
    }
    context.Transfer("SizeFadeMinimum", SizeFadeMinimum, 0.0f);
    {
        bool v = SelfIlluminating;
        context.Transfer("SelfIlluminating", v, false);
        SelfIlluminating = v;
    }
    context.Transfer("TrailWidth", TrailWidth, 0.0f);
    context.Transfer("MaxTrailLength", MaxTrailLength, 0L);
    {
        bool v = StayWithEmitter;
        context.Transfer("StayWithEmitter", v, false);
        StayWithEmitter = v;
    }
    context.Transfer("UsePosition", UsePosition, true);
    context.Transfer("UseSplinePoints", UseSplinePoints, false);
}
}
