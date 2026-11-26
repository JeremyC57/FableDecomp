// C2DLineF::GetPointOnInfiniteLine(const C2DVector&) const -- project a point onto the infinite line.
// Leaf: ?GetPointOnInfiniteLine@C2DLineF@@QBE?AVC2DVector@@ABV2@@Z
// Identity: Ego_r.pdb 0x00BAEF90 and FableWin.pdb 0x0324CE00. Retail Fable.exe 0x00A57140 is 90 bytes
//   (int3 at 0x00A5719A). No calls: dx/dy, the unclamped parameter t, then start + t * direction.
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DLINEF_POINT_ON_LINE_PASS24
#include "engine/C2DLineF.h"
C2DVector C2DLineF::GetPointOnInfiniteLine(const C2DVector& point) const
{
    float dx = Point2.X - Point1.X;
    float dy = Point2.Y - Point1.Y;
    float t = ((point.X - Point1.X) * dx + (point.Y - Point1.Y) * dy) / (dx * dx + dy * dy);
    return C2DVector(Point1.X + t * dx, Point1.Y + t * dy);
}
