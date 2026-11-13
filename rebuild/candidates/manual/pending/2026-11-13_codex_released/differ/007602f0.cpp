// NShape::IsShapeConvex(const std::vector<C3DVector>& points) -- add up the turning angles (in turns) between consecutive XY edge directions of the closed outline, closing edges included, and call the shape convex when they come to one full turn
// Grade: DIFFER(1135v1126) -- 179 diff lines: register colouring of the outline iterators (ours keeps begin in edi and computes end-1 in place but spills from; retail holds from/to/end in other registers) and the x87 spill order that follows; 160+ spellings (iterator shapes, tail forms, 128 statement toggles) only permute registers
// Next idea: `const C3DVector*` pointer walking instead of const_iterators, or a same-TU noinline C3DVector::Normalise body to change register pressure in the edge loop
// Leaf: ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007602F0, 1126 bytes, __fastcall (points in ecx), returns bool; x87 throughout.
// Identity (proven): Ego_r.pdb 0x00771990 / FableWin.pdb 0x025D50C0 ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@...@Z is the
//   function right before ?GetFirstPointThatBreaksConvexity@NShape@@ in both PDBs, as 0x007602F0 sits right before the landed
//   NShape::GetFirstPointThatBreaksConvexity 0x00760760; the body is that convexity test (the XY turning angles add up to one turn).
//   Catalogue placeholders Math_PolygonAngleSumClosureCheck / CSplineTrack_ValidateDistance are wrong. No direct caller in retail.
// Callees: _CIacos 0x00BFEBA2 (three acos calls); inline C3DVector::Normalise (Newton reciprocal square root: 3.0f 0x0122DED4,
//   0.5f 0x0122F59C, 0.0f 0x0122DEDC); INV_TWO_PI 0x01237F08 (0.15915494); 1.0f 0x0122DED8; 0.0025f 0x0126A200.
// Shape: the first and the two closing edges are built around the loop; the tail sum is `acos(c)*K + (acos(closing)*K + total)`
//   for retail's operation order; `return t * t < 0.0025f`.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

extern const float INV_TWO_PI;                       // 0x01237F08 (1 / (2 pi))

namespace NShape {

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& points);

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& points)
{
    std::vector<C3DVector>::const_iterator first = points.begin();
    std::vector<C3DVector>::const_iterator from = first;
    std::vector<C3DVector>::const_iterator to = first + 1;
    C3DVector a = *from++;
    C3DVector b = *to++;
    C3DVector previous(b.X - a.X, b.Y - a.Y, 0.0f);
    previous.Normalise();
    float total = 0.0f;
    C3DVector edge;
    for (; to != points.end(); ++from, ++to) {
        a = *from;
        b = *to;
        edge.Initialise(b.X - a.X, b.Y - a.Y, 0.0f);
        edge.Normalise();
        total += (float)acos(edge.Dot(previous)) * INV_TWO_PI;
        previous = edge;
    }
    a = *(points.end() - 1);
    b = *first;
    edge = C3DVector(b.X - a.X, b.Y - a.Y, 0.0f);
    edge.Normalise();
    double closing = edge.Dot(previous);
    previous = edge;
    a = *first;
    ++first;
    b = *first;
    edge = C3DVector(b.X - a.X, b.Y - a.Y, 0.0f);
    edge.Normalise();
    total = (float)acos(edge.Dot(previous)) * INV_TWO_PI + ((float)acos(closing) * INV_TWO_PI + total);
    total -= 1.0f;
    return total * total < 0.0025f;
}

}
