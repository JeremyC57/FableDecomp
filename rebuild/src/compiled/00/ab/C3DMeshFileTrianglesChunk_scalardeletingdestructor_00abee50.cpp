// C3DMeshFileTrianglesChunk::`scalar deleting destructor'(unsigned int flags) -- this class's vptr, free the TriangleList storage, the CChunkedFileSuperChunk destructor, then free this when bit 0 of flags is set
// Leaf: ??_GC3DMeshFileTrianglesChunk@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABEE50, 54 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0129ED34 (RTTI .?AVC3DMeshFileTrianglesChunk@@, bases CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ??_GC3DMeshFileTrianglesChunk@@UAEPAXI@Z and
//   ??1C3DMeshFileTrianglesChunk@@UAE@XZ; member TriangleList (CArray<C3DTriangle2>) at retail +0x2C.
// Callees: free 0x00BFEA14 (the CArray's inline destructor: the trivially destroyed elements fold away), CChunkedFileSuperChunk::~CChunkedFileSuperChunk
//   0x00AA8490, operator delete 0x00BFE9BC.
// Shape (family K051, representative 0x00A9D360): the class's own (empty) destructor, defined in this file, is inlined into
//   the ??_G (an in-class inline definition is not: ??_G then calls ??1 out of line): the class vptr, the member CArray's
//   STLport destructor, the base destructor out of line; a `new` of the class emits the vtable and the ??_G.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray<T>: an STLport vector<T>, 12 bytes in retail
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
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
class C3DTriangle2 { public: unsigned char _data[4]; };      // element (opaque here)
class C3DMeshFileTrianglesChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileTrianglesChunk() { }
    virtual ~C3DMeshFileTrianglesChunk();
    CArray<C3DTriangle2> TriangleList;   // +0x2C
};
C3DMeshFileTrianglesChunk::~C3DMeshFileTrianglesChunk()
{
}
C3DMeshFileTrianglesChunk* Make_00abee50() { return new C3DMeshFileTrianglesChunk; }