// CTavernGameCardBaseDef::operator=(const CTavernGameCardBaseDef&) -- def copy: CDefClassBase part, then the CTavernGameCardBaseDef fields memberwise in declaration order
// Leaf: ??4CTavernGameCardBaseDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBC16, 147 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004EBC11 in slot 19 of vtable 0x0124258C = CTavernGameCardBaseDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<CCardPositionsDef>::operator= @ 0x004EBCA9, vector<CCardDef>::operator= @ 0x004EBF32.
// Layout (engine/CTavernGameCardBaseDef.h, CardPack 4 below the PDB): ControlType +0x28, CardWidth +0x2c, CardHeight +0x30, ImpulseScaleXbox +0x34, ImpulseScalePC +0x38, PointerModel +0x3c, MoveSpeed +0x40, TurnSpeed +0x44, PointerHeightOffset +0x48, PointerParticleEffect +0x4c, HighlightWidth +0x50, HighlightColourGood +0x54, HighlightColourBad +0x58, CardSeparation +0x5c, TableSeparation +0x60, PackOffset +0x64, CardOrders +0x6c, CardPack +0x78.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// ControlType is the 4-byte ETavernGameControlType enum (long).
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CRGBColour { public:
    unsigned char B, G, R, A;
};
class C2DVector { public:
    float X, Y;
};
struct CCardPositionsVector {                       // retail std::vector<CCardPositionsDef> (retail 0xc bytes)
    unsigned long m_words[3];
    CCardPositionsVector& operator=(const CCardPositionsVector& other);   // 0x004EBCA9
};
struct CCardDefVector {                       // retail std::vector<CCardDef> (retail 0xc bytes)
    unsigned long m_words[3];
    CCardDefVector& operator=(const CCardDefVector& other);   // 0x004EBF32
};

// def vtable 0x0124258C: slot 0 0x004E2DCB (implicit inline dtor); real overrides from the def vtable scan
class CTavernGameCardBaseDef : public CDefClassBase { public:
    long                     ControlType;                 // +0x28
    float                    CardWidth;                   // +0x2c
    float                    CardHeight;                  // +0x30
    float                    ImpulseScaleXbox;            // +0x34
    float                    ImpulseScalePC;              // +0x38
    long                     PointerModel;                // +0x3c
    float                    MoveSpeed;                   // +0x40
    float                    TurnSpeed;                   // +0x44
    float                    PointerHeightOffset;         // +0x48
    long                     PointerParticleEffect;       // +0x4c
    float                    HighlightWidth;              // +0x50
    CRGBColour               HighlightColourGood;         // +0x54
    CRGBColour               HighlightColourBad;          // +0x58
    float                    CardSeparation;              // +0x5c
    float                    TableSeparation;             // +0x60
    C2DVector                PackOffset;                  // +0x64
    CCardPositionsVector     CardOrders;                  // +0x6c
    CCardDefVector           CardPack;                    // +0x78
    CTavernGameCardBaseDef& operator=(const CTavernGameCardBaseDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004F9A90
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004EBC11
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004E11BD
};

CTavernGameCardBaseDef& CTavernGameCardBaseDef::operator=(const CTavernGameCardBaseDef& other)
{
    CDefClassBase::operator=(other);
    ControlType = other.ControlType;
    CardWidth = other.CardWidth;
    CardHeight = other.CardHeight;
    ImpulseScaleXbox = other.ImpulseScaleXbox;
    ImpulseScalePC = other.ImpulseScalePC;
    PointerModel = other.PointerModel;
    MoveSpeed = other.MoveSpeed;
    TurnSpeed = other.TurnSpeed;
    PointerHeightOffset = other.PointerHeightOffset;
    PointerParticleEffect = other.PointerParticleEffect;
    HighlightWidth = other.HighlightWidth;
    HighlightColourGood = other.HighlightColourGood;
    HighlightColourBad = other.HighlightColourBad;
    CardSeparation = other.CardSeparation;
    TableSeparation = other.TableSeparation;
    PackOffset = other.PackOffset;
    CardOrders = other.CardOrders;
    CardPack = other.CardPack;
    return *this;
}