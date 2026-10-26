// CQuickAccessItemButtonGuiDef::Transfer(CPersistContext& context) -- persist the CQuickAccessItemButtonGuiDef's fields (3 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046EE6A, 46 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CQuickAccessItemButtonGuiDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012360A4, whose RTTI type descriptor is .?AVCQuickAccessItemButtonGuiDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z" agrees; family H057 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(C2DVector&) 0x004568BC.
// Layout: Ego_r.pdb CQuickAccessItemButtonGuiDef members rebased to retail (def base 4 bytes shorter); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class C2DVector {                                                  // Ego_r.pdb, 8 bytes (opaque here)
public:
    unsigned char _body[8];
};
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(C2DVector& value);                             // 0x004568BC
};
class CQuickAccessItemButtonGuiDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012360A4 slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    long ButtonGraphic;                                // +0x3C (Ego_r +0x40)
    C2DVector Position;                                // +0x40 (Ego_r +0x44)
    C2DVector Offset;                                  // +0x48 (Ego_r +0x4C)
};

void CQuickAccessItemButtonGuiDef::Transfer(CPersistContext& context)
{
    context.Transfer(ButtonGraphic);
    context.Transfer(Position);
    context.Transfer(Offset);
}