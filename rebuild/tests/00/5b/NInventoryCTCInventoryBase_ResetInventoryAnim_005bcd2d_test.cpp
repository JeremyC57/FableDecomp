// Behaviour test for 005bcd2d: src/005bcd2d.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. NInventory::CTCInventoryBase::ResetInventoryAnim(long defIndex)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NInventory::CTCInventoryBase::ResetInventoryAnim(long defIndex) -- restarts the inventory item animation for a new item def: both right-handed sets restart from the item's starting orientation and are turned back by their current/last X, Y and Z angles
// Leaf: ?ResetInventoryAnim@CTCInventoryBase@NInventory@@QAEXJ@Z
// Retail 0x005BCD2D, 258 bytes, `ret 4`; size TU with an esp frame (`#pragma optimize("s", on)`: `push 6 ; pop ecx ;
// rep movsd` copies).
// Callees: GetStartingRightHandedSet(long) 0x005D9730 (__fastcall: hidden CRightHandedSet result in ecx, the def index
// in edx), CRightHandedSet::RotateAboutAxis(const C3DVector&, float) 0x00A22F80 (manifest "CMatrix3x3::
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
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
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
            InventoryAnimLastRHSet.RotateAboutAxis(g_InventoryAnimXAxis, -InventoryAnimLastXRotationAngle);
            InventoryAnimLastRHSet.RotateAboutAxis(g_InventoryAnimYAxis, -InventoryAnimLastYRotationAngle);
            InventoryAnimLastRHSet.RotateAboutAxis(g_InventoryAnimZAxis, -InventoryAnimLastZRotationAngle);
            InventoryAnimCurrentRHSet.RotateAboutAxis(g_InventoryAnimXAxis, -InventoryAnimCurrentXRotationAngle);
            InventoryAnimCurrentRHSet.RotateAboutAxis(g_InventoryAnimYAxis, -InventoryAnimCurrentYRotationAngle);
            InventoryAnimCurrentRHSet.RotateAboutAxis(g_InventoryAnimZAxis, -InventoryAnimCurrentZRotationAngle);
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

// ---- stubs and driver ----
C3DVector g_InventoryAnimXAxis = { 1.0f, 0.0f, 0.0f };
C3DVector g_InventoryAnimYAxis = { 0.0f, 1.0f, 0.0f };
C3DVector g_InventoryAnimZAxis = { 0.0f, 0.0f, 1.0f };
static int startCalls; static long startIndex;
CRightHandedSet __fastcall GetStartingRightHandedSet(long defIndex)
{
    ++startCalls; startIndex = defIndex;
    CRightHandedSet set;
    set.Up.X = 1.0f; set.Up.Y = 2.0f; set.Up.Z = 3.0f;
    set.Forward.X = 4.0f; set.Forward.Y = 5.0f; set.Forward.Z = (float)defIndex;
    return set;
}
struct SRotateCall { CRightHandedSet* set; const C3DVector* axis; float angle; float upX; };
static SRotateCall rotations[8]; static int rotateCalls;
void CRightHandedSet::RotateAboutAxis(const C3DVector& axis, float angle)
{
    if (rotateCalls < 8) {
        rotations[rotateCalls].set = this; rotations[rotateCalls].axis = &axis;
        rotations[rotateCalls].angle = angle; rotations[rotateCalls].upX = Up.X;
    }
    ++rotateCalls;
    Up.X += 10.0f;
}
int main()
{
    static NInventory::CTCInventoryBase inv;
    memset(&inv, 0, sizeof(inv));
    inv.InventoryAnimCurrentXRotationAngle = 0.1f; inv.InventoryAnimLastXRotationAngle = 0.2f;
    inv.InventoryAnimCurrentYRotationAngle = 0.3f; inv.InventoryAnimLastYRotationAngle = 0.4f;
    inv.InventoryAnimCurrentZRotationAngle = 0.5f; inv.InventoryAnimLastZRotationAngle = 0.6f;
    // new positive index: both sets restart, last rotated X/Y/Z by -last angles, then current by -current angles
    inv.RHSetInvItemDefIndex = 3; inv.InventoryAnimThingDefIndex = 3;
    inv.ResetInventoryAnim(7);
    CHECK(startCalls == 1 && startIndex == 7 && rotateCalls == 6);
    CHECK(inv.InventoryAnimThingDefIndex == 7 && inv.RHSetInvItemDefIndex == 7);
    CHECK(rotations[0].set == &inv.InventoryAnimLastRHSet && rotations[0].axis == &g_InventoryAnimXAxis && rotations[0].angle == -0.2f && rotations[0].upX == 1.0f);
    CHECK(rotations[1].set == &inv.InventoryAnimLastRHSet && rotations[1].axis == &g_InventoryAnimYAxis && rotations[1].angle == -0.4f);
    CHECK(rotations[2].set == &inv.InventoryAnimLastRHSet && rotations[2].axis == &g_InventoryAnimZAxis && rotations[2].angle == -0.6f);
    CHECK(rotations[3].set == &inv.InventoryAnimCurrentRHSet && rotations[3].axis == &g_InventoryAnimXAxis && rotations[3].angle == -0.1f && rotations[3].upX == 1.0f);
    CHECK(rotations[4].set == &inv.InventoryAnimCurrentRHSet && rotations[4].axis == &g_InventoryAnimYAxis && rotations[4].angle == -0.3f);
    CHECK(rotations[5].set == &inv.InventoryAnimCurrentRHSet && rotations[5].axis == &g_InventoryAnimZAxis && rotations[5].angle == -0.5f);
    CHECK(inv.InventoryAnimLastRHSet.Up.X == 31.0f && inv.InventoryAnimCurrentRHSet.Up.X == 31.0f);
    CHECK(inv.InventoryAnimLastRHSet.Forward.Y == 5.0f && inv.InventoryAnimCurrentRHSet.Forward.Z == 7.0f && inv.InventoryAnimCurrentRHSet.Up.Z == 3.0f);
    // same index again: nothing restarts, the index is stored
    startCalls = 0; rotateCalls = 0;
    inv.ResetInventoryAnim(7);
    CHECK(startCalls == 0 && rotateCalls == 0 && inv.RHSetInvItemDefIndex == 7 && inv.InventoryAnimThingDefIndex == 7);
    // non-positive index: both indices cleared, then the index stored
    inv.ResetInventoryAnim(0);
    CHECK(startCalls == 0 && inv.InventoryAnimThingDefIndex == 0 && inv.RHSetInvItemDefIndex == 0);
    inv.InventoryAnimThingDefIndex = 9;
    inv.ResetInventoryAnim(-4);
    CHECK(inv.InventoryAnimThingDefIndex == 0 && inv.RHSetInvItemDefIndex == -4);
    char buf[64];
    _snprintf(buf, sizeof(buf), "%s", fails == 0 ? "005bcd2d_TEST PASS" : "005bcd2d_TEST FAIL");
    printf("%s\n", buf);
    return fails;
}
