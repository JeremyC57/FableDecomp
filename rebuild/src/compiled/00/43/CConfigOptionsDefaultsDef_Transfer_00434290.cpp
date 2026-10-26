// CConfigOptionsDefaultsDef::Transfer(CPersistContext& context) -- persist the fourteen option defaults (+0x3c..+0x70) through the one-argument CPersistContext::Transfer<T> helpers
// Leaf: ?Transfer@CConfigOptionsDefaultsDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434290, 167 bytes (176 with padding), virtual `?Transfer@CConfigOptionsDefaultsDef@@UAEXAAVCPersistContext@@@Z`.
// Callees: CPersistContext::Transfer<unsigned long> 0x00431020 (+0x3c), Transfer<long> 0x00431102 (+0x40..+0x50),
//          Transfer<float> 0x00431061 (+0x54..+0x70) (namepass TR2; each starts `push ebp ; ... push 0x122d70e` = CheckCRC("")).
// Shape: the def base is 0x3c bytes; the members are 4-byte values transferred by address. The order of the
// calls is the declaration order of the persist list, not the layout (+0x4c/+0x50 come last).
// Layout: Ego_r.pdb CConfigOptionsDefaultsDef (0x78) less 4: Antialiasing (unsigned long) +0x3c, ResolutionWidth /
// ResolutionHeight / BitDepth / MinResolutionWidth / MinResolutionHeight (long) +0x40..+0x50, the eight detail floats
// +0x54..+0x70 (namepass DEF1: the member types now follow the PDB; the old model picked them per callee).
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
