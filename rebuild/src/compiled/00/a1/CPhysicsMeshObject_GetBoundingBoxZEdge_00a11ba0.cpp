// CPhysicsMeshObject::GetBoundingBoxZEdge() const -- half the mesh bounding box's Z extent along the object's third basis row, rotated into world orientation (no translation)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A11BA0, length 113 bytes (`ret 4`, C3DVector by hidden pointer). Constants 0.5f @ 0x0122F59C, 0.0f @ 0x0122DEDC.
// Identity: Ego_r.pdb `?GetBoundingBoxZEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ`; the X/Y/Z edges are 0x00A11C20 / 0x00A11C90 / 0x00A11BA0
// (callers 0x004BC9A5.. use all three in a row). The manifest's "CEngineInternalPrimitiveParticleLine::GetWorldPosition" is propagated and wrong.
// Layout (PDB): CPhysicsMeshObject = vptr, Mesh +4 (CIVCountedPointer<const CPhysicsMesh>), PositionMatrix CMatrix3x4 +8; the mesh's
// bounding box sits at retail +0x84 (Min) / +0x90 (Max) (12-byte containers before it, like CentreOfGravity at +0xA8).
// Shape: `PositionMatrix.Rotate(C3DVector(...))` -- the landed CMatrix3x4::Rotate of GetCentreOfGravity (0x00A11EC0, NAMED result,
// memberwise copy constructor); the two zero components fold into `(a + b) * 0.0f` per row (the 0.0f pool constant), the half
// extent is the only frame word of the temporary vector; the box is reached through a reference (`add eax,84h`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class C3DBoundingBox { public:
    C3DVector Min;
    C3DVector Max;
};

class CMatrix3x4 { public:
    C3DVector Row[3];                              // the rotation rows
    C3DVector Position;
    C3DVector Rotate(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * Row[0].X + v.Y * Row[1].X + v.Z * Row[2].X;
        r.Y = v.X * Row[0].Y + v.Y * Row[1].Y + v.Z * Row[2].Y;
        r.Z = v.X * Row[0].Z + v.Y * Row[1].Z + v.Z * Row[2].Z;
        return r;
    }
};

class CPhysicsMesh { public:
    unsigned char  _pad00[0x84];
    C3DBoundingBox BoundingBox;                    // +0x84 (retail; 12-byte containers before it)
};

class CPhysicsMeshObject { public:
    virtual void v00();
    const CPhysicsMesh* Mesh;                      // +0x04 CIVCountedPointer<const CPhysicsMesh>
    CMatrix3x4          PositionMatrix;            // +0x08
    float               Scale;                     // +0x38
    float               OO_Scale;                  // +0x3c
    C3DVector GetBoundingBoxXEdge() const;
    C3DVector GetBoundingBoxYEdge() const;
    C3DVector GetBoundingBoxZEdge() const;
};

C3DVector CPhysicsMeshObject::GetBoundingBoxZEdge() const
{
    const C3DBoundingBox& box = Mesh->BoundingBox;
    return PositionMatrix.Rotate(C3DVector(0.0f, 0.0f, (box.Max.Z - box.Min.Z) * 0.5f));
}