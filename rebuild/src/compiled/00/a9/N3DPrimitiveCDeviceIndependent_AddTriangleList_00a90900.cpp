// N3DPrimitive::CDeviceIndependent::AddTriangleList(const C3DMeshFileTrianglesChunk& chunk) -- build a new counted CTriangleList from a mesh file's triangles chunk (its triangle records and, when the chunk has an "XTST" sub-chunk, that strip's vertex indices) and append it to TriangleLists
// Leaf: ?AddTriangleList@CDeviceIndependent@N3DPrimitive@@AAEXABVC3DMeshFileTrianglesChunk@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90900, 471 bytes, private thiscall `ret 4`; no frame pointer, no SEH frame (the TU is built without /EHsc).
// Identity (two sources): Ego_r.pdb and FableWin.pdb both hold the mangled name above; the manifest label only lacks the namespace.
//   The public overload ?AddTriangleList@CDeviceIndependent@N3DPrimitive@@QAEXABV?$CIVCountedPointer@VCTriangleList@N3DPrimitive@@@@@Z (FableWin.pdb)
//   is the inline `TriangleLists.push_back(triangle_list)` that retail expands at the end of this body.
// Callees: operator new 0x00BFEA1A (CTriangleList, 0x20 bytes, inline ctor: IVRefCount 1, vtable 0x0129DF7C, two empty arrays),
//   C3DMeshFileTrianglesChunk::GetList() const 0x00ABC440, C3DMeshFileTrianglesChunk::GetCount() const 0x00ABA370,
//   std::vector<C3DTriangle2>::resize(n, const C3DTriangle2&) 0x00A97A50 (out of line), CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) const
//   0x00AA7F40 (literal "XTST" @ 0x0129DF60), the count block's DeleteFunc (`call [Info+4]`), operator delete 0x00BFE9BC, memmove 0x00BFEAE6
//   (STLport __copy_trivial inside the expanded vector<unsigned short>::erase), std::vector<unsigned short>::_M_fill_insert 0x005149F0,
//   std::vector<CIVCountedPointer<CTriangleList> >::_M_insert_overflow(pos, x, __false_type, 1, true) 0x00A97BE0, the pointee's slot 1 (Delete).
// Layout: shared engine headers for the chunk classes, CCountedPointer, CIVCountedPointer / CIVCountedPointeeBase, CArray and C3DTriangle2.
//   N3DPrimitive::CTriangleList (0x20: Triangles +8, StripIndices +0x14) and N3DPrimitive::CDeviceIndependent (0x50: TriangleLists +0x10)
//   have no shared header yet (Groups +0x1C is CArray<CCountedPointer<C3DGroup2> > per Ego_r.pdb and FableWin.pdb AddGroup); they are modelled here with the Ego_r member names and the FableWin inline accessor names
//   (ResizeTriangleList / SetTriangle / ResizeStrip / SetStripIndex; the strip chunk's GetCount / GetStripVertex / GetStaticChunkType).
// Shape: the list pointer adopts the new object (no add-ref) and lives in a frame slot because push_back takes its address; the
//   never-written C3DTriangle2() default value of the one-argument resize shares the frame slot that holds `this`, which VC7.1 only does
//   when it sees STLport's _M_fill_insert body (FABLE_RETAIL_STLP_VECTOR: _vector.h includes _vector.c in the retail TU); declaration-only
//   it packs that temporary with the strip pointer instead and the frame is 0x1C, not 0x20. The strip pointer is an empty local filled by
//   SetStaticCast (ShareData against an empty pair: only the add-ref remains), released before the list pointer at the end.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS
#define FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK
#define FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK_ACCESSORS
#define FABLE_RETAIL_C3DGROUP2
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DMeshFileTrianglesChunk.h"
#include "engine/C3DMeshFileXTriangleStripChunk.h"
#include "engine/C3DGroup2.h"
class C3DVertex2;
namespace N3DPrimitive {
class CTriangleList : public CIVCountedPointeeBase {                 // vtable 0x0129DF7C, 0x20 bytes
public:
    CTriangleList() { }
    void ResizeTriangleList(unsigned long count) { Triangles.resize(count); }
    void SetTriangle(unsigned long index, const C3DTriangle2& triangle) { Triangles[index] = triangle; }
    void ResizeStrip(unsigned long count) { StripIndices.resize(count); }
    void SetStripIndex(unsigned long index, unsigned short vertex) { StripIndices[index] = vertex; }
private:
    CArray<C3DTriangle2>   Triangles;      // +0x08
    CArray<unsigned short> StripIndices;   // +0x14
};
class CDeviceIndependent : public CBaseClassNonCopyable {            // vtable 0x0129DFA8, 0x50 bytes
public:
    void AddTriangleList(const CIVCountedPointer<CTriangleList>& triangle_list) { TriangleLists.push_back(triangle_list); }
private:
    void AddTriangleList(const C3DMeshFileTrianglesChunk& chunk);
    CArray<C3DVertex2>                        Vertices;        // +0x04
    CArray<CIVCountedPointer<CTriangleList> > TriangleLists;   // +0x10
    CArray<CCountedPointer<C3DGroup2> >       Groups;          // +0x1C (Ego_r.pdb; FableWin.pdb ?AddGroup@CDeviceIndependent@N3DPrimitive@@QAEXAAV?$CCountedPointer@VC3DGroup2@@@@@Z)
    C3DBoundingSphere                         BoundingSphere;  // +0x28
    CArray<C3DVector>                         UTangents;       // +0x38
    CArray<C3DVector>                         VTangents;       // +0x44
};
void CDeviceIndependent::AddTriangleList(const C3DMeshFileTrianglesChunk& chunk)
{
    CIVCountedPointer<CTriangleList> triangle_list(new CTriangleList);
    const C3DTriangle2* triangles = chunk.GetList();
    unsigned long count = chunk.GetCount();
    triangle_list->ResizeTriangleList(count);
    for (unsigned long i = 0; i < count; ++i)
        triangle_list->SetTriangle(i, triangles[i]);
    CCountedPointer<C3DMeshFileXTriangleStripChunk> strip;
    strip.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXTriangleStripChunk::GetStaticChunkType()));
    if (strip.Get()) {
        unsigned long strip_count = strip->GetCount();
        triangle_list->ResizeStrip(strip_count);
        for (unsigned long j = 0; j < strip_count; ++j)
            triangle_list->SetStripIndex(j, (unsigned short)strip->GetStripVertex(j));
    }
    AddTriangleList(triangle_list);
}
}