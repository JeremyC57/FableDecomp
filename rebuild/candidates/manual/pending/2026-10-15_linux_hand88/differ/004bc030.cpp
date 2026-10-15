// NGraphicAppearance::CTCGraphicAppearance::Copy(const CTCGraphicAppearance& other) -- copy the persistence flags, main graphic, self-illumination / force-resend bits, scale and alpha from another appearance
// Grade: DIFFER(94v91) -- first bit-field copy: retail loads the destination flag byte ONCE (`mov al,[src+59h] ; mov dl,[ecx+59h] ;
//        xor al,dl ; and al,1 ; xor dl,al`), the lane reloads it for the final xor (+3 bytes). Tried: plain bit-field copy, `bool : 1`
//        bit-fields, a getter returning the bit, an explicit `(flags & ~1) | (other & 1)` byte merge (canonicalised to the same reload).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC030, length 91 bytes. No callees.
// Layout (Ego_r.pdb CTCGraphicAppearance, 116 bytes; retail 0x5c: the two maps 12 bytes, the two lists one pointer each):
// CTCBase +0x00..+0x0b, MeshEffects +0x0c, TintColours +0x18, PTCAnimation +0x24, CachedStaticFocalPosOffset +0x28,
// Scale +0x34, Primitives +0x38, Components +0x3c, LightingChannel +0x40, MainGraphic (CEngineGraphic, 14 bytes) +0x44,
// ActivePersistenceFlags +0x52, FrameUpdatedWasRenderedFlag +0x54, UpdatePrimitiveReasons +0x58, the flag bits +0x59
// (SelfIlluminating bit 0, ForceResendEveryRenderFrame bit 1, ResendPrimsEveryRenderFrame 2, DisableOcclusionTesting 3,
// DrawAfterWater 4, CachedStaticFocalPos 5, RenderedLastFrame 6, PrimitivesDrawn 7), Alpha +0x5a, ByPassColorFilter +0x5b.
// Shape: member-wise assignments in this order; CEngineGraphic is a 14-byte record (packed to 2: PDB size 14) copied whole
// through two `lea` bases (dword, dword, dword, word); the two one-bit bit-field copies are the `xor ; and ; xor` merges
// (the second reuses the first's stored byte); the float Scale is copied through an integer register.
// No engine header exists for CTCGraphicAppearance: local model.
struct CThing;

#pragma pack(push, 2)
struct CEngineGraphic {                                 // 14 bytes
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};
#pragma pack(pop)

struct C3DVector { float X, Y, Z; };

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    void*          __vftable;                           // +0x00 (CTCBase)
    CThing*        ParentThing;                         // +0x04
    bool           ToBeRemoved;                         // +0x08
    bool           IsInGlobalUpdateTCs;                 // +0x09
    unsigned char  MeshEffects[0x0c];                   // +0x0c map<void*, CMeshEffectPriorityEntry>
    unsigned char  TintColours[0x0c];                   // +0x18 map<void*, CRGBColour>
    void*          PTCAnimation;                        // +0x24
    C3DVector      CachedStaticFocalPosOffset;          // +0x28
    float          Scale;                               // +0x34
    void*          Primitives;                          // +0x38 list<CPrimitive> (head node)
    void*          Components;                          // +0x3c list<CAGraphicAppearanceComponent*> (head node)
    unsigned long  LightingChannel;                     // +0x40 CEnginePrimitiveThemeChannelDesc
    CEngineGraphic MainGraphic;                         // +0x44
    unsigned short ActivePersistenceFlags;              // +0x52
    long           FrameUpdatedWasRenderedFlag;         // +0x54
    unsigned char  UpdatePrimitiveReasons;              // +0x58
    unsigned char  SelfIlluminating : 1;                // +0x59 bit 0
    unsigned char  ForceResendEveryRenderFrame : 1;     //        bit 1
    unsigned char  ResendPrimsEveryRenderFrame : 1;     //        bit 2
    unsigned char  DisableOcclusionTesting : 1;         //        bit 3
    unsigned char  DrawAfterWater : 1;                  //        bit 4
    unsigned char  CachedStaticFocalPos : 1;            //        bit 5
    unsigned char  RenderedLastFrame : 1;               //        bit 6
    unsigned char  PrimitivesDrawn : 1;                 //        bit 7
    unsigned char  Alpha;                               // +0x5a
    signed char    ByPassColorFilter;                   // +0x5b
    void Copy(const CTCGraphicAppearance& other);
};

void CTCGraphicAppearance::Copy(const CTCGraphicAppearance& other)
{
    ActivePersistenceFlags = other.ActivePersistenceFlags;
    MainGraphic = other.MainGraphic;
    SelfIlluminating = other.SelfIlluminating;
    ForceResendEveryRenderFrame = other.ForceResendEveryRenderFrame;
    Scale = other.Scale;
    Alpha = other.Alpha;
}

}
