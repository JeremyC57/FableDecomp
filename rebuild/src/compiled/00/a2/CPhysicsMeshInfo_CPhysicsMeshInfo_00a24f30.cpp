// CPhysicsMeshInfo::~CPhysicsMeshInfo() -- release the counted mesh data at +0xC (count at +4, destroyed through its virtual slot 1) and null it, then the inline CMeshDataBankEntry destructor
// Leaf: ??1CPhysicsMeshInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24F30, length 41 bytes.
// Vtables: CPhysicsMeshInfo 0x0129CD50, base CMeshDataBankEntry 0x0129CD28 (rtti-vtables.tsv).
// Callees: the mesh data's virtual self-destroy (slot 1, `call [eax+4]`).
// Shape: the PDB layout (CMeshDataBankEntry: vptr, Bank +4, Index +8; MeshData
// CIVCountedPointer<CPhysicsMeshInfo::CMeshData> +0xC) modelled locally (polymorphic classes; the
// generated headers flatten the vptr). The own vtable store comes FIRST, before the member load:
// the release is the counted pointer's own inline destructor (a member object), which keeps the
// pointee in a local (`dec [ecx+4]` then `mov eax,[ecx]` without a reload) and clears the member.
struct CMeshDataBank;

struct CMeshData_a24f30 {                          // CPhysicsMeshInfo::CMeshData
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
};

template <class T> struct CIVCountedPointer {
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

struct CMeshDataBankEntry {                        // vtable 0x0129CD28
    CMeshDataBank* Bank;                           // +0x04
    unsigned long  Index;                          // +0x08
    virtual void v00();
    ~CMeshDataBankEntry() { }
};

struct CPhysicsMeshInfo : CMeshDataBankEntry {     // vtable 0x0129CD50
    CIVCountedPointer<CMeshData_a24f30> MeshData;  // +0x0c
    virtual void v00();
    ~CPhysicsMeshInfo();
};

CPhysicsMeshInfo::~CPhysicsMeshInfo()
{
}