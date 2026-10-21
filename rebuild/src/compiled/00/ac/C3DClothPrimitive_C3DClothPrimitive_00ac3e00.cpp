// C3DClothPrimitive::C3DClothPrimitive(const C3DMeshFileClothChunk* chunk) -- build the cloth's particle program from the chunk: read the program from the chunk's data, take the material index, then add one C3DGroup2 per VGRP sub-chunk
// Leaf: ??0C3DClothPrimitive
// Retail @ 0x00AC3E00, 441 bytes (`ret 4`; 8-byte aligned frame, no EH frame under the lane's /O2 flags). Callees: CBase
//   0x0099A2F0, operator new 0x00BFEA1A, CParticleProgram ctor 0x00AE3550 / ReadFromFile 0x00AE3C30 / AddGroup 0x00AE37D0,
//   the group reserve 0x00AE3330 (manifest "CSpellContainerList::AddChild" is propagated), CCountedPointer::Reset 0x00AC3FF0,
//   CountSubChunksOfType 0x00AA7EA0, the cloth chunk's FindFirst/FindNext group finders 0x00AC6490 / 0x00AC6520 (manifest
//   "C3DMeshFileSubMeshChunk::FindFirstBone/FindNextBone" is propagated), C3DGroup2 ctor 0x00AC4150, free 0x00BFEA14.
// Shape: the VGRP count is a named local (the program pointer is fetched after the count call); the data pointer is a
//   named local as in ReadCompressedFileData 0x00A4DF10; the group local's vector frees its buffer after the step.
// Layout: C3DClothPrimitive ParticleProgram CCountedPointer +4, MaterialIndex +0x10 (landed SaveBinary 0x00AC3B00);
//   C3DMeshFileClothChunk MaterialIndex +0x30, program data (STLport vector) +0x38; C3DGroup2 BoneIndex +0 / VertexBlends +4
//   (landed 0x00AC4150); retail CMemoryDataInputStream 0x1c.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> struct CCountedPointer {
    T* Object; SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    ~CCountedPointer() { if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } } Object = 0; Count = 0; }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        SCountBlock* c = o.Count; T* p = o.Object;
        if (Count != c) {
            if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
            Object = p; Count = c; if (c) ++c->references;
        }
        return *this;
    }
    void Reset(T* object);                         // 0x00AC3FF0
    T* operator->() const { return Object; }
};

template <class T> struct CArray {                 // retail STLport vector layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CArray() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    const void* GetData() const { return size() ? (const void*)_M_start : (const void*)this; }
};

struct CDataInputStream { virtual ~CDataInputStream(); };
struct CMemoryDataInputStream : CDataInputStream {
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                       // 0x00993B10
    long position, sourceLength, chunkBase, chunkPosition, chunkLength;
    void* StreamSource;
};

struct CMovableResourceMemoryPool;
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;   // 0x013D2D70

struct C3DVertexBlend2 { unsigned long VertexIndex; float BlendValue; };
struct C3DMeshFileGroupChunk;
struct C3DGroup2 {
    long BoneIndex;                                // +0x00
    CArray<C3DVertexBlend2> VertexBlends;          // +0x04
    C3DGroup2(const C3DMeshFileGroupChunk& chunk); // 0x00AC4150
};

struct CParticleProgram {
    CParticleProgram();                                            // 0x00AE3550
    bool ReadFromFile(CDataInputStream* in, CMovableResourceMemoryPool* pool);   // 0x00AE3C30
    void ReserveGroups(unsigned long count);                       // 0x00AE3330
    void AddGroup(const C3DGroup2& group);                         // 0x00AE37D0
    unsigned char _body[0x140];
};

struct C3DMeshFileClothChunk {
    unsigned char _00[0x30];
    unsigned long MaterialIndex;                   // +0x30
    unsigned long _34;
    CArray<unsigned char> ProgramData;             // +0x38
    unsigned long CountSubChunksOfType(const char* type) const;    // 0x00AA7EA0
    CCountedPointer<const C3DMeshFileGroupChunk> FindFirstGroup() const;   // 0x00AC6490
    CCountedPointer<const C3DMeshFileGroupChunk> FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& current) const;   // 0x00AC6520
};

struct CBase {
    CBase();                                       // 0x0099A2F0
    virtual ~CBase();
};

class C3DClothPrimitive : public CBase {
public:
    CCountedPointer<CParticleProgram> ParticleProgram;   // +0x04
    unsigned long PrimitiveIndex;                        // +0x0c
    unsigned long MaterialIndex;                         // +0x10
    C3DClothPrimitive(const C3DMeshFileClothChunk* chunk);
};

C3DClothPrimitive::C3DClothPrimitive(const C3DMeshFileClothChunk* chunk)
{
    ParticleProgram.Reset(new CParticleProgram);
    const void* data = chunk->ProgramData.GetData();
    CMemoryDataInputStream stream(data, chunk->ProgramData.size());
    ParticleProgram->ReadFromFile(&stream, g_ParticleMemoryPool);
    MaterialIndex = chunk->MaterialIndex;
    unsigned long groupCount = chunk->CountSubChunksOfType("VGRP");
    ParticleProgram->ReserveGroups(groupCount);
    CCountedPointer<const C3DMeshFileGroupChunk> group = chunk->FindFirstGroup();
    while (group.Object) {
        C3DGroup2 vertexGroup(*group.Object);
        ParticleProgram->AddGroup(vertexGroup);
        group = chunk->FindNextGroup(group);
    }
}