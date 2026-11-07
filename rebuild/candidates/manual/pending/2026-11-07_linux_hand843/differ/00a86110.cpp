// CIPhysicsFacesSegmentBase::CalculateDividingPlane(const CArray<C3DVector>& vertices, const CArray<C3DTriangle2>& triangles, const CArray<unsigned short>& faces, long axis) -- the splitting plane for a face set: the axis normal from a three-entry table (Z, X, Y), the bounding box of every face vertex, and the plane through the mean of the per-face points (vertex 1 and twice vertex 2, about the box centre, times 0.333333f) offset back by the centre
// Grade: DIFFER(1967v1967) -- md 26, size exact: the axis-table copy (retail `lea ecx,[eax*4+0x1398658]` then three loads through ecx, ours folds the table into the first load and keeps .Y in ecx), the average's Y / Z scale products load the reciprocal first (`fld [esp+50h] ; fmul total.Y`, retail `fld total.Y ; fmul [esp+50h]`), the loop-alignment pad (`mov edi,edi` v `lea esp,[esp]`) and one `pop ebx` scheduled 2 bytes apart
// Next idea: the axis normal through another table spelling (retail keeps the element address in ecx); the scale as a named float gives retail's Y / Z order but stores the reciprocal (`fstp ; fmul [mem]`, md 22, 1969 bytes): a form that keeps both
// Leaf: ?CalculateDividingPlane@CIPhysicsFacesSegmentBase@@KI?AVCPlane@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A86110, 1967 bytes, protected static __fastcall (hidden CPlane result in ecx, vertices in edx; triangles,
//   faces, axis on the stack), `ret 0Ch`.
// Identity (proven): Ego_r.pdb ?CalculateDividingPlane@CIPhysicsFacesSegmentBase@@KI?AVCPlane@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z
//   (0x00C09540, right before ?Calculate@CIPhysicsFacesSegmentBase@@KI_N... 0x00C09920 = retail 0x00A868C0, whose parked chunk 534 source calls
//   0x00A86110 with exactly this register / stack split).
// Data: the axis table at 0x01398658 (.data, nine floats 0,0,1 / 1,0,0 / 0,1,0, referenced only here; no PDB data name: `DividingPlaneAxes`
//   is descriptive, file static); constants 0.5f 0x0122F59C, 0.333333f (0x3EAAAA9F) 0x0129DE9C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Shape: the bounding-box pass loops over a local copy of the face's three vertices through a pointer (an index loop is fully unrolled by
//   VC7.1, retail keeps `mov ebp,3 ; .. dec ebp ; jne`), vertex-first compares; min and max both copied from the first vertex; the centroid
//   pass (VC7.1 unrolls it by four, as retail) copies vertices 1 and 2 and subtracts the centre in place (FableWin operator-=, header opt-in
//   FABLE_RETAIL_C3DVECTOR_SUBASSIGN); vertex 0 is never read (retail loads only VertexIndex[1] and [2] there); the mean scaled in place,
//   the centre added per component, the distance spelled total-first (retail's non-destructive `fld normal.Z ; fmul st(1)` dot).
// Header: engine/CPlane.h (new, chunk 843: Ego_r layout, FableWin inline constructors); no engine header for CIPhysicsFacesSegmentBase:
//   the row models only the protected static it defines.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBASSIGN
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/C3DTriangle2.h"
#include "engine/CPlane.h"

class CIPhysicsFacesSegmentBase {
protected:
    static CPlane __fastcall CalculateDividingPlane(const CArray<C3DVector>& vertices, const CArray<C3DTriangle2>& triangles, const CArray<unsigned short>& faces, long axis);
};

static C3DVector DividingPlaneAxes[3] = { C3DVector(0.0f, 0.0f, 1.0f), C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 1.0f, 0.0f) };   // 0x01398658

CPlane __fastcall CIPhysicsFacesSegmentBase::CalculateDividingPlane(const CArray<C3DVector>& vertices, const CArray<C3DTriangle2>& triangles, const CArray<unsigned short>& faces, long axis)
{
    const C3DVector& first = vertices[triangles[faces[0]].GetVertexIndex(0)];
    C3DVector minimum(first);
    C3DVector maximum(first);
    C3DVector normal = DividingPlaneAxes[axis];
    long count = faces.size();
    long i;
    for (i = 0; i < count; ++i) {
        const C3DTriangle2& triangle = triangles[faces[i]];
        C3DVector points[3] = { vertices[triangle.GetVertexIndex(0)], vertices[triangle.GetVertexIndex(1)], vertices[triangle.GetVertexIndex(2)] };
        for (const C3DVector* point = points; point < points + 3; ++point) {
            if (point->X < minimum.X) minimum.X = point->X;
            if (point->Y < minimum.Y) minimum.Y = point->Y;
            if (point->Z < minimum.Z) minimum.Z = point->Z;
            if (point->X > maximum.X) maximum.X = point->X;
            if (point->Y > maximum.Y) maximum.Y = point->Y;
            if (point->Z > maximum.Z) maximum.Z = point->Z;
        }
    }
    C3DVector centre = (minimum + maximum) * 0.5f;
    C3DVector total(0.0f, 0.0f, 0.0f);
    for (i = 0; i < count; ++i) {
        const C3DTriangle2& triangle = triangles[faces[i]];
        C3DVector a = vertices[triangle.GetVertexIndex(1)];
        C3DVector b = vertices[triangle.GetVertexIndex(2)];
        a -= centre;
        b -= centre;
        total += (b + a + b) * 0.333333f;
    }
    total *= 1.0f / count;
    total.X += centre.X;
    total.Y += centre.Y;
    total.Z += centre.Z;
    return CPlane(normal, total.X * normal.X + total.Y * normal.Y + total.Z * normal.Z);
}
