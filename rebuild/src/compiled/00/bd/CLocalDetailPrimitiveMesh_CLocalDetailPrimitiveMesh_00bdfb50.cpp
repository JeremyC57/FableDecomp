// CLocalDetailPrimitiveMesh::CLocalDetailPrimitiveMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map) -- the local-detail primitive base (no next, empty bounding sphere, type and map), this class's vtable and an empty engine primitive handle
// Leaf: ??0CLocalDetailPrimitiveMesh@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDFB50, 69 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): stores vtable 0x012A7E04 (RTTI .?AVCLocalDetailPrimitiveMesh@@ : CLocalDetailPrimitiveBase :
//   CMovableResource); Ego_r.pdb ??0CLocalDetailPrimitiveMesh@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z.
//   The catalog label "CLocalDetailPrimitiveRepeatedMesh" is propagated from the byte-similar 0x00BFAA80 (which stores
//   the RepeatedMesh vtable 0x012A82AC).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 (the PrimitiveHandle's base); vtable 0x0122F598 CEnginePrimitiveHandle.
// Layout (Ego_r.pdb): CLocalDetailPrimitiveBase {Next +4, BoundingBox +8, BoundingSphere +0x20, Type +0x30, Map +0x34},
//   Matrix CMatrix3x4 +0x38, Scale +0x68, PrimitiveHandle CEnginePrimitiveHandle +0x6C (0x74 bytes).
// Shape: CMovableResource's and the base's inline constructors (their vptr stores dead), C3DBoundingSphere's default
//   constructor zeroing centre and radius; the handle's inline constructor (CBaseClass out of line, vtable, Data = 0).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointeeBase* Object;
};
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() { }
    virtual ~CEnginePrimitiveHandle();
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};

class C3DVector {
public:
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float X, Y, Z;
};
class C3DBoundingBox {
public:
    C3DVector Min, Max;
};
class C3DBoundingSphere {
public:
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
    C3DVector Center;
    float Radius;
};
class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
};

class CMovableResource {
public:
    CMovableResource() { }
    virtual ~CMovableResource();
};
class CLocalDetailObjectCollectionType;
class CLocalDetailCacheMap;

class CLocalDetailPrimitiveBase : public CMovableResource {
public:
    CLocalDetailPrimitiveBase(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map)
        : Next(0), Type(type), Map(map) { }
    CLocalDetailPrimitiveBase* Next;                 // +0x04
    C3DBoundingBox BoundingBox;                      // +0x08
    C3DBoundingSphere BoundingSphere;                // +0x20
    const CLocalDetailObjectCollectionType* Type;    // +0x30
    CLocalDetailCacheMap* Map;                       // +0x34
};

class CLocalDetailPrimitiveMesh : public CLocalDetailPrimitiveBase {
public:
    CLocalDetailPrimitiveMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map);
    virtual ~CLocalDetailPrimitiveMesh();
    CMatrix3x4 Matrix;                               // +0x38
    float Scale;                                     // +0x68
    CEnginePrimitiveHandle PrimitiveHandle;          // +0x6C
};

CLocalDetailPrimitiveMesh::CLocalDetailPrimitiveMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map)
    : CLocalDetailPrimitiveBase(type, map)
{
}