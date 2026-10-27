// C3DMeshFileTrianglesChunk::C3DMeshFileTrianglesChunk() -- a "TRIS" CChunkedFileSuperChunk with an empty TriangleList
// Leaf: ??0C3DMeshFileTrianglesChunk@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC860, 34 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0129ED34 (RTTI .?AVC3DMeshFileTrianglesChunk@@ : CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb / Ego_r.pdb
//   ??0C3DMeshFileTrianglesChunk@@QAE@XZ; callers the C3DMeshFileXLoader constructors 0x00A9BBE0 / 0x00AA5C80 and COptimisedPrimitive::CopyTo 0x00ACC490; Ego_r.pdb member TriangleList (CArray<C3DTriangle2>) at +0x2C.
// Callees: CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) 0x00AA8360 (literal 0x0129DF28 "TRIS").
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
class CChunkedFileSuperChunk : public CChunkedFileChunk {   // retail 0x2C bytes
public:
    virtual ~CChunkedFileSuperChunk();
protected:
    CChunkedFileSuperChunk(const char* chunkType);       // 0x00AA8360
    unsigned char SubChunks[12];                         // +0x10 CArray<CCountedPointer<CChunkedFileChunk> > (built by the base)
    unsigned char RegisteredSubChunkTypes[12];           // +0x1C CArray<CCountedPointer<CChunkedFileChunk> >
    unsigned long CurrentChunkIndex;                     // +0x28
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
};
}
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray<T>: an STLport vector<T>, 12 bytes in retail
class C3DTriangle2;
class C3DMeshFileTrianglesChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileTrianglesChunk();
    CArray<C3DTriangle2> TriangleList;                                    // +0x2C
};
C3DMeshFileTrianglesChunk::C3DMeshFileTrianglesChunk()
    : CChunkedFileSuperChunk("TRIS")
{
}