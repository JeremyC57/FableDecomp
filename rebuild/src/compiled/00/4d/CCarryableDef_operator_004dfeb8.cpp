// CCarryableDef::operator=(const CCarryableDef&) -- def copy: CDefClassBase part, carry slots, attach offset vector, scalars
// Leaf: ??4CCarryableDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DFEB8, 76 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment).
// Identification: PDB CCarryableDef (0x50) = CDefClassBase + ActiveCarrySlot +0x28, SecondaryActiveCarrySlot +0x2c,
//   PassiveCarrySlot +0x30, OnKillFX +0x34, OffsetCoordRelativeToAttachToDummy (C3DVector) +0x38,
//   OffsetAmountForThingsOnTopOfMe +0x44, PassiveCarrySlotScale +0x48, IncludeInThingScansWhenCarried (bool) +0x4c.
// Shape: memberwise; the 12-byte POD C3DVector is a three-`movsd` block copy under /Os (esi/edi, so `this`/`other`
//   live in ebx/ebp), the floats move through eax.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CCarryableDef : CDefClassBase {
    long ActiveCarrySlot;               // +0x28
    long SecondaryActiveCarrySlot;      // +0x2c
    long PassiveCarrySlot;              // +0x30
    long OnKillFX;                      // +0x34
    C3DVector OffsetCoordRelativeToAttachToDummy;// +0x38
    float OffsetAmountForThingsOnTopOfMe;// +0x44
    float PassiveCarrySlotScale;        // +0x48
    bool IncludeInThingScansWhenCarried;// +0x4c
    CCarryableDef& operator=(const CCarryableDef& other);
};

CCarryableDef& CCarryableDef::operator=(const CCarryableDef& other)
{
    CDefClassBase::operator=(other);
    ActiveCarrySlot = other.ActiveCarrySlot;
    SecondaryActiveCarrySlot = other.SecondaryActiveCarrySlot;
    PassiveCarrySlot = other.PassiveCarrySlot;
    OnKillFX = other.OnKillFX;
    OffsetCoordRelativeToAttachToDummy = other.OffsetCoordRelativeToAttachToDummy;
    OffsetAmountForThingsOnTopOfMe = other.OffsetAmountForThingsOnTopOfMe;
    PassiveCarrySlotScale = other.PassiveCarrySlotScale;
    IncludeInThingScansWhenCarried = other.IncludeInThingScansWhenCarried;
    return *this;
}