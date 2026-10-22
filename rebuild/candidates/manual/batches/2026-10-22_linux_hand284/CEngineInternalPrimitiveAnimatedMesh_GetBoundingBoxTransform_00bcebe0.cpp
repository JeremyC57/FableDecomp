// CEngineInternalPrimitiveAnimatedMesh::GetBoundingBoxTransform(CMatrix3x4& transform) -- a conservative box for the animated mesh: a cube of half-size (|sphere centre| + radius) * Scale plus the renderer's bounds margin, over Scale, centred on the bounding-sphere centre, post-multiplied by the interpolated object transform; always true
// Leaf: GetBoundingBoxTransform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCEBE0, length 573 bytes (`ret 4`, virtual
// ?GetBoundingBoxTransform@CEngineInternalPrimitiveAnimatedMesh@@UAE_NAAVCMatrix3x4@@@Z).
// Callees: CalcInterpolatedObjectTransform (virtual slot 33, +0x84; `(out, float t, bool)` as the landed
// GetParentWorldSpaceBoundingSphereAnimatedMesh 0x00B395E0 calls it). Globals: the engine render object 0x01436E80 (float
// +0x23c, a bounds margin), the interpolation-time table 0x0143B934 (12-byte records indexed by InterpolationSet +0x2c, the
// landed RenderSubPrimitive 0x00BD7110 extern).
// Layout: engine/_quarantine/CEngineInternalPrimitiveMeshBase.h (MeshInfo +0x50, Scale +0x58); engine/C3DMeshInfo.h
// BoundingSphere +0x28 (centre +0x28, radius +0x34); the primitive base's InterpolationSet +0x2c.
// Shape: the sphere is read twice through the by-value C3DMeshInfo::GetBoundingSphere() (the landed 0x00B395E0 helper),
// the radius from the first copy and the length from the second's GetPos(), which is what loads the radius onto the x87
// stack first and gives the 16-byte sphere frame slot; the translation comes from a third GetPos() copy. The product is
// an inline Multiply(a, b) on the target over a named copy `a` of the transform (an operator*= with an internal copy
// loads the wrong operand of each row-2..4 product); one chained zero store, one chained diagonal store.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    float GetLength() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
};
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DVector GetPos() const { return C3DVector(Center.X, Center.Y, Center.Z); }
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void Multiply(const CMatrix3x4& a, const CMatrix3x4& b)
    {
        E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
        E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
        E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
        E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
        E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
        E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
        E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
        E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
        E41 = (a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31) + b.E41;
        E42 = (a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32) + b.E42;
        E43 = (a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33) + b.E43;
    }
};
struct C3DMeshInfo {
    unsigned char     _pad[0x28];
    C3DBoundingSphere BoundingSphere;                   // +0x28
    C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }
};
struct CEngineRenderGlobals {
    unsigned char _pad[0x23c];
    float         AnimatedBoundsMargin;                 // +0x23c
};
extern CEngineRenderGlobals* g_pEngineRender_01436e80;  // 0x01436E80
struct SInterpolationTime { float Time; float m_4; float m_8; };   // 12-byte records
extern SInterpolationTime g_interpolationTimes[];       // 0x0143B934

class CEngineInternalPrimitiveAnimatedMesh {
public:
    unsigned char _pad04[0x2c - 4];
    long          InterpolationSet;                     // +0x2c
    unsigned char _pad30[0x50 - 0x30];
    C3DMeshInfo*  MeshInfo;                             // +0x50
    C3DMeshInfo*  ShadowMeshInfo;                       // +0x54
    float         Scale;                                // +0x58
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool scale) const;   // slot 33 (+0x84)
    virtual bool GetBoundingBoxTransform(CMatrix3x4& transform);
};

bool CEngineInternalPrimitiveAnimatedMesh::GetBoundingBoxTransform(CMatrix3x4& transform)
{
    float size = ((MeshInfo->GetBoundingSphere().Radius + MeshInfo->GetBoundingSphere().GetPos().GetLength()) * Scale + g_pEngineRender_01436e80->AnimatedBoundsMargin) / Scale;
    transform.E12 = transform.E13 = transform.E21 = transform.E23 = transform.E31 = transform.E32 =
        transform.E41 = transform.E42 = transform.E43 = 0.0f;
    transform.E11 = transform.E22 = transform.E33 = size;
    {
        C3DVector centre = MeshInfo->GetBoundingSphere().GetPos();
        transform.E41 = centre.X;
        transform.E42 = centre.Y;
        transform.E43 = centre.Z;
    }
    CMatrix3x4 objectTransform;
    CalcInterpolatedObjectTransform(objectTransform, g_interpolationTimes[InterpolationSet].Time, true);
    {
        CMatrix3x4 a(transform);
        transform.Multiply(a, objectTransform);
    }
    return true;
}
