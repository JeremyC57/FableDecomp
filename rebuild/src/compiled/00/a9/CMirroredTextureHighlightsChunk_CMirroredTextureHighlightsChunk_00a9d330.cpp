// CMirroredTextureHighlightsChunk::~CMirroredTextureHighlightsChunk() -- the user destructor (empty body): reset the class vtable, free the TriangleVertexIndexList block, then ~CChunkedFileChunk
// Leaf: ??1CMirroredTextureHighlightsChunk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D330, 33 bytes, thiscall, tail `jmp ~CChunkedFileChunk`.
// Identity (proven): vtable 0x0129E0E0 stored first (RTTI .?AVCMirroredTextureHighlightsChunk@@ : CChunkedFileChunk :
//   CBaseClassNonCopyable : CBaseClass); its slot-0 `??_G` 0x00A9D360 (landed, the same out-of-line `{}` model) inlines this
//   body. FableWin.pdb ??1CMirroredTextureHighlightsChunk@@UAE@XZ (0x033D3F70) sits between the class's ctor and
//   GetChunkDescription, as retail's copy sits after GetChunkDescription 0x00A9D310 and before the ??_G. Ego_r.pdb member
//   TriangleVertexIndexList (CArray<unsigned long>) at +0x10. No callers (the ??_G has its own copy). The catalog label
//   "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0.
// Shape: the landed ??_G 0x00A9D360 model: an empty destructor defined out of line in this file (vtable store), the member
//   CArray's STLport destructor, then the base destructor as the tail jump.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {   // vtable 0x0129E474, 0x10 bytes
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char ChunkType[5];                                      // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0c
};
class CMirroredTextureHighlightsChunk : public CChunkedFileChunk {   // vtable 0x0129E0E0, 0x1c bytes
public:
    CMirroredTextureHighlightsChunk();                      // 0x00A9D2E0 (CChunkedFileChunk ctor 0x00AA7590 + the zeroed array)
    virtual ~CMirroredTextureHighlightsChunk();
    CArray<unsigned long> TriangleVertexIndexList;          // +0x10
};

CMirroredTextureHighlightsChunk::~CMirroredTextureHighlightsChunk()
{
}