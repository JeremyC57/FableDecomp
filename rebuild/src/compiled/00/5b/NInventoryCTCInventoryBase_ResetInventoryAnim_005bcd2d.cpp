// NInventory::CTCInventoryBase::ResetInventoryAnim(long defIndex) -- restarts the inventory item animation for a new item def: both right-handed sets restart from the item's starting orientation and are turned back by their current/last X, Y and Z angles
// Leaf: ?ResetInventoryAnim@CTCInventoryBase@NInventory@@QAEXJ@Z
// Retail 0x005BCD2D, 258 bytes, `ret 4`; size TU with an esp frame (`#pragma optimize("s", on)`: `push 6 ; pop ecx ;
// rep movsd` copies).
// Callees: GetStartingRightHandedSet(long) 0x005D9730 (__fastcall: hidden CRightHandedSet result in ecx, the def index
// in edx), CRightHandedSet::Rotate(const C3DVector&, float) 0x00A22F80 (manifest "CMatrix3x3::
// MultiplyByQuaternionRotation"; six calls). Globals: the three axis vectors 0x0137CDFC / 0x0137CDF0 / 0x0137CE08
// (used with the X / Y / Z angles; named from that use).
// Shape: a non-positive index clears both indices; a new positive index copies the starting set into the last and the
// current set through a `__forceinline` member taking the call result by const reference (so both implicit 24-byte
// `rep movsd` copies read the returned pointer in eax and no ebp frame is needed), rotates each by its negated angles,
// remembers the def and clears the RH-set index; every path then stores the index into RHSetInvItemDefIndex (the
// clearing store stays as retail's dead `and [m],0`).
// Layout (retail; Ego_r.pdb names, 0x14 below the PDB): InventoryAnimCurrentRHSet +0xCC, InventoryAnimLastRHSet +0xE4,
// InventoryAnimCurrentX/LastX/CurrentY/LastY/CurrentZ/LastZ RotationAngle +0xFC..+0x110,
// InventoryAnimThingDefIndex +0x124, RHSetInvItemDefIndex +0x128.
#pragma optimize("s", on)

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:                           // 24 bytes: Up, Forward
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() {}
    void Rotate(const C3DVector& axis, float angle);   // 0x00A22F80
};
CRightHandedSet __fastcall GetStartingRightHandedSet(long defIndex);   // 0x005D9730
extern C3DVector g_InventoryAnimXAxis;             // 0x0137CDFC
extern C3DVector g_InventoryAnimYAxis;             // 0x0137CDF0
extern C3DVector g_InventoryAnimZAxis;             // 0x0137CE08

namespace NInventory {
class CTCInventoryBase { public:
    unsigned char _pad_0x000[0xcc];
    CRightHandedSet InventoryAnimCurrentRHSet;     // +0x0cc
    CRightHandedSet InventoryAnimLastRHSet;        // +0x0e4
    float InventoryAnimCurrentXRotationAngle;      // +0x0fc
    float InventoryAnimLastXRotationAngle;         // +0x100
    float InventoryAnimCurrentYRotationAngle;      // +0x104
    float InventoryAnimLastYRotationAngle;         // +0x108
    float InventoryAnimCurrentZRotationAngle;      // +0x10c
    float InventoryAnimLastZRotationAngle;         // +0x110
    unsigned char _pad_0x114[0x124 - 0x114];
    long InventoryAnimThingDefIndex;               // +0x124
    long RHSetInvItemDefIndex;                     // +0x128
    __forceinline void ResetRHSets(const CRightHandedSet& start)
    {
        InventoryAnimLastRHSet = start;
        InventoryAnimCurrentRHSet = start;
    }
    void ResetInventoryAnim(long defIndex);
};

void CTCInventoryBase::ResetInventoryAnim(long defIndex)
{
    if (defIndex > 0) {
        if (defIndex != RHSetInvItemDefIndex) {
            ResetRHSets(GetStartingRightHandedSet(defIndex));
            InventoryAnimLastRHSet.Rotate(g_InventoryAnimXAxis, -InventoryAnimLastXRotationAngle);
            InventoryAnimLastRHSet.Rotate(g_InventoryAnimYAxis, -InventoryAnimLastYRotationAngle);
            InventoryAnimLastRHSet.Rotate(g_InventoryAnimZAxis, -InventoryAnimLastZRotationAngle);
            InventoryAnimCurrentRHSet.Rotate(g_InventoryAnimXAxis, -InventoryAnimCurrentXRotationAngle);
            InventoryAnimCurrentRHSet.Rotate(g_InventoryAnimYAxis, -InventoryAnimCurrentYRotationAngle);
            InventoryAnimCurrentRHSet.Rotate(g_InventoryAnimZAxis, -InventoryAnimCurrentZRotationAngle);
            InventoryAnimThingDefIndex = defIndex;
            RHSetInvItemDefIndex = 0;
        }
    } else {
        InventoryAnimThingDefIndex = 0;
        RHSetInvItemDefIndex = 0;
    }
    RHSetInvItemDefIndex = defIndex;
}
}