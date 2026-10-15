// CPhysicsMeshInfo::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (release the counted mesh data at +0xC, restore the CMeshDataBankEntry vtable), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCPhysicsMeshInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2B740, length 61 bytes.
// Vtables: CPhysicsMeshInfo 0x0129CD50 (slot 0 is this function), base CMeshDataBankEntry 0x0129CD28.
// Callees: the mesh data's virtual self-destroy (slot 1, `call [eax+4]`), operator delete @ 0x00BFE9BC.
// Shape: the body of ~CPhysicsMeshInfo (0x00A24F30) inlined into the `??_G`: that needs the virtual
// destructor AND the counted pointer's destructor `__forceinline`, plus a `new CPhysicsMeshInfo`
// factory so the TU emits the vtable (and with it the deleting destructor). The flag test is
// scheduled before the base vtable store.
struct CMeshDataBank;

struct CMeshData_a2b740 {                          // CPhysicsMeshInfo::CMeshData
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
};

template <class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
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

struct CMeshDataBankEntry {                        // vtable 0x0129CD28
    CMeshDataBank* Bank;                           // +0x04
    unsigned long  Index;                          // +0x08
    CMeshDataBankEntry() : Bank(0), Index(0) { }
    __forceinline virtual ~CMeshDataBankEntry() { }
};

struct CPhysicsMeshInfo : CMeshDataBankEntry {     // vtable 0x0129CD50
    CIVCountedPointer<CMeshData_a2b740> MeshData;  // +0x0c
    CPhysicsMeshInfo() { }
    __forceinline virtual ~CPhysicsMeshInfo() { }
};

// The allocation that pulls CPhysicsMeshInfo's vtable (and its `??_G`) into this translation unit.
CPhysicsMeshInfo* NewPhysicsMeshInfo()
{
    return new CPhysicsMeshInfo;
}