// CTCInventory::GlobalUpdate(bool) -- base update, then spins the inventory's 3D item one rotation step and blinks the quick-access highlight
// Grade: DIFFER(214v214) -- scheduling only: retail issues the def release `dec [ecx+4]` after the `fmul [ecx+1DCh]`, ours before it (operand order and a split product/division tried)
// Leaf: GlobalUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00591E8C, 214 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: NInventory::CTCInventoryBase::GlobalUpdate(bool) @ 0x005BDD39, the right-handed set's axis rotation
//          @ 0x00A22F80 (manifest "CMatrix3x3::MultiplyByQuaternionRotation"; name unproven), _ftol2 @ 0x00BFEA70,
//          CTCInventory::UpdateContextSensitiveItems() @ 0x00591B7F; globals g_framesPerSecond @ 0x01375550 and the
//          (0, -1, 0) rotation axis @ 0x0137CC74; the def's slot 1 on release.
// Shape: the previous rotation (a 24-byte POD set, `rep movsd`, and its angle) is saved; the step is
// `1.0f / (def->NumberOfSecondsForObjectRotation * fps)` over a by-value copy of the inventory def pointer (add-ref
// null-tested, release interleaved with the FPU code), kept in the dead argument slot; the set rotates by -step and the
// angle advances by step. When the flash timer runs out it is reloaded from MainQuickAccessItemFlashTime * fps
// (second def copy, `_ftol2`) and the flash state flips through a reference (`lea eax ; cmp [eax],0 ; sete`).
// Layout: retail CTCInventory (base 0x130 + the screen base vptr, 12-byte containers) as in the landed destructor
// 0x00589C55: OldObjectRHSet +0x190, OldRotationAngle +0x1A8, CurrentObjectRHSet +0x1AC, CurrentRotationAngle +0x1C4,
// QuickAccessFlashState +0x1CE, QuickAccessFlashTimer +0x1D0; CTCInventoryBase PDef +0x68; CInventoryDef
// NumberOfSecondsForObjectRotation +0x1DC (PDB +0x1F8), MainQuickAccessItemFlashTime +0xA30 (PDB +0xA5C).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct C3DVector {
    float X, Y, Z;
};

extern long g_framesPerSecond;                       // 0x01375550
extern C3DVector g_inventoryItemRotationAxis;        // 0x0137CC74 (0, -1, 0)

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
};

struct CInventoryDef : CDefBase {
    unsigned char _pad_0x08[0x1D4];
    float         NumberOfSecondsForObjectRotation;  // +0x1DC (PDB +0x1F8)
    unsigned char _pad_0x1e0[0x850];
    float         MainQuickAccessItemFlashTime;      // +0xA30 (PDB +0xA5C)
};

struct CInventoryDefPointer {                        // CDefPointer<const CInventoryDef>
    CInventoryDef* Object;
    CInventoryDefPointer(const CInventoryDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CInventoryDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    CInventoryDef* operator->() const { return Object; }
};

struct CRightHandedSet {
    float m[6];
    void RotateAboutAxis_00a22f80(const C3DVector& axis, float angle);   // 0x00A22F80
};

namespace NInventory {
struct CTCInventoryBase {
    void*                __vftable;                  // +0x00
    unsigned char        _pad_0x04[0x64];
    CInventoryDefPointer PDef;                       // +0x68
    unsigned char        _pad_0x6c[0xC4];
    void GlobalUpdate(bool paused);                  // 0x005BDD39
};
}

struct CTCInventory : NInventory::CTCInventoryBase {
    void*           __vftable_130;                   // +0x130 NHeroInformationScreens::CBase
    unsigned char   _pad_0x134[0x5C];
    CRightHandedSet OldObjectRHSet;                  // +0x190
    float           OldRotationAngle;                // +0x1A8
    CRightHandedSet CurrentObjectRHSet;              // +0x1AC
    float           CurrentRotationAngle;            // +0x1C4
    long            ItemRHSetsApplyTo;               // +0x1C8
    bool            EnableQuickAccessAssign;         // +0x1CC
    bool            EnableQuickAccessRemove;         // +0x1CD
    bool            QuickAccessFlashState;           // +0x1CE
    long            QuickAccessFlashTimer;           // +0x1D0

    void UpdateContextSensitiveItems();              // 0x00591B7F
    void GlobalUpdate(bool paused);
};

void CTCInventory::GlobalUpdate(bool paused)
{
    NInventory::CTCInventoryBase::GlobalUpdate(paused);
    OldObjectRHSet = CurrentObjectRHSet;
    OldRotationAngle = CurrentRotationAngle;
    float step;
    {
        CInventoryDefPointer def = PDef;
        step = 1.0f / (def->NumberOfSecondsForObjectRotation * g_framesPerSecond);
    }
    CurrentObjectRHSet.RotateAboutAxis_00a22f80(g_inventoryItemRotationAxis, -step);
    CurrentRotationAngle += step;
    if (--QuickAccessFlashTimer <= 0) {
        {
            CInventoryDefPointer def = PDef;
            QuickAccessFlashTimer = (long)(def->MainQuickAccessItemFlashTime * g_framesPerSecond);
        }
        bool& flash = QuickAccessFlashState;
        flash = !flash;
    }
    UpdateContextSensitiveItems();
}
