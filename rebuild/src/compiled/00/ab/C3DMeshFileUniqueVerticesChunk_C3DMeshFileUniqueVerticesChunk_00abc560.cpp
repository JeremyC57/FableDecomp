// C3DMeshFileUniqueVerticesChunk::C3DMeshFileUniqueVerticesChunk() -- a "UNIV" CChunkedFileChunk with an empty UniqueVerticesList
// Leaf: ??0C3DMeshFileUniqueVerticesChunk@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC560, 34 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0129EC6C (RTTI .?AVC3DMeshFileUniqueVerticesChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb / Ego_r.pdb
//   ??0C3DMeshFileUniqueVerticesChunk@@QAE@XZ; callers the C3DMeshFileXLoader constructor 0x00A9BBE0; Ego_r.pdb member UniqueVerticesList (CArray<unsigned long>) at +0x10.
// Callees: CChunkedFileChunk::CChunkedFileChunk(const char*) 0x00AA7590 (literal 0x0129EB0C "UNIV").
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
class C3DMeshFileUniqueVerticesChunk : public CChunkedFileChunk {
public:
    C3DMeshFileUniqueVerticesChunk();
    CArray<unsigned long> UniqueVerticesList;                                    // +0x10
};
C3DMeshFileUniqueVerticesChunk::C3DMeshFileUniqueVerticesChunk()
    : CChunkedFileChunk("UNIV")
{
}