// CTCInventoryBase::UpdateInventoryAnimZAxis() -- eases InventoryAnimZRotationAngleGain toward the right stick's Y while the stick is deflected past 0.01, else zeroes it
// Leaf: UpdateInventoryAnimZAxis
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCEF8, length 84 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// The catalog label "UpdateInventoryAnimXAxis" is propagated from the byte-twin 0x005BCEA4; this body writes +0x11C
// (InventoryAnimZRotationAngleGain) from the stick's Y, so it is the Z-axis sibling (PDB: EnableAnimXAxis / EnableAnimZAxis).
// Callees: CTCInventoryBase::GetRightStickPosition(C2DVector*) @ 0x005BC5C6.
// Constants: 0.01f @ 0x01230A5C, 0.1f @ 0x0122EE58; the else arm's 0.0f is a literal `fldz`.
// Retail CTCInventoryBase: no engine header; PDB layout minus 4 bytes per preceding std::vector (see 0x005BCEA4).
// Shape: x*x is multiplied first here, i.e. the source sum is `y*y + x*x` (VC7.1 evaluates the right operand first).
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
    void UpdateInventoryAnimZAxis();
};
void CTCInventoryBase_Retail::UpdateInventoryAnimZAxis()
{
    C2DVector stick;
    GetRightStickPosition(&stick);
    if (sqrtf(stick.y * stick.y + stick.x * stick.x) > 0.01f)
        InventoryAnimZRotationAngleGain += (stick.y - InventoryAnimZRotationAngleGain) * 0.1f;
    else
        InventoryAnimZRotationAngleGain = 0.0f;
}