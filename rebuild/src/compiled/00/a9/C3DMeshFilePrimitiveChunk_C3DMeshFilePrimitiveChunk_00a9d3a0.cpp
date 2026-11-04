// C3DMeshFilePrimitiveChunk::~C3DMeshFilePrimitiveChunk() -- the compiler-generated destructor: free the MaterialIndices block, then run
//   ~CChunkedFileSuperChunk
// Leaf: ??1C3DMeshFilePrimitiveChunk@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Naming wave 30 (family c699): was "CTCInventoryItem::OnDie" (?OnDie@CTCInventoryItem@@QAEXXZ over an invented BaseItem whose OnDie
//   named ~CChunkedFileSuperChunk 0x00AA8490; the catalog label is propagated from the 0x00A9D3C0 twin's caller set).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D3A0, 30 bytes, thiscall, tail `jmp ~CChunkedFileSuperChunk`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00ABEEB0 of vtable 0x0129ED74 (RTTI .?AVC3DMeshFilePrimitiveChunk@@ :
//   CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb ??1C3DMeshFilePrimitiveChunk@@UAE@XZ
//   (donor 0x033D41A0, directly before ??1C3DMeshFileClothChunk 0x033D4200 -- retail has the same pair, 0x00A9D3A0 then the landed
//   0x00A9D3C0). Ego_r.pdb C3DMeshFilePrimitiveChunk: MaterialIndices (CArray<long>) at retail +0x2C (CChunkedFileSuperChunk is 0x2C).
//   No 0x0129ED74 store: the destructor is the implicit one.
// Callees: free 0x00BFEA14, CChunkedFileSuperChunk::~CChunkedFileSuperChunk 0x00AA8490.
// Layout: local, as the sibling 0x00A9D3C0 (engine/C3DMeshFilePrimitiveChunk.h / CChunkedFileSuperChunk.h flatten the vptr).
// Shape: the sibling's model. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"                         // retail model (8 bytes: Data, Info)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

#include "engine/CArray.h"


class CChunkedFileSuperChunk;

class CChunkedFileChunk : public CBaseClassNonCopyable {   // vtable 0x0129E474, 0x10 bytes
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char ChunkType[5];                                      // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0c
};

class CChunkedFileSuperChunk : public CChunkedFileChunk {   // vtable 0x0129E4F0, 0x2c bytes in retail
public:
    virtual ~CChunkedFileSuperChunk();                      // 0x00AA8490
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;               // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes; // +0x1c
    unsigned long CurrentChunkIndex;                        // +0x28
};

class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk {   // vtable 0x0129ED74, 0x38 bytes in retail
public:
    C3DMeshFilePrimitiveChunk();
    CArray<long>  MaterialIndices;                          // +0x2c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(C3DMeshFilePrimitiveChunk* chunk)
{
    chunk->C3DMeshFilePrimitiveChunk::~C3DMeshFilePrimitiveChunk();
}