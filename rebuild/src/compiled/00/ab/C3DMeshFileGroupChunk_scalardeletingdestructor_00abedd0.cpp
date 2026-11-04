// C3DMeshFileGroupChunk::`scalar deleting destructor'(unsigned int flags) -- this class's vptr, free the VertexList storage, the CChunkedFileSuperChunk destructor, then free this when bit 0 of flags is set
// Leaf: ??_GC3DMeshFileGroupChunk@@UAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABEDD0, 54 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0129ECB8 (RTTI .?AVC3DMeshFileGroupChunk@@, bases CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ??_GC3DMeshFileGroupChunk@@UAEPAXI@Z and
//   ??1C3DMeshFileGroupChunk@@UAE@XZ; member VertexList (CArray<C3DVertexBlend2>) at retail +0x30.
// Callees: free 0x00BFEA14 (the CArray's inline destructor: the trivially destroyed elements fold away), CChunkedFileSuperChunk::~CChunkedFileSuperChunk
//   0x00AA8490, operator delete 0x00BFE9BC.
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
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
class CChunkedFileSuperChunk : public CChunkedFileChunk {   // Ego_r.pdb 52 bytes; retail 0x2C
public:
    CChunkedFileSuperChunk();
    virtual ~CChunkedFileSuperChunk();                   // 0x00AA8490
    unsigned char SubChunks[12];                         // +0x10 CArray<CCountedPointer<CChunkedFileChunk> > (not touched here)
    unsigned char RegisteredSubChunkTypes[12];           // +0x1C CArray<CCountedPointer<CChunkedFileChunk> >
    unsigned long CurrentChunkIndex;                     // +0x28
};
class C3DVertexBlend2 { public: unsigned char _data[4]; };   // element (opaque here)
class C3DMeshFileGroupChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileGroupChunk() { }
    virtual ~C3DMeshFileGroupChunk();
    long BoneIndex;                                      // +0x2C
    CArray<C3DVertexBlend2> VertexList;   // +0x30
};
C3DMeshFileGroupChunk::~C3DMeshFileGroupChunk()
{
}
C3DMeshFileGroupChunk* Make_00abedd0() { return new C3DMeshFileGroupChunk; }