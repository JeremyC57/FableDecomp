// CPhysicsMeshInfo::GetMesh() -- make sure the physics mesh is loaded and built (blocking with an "Asset Wait" progress line when the file is still streaming), touch its data in the resource list and return a counted pointer to the mesh
// Leaf: GetMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A262B0, 191 bytes (+ int3 padding), `ret 4` (hidden result).
// Callees: CPhysicsMeshInfo::BeginLoading 0x00A25BD0, CBankFileAsyncData::IsFinished 0x009D54E0 / WaitUntilFinished
//   0x009D5B60, CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, DisplayProgress 0x009E9F40
//   (`__fastcall` text, bool in dl, then -1.0f and false on the stack), CPhysicsMeshInfo::CMeshData::BuildMesh 0x00A25D20,
//   CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90.
// Shape: the sibling of the landed NParticleEngine::CParticleEmitterInfo::GetEmitterTemplate 0x00AF1480 and
//   CLipSyncDataBank::GetLipSyncData 0x00A42FC0. MeshData (Ego_r.pdb: CIVCountedPointer<CMeshData> +0xC) is re-read
//   through `this` (edi) every time; CMeshData is a CResource (list +8, LastUsedFrame +0x18) with the file
//   CCountedPointer<CBankFileAsyncData> at +0x1C and the intrusive mesh pointer at +0x24. The progress text is an
//   implicit `const char*` -> CCharString conversion, so its temporary is built before the `false`/-1.0f pushes (an
//   explicit CCharString("...") is built after them); the touch block is the inline
//   resource-list move-to-tail; the result is the add-ref copy (`inc [ecx+4]`) straight into the hidden result.
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

struct CResourceList;
struct CResource {
    virtual void v00();
    unsigned long m_pad4;                  // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void Touch();
};
struct CResourceList {
    long m_pad0;                           // +0
    CResource Head;                        // +4
    unsigned long ResourceCount;           // +0x20
    long AllocatedMemory;                  // +0x24
    long MaximumMemory;                    // +0x28
    unsigned long CurrentFrame;            // +0x2c
};
inline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

struct CBankFileAsyncData {
    bool IsFinished();                     // 0x009D54E0
    bool WaitUntilFinished();              // 0x009D5B60
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

struct CIVCountedObject {                  // intrusive reference count at +4
    virtual void v00();
    mutable long RefCount;
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    T* operator->() const { return Object; }
};

class CPhysicsMesh : public CIVCountedObject {};

class CPhysicsMeshInfo {
public:
    struct CMeshData : CResource {
        CCountedPointer<CBankFileAsyncData>     FileData;   // +0x1C
        CIVCountedPointer<const CPhysicsMesh>   Mesh;       // +0x24
        void BuildMesh();                                   // 0x00A25D20
    };
    void*                         __vftable;               // +0x00
    long                          Bank;                    // +0x04
    long                          Index;                    // +0x08
    CMeshData*                    MeshData;                // +0x0C CIVCountedPointer<CMeshData>::Object
    void BeginLoading();                                   // 0x00A25BD0
    CIVCountedPointer<const CPhysicsMesh> GetMesh();
};

CIVCountedPointer<const CPhysicsMesh> CPhysicsMeshInfo::GetMesh()
{
    if (MeshData == 0 || MeshData->Mesh.Object == 0) {
        if (MeshData == 0 || MeshData->FileData.Data == 0)
            BeginLoading();
        if (!MeshData->FileData->IsFinished()) {
            DisplayProgress("Asset Wait: Physics Mesh", false, -1.0f, false);
            MeshData->FileData->WaitUntilFinished();
        }
        MeshData->BuildMesh();
    }
    MeshData->Touch();
    return MeshData->Mesh;
}