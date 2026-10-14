// CChunkedFileSuperChunk::CChunkedFileSuperChunk(const SChunkInfo&) -- inlined CChunkedFileChunk(info.Type) (CBase(), null parent, 5-byte type cleared then the 4-char code copied), own vtable, both CArray members empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA83B0, length 70 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtables: 0x0129E474 = CChunkedFileChunk (the inlined base
// constructor's store), 0x0129E4F0 = CChunkedFileSuperChunk (rtti-vtables.tsv). The manifest's
// "CCombatAbilityStrafeDef" label is propagated.
// Layout: retail CChunkedFileChunk = {vptr, char ChunkType[5] +4, CChunkedFileSuperChunk* Parent +0xc}
// (0x10 bytes, as engine/CChunkedFileChunk.h); the super chunk's two CArray<CCountedPointer<...>>
// are 12 bytes each in retail (PDB 16), so SubChunks is at +0x10, RegisteredSubChunkTypes at +0x1c
// and CurrentChunkIndex (left uninitialised) at +0x28 -- hence the `_Retail` models.
// The out-of-line CChunkedFileChunk constructor @ 0x00AA7590 copies its argument's FIRST dword into
// ChunkType; this body copies the argument's dword at +4, i.e. the base is built from a 4-byte
// type code that sits at +4 of the super chunk's own argument (`CChunkedFileChunk(info.Type)`).
// The base body is: Parent null (init list), the 5-byte array cleared (intrinsic memset: dword +
// byte through a copied pointer register), then the 4 code bytes copied (intrinsic memcpy).
#include <string.h>

struct CChunkedFileSuperChunk_Retail;

struct CBase {
    virtual void v00();
    CBase();                                                // 0x0099A2F0
};

struct SChunkTypeCode { char Code[4]; };                    // a four-character chunk id
struct SChunkInfo {
    unsigned long  Flags;                                   // +0
    SChunkTypeCode Type;                                    // +4
};

struct CChunkedFileChunk_Retail : CBase {
    char ChunkType[5];                                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk_Retail* Parent;                  // +0x0c
    virtual void v00();                                     // own vtable 0x0129E474
    CChunkedFileChunk_Retail(const SChunkTypeCode& type) : CBase(), Parent(0)
    {
        memset(ChunkType, 0, sizeof(ChunkType));
        memcpy(ChunkType, &type, sizeof(type));
    }
};

struct CCountedChunk { void* m_pObject; void* m_pCount; };

template<class T> struct CRetailArray {                     // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    CRetailArray() : first(0), last(0), end_of_storage(0) { }
};

struct CChunkedFileSuperChunk_Retail : CChunkedFileChunk_Retail {
    CRetailArray<CCountedChunk> SubChunks;                  // +0x10
    CRetailArray<CCountedChunk> RegisteredSubChunkTypes;    // +0x1c
    unsigned long CurrentChunkIndex;                        // +0x28 (not initialised)
    virtual void v00();                                     // own vtable 0x0129E4F0
    CChunkedFileSuperChunk_Retail(const SChunkInfo& info);
};

CChunkedFileSuperChunk_Retail::CChunkedFileSuperChunk_Retail(const SChunkInfo& info)
    : CChunkedFileChunk_Retail(info.Type)
    , SubChunks()
    , RegisteredSubChunkTypes()
{
}
