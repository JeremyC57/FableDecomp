// CPhysicsMeshObject::HasNavigationLineListInfo() const -- ask the physics mesh (held through a counted-pointer copy) whether it carries navigation line-list info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A12C90, length 42 bytes.
// Identity: Ego_r.pdb `?HasNavigationLineListInfo@CPhysicsMeshObject@@QBE_NXZ`.
// Callees: the mesh's query @ 0x00A73CE0 (no arguments, bool result; manifest label
// "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated; PDB CPhysicsMesh has the bool
// NavigationLineListInfo member), the mesh's virtual release (slot 1, `call [eax+4]`).
// Shape: the HasContainmentVolume twin (0x00A128A0) without the argument: a LOCAL copy of the Mesh
// counted pointer, the bool result kept in bl across the release, plain `ret`.
struct CPhysicsMesh {
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
    bool HasNavigationLineListInfo() const;        // 0x00A73CE0
};

template <class T> struct CIVCountedPointer {
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

struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
};

struct CPhysicsMeshObject : CBaseClass {           // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    float PositionMatrix[12];                      // +0x08 CMatrix3x4
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    virtual void v00();
    bool HasNavigationLineListInfo() const;
};

bool CPhysicsMeshObject::HasNavigationLineListInfo() const
{
    CIVCountedPointer<CPhysicsMesh> mesh(Mesh);
    return mesh->HasNavigationLineListInfo();
}
