// CLocalDetailLayer::CObject::CObject() -- default constructor of a local-detail object instance: zeroed bounding sphere and an empty engine primitive handle
// Leaf: ??0CObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFBBC0, length 44 bytes (no direct callers: an array-construction
// callback / emitted inline).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (the handle's base). Vtable 0x0122F598 = CEnginePrimitiveHandle (RTTI).
// Label correction: the chunk calls this CTCStealth::OnCreate; it takes no argument, returns `this` and initialises only
// +0x34..+0x4b, which is exactly the 76-byte Ego_r.pdb `CObject` (Matrix CMatrix3x4 +0x00, Scale +0x30, BoundingSphere
// (C3DBoundingSphere: Center +0x34, Radius +0x40) +0x34, PrimitiveHandle (CEnginePrimitiveHandle) +0x44): the matrix and
// scale have no constructors. The owner (CLocalDetailLayer holds a CArray<CObject>) is inferred from the members.
// Shape: the bounding sphere's inline constructor zeroes its four words through one register, then the handle's inline
// constructor: base call, vtable, and the data pointer as a CIVCountedPointer MEMBER OBJECT with its own inline
// constructor (its zero store follows the vtable store; a plain `Data(0)` initialiser or body assignment of a raw
// pointer is scheduled before it).
class CBaseClass { public:                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

class CEnginePersistentPrimitiveBase;
struct CIVCountedPointer {                                  // CIVCountedPointer<CEnginePersistentPrimitiveBase>
    CEnginePersistentPrimitiveBase* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass { public:                // vtable 0x0122F598, 8 bytes
    CIVCountedPointer Data;                                 // +0x04
    CEnginePrimitiveHandle() { }
    virtual void v00();
};

class C3DVector { public:
    float X, Y, Z;
};

class C3DBoundingSphere { public:
    C3DVector Center;                                       // +0x00
    float     Radius;                                       // +0x0c
    C3DBoundingSphere() { Center.X = 0; Center.Y = 0; Center.Z = 0; Radius = 0; }
};

namespace CLocalDetailLayer {

struct CObject {
    float                  Matrix[12];                      // +0x00 CMatrix3x4 (no constructor)
    float                  Scale;                           // +0x30
    C3DBoundingSphere      BoundingSphere;                  // +0x34
    CEnginePrimitiveHandle PrimitiveHandle;                 // +0x44
    CObject();
};

CObject::CObject()
{
}

}