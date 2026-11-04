// C3DMeshFileTrianglesChunk::~C3DMeshFileTrianglesChunk() -- store the own vtable, free TriangleList's element block (the STLport vector destructor inline: trivial elements, then `if (_M_start) free(_M_start)`), then the CChunkedFileSuperChunk destructor as a tail jump
// Leaf: ??1C3DMeshFileTrianglesChunk@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABC890, 33 bytes (+ int3 padding to 48), thiscall, tail `jmp` to the base destructor.
// Identity (proven): C3DMeshFileTrianglesChunk::~C3DMeshFileTrianglesChunk() (Ego_r.pdb / FableWin.pdb ??1C3DMeshFileTrianglesChunk@@UAE@XZ); evidence: it stores the C3DMeshFileTrianglesChunk vtable 0x0129ED34 first (RTTI .?AVC3DMeshFileTrianglesChunk@@; bases CChunkedFileSuperChunk <- CChunkedFileChunk <- CBaseClassNonCopyable <- CBaseClass) and tail-jumps to the parent's destructor
// Label: the manifest's "OnDie" is propagated across byte-identical bodies (family J023: six destructors of a class
//   that adds one STLport vector of trivially destroyed elements; one retail address is one class's destructor, named
//   from the vtable it stores first).
// Callees: free 0x00BFEA14, CChunkedFileSuperChunk::~CChunkedFileSuperChunk() 0x00AA8490 (catalog-bound ??1CChunkedFileSuperChunk@@UAE@XZ (Ego_r.pdb; QAE / OnDie stand-ins also)).
// Layout: retail CChunkedFileSuperChunk is 0x2C bytes, TriangleList (CArray<C3DTriangle2>) +0x2C.
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
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };   // 6 bytes (Ego_r.pdb)
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {   // 0x10 bytes (Ego_r.pdb, no STL member)
public:
    virtual ~CChunkedFileChunk();                               // 0x00AA75C0
    char                    ChunkType[5];                       // +0x04
    CChunkedFileSuperChunk* Parent;                             // +0x0C
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // 8 bytes
class CChunkedFileSuperChunk : public CChunkedFileChunk {   // retail 0x2C bytes (Ego_r.pdb 0x34)
public:
    virtual ~CChunkedFileSuperChunk();                          // 0x00AA8490
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;              // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes; // +0x1C
    unsigned long CurrentChunkIndex;                            // +0x28
};
class C3DMeshFileTrianglesChunk : public CChunkedFileSuperChunk {                              // RTTI: C3DMeshFileTrianglesChunk <- CChunkedFileSuperChunk <- CChunkedFileChunk <- CBaseClassNonCopyable <- CBaseClass
public:
    virtual ~C3DMeshFileTrianglesChunk();
    CArray<C3DTriangle2>  TriangleList;                         // +0x2C
};

C3DMeshFileTrianglesChunk::~C3DMeshFileTrianglesChunk()
{
}