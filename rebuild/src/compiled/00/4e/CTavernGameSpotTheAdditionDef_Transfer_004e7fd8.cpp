// CTavernGameSpotTheAdditionDef::Transfer(CPersistContext&) -- persist every Spot-the-Addition tunable in declaration order
// Leaf: ?Transfer@CTavernGameSpotTheAdditionDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7FD8, 267 bytes, virtual, `ret 4`, size-optimised def TU.
// Callees (out-of-line CPersistContext typed transfers, GOTCHAS 2026-10-19 identities): Transfer(long&) 0x00431102,
//   Transfer(float&) 0x00431061, Transfer(vector<long>&) 0x00466A47, Transfer(CRGBColour&) 0x004595A3.
// Shape: no base transfer; each member address is formed with `lea eax,[esi+N]` and pushed, the context kept in edi as
//   `this` of every call; the last address reuses esi (`add esi,8Ch`).
// Layout (PDB CTavernGameSpotTheAdditionDef 0x98, CDefClassBase head): Pointer +0x28, Cover +0x2C, CoverWidth +0x30,
//   CoverLength +0x34, Knicknacks +0x38 and ItemsPerRound +0x44 (12-byte STLport vectors, PDB 16), so JoystickScale ..
//   HighlightColourError sit 8 below the PDB at +0x50..+0x8C. No engine header for this class.
#pragma optimize("s", on)

class CRGBColour { public: unsigned long m_argb; };
struct CLongVector {                             // std::vector<long> (retail 12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
class CPersistContext { public:
    template <class T> void Transfer(T& value);                  // 0x00431102
    void Transfer(CLongVector& value);           // 0x00466A47
};
class CTavernGameSpotTheAdditionDef { public:
    unsigned char _head[0x24];                   // CDefClassBase body after the vptr (links, index, flag bits)
    long        Pointer;                         // +0x28
    long        Cover;                           // +0x2c
    float       CoverWidth;                      // +0x30
    float       CoverLength;                     // +0x34
    CLongVector Knicknacks;                      // +0x38
    CLongVector ItemsPerRound;                   // +0x44
    float       JoystickScale;                   // +0x50
    float       TimeToThink;                     // +0x54
    float       MaxCursorHeight;                 // +0x58
    float       KnickKnackRadius;                // +0x5c
    float       ImpulseScale;                    // +0x60
    long        PointerModel;                    // +0x64
    float       PointerHeightOffset;             // +0x68
    float       TimeToMoveCover;                 // +0x6c
    float       TimeToPauseCover;                // +0x70
    float       MaxCoverHeight;                  // +0x74
    float       MinimumKnickKnackGap;            // +0x78
    float       LengthMovementScale;             // +0x7c
    float       HeightMovementScale;             // +0x80
    float       HighlightWidth;                  // +0x84
    CRGBColour  HighlightColour;                 // +0x88
    CRGBColour  HighlightColourError;            // +0x8c
    virtual void Transfer(CPersistContext& context);
};

void CTavernGameSpotTheAdditionDef::Transfer(CPersistContext& context)
{
    context.Transfer(Pointer);
    context.Transfer(Cover);
    context.Transfer(CoverWidth);
    context.Transfer(CoverLength);
    context.Transfer(Knicknacks);
    context.Transfer(ItemsPerRound);
    context.Transfer(JoystickScale);
    context.Transfer(TimeToThink);
    context.Transfer(MaxCursorHeight);
    context.Transfer(KnickKnackRadius);
    context.Transfer(ImpulseScale);
    context.Transfer(PointerModel);
    context.Transfer(PointerHeightOffset);
    context.Transfer(TimeToMoveCover);
    context.Transfer(TimeToPauseCover);
    context.Transfer(MaxCoverHeight);
    context.Transfer(MinimumKnickKnackGap);
    context.Transfer(LengthMovementScale);
    context.Transfer(HeightMovementScale);
    context.Transfer(HighlightWidth);
    context.Transfer(HighlightColour);
    context.Transfer(HighlightColourError);
}