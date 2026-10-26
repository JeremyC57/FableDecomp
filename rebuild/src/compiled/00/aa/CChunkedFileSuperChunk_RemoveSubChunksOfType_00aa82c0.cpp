// CChunkedFileSuperChunk::RemoveSubChunksOfType(const char*) -- detaches and erases every sub-chunk whose four-character code equals the given type
// Leaf: RemoveSubChunksOfType
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA82C0, 145 bytes of code (`ret 4`).
// Callees: the SubChunks vector's erase(iterator) @ 0x00AA91F0 (thiscall on the vector, result dropped; manifest label
//          "CScriptBase::RemovePredicate" is propagated). strcmp is the VC7.1 intrinsic.
// Shape: an index loop against size() (re-evaluated each pass, unsigned `jb`), the index in the `push ecx` frame word;
//          the element is reached the STLport way, `*(begin() + i)`, so the scaled offset (`lea ebp,[eax*8]`) is formed
//          once and reused for the erase position; on a match the chunk's Parent is cleared (with strcmp's zero in
//          eax), the element erased and the index stepped back.
// Retail layout: SubChunks at +0x10 as a 12-byte STLport vector (see 0x00AA78C0), so `this` is modelled locally.
#include <string.h>
#pragma intrinsic(strcmp)

class CDataOutputStream { public:
    virtual void v00();
    virtual void SetPosition(long position);        // slot 1
    virtual long GetPosition();                     // slot 2
    virtual void v03();
    virtual void Write(const void* data, long size);// slot 4
    void WriteLong(long value);                     // 0x00993EA0 (Write(&value, 4); manifest "WriteCBYTE" is propagated)
};
class CDataInputStream;
class CChunkedFileSaver;
class CChunkedFileLoader;
class CChunkedFileSuperChunk;

class CChunkedFileChunk { public:                          // retail layout = engine/CChunkedFileChunk.h (+ the vtable it cannot declare)
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
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    CChunkPointer& operator[](unsigned long n) const { return *(begin() + n); }
    CChunkPointer* erase(CChunkPointer* position);  // 0x00AA91F0
};

class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    CChunkArray SubChunks;                          // +0x10
    CChunkArray RegisteredSubChunkTypes;            // +0x1c
    unsigned long CurrentChunkIndex;                // +0x28
    void RemoveSubChunksOfType(const char* type);
};

void CChunkedFileSuperChunk::RemoveSubChunksOfType(const char* type)
{
    for (unsigned long i = 0; i < SubChunks.size(); ++i) {
        CChunkedFileChunk* chunk = SubChunks[i].Object;
        if (strcmp(type, chunk->ChunkType) == 0) {
            chunk->Parent = 0;
            SubChunks.erase(SubChunks.begin() + i);
            --i;
        }
    }
}