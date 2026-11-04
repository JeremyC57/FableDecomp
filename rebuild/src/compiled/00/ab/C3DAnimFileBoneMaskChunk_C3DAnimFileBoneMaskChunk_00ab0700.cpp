// C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk() -- a "AMSK" CChunkedFileChunk with an empty EnableMask
// Leaf: ??0C3DAnimFileBoneMaskChunk@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB0700, 34 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0129E980 (RTTI .?AVC3DAnimFileBoneMaskChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb / Ego_r.pdb
//   ??0C3DAnimFileBoneMaskChunk@@QAE@XZ; callers C3DAnimFileLoader::C3DAnimFileLoader 0x00A999B0; Ego_r.pdb member EnableMask (CArray<bool>) at +0x10.
// Callees: CChunkedFileChunk::CChunkedFileChunk(const char*) 0x00AA7590 (literal 0x0129E8F8 "AMSK").
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
class C3DAnimFileBoneMaskChunk : public CChunkedFileChunk {
public:
    C3DAnimFileBoneMaskChunk();
    CArray<bool> EnableMask;                                    // +0x10
};
C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk()
    : CChunkedFileChunk("AMSK")
{
}