// std::vector<CMatrix3x4>::vector(size_type n) -- STLport sized constructor for 48-byte bone matrices: allocate n elements and copy-construct a default (uninitialised) matrix into each
// Leaf: ??0?$vector@VCMatrix3x4@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD8730, 117 bytes, thiscall, `ret 4`, returns this.
// Identity: callers CEnginePrimitiveManagerAnimatedMeshesBase::RenderDebugBoneHierarchy 0x00BD6855 and 0x00BD88A8 (the
//   animated-mesh bone-matrix buffers). The 48-byte element with an empty default constructor is taken to be Ego_r.pdb
//   CMatrix3x4 (vector<CMatrix3x4> is the 48-byte matrix vector of the engine; element name descriptive). Catalog
//   "MatrixBuffer_Allocate" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(n)).
// Shape: the same STLport `explicit vector(size_type n)` as the 32-byte twin 0x00BB9FD0: base constructor zero stores,
//   allocate (`n * 3 << 4`), start/finish/end, the __false_type fill loop inline (`rep movsd` x12 of the uninitialised
//   48-byte frame temporary, placement-new null test).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CMatrix3x4 {                                     // 48 bytes: a 3x4 bone matrix
public:
    float m[3][4];
    CMatrix3x4() { }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(size_type __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};

template vector<CMatrix3x4>::vector(size_type);

}   // namespace std