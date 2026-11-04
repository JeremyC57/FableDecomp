// C3DMeshObject::GetBoundingCentre() const -- the mesh info's bounding-sphere centre transformed by the object's position matrix (axes as columns plus the translation), returned by hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35F80, length 130 bytes.
// Leaf: GetBoundingCentre
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Layout (PDB C3DMeshObject; PDB C3DMeshInfo as modelled by the landed constructor 0x00A248F0): MeshInfo +4,
// PositionMatrix +0x10 (XAxis, YAxis, ZAxis, Position); the mesh info's BoundingSphere centre at +0x28.
// Shape: the WHOLE bounding sphere is copied into a 16-byte frame local through memberwise copy constructors
// (integer moves; the unread radius store is dropped but its slot stays -- a centre-only copy gives a 12-byte
// frame), each component is `XAxis * x + YAxis * y + ZAxis * z + Position` read straight from the sphere local
// (a reference to its centre reorders the products) into a NAMED result local, and the result goes out through
// the copy constructor (the sums stay on the x87 stack: `fxch st(2)` + three fstp). VC7.1 emits the z product first.
#include "engine/C3DVector.h"
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };
class C3DBoundingSphere { public:
    C3DVector Centre;
    float     Radius;
    C3DBoundingSphere(const C3DBoundingSphere& o) : Centre(o.Centre), Radius(o.Radius) { }
};
class C3DMeshInfo { public:
    unsigned char     _pad00[0x28];
    C3DBoundingSphere BoundingSphere;                    // +0x28
};

class C3DMeshObject { public:
    virtual void v00();
    C3DMeshInfo*  MeshInfo;                              // +0x04
    void*         Mesh_Data;                             // +0x08 CCountedPointer<const C3DMeshStats>
    void*         Mesh_Info;                             // +0x0c
    CMatrix3x4    PositionMatrix;                        // +0x10
    C3DVector GetBoundingCentre() const;
};

C3DVector C3DMeshObject::GetBoundingCentre() const
{
    C3DBoundingSphere sphere = MeshInfo->BoundingSphere;

    C3DVector result;
    result.X = PositionMatrix.XAxis.X * sphere.Centre.X + PositionMatrix.YAxis.X * sphere.Centre.Y + PositionMatrix.ZAxis.X * sphere.Centre.Z + PositionMatrix.Position.X;
    result.Y = PositionMatrix.XAxis.Y * sphere.Centre.X + PositionMatrix.YAxis.Y * sphere.Centre.Y + PositionMatrix.ZAxis.Y * sphere.Centre.Z + PositionMatrix.Position.Y;
    result.Z = PositionMatrix.XAxis.Z * sphere.Centre.X + PositionMatrix.YAxis.Z * sphere.Centre.Y + PositionMatrix.ZAxis.Z * sphere.Centre.Z + PositionMatrix.Position.Z;
    return result;
}