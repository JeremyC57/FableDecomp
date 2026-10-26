// CTCInventoryBase::UpdateInventoryAnimXAxis() -- eases InventoryAnimXRotationAngleGain toward the right stick's X while the stick is deflected past 0.01, else zeroes it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCEA4, length 84 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::GetRightStickPosition(C2DVector*) @ 0x005BC5C6 (out-pointer to the 8-byte frame local).
// Constants: 0.01f @ 0x01230A5C (dead-zone), 0.1f @ 0x0122EE58 (easing factor); the else arm's 0.0f is a literal `fldz`.
// Retail CTCInventoryBase has no generated engine header; the PDB layout (Ego_r.pdb) shifted down by 4 bytes per
// std::vector before the member (5 vectors: Components, Categories, MenuDisplayListItem, MenuDisplayListCategory,
// ConfiscatedItems) puts InventoryAnimXRotationAngleGain at +0x114 and InventoryAnimZRotationAngleGain at +0x11C.
// Shape: the stick length is computed for the compare only (`fld st(0) ; fcomp ; fstp st(0)`); y*y is multiplied
// first, which VC7.1 does for the source `x*x + y*y` (the right operand of `+` is evaluated first).
// `gain += (x - gain) * 0.1f` forms `lea eax,[esi+114h]` because the member is addressed three times.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <math.h>
class C2DVector { public: float x; float y; };
struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0x114];
    float InventoryAnimXRotationAngleGain;      // +0x114
    float InventoryAnimYRotationAngleGain;      // +0x118
    float InventoryAnimZRotationAngleGain;      // +0x11C
    void GetRightStickPosition(C2DVector* out); // 0x005BC5C6
    void UpdateInventoryAnimXAxis();
};
void CTCInventoryBase_Retail::UpdateInventoryAnimXAxis()
{
    C2DVector stick;
    GetRightStickPosition(&stick);
    if (sqrtf(stick.x * stick.x + stick.y * stick.y) > 0.01f)
        InventoryAnimXRotationAngleGain += (stick.x - InventoryAnimXRotationAngleGain) * 0.1f;
    else
        InventoryAnimXRotationAngleGain = 0.0f;
}