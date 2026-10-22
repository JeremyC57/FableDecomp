// Grade: DIFFER(987v886) -- same residue as the sibling GetPointIntersectionFromTo 0x00A12450 (same inline inverse transform): x87 cofactor operand order and a larger spill frame, 101 bytes long
// CPhysicsMeshObject::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const -- ray query in the mesh's frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A120D0, 896 bytes incl. padding (886 code), `ret 10h`.
// Callees: CPhysicsMesh::GetSphereIntersectionFromTo 0x00A187C0; g_InvSqrtMantissaTable 0x013CE558.
// Layout: as 0x00A12450 (Ego_r.pdb CPhysicsMeshObject).
// Shape: the sibling 0x00A12450 body with the radius argument forwarded.
extern const unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CMatrix3x4 {
    C3DVector Row[3];
    C3DVector Position;
    __forceinline C3DVector Rotate(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * Row[0].X + v.Y * Row[1].X + v.Z * Row[2].X;
        r.Y = v.X * Row[0].Y + v.Y * Row[1].Y + v.Z * Row[2].Y;
        r.Z = v.X * Row[0].Z + v.Y * Row[1].Z + v.Z * Row[2].Z;
        return r;
    }
    __forceinline C3DVector Transform(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * Row[0].X + v.Y * Row[1].X + v.Z * Row[2].X + Position.X;
        r.Y = v.X * Row[0].Y + v.Y * Row[1].Y + v.Z * Row[2].Y + Position.Y;
        r.Z = v.X * Row[0].Z + v.Y * Row[1].Z + v.Z * Row[2].Z + Position.Z;
        return r;
    }
    __forceinline void GetInverse(CMatrix3x4& r) const
    {
        float c01 = Row[2].Y * Row[0].Z - Row[0].Y * Row[2].Z;
        float c02 = Row[0].Y * Row[1].Z - Row[1].Y * Row[0].Z;
        float c00 = Row[1].Y * Row[2].Z - Row[2].Y * Row[1].Z;
        float ooDet = 1.0f / (c02 * Row[2].X + c00 * Row[0].X + c01 * Row[1].X);
        r.Row[0].X = c00 * ooDet;
        r.Row[1].X = (Row[2].X * Row[1].Z - Row[1].X * Row[2].Z) * ooDet;
        r.Row[2].X = (Row[1].X * Row[2].Y - Row[2].X * Row[1].Y) * ooDet;
        r.Row[0].Y = c01 * ooDet;
        r.Row[1].Y = (Row[0].X * Row[2].Z - Row[2].X * Row[0].Z) * ooDet;
        r.Row[2].Y = (Row[2].X * Row[0].Y - Row[0].X * Row[2].Y) * ooDet;
        r.Row[0].Z = c02 * ooDet;
        r.Row[1].Z = (Row[1].X * Row[0].Z - Row[0].X * Row[1].Z) * ooDet;
        r.Row[2].Z = (Row[0].X * Row[1].Y - Row[1].X * Row[0].Y) * ooDet;
        r.Position.X = -(Position.X * r.Row[0].X + Position.Y * r.Row[1].X + Position.Z * r.Row[2].X);
        r.Position.Y = -(Position.X * r.Row[0].Y + Position.Y * r.Row[1].Y + Position.Z * r.Row[2].Y);
        r.Position.Z = -(Position.X * r.Row[0].Z + Position.Y * r.Row[1].Z + Position.Z * r.Row[2].Z);
    }
};
struct CMeshIntersectionData {
    C3DVector Position;                            // +0x00
    C3DVector Normal;                              // +0x0c
    float     DistanceSquared;                     // +0x18
};
struct CPhysicsMesh {
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A187C0
};
struct CPhysicsMeshObject {
    virtual void v00();
    const CPhysicsMesh* Mesh;                      // +0x04
    CMatrix3x4          PositionMatrix;            // +0x08
    float               Scale;                     // +0x38
    float               OO_Scale;                  // +0x3c
    bool GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;
};

bool CPhysicsMeshObject::GetSphereIntersectionFromTo(float radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    float localRadius = radius * OO_Scale;
    CMatrix3x4 inverse;
    PositionMatrix.GetInverse(inverse);
    C3DVector localFrom = inverse.Transform(from);
    C3DVector localTo = inverse.Transform(to);
    if (!Mesh->GetSphereIntersectionFromTo(localRadius, localFrom, localTo, data))
        return false;
    data.Normal = PositionMatrix.Rotate(data.Normal);
    data.Normal.Normalise();
    data.Position = PositionMatrix.Transform(data.Position);
    data.DistanceSquared = (data.Position - from).GetLengthSquared();
    return true;
}
