// CPhysicsMeshObject::GetRigidBodyBoundingPoints(std::vector<C3DVector>& points) const -- ask the physics mesh (held through a counted-pointer copy) for its rigid-body bounding points, then scale every point by the object's Scale (+0x38)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A12840, length 74 bytes.
// Identity: Ego_r.pdb `?GetRigidBodyBoundingPoints@CPhysicsMeshObject@@QBEXAAV?$vector@VC3DVector@@...@Z`.
// Callees: CPhysicsMesh::GetRigidBodyBoundingPoints(std::vector<C3DVector>&) const @ 0x00A6CB20, the
// mesh's virtual release (slot 1), the SGI `for_each<C3DVector*, scale functor>` @ 0x00A13440
// (`__fastcall`: hidden functor result in ecx, `first` in edx, `last` and the by-value functor on
// the stack; it multiplies each component by the functor's float; manifest label
// `_For_each<C3DVector*,CAddElements<C3DVector>>`).
// Shape: a LOCAL copy of the Mesh counted pointer (add-ref `inc [esi+4]`, release with no clearing
// store, the pointee kept in esi across the call); the scale functor is a 4-byte record whose float
// is copied as a dword (`mov ecx,[ebx+38h]`), built in the dead `points` argument slot that also
// receives the discarded functor result (`mov [esp+14h],ecx` ... `lea ecx,[esp+18h]`).
struct C3DVector { float X, Y, Z; };
struct C3DVectorVector {                           // retail std::vector: first/last/end
    C3DVector* first;
    C3DVector* last;
    C3DVector* end_of_storage;
};

struct CPhysicsMesh {
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
    void GetRigidBodyBoundingPoints(C3DVectorVector& points) const;   // 0x00A6CB20
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

struct CScaleElements {
    float m_scale;
    CScaleElements(float scale) : m_scale(scale) { }
};
CScaleElements __fastcall ForEach_ScaleElements(C3DVector* first, C3DVector* last, CScaleElements f);   // 0x00A13440

struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
};

struct CPhysicsMeshObject : CBaseClass {           // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    float PositionMatrix[12];                      // +0x08 CMatrix3x4
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    virtual void v00();
    void GetRigidBodyBoundingPoints(C3DVectorVector& points) const;
};

void CPhysicsMeshObject::GetRigidBodyBoundingPoints(C3DVectorVector& points) const
{
    {
        CIVCountedPointer<CPhysicsMesh> mesh(Mesh);
        mesh->GetRigidBodyBoundingPoints(points);
    }
    ForEach_ScaleElements(points.first, points.last, CScaleElements(Scale));
}