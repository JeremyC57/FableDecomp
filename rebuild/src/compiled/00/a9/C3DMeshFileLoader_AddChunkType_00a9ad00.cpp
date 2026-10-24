// C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent) -- make a fresh prototype instance of a chunk type, register it with the parent super chunk and return a counted pointer to it
// Leaf: ?AddChunkType@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AD00, 119 bytes (+ int3 padding), thiscall `ret 0Ch` (hidden result,
//   chunkType, parent; `this` unused), returns the hidden pointer.
// Identity: Ego_r.pdb `?AddChunkType@C3DMeshFileLoader@@AAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@
//   PAVCChunkedFileSuperChunk@@@Z`: the 32 callers are the loader's Register*SubChunkType helpers 0x00A9B840..0x00A9C6CE
//   (`AddChunkType(chunkType, XChunkType->GetAsSuperChunk())`), the virtual slot 9 is CreateNewInstance (vtable 0x0129E4B8
//   slot 9 = CChunkedFileRawDataChunk::CreateNewInstance 0x00AA7850), the Reset is CCountedPointer<CChunkedFileChunk>'s
//   (its deleter 0x00A99FF0) and 0x00AA87E0 is CChunkedFileSuperChunk::RegisterSubChunkType. The catalog's
//   "CCountedPointer_Construct" is descriptive; the landed callers 0x00A9BA10/0x00A9BA40 call it "MakeSegment".
// Callees: chunkType's virtual CreateNewInstance (slot 9, +0x24), CCountedPointer<CChunkedFileChunk>::Reset 0x00A9CF40,
//   CChunkedFileSuperChunk::RegisterSubChunkType 0x00AA87E0, the count block's destroy hook, operator delete 0x00BFE9BC.
// Shape: `CCountedPointer<CChunkedFileChunk> chunk; chunk.Reset(chunkType.CreateNewInstance());
//   parent->RegisterSubChunkType(chunk); return chunk;` -- the default constructor's two zero stores scheduled around the
//   virtual call, no NRVO in VC7.1 (the result is copy-constructed into the hidden slot, sharing the count's null test
//   with the local's destructor), the local released with its block re-read after the decrement (its address went to
//   Reset and RegisterSubChunkType). No EH frame (the TU has no /EHsc).
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

CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent)
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk.Reset(chunkType.CreateNewInstance());
    parent->RegisterSubChunkType(chunk);
    return chunk;
}