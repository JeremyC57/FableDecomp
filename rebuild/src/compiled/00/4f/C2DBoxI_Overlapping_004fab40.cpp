// C2DBoxI::Overlapping(const C2DBoxI&) const -- separating-axis overlap test of two integer boxes
// Grade: MATCH
// Leaf: Overlapping
// Shape: the y axis distance is a named local after the x test (retail recomputes both y half-heights for the compare -- the
//   statement boundary stops the CSE) inside a nested if with one shared `return 0`. Return type: retail materialises the
//   result in the full eax (`mov eax,1` / `xor eax,eax`), which a bool body does not give (`mov al,1`, 2 lines, tried as a
//   plain, two-return and inline-COMDAT body); both PDBs spell ?Overlapping@C2DBoxI@@QBE_NABV1@@Z (REVIEW at landing; the
//   landed sibling C2DBoxI::ContainsPoint 0x004FAB10 is int for the same reason). Members: Ego_r.pdb TLX/TLY/BRX/BRY (long).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FAB40, 157 bytes, no callees. `ret 4` = one reference.
// Layout as the landed C2DBoxI::ContainsPoint (0x004FAB10): min X/Y +0/+4, max X/Y +8/+0xC.
// Per axis: |centre - other.centre| > halfExtent + other.halfExtent -> no overlap. The halves are
// signed `/ 2` (`cdq ; sub eax,edx ; sar 1`), the absolute value the `cdq ; xor ; sub` intrinsic.
#include <stdlib.h>

class C2DBoxI {
public:
    long TLX;   // +0x0
    long TLY;   // +0x4
    long BRX;   // +0x8
    long BRY;   // +0xc
    long GetHalfWidth() const { return (BRX - TLX) / 2; }
    long GetHalfHeight() const { return (BRY - TLY) / 2; }
    long GetCentreX() const { return TLX + GetHalfWidth(); }
    long GetCentreY() const { return TLY + GetHalfHeight(); }
    int Overlapping(const C2DBoxI& other) const;
};

int C2DBoxI::Overlapping(const C2DBoxI& other) const
{
    if (abs(GetCentreX() - other.GetCentreX()) <= GetHalfWidth() + other.GetHalfWidth()) {
        long dy = abs(GetCentreY() - other.GetCentreY());
        if (dy <= GetHalfHeight() + other.GetHalfHeight())
            return 1;
    }
    return 0;
}