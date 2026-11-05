// C3DAnimFileRootChunk::HasSequences() const -- is there an "ASEQ" sub-chunk?
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAE920, length 80 bytes.
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType @ 0x00AA7F40, operator delete @ 0x00BFE9BC.
// Literal @ 0x0129E7B4 is the sequence chunk's type string (the one its constructor @ 0x00AAF730 passes).
// The manifest attributes this body to C3DAnimFileObjectChunk by propagation; the body only needs a
// CChunkedFileSuperChunk `this` and the root chunk is the natural owner of the sequence sub-chunks.
// FindFirstSubChunkOfType returns a counted pointer by value (hidden result slot at [esp+4], pushed after the
// literal; `this` stays in ecx). The object pointer is tested while the temporary is alive (`setne bl` rides
// across the release), then the temporary's count block is released: decrement, re-read, destroy through the
// block's deleter with the object in ecx, operator delete @ 0x00BFE9BC. A bool result (`mov al,bl`, no `xor eax`).
class CChunkedFileSaver;

class CCharString { public:
    void* m_pData;                          // CBasicString<char> data block
    CCharString();                          // 0x0099E4B0
};

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void WriteBytes(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteLONG(long value);                                    // 0x00993EB0
    void WriteULONG(unsigned long value);                          // 0x00993EA0
    void WriteFLOAT(float value);                                  // 0x00993EE0
    void WriteBOOL(bool value);                                    // 0x00993E30
    void WriteNullTerminatedString(const CCharString& text);       // 0x00995AE0
};

#include "engine/CArray.h"

#include "engine/CCountedPointer.h"


class CChunkedFileSuperChunk;

class CChunkedFileChunk { public:                  // PDB sizeof 0x10
    virtual void v00();                     // slot 0: deleting destructor
    virtual void v01();                     // slot 1: GetChunkDescription
    virtual void v02();                     // slot 2: GetChunkInfo
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();                     // slot 7: ReadFromFile
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // slot 8
    virtual CChunkedFileChunk* CreateNewInstance() const;                                // slot 9
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;          // slot 10 (0x00AA8460 in the super chunk)
    char ChunkType[5];                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk* Parent;         // +0x0c
    CChunkedFileChunk(const char* type);    // 0x00AA7590
};

class CChunkedFileSuperChunk : public CChunkedFileChunk { public:   // retail sizeof 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c
    unsigned long CurrentChunkIndex;                                       // +0x28
    CChunkedFileSuperChunk(const char* type);                              // 0x00AA8360
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // 0x00AA78C0
public:
    CCountedPointer<CChunkedFileChunk> const FindFirstSubChunkOfType(const char*) const;   // 0x00AA7F40
};

class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { public:   // retail: LoopFlag +0x2c, Duration +0x30 (PDB +0x34/+0x38)
    bool LoopFlag;                          // +0x2c
    float Duration;                         // +0x30
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
    bool HasMovementVector() const;
    bool HasSequences() const;
};

bool C3DAnimFileRootChunk::HasSequences() const
{
    return FindFirstSubChunkOfType("ASEQ").Data != 0;
}