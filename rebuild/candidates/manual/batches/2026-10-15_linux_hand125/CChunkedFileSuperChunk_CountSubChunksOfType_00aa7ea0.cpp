// CChunkedFileSuperChunk::CountSubChunksOfType(const char*) const -- counts the known (not unknown-type) sub-chunks whose four-character code equals the given type
// Leaf: CountSubChunksOfType
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7EA0, 145 bytes of code (`ret 4`).
// Callees: each chunk's virtual IsUnknownType (slot 4), operator delete @ 0x00BFE9BC, the count block's destroy hook
//          (`call [block+4]`, object at [block+8] in ecx). strcmp is the VC7.1 intrinsic.
// Shape: every element is taken BY VALUE into a counted-pointer local (object in esi, block in edi; the copy
//          constructor bumps the count, the inline destructor `dec [edi] ; jne` destroys and frees at zero and its
//          dead zero stores vanish); the counter lives in the `push ecx` frame word; the array end is re-read every
//          iteration because the body calls out.
// Retail layout: SubChunks at +0x10 as a 12-byte STLport vector (see 0x00AA78C0), so `this` is modelled locally.
#include <string.h>
#pragma intrinsic(strcmp)

struct CDataOutputStream {
    virtual void v00();
    virtual void SetPosition(long position);        // slot 1
    virtual long GetPosition();                     // slot 2
    virtual void v03();
    virtual void Write(const void* data, long size);// slot 4
    void WriteLong(long value);                     // 0x00993EA0 (Write(&value, 4); manifest "WriteCBYTE" is propagated)
};
struct CDataInputStream;
struct CChunkedFileSaver;
struct CChunkedFileLoader;
struct CChunkedFileSuperChunk;

struct CChunkedFileChunk {                          // retail layout = engine/CChunkedFileChunk.h (+ the vtable it cannot declare)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long IsUnknownType() const;             // slot 4 (CChunkedFileRawDataChunk @ 0x00AA7740: `mov eax,1`)
    virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);   // slot 7
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                    // slot 8
    char ChunkType[5];                              // +0x04 four-character code + terminator
    CChunkedFileSuperChunk* Parent;                 // +0x0c
};

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

struct CChunkPointer {                              // CCountedPointer<CChunkedFileChunk>
    CChunkedFileChunk* Object;                      // +0
    SCountBlock* Count;                             // +4
};

struct CChunkArray {                                // retail CArray<CCountedPointer<CChunkedFileChunk>>: STLport vector, 12 bytes
    CChunkPointer* m_pFirst;
    CChunkPointer* m_pLast;
    CChunkPointer* m_pEnd;
    CChunkPointer* begin() const { return m_pFirst; }
    CChunkPointer* end() const { return m_pLast; }
};

struct CChunkRef {                                  // a counted-pointer local copy
    CChunkedFileChunk* Object;
    SCountBlock* Count;
    CChunkRef(const CChunkPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CChunkRef()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
        Object = 0;
        Count = 0;
    }
    CChunkedFileChunk* operator->() const { return Object; }
};

struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CChunkArray SubChunks;                          // +0x10
    CChunkArray RegisteredSubChunkTypes;            // +0x1c
    unsigned long CurrentChunkIndex;                // +0x28
    long CountSubChunksOfType(const char* type) const;
};

long CChunkedFileSuperChunk::CountSubChunksOfType(const char* type) const
{
    long count = 0;
    for (CChunkPointer* it = SubChunks.begin(); it != SubChunks.end(); ++it) {
        CChunkRef chunk(*it);
        if (!chunk->IsUnknownType() && strcmp(type, chunk->ChunkType) == 0)
            ++count;
    }
    return count;
}
