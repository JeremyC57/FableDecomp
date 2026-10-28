// CEngineInternalPrimitiveAnimatedMesh::GetBoundingBoxWorldSpace(C3DBoundingBox& box) -- the world-space box around the mesh bounding sphere: the sphere centre through the interpolated object transform, grown by the safe bounding radius times the scale plus the renderer's radius fix; always true
// Grade: DIFFER(342v351) -- x87 schedule + frame: retail frame 0x50 (ours 0x4C, retail leaves a dead slot at [esp+20h] above the centre copy); retail sums each transformed component Z-term first (E31*Z, E21*Y, E11*X; ours E21*Y first -- source term order has no effect); retail loads Radius, X, Y, Z of the sphere onto the x87 stack (an enregistered by-value copy) and stores the magnitude to [esp+4] before adding, ours ends with `fadd [eax+34h]` | tried: 4 operator term orders, Radius+mag vs mag+Radius, GetBoundingSphere() by-value copies (worse, 351v351/130 lines) | next: C3DMeshInfo::GetSafeBoundingRadius over an enregistered C3DBoundingSphere local that is NOT copied through the frame
// Leaf: ?GetBoundingBoxWorldSpace@CEngineInternalPrimitiveAnimatedMesh@@UAE_NAAVC3DBoundingBox@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCEE20, length 351 bytes, `ret 4`.
// Name: FableWin.pdb `?GetBoundingBoxWorldSpace@CEngineInternalPrimitiveAnimatedMesh@@UAE_NAAVC3DBoundingBox@@@Z` (virtual) and Ego_r.pdb agree.
// Callee: virtual CEngineInternalPrimitiveAnimatedMesh::CalcInterpolatedObjectTransform (vtable +0x84, slot 33; FableWin name).
// Inlines (FableWin.pdb): CMatrix3x4::operator*(const C3DVector&) const, C3DMeshInfo::GetSafeBoundingRadius() const, C3DVector::GetMagnitude.
// Globals: 0x01436E80 engine primitive renderer (BoundingRadiusFix +0x23C; spelling g_pPrimitiveRenderer_01436e80 is descriptive),
//   g_EngineInterpolationInfo 0x0143B8E0 (InterpolationSetList[set].WFInterpolate -> fixup 0x0143B934).
// Layout: Ego_r.pdb CEngineInternalPrimitiveBase InterpolationSet +0x2C, CEngineInternalPrimitiveMeshBase MeshInfo +0x50, Scale +0x58;
//   retail C3DMeshInfo BoundingSphere +0x28. Polymorphic local model (the flattened engine headers carry the vptr as a data member,
//   so a virtual method declared on them adds a second vptr); slots 0..31 are placeholders for the vtable position only.
#include <math.h>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    C3DVector operator*(const C3DVector& v) const
    {
        return C3DVector(v.Z * E31 + v.Y * E21 + v.X * E11 + E41,
                         v.Z * E32 + v.Y * E22 + v.X * E12 + E42,
                         v.Z * E33 + v.Y * E23 + v.X * E13 + E43);
    }
};
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class C3DBoundingBox { public:
    C3DVector MinCoord, MaxCoord;
    void SetMinMax(const C3DVector& minCoord, const C3DVector& maxCoord) { MinCoord = minCoord; MaxCoord = maxCoord; }
};
class C3DMeshInfo { public:
    unsigned char _pad00[0x28];
    C3DBoundingSphere BoundingSphere;   // +0x28
    float GetSafeBoundingRadius() const { return BoundingSphere.Radius + BoundingSphere.Center.GetMagnitude(); }
};
class CEnginePrimitiveRenderer { public:
    unsigned char _pad00[0x23c];
    float BoundingRadiusFix;            // +0x23c
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;
class CEngineInterpolationInfoSet { public: float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate; };
class CEngineInterpolationInfo { public:
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long WFServerCurrentTime;
    CEngineInterpolationInfoSet InterpolationSetList[3];
};
extern CEngineInterpolationInfo g_EngineInterpolationInfo;
class CEngineInternalPrimitiveAnimatedMesh { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& transform, float interpolate, bool flag) const;   // slot 33 (+0x84)
    unsigned char _pad04[0x2c - 4];
    long InterpolationSet;              // +0x2c
    unsigned char _pad30[0x50 - 0x30];
    const C3DMeshInfo* MeshInfo;        // +0x50
    unsigned char _pad54[0x58 - 0x54];
    float Scale;                        // +0x58
};
bool CEngineInternalPrimitiveAnimatedMesh::GetBoundingBoxWorldSpace(C3DBoundingBox& box)
{
    C3DVector centre = MeshInfo->BoundingSphere.Center;
    CMatrix3x4 transform;
    CalcInterpolatedObjectTransform(transform, g_EngineInterpolationInfo.InterpolationSetList[InterpolationSet].WFInterpolate, true);
    C3DVector worldCentre = transform * centre;
    float radius = MeshInfo->GetSafeBoundingRadius() * Scale + g_pPrimitiveRenderer_01436e80->BoundingRadiusFix;
    C3DVector extent(radius, radius, radius);
    box.MinCoord = worldCentre - extent;
    box.MaxCoord = worldCentre + extent;
    return true;
}
