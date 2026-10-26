// CCarryableDef::Transfer(CPersistContext& context) -- persist the carryable def: carry slots, scan flag, kill FX, attach offset, stacking offset and passive slot scale
// Leaf: ?Transfer@CCarryableDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD924, 101 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers, catalog labels propagated): long 0x00431102, bool 0x0043314A,
//   C3DVector 0x00456903 (copies the 12-byte value with three movsd), float 0x00431061.
// Layout: engine/CCarryableDef.h (Ego_r.pdb, 0x50) agrees with every transferred offset (+0x28..+0x4c), but it models
//   the vptr as a data word and flattens the C3DVector into three floats, so a class declaring the virtual Transfer
//   cannot be built on it: the same offsets and member names are modelled locally (vptr + 0x24 base bytes).
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member, in the def's TRANSFER list order (IncludeInThingScansWhenCarried, +0x4c, comes
//   straight after the three carry slots; the last member's address is the natural `add esi,48h ; push esi`).
#pragma optimize("s", on)

class C3DVector { public: float X, Y, Z; };

class CPersistContext {
public:
    template <class T> void Transfer(T& value);                 // 0x00431102
    void Transfer(C3DVector& value);            // 0x00456903
};

class CCarryableDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                  // CDefClassBase data (+0x04..+0x27)
    long ActiveCarrySlot;                       // +0x28
    long SecondaryActiveCarrySlot;              // +0x2c
    long PassiveCarrySlot;                      // +0x30
    long OnKillFX;                              // +0x34
    C3DVector OffsetCoordRelativeToAttachToDummy;   // +0x38
    float OffsetAmountForThingsOnTopOfMe;       // +0x44
    float PassiveCarrySlotScale;                // +0x48
    bool IncludeInThingScansWhenCarried;        // +0x4c
};

void CCarryableDef::Transfer(CPersistContext& context)
{
    context.Transfer(ActiveCarrySlot);
    context.Transfer(SecondaryActiveCarrySlot);
    context.Transfer(PassiveCarrySlot);
    context.Transfer(IncludeInThingScansWhenCarried);
    context.Transfer(OnKillFX);
    context.Transfer(OffsetCoordRelativeToAttachToDummy);
    context.Transfer(OffsetAmountForThingsOnTopOfMe);
    context.Transfer(PassiveCarrySlotScale);
}