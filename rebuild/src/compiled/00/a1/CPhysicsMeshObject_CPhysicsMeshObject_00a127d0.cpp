// CPhysicsMeshObject::~CPhysicsMeshObject() -- release the counted physics mesh at +4 (count at +4 of the mesh, destroyed through its virtual slot 1) and null it, then the out-of-line CBaseClass destructor (tail jump)
// Leaf: ??1CPhysicsMeshObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A127D0, length 41 bytes.
// Vtables: CPhysicsMeshObject 0x0125D084 (rtti-vtables.tsv); the base destructor @ 0x0099A2E0 only
// stores the CBaseClass vtable 0x01231710 (its constructor 0x0099A2D0 is the base of the landed
// CPhysicsMeshObject constructor 0x006A6B80).
// Callees: the mesh's virtual release (slot 1, `call [eax+4]`), CBaseClass::~CBaseClass @ 0x0099A2E0.
// Shape: own vtable store first, then the Mesh member's own inline (`__forceinline`) counted-pointer
// destructor (pointee kept in ecx for `dec [ecx+4]` and the vtable fetch, member cleared inside the
// null test), then the base destructor as a sibling tail jump. PDB layout: Mesh
// CIVCountedPointer<const CPhysicsMesh> +4, PositionMatrix +8, Scale +0x38, OO_Scale +0x3C (modelled
// locally: polymorphic class, no engine header).
class CPhysicsMesh { public:
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
};

template <class T> class CIVCountedPointer { public:
    T* m_pObject;
    __forceinline ~CIVCountedPointer()
    {
        T* object = m_pObject;
        if (object) {
            if (--object->m_refCount == 0)
                object->DeleteThis();
            m_pObject = 0;
        }
    }
};

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

class CPhysicsMeshObject : public CBaseClass { public:           // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    float PositionMatrix[12];                      // +0x08 CMatrix3x4
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    virtual void v00();
    ~CPhysicsMeshObject();
};

CPhysicsMeshObject::~CPhysicsMeshObject()
{
}