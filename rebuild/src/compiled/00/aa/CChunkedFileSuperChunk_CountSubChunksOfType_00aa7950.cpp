// CChunkedFileSuperChunk::CountSubChunksOfType(const char*) const -- (label propagated) returns the registered prototype chunk whose four-character code equals the given type, or null
// Leaf: FindRegisteredSubChunkType
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7950, 100 bytes of code (`ret 4`). The manifest names this body after its
// neighbour 0x00AA7EA0, which really counts; this one walks RegisteredSubChunkTypes (+0x1c) and returns a chunk pointer.
// Callees: none (strcmp is the VC7.1 intrinsic: two bytes per step, `sbb eax,eax ; sbb eax,-1` for the sign).
// Shape: the array end is read once into edx before the loop (a const method calling nothing); `strcmp(type, code)`
//          keeps the argument as the first operand (esi) and the chunk's ChunkType (+4) as the second (eax).
// Retail layout: two 12-byte STLport vectors at +0x10 and +0x1c (the PDB header's 16-byte CArray puts the second at
//          +0x20), so `this` is modelled locally.
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
};

class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    CChunkArray SubChunks;                          // +0x10
    CChunkArray RegisteredSubChunkTypes;            // +0x1c
    unsigned long CurrentChunkIndex;                // +0x28
    CChunkedFileChunk* FindRegisteredSubChunkType(const char* type) const;
};

CChunkedFileChunk* CChunkedFileSuperChunk::FindRegisteredSubChunkType(const char* type) const
{
    for (CChunkPointer* it = RegisteredSubChunkTypes.begin(); it != RegisteredSubChunkTypes.end(); ++it) {
        if (strcmp(type, it->Object->ChunkType) == 0)
            return it->Object;
    }
    return 0;
}