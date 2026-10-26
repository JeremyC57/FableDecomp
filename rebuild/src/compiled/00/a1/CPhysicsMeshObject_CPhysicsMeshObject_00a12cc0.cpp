// CPhysicsMeshObject::CPhysicsMeshObject(const CIVCountedPointer<const CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale) -- base and empty Mesh, then `Mesh = mesh` (release/add-ref) and SetPositionMatrix
// Leaf: ??0CPhysicsMeshObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A12CC0, length 95 bytes.
// Identity: Ego_r.pdb CPhysicsMeshObject (Mesh +4, PositionMatrix +8, Scale +0x38, OO_Scale +0x3C);
// vtable 0x0125D084 (rtti-vtables.tsv).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, the old mesh's virtual release (slot 1),
// CPhysicsMeshObject::SetPositionMatrix(const C3DVector&, const CRightHandedSet&, float) @ 0x00A114B0.
// Shape: the counted pointer is default-constructed (zero store after the vtable store) and then
// ASSIGNED in the body: its inline operator= re-reads the member, skips equal pointers, releases
// the old pointee (kept in ecx) and clears the member, then stores the new pointer and add-refs it
// (`inc [eax+4]`). `ret 10h` = four arguments.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };

class CPhysicsMesh { public:
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
};

template <class T> class CIVCountedPointer { public:
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            T* object = m_pObject;
            if (object) {
                if (--object->m_refCount == 0)
                    object->DeleteThis();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                ++m_pObject->m_refCount;
        }
        return *this;
    }
};

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
};

class CPhysicsMeshObject : public CBaseClass { public:           // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    float PositionMatrix[12];                      // +0x08 CMatrix3x4
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    virtual void v00();
    CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale);
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A114B0
};

CPhysicsMeshObject::CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale)
{
    Mesh = mesh;
    SetPositionMatrix(position, orientation, scale);
}