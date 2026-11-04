// C3DMeshFileVerticesChunk::`scalar deleting destructor'(unsigned int flags) -- this class's vptr, free the VertexList storage, the CChunkedFileChunk destructor, then free this when bit 0 of flags is set
// Leaf: ??_GC3DMeshFileVerticesChunk@@UAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABEE10, 54 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0129ECF8 (RTTI .?AVC3DMeshFileVerticesChunk@@, bases CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ??_GC3DMeshFileVerticesChunk@@UAEPAXI@Z and
//   ??1C3DMeshFileVerticesChunk@@UAE@XZ; member VertexList (CArray<C3DVertex2>) at retail +0x10.
// Callees: free 0x00BFEA14 (the CArray's inline destructor: the trivially destroyed elements fold away), CChunkedFileChunk::~CChunkedFileChunk
//   0x00AA75C0, operator delete 0x00BFE9BC.
// Shape (family K051, representative 0x00A9D360): the class's own (empty) destructor, defined in this file, is inlined into
//   the ??_G (an in-class inline definition is not: ??_G then calls ??1 out of line): the class vptr, the member CArray's
//   STLport destructor, the base destructor out of line; a `new` of the class emits the vtable and the ??_G.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#include "engine/CArray.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable { // Ego_r.pdb 16 bytes
public:
    CChunkedFileChunk();
    virtual ~CChunkedFileChunk();                        // 0x00AA75C0
    char ChunkType[5];                                   // +0x04
    CChunkedFileSuperChunk* Parent;                      // +0x0C
};
class C3DVertex2 { public: unsigned char _data[4]; };        // element (opaque here)
class C3DMeshFileVerticesChunk : public CChunkedFileChunk {
public:
    C3DMeshFileVerticesChunk() { }
    virtual ~C3DMeshFileVerticesChunk();
    CArray<C3DVertex2> VertexList;   // +0x10
};
C3DMeshFileVerticesChunk::~C3DMeshFileVerticesChunk()
{
}
C3DMeshFileVerticesChunk* Make_00abee10() { return new C3DMeshFileVerticesChunk; }