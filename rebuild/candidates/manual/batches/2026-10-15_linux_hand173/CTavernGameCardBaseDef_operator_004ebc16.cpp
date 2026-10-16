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

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CRGBColour {
    unsigned char B, G, R, A;
};
struct C2DVector {
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

struct CTavernGameCardBaseDef : CDefClassBase {
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
