// CChunkedFileSuperChunk::~CChunkedFileSuperChunk() -- own vtable, RemoveAllSubChunks(), destroy and free both CArray<CCountedPointer<CChunkedFileChunk>> members, then the inlined empty ~CChunkedFileChunk and ~CBaseClassNonCopyable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA8490, length 95 bytes (+ int3 padding).
// Callees: CChunkedFileSuperChunk::RemoveAllSubChunks @ 0x00AA8400, _Destroy_range(first, last, tag&)
// @ 0x00AA9140 (`__fastcall`, releases each counted pointer; the by-reference tag is a 1-byte
// temporary homed in the high byte of the frame slot: `lea eax,[esp+7]`), free @ 0x00BFEA14 (x2,
// null-tested), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (manifest label "CBase").
// Vtables: 0x0129E4F0 = CChunkedFileSuperChunk (stored first), 0x0129E474 = CChunkedFileChunk (the
// inlined empty intermediate destructor's store). The manifest's "~CCompressedMemoryDataInputStream"
// label is propagated. Layout as the landed constructor @ 0x00AA83B0: retail CArrays are 12 bytes, so
// SubChunks is at +0x10 and RegisteredSubChunkTypes at +0x1c (engine/CChunkedFileSuperChunk.h has
// them 16 bytes apart, hence the `_Retail` model); members are destroyed in reverse order. The
// `push ecx` frame slot (not a tail jump) is the tag temporary.
#include "engine/CChunkedFileSuperChunk.h"

extern "C" void __cdecl free(void* block);                      // 0x00BFEA14

struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                                   // 0x0099A300
};

struct CChunkedFileChunk_Retail : CBaseClassNonCopyable {       // vtable 0x0129E474
    unsigned char _body[0x0c];                                  // ChunkType[5] +4, Parent +0xc
    virtual void v00();
    ~CChunkedFileChunk_Retail() { }                             // inline, empty
};

struct CCountedChunk { void* m_pObject; void* m_pCount; };      // CCountedPointer<CChunkedFileChunk>

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

void __fastcall DestroyRange(CCountedChunk* first, CCountedChunk* last, const SNonscalarPtrIteratorTag& tag);   // 0x00AA9140

template<class T> struct CRetailArray {                         // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    ~CRetailArray()
    {
        DestroyRange(first, last, SNonscalarPtrIteratorTag());
        if (first != 0)
            free(first);
    }
};

struct CChunkedFileSuperChunk_Retail : CChunkedFileChunk_Retail {
    CRetailArray<CCountedChunk> SubChunks;                      // +0x10
    CRetailArray<CCountedChunk> RegisteredSubChunkTypes;        // +0x1c
    unsigned long CurrentChunkIndex;                            // +0x28
    virtual void v00();                                         // own vtable 0x0129E4F0
    void RemoveAllSubChunks();                                  // 0x00AA8400
    ~CChunkedFileSuperChunk_Retail();
};

FABLE_STATIC_ASSERT(sizeof(CChunkedFileSuperChunk_Retail) == 0x2c);

CChunkedFileSuperChunk_Retail::~CChunkedFileSuperChunk_Retail()
{
    RemoveAllSubChunks();
}