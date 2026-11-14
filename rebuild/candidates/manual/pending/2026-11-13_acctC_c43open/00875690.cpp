// NCameraModes::CGeneralCircle::CGeneralCircle(const C3DVector&, const C3DVector&, const C3DVector&) -- the circle through three points: plane axes from the first edge, the 2-D circumcentre in that plane, then Origin, Radius and the unit axes Ei (towards the first point) and Ej
// Grade: DIFFER(726v762) -- identical to retail through 0x008757C5 (both edge vectors, normal, in-plane axis, the two AccurateNormalise calls, the edge length, both dot products incl. the two (c - a) temporaries); from there the 2-D solve keeps values on the x87 stack where retail reads c.X [esp+10h], c.Y [esp+14h], b.X [esp+28h] from frame words (see NOTES.md for the decoded formula)
// Leaf: ??0CGeneralCircle@NCameraModes@@QAE@ABVC3DVector@@00@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00875690, 762 bytes, `ret 0Ch`.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/NCameraModes_CGeneralCircle.h"

namespace NCameraModes
{
CGeneralCircle::CGeneralCircle(const C3DVector& point_a, const C3DVector& point_b, const C3DVector& point_c)
{
    C3DVector ab = point_b - point_a;
    C3DVector normal = ab ^ (point_c - point_a);
    C3DVector in_plane = ab ^ normal;
    ab.AccurateNormalise();
    in_plane.AccurateNormalise();
    C2DVector b((point_b - point_a).GetMagnitude(), 0.0f);
    C2DVector c((point_c - point_a).Dot(ab), (point_c - point_a).Dot(in_plane));
    float half_c = c.GetSquaredMagnitude() * 0.5f;
    float half_b = b.X * b.X * 0.5f;
    float u = (b.Y * half_c - half_b * c.Y) / (c.X * b.Y - b.X * c.Y);
    float v = (half_c - u * c.X) / c.Y;
    C2DVector centre(u, v);
    Origin = point_a + ab * centre.X + in_plane * centre.Y;
    Radius = centre.GetMagnitude();
    Ei = point_a - Origin;
    Ej = Ei ^ normal;
    Ei.AccurateNormalise();
    Ej.AccurateNormalise();
}
}
