// CPhysicsMeshObject::HasContainmentVolume(const CCharString& name) const -- ask the physics mesh (held through a counted-pointer copy) whether it has the named containment volume
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A128A0, length 49 bytes.
// Identity: Ego_r.pdb `?HasContainmentVolume@CPhysicsMeshObject@@QBE_NABVCCharString@@@Z`.
// Callees: CPhysicsMesh::HasContainmentVolume(const CCharString&) const @ 0x00A6CBC0 (manifest label
// "C3DMeshVolume_HasVolume"), the mesh's virtual release (slot 1, `call [edx+4]`).
// Shape: a LOCAL copy of the Mesh counted pointer (add-ref, the pointee in esi across the call,
// release with no clearing store); the bool result survives the release in bl (`mov bl,al` ...
// `mov al,bl`).
class CCharString { public: void* m_pData; };
class CPhysicsMesh { public:
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
    bool HasContainmentVolume(const CCharString& name) const;   // 0x00A6CBC0
};

template <class T> class CIVCountedPointer { public:
    T* m_pObject;
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject)
    {
        if (m_pObject)
            ++m_pObject->m_refCount;
    }
    ~CIVCountedPointer()
    {
        T* object = m_pObject;
        if (object) {
            if (--object->m_refCount == 0)
                object->DeleteThis();
            m_pObject = 0;
        }
    }
    T* operator->() const { return m_pObject; }
};

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
};

class CPhysicsMeshObject : public CBaseClass { public:           // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    float PositionMatrix[12];                      // +0x08 CMatrix3x4
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    virtual void v00();
    bool HasContainmentVolume(const CCharString& name) const;
};

bool CPhysicsMeshObject::HasContainmentVolume(const CCharString& name) const
{
    CIVCountedPointer<CPhysicsMesh> mesh(Mesh);
    return mesh->HasContainmentVolume(name);
}