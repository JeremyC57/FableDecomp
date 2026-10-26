// C3DMeshFileLoader::RegisterPrimitiveSubChunkType(const CChunkedFileChunk& chunkType) -- register a chunk type under the Primitive super chunk: AddChunkType(chunkType, PrimitiveChunkType->GetAsSuperChunk()), returning the registered prototype
// Leaf: ?RegisterPrimitiveSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9B8A0, 40 bytes (48 with int3 padding), thiscall `ret 8` (the counted pointer
//   through the hidden pointer).
// Identity (proven): C3DMeshFileLoader::RegisterPrimitiveSubChunkType; evidence: the body reads the member at +0x30, which
//   is Ego_r.pdb C3DMeshFileLoader::PrimitiveChunkType (retail and PDB layouts agree: the loader holds only 8-byte counted pointers,
//   as the landed sibling RegisterHelpersSubChunkType 0x00A9BA40 pins at +0x98), and Ego_r.pdb has
//   ?RegisterPrimitiveSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z (Ego_r 0x00C0C9E0); the retail run 0x00A9B840 .. 0x00A9BA40 follows the Ego_r order Root, SubMesh,
//   Primitive, Triangles, Group, (Cloth, 0x80 bytes), MaterialList, Material, Map, Helpers.
// Label: the manifest's "HasCameraMode" is propagated across family H032.
// Callees: the member chunk's virtual GetAsSuperChunk (slot 6, +0x18; ?GetAsSuperChunk@CChunkedFileChunk@@UAEPAVCChunkedFile
//   SuperChunk@@XZ) and the private C3DMeshFileLoader::AddChunkType 0x00A9AD00.
// Shape: the landed RegisterHelpersSubChunkType 0x00A9BA40 model: the result is forwarded (NRVO, `mov eax,edi ; ret 8`), the
//   virtual call is evaluated first (rightmost argument).
// Model: local classes (no engine header for the loader) -- Review at landing.
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
class CChunkedFileSuperChunk : public CChunkedFileChunk { };   // Ego_r.pdb, 52 bytes
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { };  // Ego_r.pdb: same 52 bytes, no members of its own

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
    CCountedPointer<CChunkedFileChunk> RegisterPrimitiveSubChunkType(const CChunkedFileChunk& chunkType);
private:
    CCountedPointer<CChunkedFileChunk> AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent);   // 0x00A9AD00
};

CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterPrimitiveSubChunkType(const CChunkedFileChunk& chunkType)
{
    return AddChunkType(chunkType, PrimitiveChunkType->GetAsSuperChunk());
}