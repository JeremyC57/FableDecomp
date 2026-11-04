// C3DAnimFileTimingEventChunk::C3DAnimFileTimingEventChunk() -- "TMEV" leaf chunk with a default-constructed name
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAE170, length 32 bytes.
// Callees: CChunkedFileChunk::CChunkedFileChunk(const char*) @ 0x00AA7590, CCharString::CCharString() @ 0x0099E4B0.
// Base constructor with the chunk type literal (@ 0x0129E7AC), this class's vtable (0x0129E7F0), then the Name
// member at +0x10; Time is left uninitialised. Derives from the plain chunk (no sub-chunk arrays), so Name sits at +0x10.
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

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4: per-type deleter, object in ecx
    void* m_pObject;                        // +8
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
};

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

class C3DAnimFileTimingEventChunk : public CChunkedFileChunk { public:   // sizeof 0x18
    CCharString Name;                       // +0x10
    float Time;                             // +0x14
    C3DAnimFileTimingEventChunk();
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
};

C3DAnimFileTimingEventChunk::C3DAnimFileTimingEventChunk()
    : CChunkedFileChunk("TMEV")             // literal @ 0x0129E7AC
{
}