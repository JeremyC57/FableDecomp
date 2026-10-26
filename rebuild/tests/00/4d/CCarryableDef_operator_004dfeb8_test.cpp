#include <cstdio>
// CCarryableDef::operator=(const CCarryableDef&) -- def copy: CDefClassBase part, carry slots, attach offset vector, scalars
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

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CCarryableDef : public CDefClassBase { public:
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }

int main()
{
    static CCarryableDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.ActiveCarrySlot = 1;
    b.SecondaryActiveCarrySlot = 2;
    b.PassiveCarrySlot = 3;
    b.OnKillFX = 4;
    b.OffsetCoordRelativeToAttachToDummy.X = 1.0f;
    b.OffsetCoordRelativeToAttachToDummy.Y = 2.0f;
    b.OffsetCoordRelativeToAttachToDummy.Z = 3.0f;
    b.OffsetAmountForThingsOnTopOfMe = 0.5f;
    b.PassiveCarrySlotScale = 0.75f;
    b.IncludeInThingScansWhenCarried = true;
    CCarryableDef* r = &(a = b);
    if (r != &a) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004dfeb8"); return 1; }
    if (!(a.ActiveCarrySlot == b.ActiveCarrySlot)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.SecondaryActiveCarrySlot == b.SecondaryActiveCarrySlot)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.PassiveCarrySlot == b.PassiveCarrySlot)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.OnKillFX == b.OnKillFX)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.OffsetCoordRelativeToAttachToDummy.X == b.OffsetCoordRelativeToAttachToDummy.X)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.OffsetCoordRelativeToAttachToDummy.Y == b.OffsetCoordRelativeToAttachToDummy.Y)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.OffsetCoordRelativeToAttachToDummy.Z == b.OffsetCoordRelativeToAttachToDummy.Z)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.OffsetAmountForThingsOnTopOfMe == b.OffsetAmountForThingsOnTopOfMe)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.PassiveCarrySlotScale == b.PassiveCarrySlotScale)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    if (!(a.IncludeInThingScansWhenCarried == b.IncludeInThingScansWhenCarried)) { std::printf("004dfeb8_TEST FAIL\n"); return 1; }
    std::printf("004dfeb8_TEST PASS\n");
    return 0;
}