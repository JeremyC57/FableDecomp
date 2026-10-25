// C3DMeshFileLoader::RegisterHelpersSubChunkType(const CChunkedFileChunk& chunkType) -- register a chunk type under the Helpers
// super chunk: AddChunkType(chunkType, HelpersChunkType->GetAsSuperChunk()), returning the registered prototype
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9BA40, length 40 bytes, thiscall `ret 8` (the counted pointer through the
//   hidden pointer).
// Name: Ego_r.pdb 0x00C0CC90 / FableWin.pdb 0x033D4F00 ?RegisterHelpersSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@
//   ABVCChunkedFileChunk@@@Z; the PDB layout puts HelpersChunkType at +0x98 (the member read here: retail and PDB agree, the
//   loader holds only 8-byte counted pointers). Sibling of RegisterMapSubChunkType 0x00A9BA10. Was catalogued `C3DMeshFileLoader::CreatePhysicsFacesSegmentFromPhysicsChunks`
//   (a "MakeSegment" model; manifest "CGameCameraManager::HasCameraMode" is propagated).
// Callees: the member chunk's virtual GetAsSuperChunk (slot 6, +0x18; ?GetAsSuperChunk@CChunkedFileChunk@@UAEPAVCChunkedFile
//   SuperChunk@@XZ) and the private C3DMeshFileLoader::AddChunkType @ 0x00A9AD00 (?AddChunkType@C3DMeshFileLoader@@AAE...).
// Shape: the result is forwarded (NRVO, `mov eax,edi ; ret 8`): the virtual call is evaluated first (rightmost argument),
//   then `push eax ; push chunkType ; push hidden`.
// Model: local classes (agent423's C3DMeshFileLoader / CChunkedFileChunk models; no engine header for the loader).
// Leaf: ?RegisterHelpersSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z
void __cdecl operator delete(void* block);              // 0x00BFE9BC

struct CCPPointerInfo {
    long  RefCount;                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};

class CChunkedFileSuperChunk;
class C3DMeshFileRootChunk;

class CChunkedFileChunk {                               // vtable slots as in the landed chunk sources
public:
    virtual ~CChunkedFileChunk();                       // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual long IsUnknownType() const;                 // slot 4
    virtual void v05();
    virtual CChunkedFileSuperChunk* GetAsSuperChunk();  // slot 6
};

class C3DMeshFileLoader {                               // 0xC0 bytes (Ego_r.pdb)
public:
    unsigned char                         _pad00[0x10]; // +0x00 (vptr / base)
    CCountedPointer<C3DMeshFileRootChunk> RootChunkType;   // +0x10
    CCountedPointer<CChunkedFileChunk>    SubMeshChunkType;            // +0x18
    CCountedPointer<CChunkedFileChunk>    TransformationChunkType;     // +0x20
    CCountedPointer<CChunkedFileChunk>    BoneChunkType;               // +0x28
    CCountedPointer<CChunkedFileChunk>    PrimitiveChunkType;          // +0x30
    CCountedPointer<CChunkedFileChunk>    TrianglesChunkType;          // +0x38
    CCountedPointer<CChunkedFileChunk>    SmoothingGroupsChunkType;    // +0x40
    CCountedPointer<CChunkedFileChunk>    VerticesChunkType;           // +0x48
    CCountedPointer<CChunkedFileChunk>    UniqueVerticesChunkType;     // +0x50
    CCountedPointer<CChunkedFileChunk>    GroupChunkType;              // +0x58
    CCountedPointer<CChunkedFileChunk>    PrimitiveClothChunkType;     // +0x60
    CCountedPointer<CChunkedFileChunk>    SubMeshClothChunkType;       // +0x68
    CCountedPointer<CChunkedFileChunk>    MaterialListChunkType;       // +0x70
    CCountedPointer<CChunkedFileChunk>    MaterialChunkType;           // +0x78
    CCountedPointer<CChunkedFileChunk>    MaterialExtChunkType;        // +0x80
    CCountedPointer<CChunkedFileChunk>    MapChunkType;                // +0x88
    CCountedPointer<CChunkedFileChunk>    EngineMaterialChunkType;     // +0x90
    CCountedPointer<CChunkedFileChunk>    HelpersChunkType;            // +0x98
    CCountedPointer<CChunkedFileChunk>    HelperPointChunkType;        // +0xA0
    CCountedPointer<CChunkedFileChunk>    DummyObjectChunkType;        // +0xA8
    CCountedPointer<CChunkedFileChunk>    ConvexVolumeChunkType;       // +0xB0
    CCountedPointer<CChunkedFileChunk>    MirroredTextureChunkType;    // +0xB8
    CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk& chunkType);
private:
    CCountedPointer<CChunkedFileChunk> AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent);   // 0x00A9AD00
};

CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterHelpersSubChunkType(const CChunkedFileChunk& chunkType)
{
    return AddChunkType(chunkType, HelpersChunkType->GetAsSuperChunk());
}
