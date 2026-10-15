// C2DBoxI::Overlapping(const C2DBoxI&) const -- separating-axis overlap test of two integer boxes
// Grade: DIFFER(127v157) -- x axis and branch layout exact; retail recomputes both y half-heights for the compare (o.minY reloaded) and evaluates this-half before other-half on y, the lane CSEs them
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FAB40, 157 bytes, no callees. `ret 4` = one reference.
// Layout as the landed C2DBoxI::ContainsPoint (0x004FAB10): min X/Y +0/+4, max X/Y +8/+0xC.
// Per axis: |centre - other.centre| > halfExtent + other.halfExtent -> no overlap. The halves are
// signed `/ 2` (`cdq ; sub eax,edx ; sar 1`), the absolute value the `cdq ; xor ; sub` intrinsic.
#include <stdlib.h>

struct C2DBoxI {
    int minX;   // +0x0
    int minY;   // +0x4
    int maxX;   // +0x8
    int maxY;   // +0xc
    int GetHalfWidth() const { return (maxX - minX) / 2; }
    int GetHalfHeight() const { return (maxY - minY) / 2; }
    int GetCentreX() const { return minX + GetHalfWidth(); }
    int GetCentreY() const { return minY + GetHalfHeight(); }
    int Overlapping(const C2DBoxI& other) const;
};

int C2DBoxI::Overlapping(const C2DBoxI& other) const
{
    if (abs(GetCentreX() - other.GetCentreX()) <= GetHalfWidth() + other.GetHalfWidth() &&
        abs(GetCentreY() - other.GetCentreY()) <= GetHalfHeight() + other.GetHalfHeight())
        return 1;
    return 0;
}
