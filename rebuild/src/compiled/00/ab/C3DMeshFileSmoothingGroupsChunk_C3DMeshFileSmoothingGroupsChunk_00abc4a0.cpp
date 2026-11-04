// C3DMeshFileSmoothingGroupsChunk::~C3DMeshFileSmoothingGroupsChunk() -- store the own vtable, free FaceGroupsList's element block (the STLport vector destructor inline: trivial elements, then `if (_M_start) free(_M_start)`), then the CChunkedFileChunk destructor as a tail jump
// Leaf: ??1C3DMeshFileSmoothingGroupsChunk@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABC4A0, 33 bytes (+ int3 padding to 48), thiscall, tail `jmp` to the base destructor.
// Identity (proven): C3DMeshFileSmoothingGroupsChunk::~C3DMeshFileSmoothingGroupsChunk() (Ego_r.pdb / FableWin.pdb ??1C3DMeshFileSmoothingGroupsChunk@@UAE@XZ); evidence: it stores the C3DMeshFileSmoothingGroupsChunk vtable 0x0129EC28 first (RTTI .?AVC3DMeshFileSmoothingGroupsChunk@@; bases CChunkedFileChunk <- CBaseClassNonCopyable <- CBaseClass) and tail-jumps to the parent's destructor
// Label: the manifest's "OnDie" is propagated across byte-identical bodies (family J023: six destructors of a class
//   that adds one STLport vector of trivially destroyed elements; one retail address is one class's destructor, named
//   from the vtable it stores first).
// Callees: free 0x00BFEA14, CChunkedFileChunk::~CChunkedFileChunk() 0x00AA75C0 (catalog-bound ??1CChunkedFileChunk@@UAE@XZ (Ego_r.pdb; a QAE stand-in also)).
// Layout: CChunkedFileChunk is 0x10 bytes, FaceGroupsList (CArray<unsigned long>) +0x10.
//   Local model: the generated engine headers for these classes carry a data-member vptr (and Dinkumware vector sizes).
#include <stddef.h>
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                           // 0x0099A300
};
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {   // 0x10 bytes (Ego_r.pdb, no STL member)
public:
    virtual ~CChunkedFileChunk();                               // 0x00AA75C0
    char                    ChunkType[5];                       // +0x04
    CChunkedFileSuperChunk* Parent;                             // +0x0C
};
class C3DMeshFileSmoothingGroupsChunk : public CChunkedFileChunk {                              // RTTI: C3DMeshFileSmoothingGroupsChunk <- CChunkedFileChunk <- CBaseClassNonCopyable <- CBaseClass
public:
    virtual ~C3DMeshFileSmoothingGroupsChunk();
    CArray<unsigned long> FaceGroupsList;                       // +0x10
};

C3DMeshFileSmoothingGroupsChunk::~C3DMeshFileSmoothingGroupsChunk()
{
}