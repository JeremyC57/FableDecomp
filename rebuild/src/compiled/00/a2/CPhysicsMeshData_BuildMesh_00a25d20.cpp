// CPhysicsMeshData::BuildMesh() -- build the physics mesh from the loaded file data and register the resource
// Shape: the pool for the loader call is a named local read before the size and loader locals (retail loads pool, FileData, size, then the Owner->Manager->MeshFileLoader chain in ebx); the CPhysicsMesh ctor re-reads it inside the new-expression (`pool = manager->MemoryPool`) so it lands in the dying manager register edi
void operator delete(void* p);                     // 0x00BFE9BC
class CCPPointerInfo { public:                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    unsigned long Sizeof() const { return Data ? Data->DoSizeof() + 8 : 8; }
    void Reset()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
template <class T> class CIVCountedPointer { public:      // intrusive counted pointer
    T* Data;
    void Clear() { if (Data) { Data->Release(); Data = 0; } }
    CIVCountedPointer& operator=(T* p) { Clear(); Data = p; return *this; }
    long Sizeof() const { return Data ? Data->Sizeof() + 4 : 4; }
};
class CBankFileAsyncData { public:
    unsigned char _pad00[0x10];
    unsigned long Size;                            // +0x10
    void* GetData();                               // 0x009D5530
    unsigned long DoSizeof();                      // 0x009D5620
};
class C3DMeshFile;
class CMovableResourceMemoryPool;
class CPhysicsMesh { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    unsigned char _pad08[0xc0 - 0x08];
    CPhysicsMesh(C3DMeshFile* file, CMovableResourceMemoryPool* pool);   // 0x00A6DA40
    long Sizeof();                                 // 0x00A6C900
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct C3DMeshFileLoader {
    CCountedPointer<C3DMeshFile> LoadMeshFileCompressed(void* data, unsigned long size, CMovableResourceMemoryPool* pool);   // 0x00A9B740
};
class CResource;
class CResourceList { public:
    bool MakeFreeSpace(unsigned long size);        // 0x009FC4F0
    void AddResource(CResource* resource);         // 0x009FBFE0
};
struct CPhysicsMeshManager {
    unsigned char               _pad000[0x164];
    C3DMeshFileLoader*          MeshFileLoader;    // +0x164
    unsigned char               _pad168[0x2e8 - 0x168];
    CResourceList               ResourceList;      // +0x2e8
    unsigned char               _pad2e9[0x370 - 0x2e9];
    CMovableResourceMemoryPool* MemoryPool;        // +0x370
};
class CPhysicsMeshInfo { public:
    void*                __vftable;
    CPhysicsMeshManager* Manager;                  // +0x04
};
class CResource { public:                                 // 0x1c bytes
    void* __vftable;
    unsigned char _pad04[0x18];
    void SetResourceSize(unsigned long size);      // 0x009FBF20
    void SetBuilt();                               // 0x009FC440 (labelled CTCActivationTrigger::OnKill)
};
struct CPhysicsMeshData : CResource {
    CCountedPointer<CBankFileAsyncData> FileData;  // +0x1c
    CIVCountedPointer<CPhysicsMesh>     Mesh;      // +0x24
    CPhysicsMeshInfo*                   Owner;     // +0x28
    void BuildMesh();
};

void CPhysicsMeshData::BuildMesh()
{
    CPhysicsMeshManager* manager = Owner->Manager;
    CResourceList& resources = manager->ResourceList;
    resources.MakeFreeSpace(FileData->Size);
    CMovableResourceMemoryPool* pool = manager->MemoryPool;
    unsigned long size = FileData->Size;
    C3DMeshFileLoader* loader = Owner->Manager->MeshFileLoader;
    CCountedPointer<C3DMeshFile> file = loader->LoadMeshFileCompressed(FileData->GetData(), size, pool);
    Mesh = new CPhysicsMesh(file.Data, pool = manager->MemoryPool);
    FileData.Reset();
    SetResourceSize(0x2c + Mesh.Sizeof() + FileData.Sizeof());
    SetBuilt();
    resources.AddResource(this);
    resources.MakeFreeSpace(0);
}