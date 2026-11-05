// C3DAnimFileSequenceChunk::CreateNewInstance() const -- allocate and register a fresh "ASEQ" chunk
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAF7A0, length 96 bytes.
// Callees: operator new @ 0x00BFEA1A, CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) @ 0x00AA8360, CCharString::CCharString() @ 0x0099E4B0, virtual InitNewInstance (slot 10).
// `new` with the constructor inlined (null-checked: on allocation failure the virtual gets a null), then the
// protected virtual InitNewInstance (slot 10, 0x00AA8460 in the super chunk) registers the chunk and its result
// is returned. VC duplicates the virtual-call tail on both sides of the null test.
// The inlined constructor is the one @ 0x00AAF730: BoneIndex = -1, BoneName default, BoneType and the
// Matrices array zeroed (through the register that already holds the null-test zero).
// the generated engine headers for these chunk classes cannot model the bodies -- a local model is used instead.
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
    protected:
    CChunkedFileChunk(const char* type);    // 0x00AA7590 (Ego_r / FableWin: protected ??0...@@IAE@PBD@Z)
    public:
};

class CChunkedFileSuperChunk : public CChunkedFileChunk { public:   // retail sizeof 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c
    unsigned long CurrentChunkIndex;                                       // +0x28
    protected:
    CChunkedFileSuperChunk(const char* type);                              // 0x00AA8360 (Ego_r / FableWin: protected ??0...@@IAE@PBD@Z)
    public:
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // 0x00AA78C0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const; // 0x00AA7F40
};

class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk { public:   // retail sizeof 0x4c (PDB 0x58: two CArrays 4 bytes wider each)
    long BoneIndex;                         // +0x2c
    long ParentIndex;                       // +0x30
    CCharString BoneName;                   // +0x34
    float SamplesPerSecond;                 // +0x38
    long BoneType;                          // +0x3c EBoneType
    CArray<unsigned char> Matrices;         // +0x40 CArray<CMatrix3x4>
    C3DAnimFileSequenceChunk()              // literal @ 0x0129E7B4, vtable 0x0129E8B8
        : CChunkedFileSuperChunk("ASEQ"), BoneIndex(-1), BoneName(), BoneType(0), Matrices() {}
    virtual CChunkedFileChunk* CreateNewInstance() const;
};

CChunkedFileChunk* C3DAnimFileSequenceChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileSequenceChunk);
}