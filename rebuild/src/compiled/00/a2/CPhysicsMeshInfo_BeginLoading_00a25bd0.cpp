// CPhysicsMeshInfo::BeginLoading() -- if the physics mesh data is not loaded yet, create it, open the bank entry, register the data with the bank's resource list, start an asynchronous read of the whole entry and record the resource size
// Leaf: BeginLoading
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A25BD0, length 321 bytes (+ int3 padding), plain `ret`.
// Callees: operator new 0x00BFEA1A (0x2C bytes), CResource::CResource 0x009FBEC0 (CMeshData vtable 0x0129CD78),
// CBankFileAsync::CreateEntry(ulong) 0x009D58D0, CBankFile::GetEntryDataSize(ulong) 0x009CC240, CResourceBank::
// AddResource 0x009FC3D0, CBankFileAsyncEntry::BeginReadDataAutoAlloc 0x009D6100, the counted pointer operator=
// 0x009A4440, CPhysicsMesh::Sizeof 0x00A6C900, CBankFileAsyncData::Sizeof 0x009D5620, CResource::SetResourceSize
// 0x009FBF20, operator delete 0x00BFE9BC; the read priority global 0x0139723C. Same family as the landed
// NParticleEngine::CEmitterData::BeginLoading 0x00AF1360.
// Layout: CPhysicsMeshInfo = CMeshDataBankEntry {vptr, Bank +4, Index +8} + MeshData (CIVCountedPointer<CMeshData>)
// +0xC (landed Reset 0x00A24F90); CMeshData = CResource (0x1C) + FileData (counted pointer) +0x1C + Mesh +0x24 +
// Owner +0x28 (landed CMeshData ctor 0x00A2AA80 shape); the entry's bank +4 / index +8; the bank's resource list +0x2E4.
// Shape: the landed CEmitterData::BeginLoading form; the new data goes in through the inline CIVCountedPointer
// operator=(T*) (landed Reset), the CMeshData ctor's member objects zero +0x1C..+0x24 after the vtable store, and the
// two size ternaries read the data through one named pointer (edi) while SetResourceSize re-reads MeshData.
// Model note: engine/CResource.h is data-only (vptr as a placeholder word), so CResourceBase restates its layout with
// the virtual interface the vtable store and the slot-1 release need.
#include "engine/CResource.h"
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T* Object;                                     // +0
    SCountBlock* Info;                             // +4
    CCountedPointer() : Object(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x009A4440
    T* operator->() const { return Object; }
};
enum EThreadedFilePriority { };
extern EThreadedFilePriority g_PhysicsMeshReadPriority;   // 0x0139723C
class CBankFileAsyncData { public:
    unsigned long Sizeof() const;                  // 0x009D5620
};
class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long index);   // 0x009CC240
};
class CBankFileAsyncEntry { public:
    void*          __vftable;
    CBankFile*     Bank;                           // +4
    unsigned long  Index;                          // +8
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
class CPhysicsMesh { public: unsigned long Sizeof() const; };   // 0x00A6C900

struct CResourceBase {                             // CResource with its virtual interface (engine/CResource.h layout)
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long          RefCount;                        // +0x04 (the engine header's +0x04 word)
    unsigned char _body[0x1c - 8];
    CResourceBase();                               // 0x009FBEC0
    void SetResourceSize(unsigned long size);      // 0x009FBF20
};
class CResourceBank { public:
    void AddResource(CResourceBase* resource);     // 0x009FC3D0
};
class CMeshDataBank { public:
    unsigned char _pad000[0x2e4];
    CResourceBank ResourceBank;                    // +0x2e4
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long index);   // 0x009D58D0
};

class CPhysicsMeshInfo;
struct CMeshPointer {                              // CIVCountedPointer<const CPhysicsMesh>, nulled by its ctor
    CPhysicsMesh* m_pObject;
    CMeshPointer() : m_pObject(0) {}
};
struct CMeshData : CResourceBase {                 // CPhysicsMeshInfo::CMeshData, vtable 0x0129CD78
    CCountedPointer<CBankFileAsyncData> FileData;  // +0x1c
    CMeshPointer      Mesh;                        // +0x24
    CPhysicsMeshInfo* Owner;                       // +0x28
    virtual void v00();
    CMeshData(CPhysicsMeshInfo* owner) : FileData(), Mesh() { Owner = owner; }
};

template <class T> class CIVCountedPointer { public:
    T* m_pObject;
    void Release()
    {
        T* object = m_pObject;
        if (object) {
            if (--object->RefCount == 0)
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
    T* operator->() const { return m_pObject; }
};

class CPhysicsMeshInfo { public:                          // CMeshDataBankEntry + MeshData
    virtual void v00();
    CMeshDataBank*               Bank;             // +0x04
    unsigned long                Index;            // +0x08
    CIVCountedPointer<CMeshData> MeshData;         // +0x0c
    void BeginLoading();
};

void CPhysicsMeshInfo::BeginLoading()
{
    if (MeshData.m_pObject)
        return;
    MeshData = new CMeshData(this);
    CCountedPointer<CBankFileAsyncEntry> entry = Bank->CreateEntry(Index);
    unsigned long size = entry->Bank->GetEntryDataSize(entry->Index);
    Bank->ResourceBank.AddResource(MeshData.m_pObject);
    MeshData->FileData = entry->BeginReadDataAutoAlloc(0, size, g_PhysicsMeshReadPriority);
    CMeshData* data = MeshData.m_pObject;
    unsigned long meshSize = data->Mesh.m_pObject ? data->Mesh.m_pObject->Sizeof() + 4 : 4;
    unsigned long dataSize = data->FileData.Object ? data->FileData->Sizeof() + 8 : 8;
    MeshData->SetResourceSize(dataSize + meshSize + 0x2c);
}