// C3DMeshLODInfo::BuildMesh(bool, bool) -- build the C3DMesh2 from the loaded file bytes (retrying once after a pool defragment), upload every primitive's buffers by entry type, then register the LOD with the bank and record its size
// Grade: DIFFER(849v847) -- register allocation: retail spills the primitive index to the dead flag slot and reuses esi for Materials; ours spills ok there and keeps i in edi (bank/index/&Mesh esi-edi flip at the top); advisor c1: ok declared first, ok = true just before the loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24520, length 847 bytes (`ret 8`; the first flag is unused).
// Callees: CBankFile::GetEntryDataType 0x009CC210 / GetEntrySymbol 0x009CC1D0, CBankFileAsyncData::GetData 0x009D5530,
//   CMemoryDataInputStream ctor 0x00994930 / OpenMemorySource 0x00994570 / dtor 0x00993B10, CResourceList::MakeFreeSpace
//   0x009FC4F0, C3DMesh2 ctor 0x00A89190 / LoadBinary 0x00A8AD40 / CalculateTextureChangeFlags 0x00A89380 / LockPrimitive
//   0x00A89250 / UnlockPrimitive 0x00A892A0 / Sizeof 0x00A893C0, CCountedPointer<C3DMesh2>::Reset 0x00A299C0 (manifest
//   label propagated from CIPhysicsFacesSegmentBase), CHandleBasedMemoryPool::Defragment 0x00A02730,
//   C3DMeshStats::InitialiseMeshGenerators 0x00A2EC40, C3DPrimitive2 upload 0x00A92870 / 0x00A932C0 / 0x00A93320,
//   CResourceBank::AddResource 0x009FC380, CBankFileAsyncData::Sizeof 0x009D5620, CResource::SetResourceSize 0x009FBF20.
// Layout: retail CMeshDataBank is 0x74 below the PDB view from the resource banks on (MeshResourceBank +0x170,
//   MeshStatsResourceBank +0x1ec, ClothMemoryPool +0x368, MeshDataMemoryPool +0x370; +0x3d4 load flags, +0x3d8 bool).
// Leaf: BuildMesh
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template <class T> struct CCountedPointer {
    T* Object;
    SCountBlock* Count;
    void Reset(T* object);                             // 0x00A299C0
    void Release()
    {
        if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
        Count = 0;
        Object = 0;
    }
    T* operator->() const { return Object; }
};
struct CDataInputStream {
    virtual void v00();
    unsigned long Position;                            // +4
    unsigned long Length;                              // +8
    const void* Data;                                  // +0xc
    unsigned long m_10;                                // +0x10
    unsigned long m_14;                                // +0x14
};
struct CMemoryDataInputStream : CDataInputStream {
    CMemoryDataInputStream(const void* data, unsigned long size);   // 0x00994930
    ~CMemoryDataInputStream();                     // 0x00993B10
    void OpenMemorySource(const void* data, unsigned long size);    // 0x00994570
    const void* Source;                            // +0x18
    unsigned long GetLength() const { return Length; }
};
struct CBankFileAsyncData {
    unsigned char _pad00[0x10];
    unsigned long Size;                            // +0x10
    const void* GetData();                         // 0x009D5530
    unsigned long Sizeof() const;                  // 0x009D5620
};
struct CMovableResourceMemoryPool { void Defragment(); };   // 0x00A02730 CHandleBasedMemoryPool::Defragment
struct CResource;
struct CResourceList { void MakeFreeSpace(unsigned long bytes); };   // 0x009FC4F0
struct CResourceBank {
    long m_00;
    CResourceList List;                            // +4
    unsigned char _pad08[0x74];
    void AddResource(CResource* resource);         // 0x009FC380
};
struct CMeshDataBank {
    unsigned char _pad000[0x170];
    CResourceBank MeshResourceBank;                // +0x170
    CResourceBank MeshStatsResourceBank;           // +0x1ec
    unsigned char _pad268[0x100];
    CMovableResourceMemoryPool* ClothMemoryPool;   // +0x368
    long ClothMemoryPoolInfo;                      // +0x36c
    CMovableResourceMemoryPool* MeshDataMemoryPool;   // +0x370
    unsigned char _pad374[0x60];
    unsigned long MeshLoadFlags;                   // +0x3d4
    bool UseSoftwareVertices;                      // +0x3d8
    unsigned long GetEntryDataType(unsigned long index);        // 0x009CC210
    CCharString GetEntrySymbol(unsigned long index);            // 0x009CC1D0
};
struct C3DMeshMaterial { unsigned char _pad00[0x20]; unsigned char Flags; unsigned char _pad21[0xf]; };   // 0x30 bytes
struct C3DPrimitive2 {
    long m_00;
    unsigned long MaterialIndex;                   // +4
    bool UploadAnimatedBuffers(unsigned long flags);                  // 0x00A92870
    bool UploadRepeatedBuffers(unsigned long flags, unsigned long n); // 0x00A932C0
    bool UploadStaticBuffers(unsigned long flags);                    // 0x00A93320
};
struct C3DMeshStats {
    void InitialiseMeshGenerators(CMeshDataBank* bank, const CCharString& symbol);   // 0x00A2EC40
};
struct C3DMesh2 : C3DMeshStats {
    unsigned char _pad00[0x8c];
    C3DMeshMaterial* Materials;                    // +0x8c
    unsigned long NumPrimitives;                   // +0x90
    unsigned char _pad94[0x54];
    unsigned long MeshID;                          // +0xe8
    C3DMesh2();                                    // 0x00A89190
    bool LoadBinary(CMemoryDataInputStream& stream, CMovableResourceMemoryPool* pool, unsigned long flags, bool flag);   // 0x00A8AD40
    void CalculateTextureChangeFlags();            // 0x00A89380
    C3DPrimitive2* LockPrimitive(unsigned long index);   // 0x00A89250
    void UnlockPrimitive(unsigned long index);     // 0x00A892A0
    unsigned long Sizeof() const;                  // 0x00A893C0
};
extern unsigned long g_NextMeshID;                         // 0x01397248
extern CMovableResourceMemoryPool* g_MeshDataMemoryPool;   // 0x013D2D6C
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;   // 0x013D2D70

struct CResource {
    virtual void v00();
    long RefCount;                                 // +0x04
    void* ResourceList;                            // +0x08
    CResource* PrevResource;                       // +0x0c
    CResource* NextResource;                       // +0x10
    unsigned long ResourceSize;                    // +0x14
    unsigned long LastUsedFrame;                   // +0x18
    void SetResourceSize(unsigned long size);      // 0x009FBF20
};
struct C3DMeshInfo {
    void* __vftable;
    CMeshDataBank* Bank;                           // +4
    unsigned long Index;                           // +8
};
struct C3DMeshLODInfo : CResource {
    CCountedPointer<C3DMesh2>           Mesh;            // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;        // +0x24
    C3DMeshInfo*                        MeshInfo;        // +0x2c
    unsigned char                       LOD;             // +0x30
    bool BuildMesh(bool unusedFlag, bool flag);
};

bool C3DMeshLODInfo::BuildMesh(bool unusedFlag, bool flag)
{
    bool ok;
    CMeshDataBank* bank = MeshInfo->Bank;
    unsigned long index = MeshInfo->Index;
    unsigned long type = bank->GetEntryDataType(index) & 0xf;
    bool animated = type == 5;
    bool repeated = type == 2;
    bool type4 = type == 4;
    CMemoryDataInputStream stream(FileData->GetData(), FileData->Size);
    unsigned long id = g_NextMeshID++;
    CMovableResourceMemoryPool* oldMeshPool = g_MeshDataMemoryPool;
    g_MeshDataMemoryPool = bank->MeshDataMemoryPool;
    CMovableResourceMemoryPool* oldClothPool = g_ParticleMemoryPool;
    g_ParticleMemoryPool = bank->ClothMemoryPool;
    if (LOD == 4)
        MeshInfo->Bank->MeshStatsResourceBank.List.MakeFreeSpace(stream.GetLength());
    unsigned long flags = bank->MeshLoadFlags;
    CMovableResourceMemoryPool* pool = bank->MeshDataMemoryPool;
    CCharString symbol = bank->GetEntrySymbol(index);
    Mesh.Reset(new C3DMesh2);
    if (!Mesh->LoadBinary(stream, pool, flags, flag)) {
        Mesh.Reset(new C3DMesh2);
        pool->Defragment();
        stream.OpenMemorySource(FileData->GetData(), FileData->Size);
        if (!Mesh->LoadBinary(stream, pool, flags, false)) {
            Mesh.Release();
            return false;
        }
    }
    Mesh->MeshID = id;
    if (flags)
        Mesh->CalculateTextureChangeFlags();
    Mesh->InitialiseMeshGenerators(bank, symbol);
    ok = true;
    for (unsigned long i = 0; i < Mesh->NumPrimitives && ok; ++i) {
        C3DPrimitive2* prim = Mesh->LockPrimitive(i);
        unsigned long uploadFlags = 0;
        if ((Mesh->Materials[prim->MaterialIndex].Flags & 2) && MeshInfo->Bank->UseSoftwareVertices)
            uploadFlags = 2;
        bool result;
        if (animated)
            result = prim->UploadAnimatedBuffers(uploadFlags | 8);
        else if (repeated)
            result = prim->UploadRepeatedBuffers(uploadFlags, 0x20);
        else if (type4)
            result = prim->UploadRepeatedBuffers(uploadFlags, 0x20);
        else
            result = prim->UploadStaticBuffers(uploadFlags | 8);
        ok &= result;
        Mesh->UnlockPrimitive(i);
    }
    g_MeshDataMemoryPool = oldMeshPool;
    g_ParticleMemoryPool = oldClothPool;
    if (!ok) {
        Mesh.Release();
        return false;
    }
    if (LOD == 4)
        MeshInfo->Bank->MeshStatsResourceBank.AddResource(this);
    else
        MeshInfo->Bank->MeshResourceBank.AddResource(this);
    FileData.Release();
    unsigned long meshSize = Mesh.Object ? Mesh->Sizeof() + 8 : 8;
    unsigned long dataSize = FileData.Object ? FileData->Sizeof() + 8 : 8;
    SetResourceSize(dataSize + meshSize + 0x34);
    return true;
}
