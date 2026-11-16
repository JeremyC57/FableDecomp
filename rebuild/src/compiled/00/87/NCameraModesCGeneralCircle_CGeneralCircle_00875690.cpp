// NCameraModes::CGeneralCircle::CGeneralCircle(const C3DVector&, const C3DVector&, const C3DVector&) -- the circle through three points: plane axes from the first edge, the 2-D circumcentre in that plane, then Origin, Radius and the unit axes Ei (towards the first point) and Ej
// Leaf: ??0CGeneralCircle@NCameraModes@@QAE@ABVC3DVector@@00@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00875690, 762 bytes, `ret 0Ch`.
// EVIDENCED: the 2-D origin subtraction (b = p2 - origin, c = p3 - origin, centre = offset + origin) that keeps retail's separate
//   copies of b.X / c.X / c.Y / centre (wfB6 sixth agent, mini TUs); the call / store structure (retail disassembly).
// FITTED (chosen by grading, mechanism measured with c2's list-scheduler trace, wfB6 seventh agent): the C3DVECTOR_CROSS_XZ_GROUP +
//   C3DVECTOR_SUBTRACT_XY_GROUP opt-ins; the edge length through GetAccurateMagnitude (Ego_r / FableWin PDB name; one tuple fewer than
//   GetMagnitude, which moves c2's second 81-tuple scheduling-region cut one instruction later: Origin `mov edx,edi` after `fld [esp+4Ch]`,
//   retail 0x00875853); Ei built from its three component differences (no PAREN tuple on X: `mov eax,[esp+28h]` issues with
//   `fstp [esp+28h]`'s cycle, before `fld [esi+4]`, retail 0x008758FB); Radius = (float)sqrt(centre.GetSquaredMagnitude()) (the C3DVector::GetMagnitude
//   idiom; restores the third region cut inside Ej). FableWin /Od row size hint: 216i/690b v FableWin 215i/702b (old text 198i/634b).
#define FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
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
    C2DVector origin(0.0f, 0.0f);
    C2DVector p2((point_b - point_a).GetAccurateMagnitude(), 0.0f);
    C2DVector p3((point_c - point_a).Dot(ab), (point_c - point_a).Dot(in_plane));
    C2DVector b = p2 - origin;
    C2DVector c = p3 - origin;
    float half_c = p3.GetSquaredMagnitude() * 0.5f;
    float half_b = p2.X * p2.X * 0.5f;
    C2DVector offset;
    offset.X = (b.Y * half_c - half_b * c.Y) / (c.X * b.Y - b.X * c.Y);
    offset.Y = (half_c - offset.X * c.X) / c.Y;
    C2DVector centre = offset + origin;
    Origin = point_a + ab * offset.X + in_plane * offset.Y;
    Radius = (float)sqrt(centre.GetSquaredMagnitude());
    Ei = C3DVector(point_a.X - Origin.X, point_a.Y - Origin.Y, point_a.Z - Origin.Z);
    Ej = Ei ^ normal;
    Ei.AccurateNormalise();
    Ej.AccurateNormalise();
}
}