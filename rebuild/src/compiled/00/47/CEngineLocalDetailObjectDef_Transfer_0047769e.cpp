// CEngineLocalDetailObjectDef::Transfer(CPersistContext&) -- persist the local detail object definition: meshes, probability / scale / fade floats, z-sprite and slope fades, alpha and wind-skew parameters, then the eight named flag bits
// Leaf: ?Transfer@CEngineLocalDetailObjectDef
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047769E, length 677 bytes (virtual, `ret 4`; unaligned entry, def module).
// Pragma: #pragma optimize("s", on) (ebp frame).
// Callees: CPersistContext::Transfer(long&) @ 0x00431102, Transfer(float&) @ 0x00431061 (the manifest's
//   "UpdateShadowScene" labels on both are propagated; the bodies are the typed CheckCRC + switch transfers),
//   Transfer<bool>(name, value, default) @ 0x004045C0 (catalogued Transfer<signed char>).
// Layout: Ego_r.pdb CEngineLocalDetailObjectDef (0x8c) minus 4 (retail CDefClassBase is 0x3c bytes): Mesh +0x3c ..
//   WindSkewSpeedFactor +0x80, the eight flag bits at +0x84. No generated engine header for the class. The def base
//   transfer is not called (an empty base Transfer, as in the landed CEngineThemeDef @ 0x0045979B).
// Shape: one transfer per member in declaration order; the flags share ONE bool local homed in the dead
//   context-argument slot ([ebp+0Bh]) with a one-byte default temporary at [ebp-1]; the bit is merged back with
//   xor/and/xor (the top bit with and 7Fh / or). Only ReceiveShadows defaults to true.
class CPersistContext { public:
    void Transfer(long& value);                                              // 0x00431102
    void Transfer(float& value);                                             // 0x00431061
    void Transfer(const char* name, bool& value, const bool& defaultValue);  // 0x004045C0
};

class CDefClassBase { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

class CEngineLocalDetailObjectDef : public CDefClassBase { public:
    long  Mesh;                          // +0x3c
    long  ShadowMesh;                    // +0x40
    float Probability;                   // +0x44
    float ThemeBlendThreshold;           // +0x48
    float Scale;                         // +0x4c
    float ScaleRandomElement;            // +0x50
    float FadeStart;                     // +0x54
    float FadeEnd;                       // +0x58
    long  ZSpriteMesh;                   // +0x5c
    float ZSpriteFadeStart;              // +0x60
    float ZSpriteFadeEnd;                // +0x64
    float SlopeFadeStart;                // +0x68
    float SlopeFadeEnd;                  // +0x6c
    long  AlphaRef;                      // +0x70
    float AlphaMipBias;                  // +0x74
    float WindSkewConstantFactor;        // +0x78
    float WindSkewRandomFactor;          // +0x7c
    float WindSkewSpeedFactor;           // +0x80
    bool  CastShadows : 1;               // +0x84 bit 0
    bool  ReceiveShadows : 1;            // +0x84 bit 1
    bool  IsRepeatedMesh : 1;            // +0x84 bit 2
    bool  IsZSprite : 1;                 // +0x84 bit 3
    bool  AlphaIsBoolean : 1;            // +0x84 bit 4
    bool  HasLandscapeNormalLighting : 1;// +0x84 bit 5
    bool  TiltToSlope : 1;               // +0x84 bit 6
    bool  HasWindSkew : 1;               // +0x84 bit 7
    virtual void Transfer(CPersistContext& context);
};

void CEngineLocalDetailObjectDef::Transfer(CPersistContext& context)
{
    context.Transfer(Mesh);
    context.Transfer(ShadowMesh);
    context.Transfer(Probability);
    context.Transfer(ThemeBlendThreshold);
    context.Transfer(Scale);
    context.Transfer(ScaleRandomElement);
    context.Transfer(FadeStart);
    context.Transfer(FadeEnd);
    context.Transfer(ZSpriteMesh);
    context.Transfer(ZSpriteFadeStart);
    context.Transfer(ZSpriteFadeEnd);
    context.Transfer(SlopeFadeStart);
    context.Transfer(SlopeFadeEnd);
    context.Transfer(AlphaRef);
    context.Transfer(AlphaMipBias);
    context.Transfer(WindSkewConstantFactor);
    context.Transfer(WindSkewRandomFactor);
    context.Transfer(WindSkewSpeedFactor);
    bool flag = CastShadows;
    context.Transfer("CastShadows", flag, false);
    CastShadows = flag;
    flag = ReceiveShadows;
    context.Transfer("ReceiveShadows", flag, true);
    ReceiveShadows = flag;
    flag = IsRepeatedMesh;
    context.Transfer("IsRepeatedMesh", flag, false);
    IsRepeatedMesh = flag;
    flag = IsZSprite;
    context.Transfer("IsZSprite", flag, false);
    IsZSprite = flag;
    flag = AlphaIsBoolean;
    context.Transfer("AlphaIsBoolean", flag, false);
    AlphaIsBoolean = flag;
    flag = HasLandscapeNormalLighting;
    context.Transfer("HasLandscapeNormalLighting", flag, false);
    HasLandscapeNormalLighting = flag;
    flag = TiltToSlope;
    context.Transfer("TiltToSlope", flag, false);
    TiltToSlope = flag;
    flag = HasWindSkew;
    context.Transfer("HasWindSkew", flag, false);
    HasWindSkew = flag;
}