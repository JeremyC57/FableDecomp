// CTavernGameSpotTheAdditionDef::operator=(const CTavernGameSpotTheAdditionDef&) -- memberwise def copy: CDefClassBase::operator=, the pointer/cover scalars, the two vector<long> by their operator=, the tuning floats and the two highlight colours; returns *this
// Leaf: ??4CTavernGameSpotTheAdditionDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2E0D, length 189 bytes.
// Identity: the manifest label "CThingBuildingDef::operator=_extended" is propagated. The body sits behind
//   the 5-byte `jmp` thunk at 0x004E2E08 = slot 19 of vtable 0x01241A4C = CTavernGameSpotTheAdditionDef
//   (rtti-vtables.tsv); the touched offsets are the PDB fields with 12-byte retail vectors (everything
//   after Knicknacks/ItemsPerRound sits 8 bytes below Ego_r).
// Callees: CDefClassBase::operator= @ 0x00431F10, std::vector<long>::operator= @ 0x00454886.
// Shape: compiler-generated memberwise assignment in declaration order; each scalar through eax alone
//   (size-optimised def module), the vectors through `lea`+push; CRGBColour is a 4-byte POD (one dword).
// No engine header exists for the class: modelled locally from struct_layouts_egor.tsv.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CLongVector {                                                      // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);                     // 0x00454886
};

class CRGBColour { public:                                                       // B, G, R, A bytes
    unsigned char B, G, R, A;
};

// def vtable 0x01241A4C: slot 0 0x004E11DC (implicit inline dtor); real overrides from the def vtable scan
class CTavernGameSpotTheAdditionDef : public CDefClassBase { public:                    // vtable 0x01241A4C
    long        Pointer;                                                  // +0x28
    long        Cover;                                                    // +0x2c
    float       CoverWidth;                                               // +0x30
    float       CoverLength;                                              // +0x34
    CLongVector Knicknacks;                                               // +0x38
    CLongVector ItemsPerRound;                                            // +0x44
    float       JoystickScale;                                            // +0x50
    float       TimeToThink;                                              // +0x54
    float       MaxCursorHeight;                                          // +0x58
    float       KnickKnackRadius;                                         // +0x5c
    float       ImpulseScale;                                             // +0x60
    long        PointerModel;                                             // +0x64
    float       PointerHeightOffset;                                      // +0x68
    float       TimeToMoveCover;                                          // +0x6c
    float       TimeToPauseCover;                                         // +0x70
    float       MaxCoverHeight;                                           // +0x74
    float       MinimumKnickKnackGap;                                     // +0x78
    float       LengthMovementScale;                                      // +0x7c
    float       HeightMovementScale;                                      // +0x80
    float       HighlightWidth;                                           // +0x84
    CRGBColour  HighlightColour;                                          // +0x88
    CRGBColour  HighlightColourError;                                     // +0x8c
    virtual void v00();
    CTavernGameSpotTheAdditionDef& operator=(const CTavernGameSpotTheAdditionDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E7FD8
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E2E08
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DED48
};

CTavernGameSpotTheAdditionDef& CTavernGameSpotTheAdditionDef::operator=(const CTavernGameSpotTheAdditionDef& other)
{
    CDefClassBase::operator=(other);
    Pointer = other.Pointer;
    Cover = other.Cover;
    CoverWidth = other.CoverWidth;
    CoverLength = other.CoverLength;
    Knicknacks = other.Knicknacks;
    ItemsPerRound = other.ItemsPerRound;
    JoystickScale = other.JoystickScale;
    TimeToThink = other.TimeToThink;
    MaxCursorHeight = other.MaxCursorHeight;
    KnickKnackRadius = other.KnickKnackRadius;
    ImpulseScale = other.ImpulseScale;
    PointerModel = other.PointerModel;
    PointerHeightOffset = other.PointerHeightOffset;
    TimeToMoveCover = other.TimeToMoveCover;
    TimeToPauseCover = other.TimeToPauseCover;
    MaxCoverHeight = other.MaxCoverHeight;
    MinimumKnickKnackGap = other.MinimumKnickKnackGap;
    LengthMovementScale = other.LengthMovementScale;
    HeightMovementScale = other.HeightMovementScale;
    HighlightWidth = other.HighlightWidth;
    HighlightColour = other.HighlightColour;
    HighlightColourError = other.HighlightColourError;
    return *this;
}