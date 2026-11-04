// C3DMeshFileSmoothingGroupsChunk::C3DMeshFileSmoothingGroupsChunk() -- a "SMTH" CChunkedFileChunk with an empty FaceGroupsList
// Leaf: ??0C3DMeshFileSmoothingGroupsChunk@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC470, 34 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0129EC28 (RTTI .?AVC3DMeshFileSmoothingGroupsChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb / Ego_r.pdb
//   ??0C3DMeshFileSmoothingGroupsChunk@@QAE@XZ; callers the C3DMeshFileXLoader constructor 0x00A9BBE0; Ego_r.pdb member FaceGroupsList (CArray<unsigned long>) at +0x10.
// Callees: CChunkedFileChunk::CChunkedFileChunk(const char*) 0x00AA7590 (literal 0x0129EB04 "SMTH").
// Shape (family K053, representative 0x00AB0700): the base call, the vtable store, then the member CArray's inline default
//   constructor (xor eax; three stores).
class CChunkedFileSuperChunk;
class CChunkedFileChunk {                                // Ego_r.pdb 16 bytes (its CBaseClassNonCopyable bases are empty)
public:
    virtual ~CChunkedFileChunk();
protected:
    CChunkedFileChunk(const char* chunkType);            // 0x00AA7590
    char ChunkType[5];                                   // +0x04
    CChunkedFileSuperChunk* Parent;                      // +0x0C
};
#include "engine/CArray.h"
class C3DMeshFileSmoothingGroupsChunk : public CChunkedFileChunk {
public:
    C3DMeshFileSmoothingGroupsChunk();
    CArray<unsigned long> FaceGroupsList;                                    // +0x10
};
C3DMeshFileSmoothingGroupsChunk::C3DMeshFileSmoothingGroupsChunk()
    : CChunkedFileChunk("SMTH")
{
}