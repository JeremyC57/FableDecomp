// CPhysicsMeshInfo::Reset() -- drop the counted mesh data at +0xC (`MeshData = 0`): release it through its virtual slot 1 when the count reaches zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24F90, length 36 bytes. Mangled `?Reset@CPhysicsMeshInfo@@EAEXXZ`
// (a private virtual; CPhysicsMeshInfo vtable 0x0129CD50 slot 3).
// Callees: the mesh data's virtual self-destroy (slot 1, `call [eax+4]`).
// Shape: CIVCountedPointer::operator=(T*) inlined with a null argument: the release (pointee kept in
// a local for `dec [ecx+4]` and the vtable fetch, member cleared inside the null test) followed by
// the unconditional store of the new pointer, so retail keeps BOTH `mov dword ptr [esi+0Ch],0`.
struct CMeshDataBank;

struct CMeshData_a24f90 {                          // CPhysicsMeshInfo::CMeshData
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
};

template <class T> struct CIVCountedPointer {
    T* m_pObject;
    void Release()
    {
        T* object = m_pObject;
        if (object) {
            if (--object->m_refCount == 0)
                object->DeleteThis();
            m_pObject = 0;
        }
    }
    CIVCountedPointer& operator=(T* object)
    {
        Release();
        m_pObject = object;
        return *this;
    }
};

struct CMeshDataBankEntry {                        // vtable 0x0129CD28
    CMeshDataBank* Bank;                           // +0x04
    unsigned long  Index;                          // +0x08
    virtual void v00();
};

struct CPhysicsMeshInfo : CMeshDataBankEntry {     // vtable 0x0129CD50
    CIVCountedPointer<CMeshData_a24f90> MeshData;  // +0x0c
    virtual void v00();
private:
    virtual void Reset();
};

void CPhysicsMeshInfo::Reset()
{
    MeshData = 0;
}