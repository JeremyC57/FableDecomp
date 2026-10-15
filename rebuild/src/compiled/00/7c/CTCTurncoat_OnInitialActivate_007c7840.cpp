// CTCTurncoat::OnInitialActivate() -- derives the creature's turncoat resistance from its health (PercentageOfHealthAsResistance * health + ExtraResistance), clamped from below by MinimumResistance
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C7840, length 38 bytes. CTCTurncoat vtable 0x0126E6F4 slot 6, the CTCBase
// OnInitialActivate slot (PDB `?OnInitialActivate@CTCTurncoat@@UAEXXZ`); the catalog's "vfunc_6" is the slot name.
// Shape: `fld [thing+0B0h] ; fmul [def+4Ch]` is `def * health` (right operand loaded first), `fadd` the extra
// resistance, `fst` stores the member and keeps the value for the compare; `fcomp ; test ah,5 ; jp` is the ordered
// `<` (as the landed SetTurncoatTimeLast 0x007C7810) and the clamp copies the minimum with an integer `mov`.
// Layout: engine/CTCTurncoat.h is the PDB layout (CreatureTurncoatResistance +0x4C); retail has it at +0x48 (the
// TurncoatStageEffects vector is 4 bytes smaller), so a local model as the landed 0x007C7810. CTurncoatDef (PDB):
// MinimumResistance +0x34; PercentageOfHealthAsResistance / ExtraResistance at retail +0x4C / +0x50 (after the
// 12-byte vector). The owner's float at +0xB0 is its health (descriptive name).
struct CTurncoatDef {
    unsigned char _pad[0x34];
    float         MinimumResistance;                // +0x34
    unsigned char _pad38[0x14];
    float         PercentageOfHealthAsResistance;   // +0x4C
    float         ExtraResistance;                  // +0x50
};

struct CThingCreatureBase {
    unsigned char _pad[0xB0];
    float         Health;                           // +0xB0
};

struct CTCTurncoat {
    void*               __vftable;                  // +0x00
    CThingCreatureBase* Thing;                      // +0x04
    unsigned char       _pad_0x08[0xC];
    const CTurncoatDef* PDef_Object;                // +0x14 CDefPointer<const CTurncoatDef>::Object
    unsigned char       _pad_0x18[0x30];
    float               CreatureTurncoatResistance; // +0x48
    void OnInitialActivate();
};

void CTCTurncoat::OnInitialActivate()
{
    CreatureTurncoatResistance = PDef_Object->PercentageOfHealthAsResistance * Thing->Health + PDef_Object->ExtraResistance;
    if (CreatureTurncoatResistance < PDef_Object->MinimumResistance)
        CreatureTurncoatResistance = PDef_Object->MinimumResistance;
}