// C3DMeshFileLoader::RegisterClothSubChunkType(const CChunkedFileChunk& chunkType) -- register a cloth chunk type under both the sub-mesh cloth and the primitive cloth super chunks, dropping the returned prototypes
// Leaf: ?RegisterClothSubChunkType@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9B930, 117 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb `?RegisterClothSubChunkType@C3DMeshFileLoader@@QAEXABVCChunkedFileChunk@@@Z` -- the one void
//   Register*SubChunkType (its siblings return the counted pointer), and the only one that registers under two parents:
//   Ego_r C3DMeshFileLoader has PrimitiveClothChunkType at +0x60 and SubMeshClothChunkType at +0x68, the two members read
//   here. Its only caller 0x00A9C3AC sits in the loader's chunk-type setup 0x00A9BBE0. The catalog's
//   "PhysicsFacesSegment_InitCountedPointers" is a guess.
// Callees: each member chunk's virtual GetAsSuperChunk (slot 6, +0x18), C3DMeshFileLoader::AddChunkType 0x00A9AD00, the
//   count block's destroy hook, operator delete 0x00BFE9BC.
// Shape: two `AddChunkType(chunkType, X->GetAsSuperChunk());` statements, sub-mesh cloth first: the virtual call is made
//   before the arguments are pushed (`push eax ; push ebx ; lea ecx,[esp+..] ; push ecx`), each discarded result is a
//   frame temporary released right after its call (`dec [edi] ; jne`: with AddChunkType's body in the TU -- the same-TU
//   noinline copy below -- the hidden-return slot is not address-exposed, so the block stays in a register).
void __cdecl operator delete(void* block);              // 0x00BFE9BC

class CCPPointerInfo { public:
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
    void Reset(T* data);                                // 0x00A9CF40 (CCountedPointer<CChunkedFileChunk>)
    T* operator->() const { return Data; }
};

class CChunkedFileSuperChunk;
class CChunkedFileLoader;
class CDataInputStream;
class CDataOutputStream;
class CChunkedFileSaver;

class CChunkedFileChunk {                               // vtable slots as in the landed chunk sources
public:
    virtual ~CChunkedFileChunk();                                       // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual long IsUnknownType() const;                                 // slot 4
    virtual void v05();
    virtual CChunkedFileSuperChunk* GetAsSuperChunk();                  // slot 6
    virtual void ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&);   // slot 7
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;            // slot 8
    virtual CChunkedFileChunk* CreateNewInstance() const;               // slot 9
    char                    ChunkType[5];               // +0x04
    CChunkedFileSuperChunk* Parent;                     // +0x0c
};

class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    void RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk);   // 0x00AA87E0
};

class C3DMeshFileRootChunk;

class C3DMeshFileLoader {                               // Ego_r layout (no STL members before +0x70, so the offsets hold)
public:
    virtual ~C3DMeshFileLoader();
    unsigned char _base[0x0c];                          // +0x04 CChunkedFileLoader base
    CCountedPointer<C3DMeshFileRootChunk> RootChunkType;          // +0x10
    CCountedPointer<CChunkedFileChunk> SubMeshChunkType;          // +0x18
    CCountedPointer<CChunkedFileChunk> TransformationChunkType;   // +0x20
    CCountedPointer<CChunkedFileChunk> BoneChunkType;             // +0x28
    CCountedPointer<CChunkedFileChunk> PrimitiveChunkType;        // +0x30
    CCountedPointer<CChunkedFileChunk> TrianglesChunkType;        // +0x38
    CCountedPointer<CChunkedFileChunk> SmoothingGroupsChunkType;  // +0x40
    CCountedPointer<CChunkedFileChunk> VerticesChunkType;         // +0x48
    CCountedPointer<CChunkedFileChunk> UniqueVerticesChunkType;   // +0x50
    CCountedPointer<CChunkedFileChunk> GroupChunkType;            // +0x58
    CCountedPointer<CChunkedFileChunk> PrimitiveClothChunkType;   // +0x60
    CCountedPointer<CChunkedFileChunk> SubMeshClothChunkType;     // +0x68
    CCountedPointer<CChunkedFileChunk> AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent);
    void RegisterClothSubChunkType(const CChunkedFileChunk& chunkType);
};

// Same-TU body of AddChunkType 0x00A9AD00 (retail compiled it in this TU; this is its matched source): with a declaration
// only, VC7.1 treats each hidden-return temporary as address-exposed and re-reads its count block after the decrement.
#pragma auto_inline(off)
__declspec(noinline) CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent)
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk.Reset(chunkType.CreateNewInstance());
    parent->RegisterSubChunkType(chunk);
    return chunk;
}
#pragma auto_inline(on)

void C3DMeshFileLoader::RegisterClothSubChunkType(const CChunkedFileChunk& chunkType)
{
    AddChunkType(chunkType, SubMeshClothChunkType->GetAsSuperChunk());
    AddChunkType(chunkType, PrimitiveClothChunkType->GetAsSuperChunk());
}