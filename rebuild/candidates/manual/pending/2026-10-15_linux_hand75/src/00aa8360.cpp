// CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char* type) -- the CChunkedFileChunk base inlined (CBaseClassNonCopyable base called, its vtable, Parent cleared, ChunkType zero-filled then the four type characters copied), this class's vtable, both sub-chunk arrays and CurrentChunkIndex cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA8360, length 74 bytes.
// Leaf: ??0CChunkedFileSuperChunk
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0.
// The manifest label "CShopItemDef::CShopItemDef" is propagated: vtables 0x0129E474 (CChunkedFileChunk) and 0x0129E4F0
// (CChunkedFileSuperChunk) in rtti-vtables.tsv; the landed C3DAnimFileObjectChunk constructor 0x00AB0020 calls this body
// as CChunkedFileSuperChunk(const char*). The out-of-line CChunkedFileChunk(const char*) is 0x00AA7590 (same base body).
// Layout: retail CArray is 12 bytes, so the super chunk is 0x2c (PDB 0x34) -- local model as in 0x00AB0020; the generated
// engine/CChunkedFileChunk.h and CChunkedFileSuperChunk.h are flat records without virtual functions (a constructor that
// stores both vtables needs the polymorphic classes) and the latter carries the PDB's 16-byte CArrays.
// Shape: the 5-byte ChunkType cleared by an inline memset (`mov [edi],edx ; mov [edi+4],dl`, the intrinsic's own zero
// and destination copy), the type copied by a 4-byte memcpy whose store lands after the derived vtable store; the
// derived members all share the eax zero of the base's Parent store.
#include <string.h>

struct CBaseClassNonCopyable {                  // vtable 0x0129A7C4
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                    // 0x0099A2F0
};

template <class T> struct CArray {              // retail CArray is 12 bytes (the PDB's is 16)
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    CArray() : m_pBegin(0), m_pEnd(0), m_pEndOfStorage(0) { }
};

struct SCountBlock;

template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
};

struct CChunkedFileSuperChunk;

struct CChunkedFileChunk : CBaseClassNonCopyable {      // vtable 0x0129E474, PDB sizeof 0x10
    char                    ChunkType[5];               // +0x04
    CChunkedFileSuperChunk* Parent;                     // +0x0c
    CChunkedFileChunk(const char* type) : Parent(0)     // out of line at 0x00AA7590
    {
        memset(ChunkType, 0, sizeof(ChunkType));
        memcpy(ChunkType, type, 4);
    }
    virtual ~CChunkedFileChunk();
};

struct CChunkedFileSuperChunk : CChunkedFileChunk {     // vtable 0x0129E4F0, retail sizeof 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c
    unsigned long                               CurrentChunkIndex;         // +0x28
    CChunkedFileSuperChunk(const char* type);
    virtual ~CChunkedFileSuperChunk();
};

CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char* type)
    : CChunkedFileChunk(type), CurrentChunkIndex(0)
{
}
