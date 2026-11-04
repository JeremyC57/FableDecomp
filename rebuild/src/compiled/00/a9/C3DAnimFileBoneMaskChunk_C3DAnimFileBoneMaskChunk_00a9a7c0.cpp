// C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk() -- compiler-generated destructor: free the EnableMask array block, then ~CChunkedFileChunk
// Leaf: ??1C3DAnimFileBoneMaskChunk@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9A7C0, 27 bytes, thiscall, tail jump to CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0.
// Identity (proven): the sole caller 0x00AB0B53 is the scalar deleting destructor 0x00AB0B50, slot 0 of vtable 0x0129E980
//   (rtti-vtables.tsv, RTTI .?AVC3DAnimFileBoneMaskChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass). No vtable
//   store inside: the destructor is compiler-generated (Ego_r.pdb ??1C3DAnimFileBoneMaskChunk@@UAE@XZ). The member freed
//   (+0x10) is Ego_r C3DAnimFileBoneMaskChunk::EnableMask (CArray<bool>). The catalog label "CTCInventoryItem::OnDie" is
//   propagated. The tail target 0x00AA75C0 stores the CChunkedFileChunk vtable 0x0129E474 (its own user dtor,
//   ??1CChunkedFileChunk@@UAE@XZ).
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), ??1CChunkedFileChunk@@UAE@XZ 0x00AA75C0 (tail jump).
// Layout: Ego_r.pdb C3DAnimFileBoneMaskChunk: CChunkedFileChunk (ChunkType char[5] +0x04, Parent +0x0C), EnableMask +0x10
//   (retail CArray<bool> = 12-byte STLport vector, one byte per element: the ctor 0x00AB0700 zeroes +0x10/+0x14/+0x18 and
//   WriteToFile 0x00AB06B0 reads `byte [start+i]` up to finish-start). engine/C3DAnimFileBoneMaskChunk.h and
//   engine/CChunkedFileChunk.h are flattened records (vptr as data), so the polymorphic chain is a local model.
// TU: the PDB constructor ??0C3DAnimFileBoneMaskChunk@@QAE@XZ is defined with an empty body only to emit the vtable, the ??_G
//   and this implicit ??1 (not graded).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#include "engine/CArray.h"


#include "engine/CBaseClassNonCopyable.h"

class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {    // vtable 0x0129E474
protected:
    CChunkedFileChunk(const char* chunkType);               // 0x00AA7590
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char                    ChunkType[5];                   // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0C
};

class C3DAnimFileBoneMaskChunk : public CChunkedFileChunk { // vtable 0x0129E980
public:
    C3DAnimFileBoneMaskChunk();                             // 0x00AB0700
    static const char* GetStaticChunkType();
    CArray<bool> EnableMask;                                // +0x10
};

C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk()
    : CChunkedFileChunk(GetStaticChunkType())
{
}