// CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum(const CEngineCamera& camera, bool distanceOnly) -- visibility test: false beyond the primitive's fade distance from the camera; with distanceOnly that is all; otherwise the mesh's safe bounding sphere (centre through the interpolated object transform, radius scaled) must overlap the near/far depth range and lie inside all four side planes
// Grade: DIFFER(658v658) -- one x87 operand choice: the row-1 c.X term of the inline centre transform is `fld st(3); fmul [E11]` in retail and `fld [E11]; fmul st(4)` here (term order, operand swap, by-value / member-operator forms do not move it); everything else exact
// Leaf: IsInsideFrustum
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCF040, length 658 bytes (`ret 8`, virtual
// ?IsInsideFrustum@CEngineInternalPrimitiveAnimatedMesh@@UAE_NABVCEngineCamera@@_N@Z).
// Callees: 0x00BEB660 (thiscall, returns the primitive's fade distance as a float; forge autoname
// "CEnginePrimitive_ComputeFadeDistance"), CalcInterpolatedObjectTransform (virtual slot 33, +0x84; `(out, float t, bool)`).
// Globals: the interpolation info 0x0143B8E0 (InterpolationSetList[set].WFInterpolate at 0x0143B934, as the landed
// GetWorldPosition 0x00BCEB40).
// Layout: as the landed GetWorldPosition 0x00BCEB40 (InterpolationSet +0x2c, start/end positions +0x88/+0x94);
// CEngineInternalPrimitiveMeshBase MeshInfo +0x50 / Scale +0x58; Ego_r.pdb C3DMeshInfo BoundingSphere +0x28,
// SafeBoundingRadius +0x38; Ego_r.pdb CEngineCamera (CameraDesc +0x14: WorldPosition +0x54, NearZPlane +0x6c, FarZPlane
// +0x70; LookVector +0x1b4; FrustrumPlanes +0x1c0).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
static __forceinline C3DVector Transform(const C3DVector& v, const CMatrix3x4& m)
{
    C3DVector c(v);
    return C3DVector(c.X * m.E11 + c.Y * m.E21 + c.Z * m.E31 + m.E41,
                     c.X * m.E12 + c.Y * m.E22 + c.Z * m.E32 + m.E42,
                     c.X * m.E13 + c.Y * m.E23 + c.Z * m.E33 + m.E43);
}
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct C3DMeshInfo {
    unsigned char     _pad00[0x28];
    C3DBoundingSphere BoundingSphere;                   // +0x28
    float             SafeBoundingRadius;               // +0x38
};
struct CPlane { C3DVector Normal; float Distance; };
class CEngineCamera {
public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                        // +0x54
    unsigned char _pad60[0x6c - 0x60];
    float         NearZPlane;                           // +0x6c
    float         FarZPlane;                            // +0x70
    unsigned char _pad74[0x1b4 - 0x74];
    C3DVector     LookVector;                           // +0x1b4
    CPlane        FrustrumPlanes[4];                    // +0x1c0
};
struct CEngineInterpolationInfoSet { float GTPredictedRenderAt; float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
struct CEngineInterpolationInfo {
    CEngineInterpolationInfoSet Sets[6];
    long                        WFServerCurrentTime;    // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4c
};
extern CEngineInterpolationInfo g_EngineInterpolationInfo;   // 0x0143B8E0

class CEngineInternalPrimitiveAnimatedMesh {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool scale) const;   // slot 33 (+0x84)
    virtual bool IsInsideFrustum(const CEngineCamera& camera, bool distanceOnly);
    float GetFadeDistance() const;                      // 0x00BEB660
    unsigned char _pad04[0x2c - 4];
    long          InterpolationSet;                     // +0x2c
    unsigned char _pad30[0x50 - 0x30];
    C3DMeshInfo*  MeshInfo;                             // +0x50
    C3DMeshInfo*  ShadowMeshInfo;                       // +0x54
    float         Scale;                                // +0x58
    unsigned char _pad5c[0x88 - 0x5c];
    C3DVector     m_positionStart;                      // +0x88
    C3DVector     m_positionEnd;                        // +0x94
};

bool CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum(const CEngineCamera& camera, bool distanceOnly)
{
    float t = g_EngineInterpolationInfo.InterpolationSetList[InterpolationSet].WFInterpolate;
    C3DVector position = (m_positionEnd - m_positionStart) * t + m_positionStart;
    C3DVector offset = position - camera.WorldPosition;
    float distance = GetFadeDistance();
    if (offset.GetLengthSquared() > distance * distance)
        return false;
    if (distanceOnly)
        return true;
    CMatrix3x4 objectTransform;
    CalcInterpolatedObjectTransform(objectTransform, t, true);
    C3DVector centre = Transform(MeshInfo->BoundingSphere.Center, objectTransform);
    float radius = MeshInfo->SafeBoundingRadius * Scale;
    C3DVector toCentre = centre - camera.WorldPosition;
    float depth = toCentre.Dot(camera.LookVector);
    if (radius + depth < camera.NearZPlane || depth - radius > camera.FarZPlane)
        return false;
    if (toCentre.Dot(camera.FrustrumPlanes[2].Normal) > radius)
        return false;
    if (toCentre.Dot(camera.FrustrumPlanes[3].Normal) > radius)
        return false;
    if (toCentre.Dot(camera.FrustrumPlanes[0].Normal) > radius)
        return false;
    if (toCentre.Dot(camera.FrustrumPlanes[1].Normal) > radius)
        return false;
    return true;
}
