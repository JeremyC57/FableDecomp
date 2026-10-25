// std::vector<C3DKeyframe>::vector(unsigned int n) -- size constructor: allocate n keyframes and fill them with copies of a default-constructed keyframe
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A54580, length 117 bytes, `ret 4`.
// Callers: 0x00A4F082 and 0x00A54F08 (C3DAnimation keyframe arrays). Callee: malloc 0x00BFEA0E.
// Identity: catalogued as `Vector_InitialiseAsConstant_C3DKeyframe`. Element (Ego_r.pdb): C3DKeyframe = Rotation
//   CQuaternion +0, Position C4DVector +0x10, Scaling C4DVector +0x20 (48 bytes; the default constructors store nothing,
//   the copy is trivial).
// Shape: STLport 4.x `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }`: the base's init list zeroes the three words with immediates, then
//   `_M_start = allocate(n)` (`n != 0 ? malloc(n * 48) : 0`), `_M_finish = _M_start`, `_M_end_of_storage = _M_start + n`;
//   the `_Tp()` temporary lives in a 0x30-byte frame slot and is never initialised; the fill (unsigned `n > 0`, placement
//   null test) copies it with `rep movsd` (12 dwords); the advanced pointer is stored as the finish.
extern "C" void* __cdecl malloc(unsigned int);       // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CQuaternion {
public:
    float X, Y, Z, W;
    CQuaternion() { }
};
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
};
class C3DKeyframe {                                  // 48 bytes
public:
    CQuaternion Rotation;                            // +0x00
    C4DVector   Position;                            // +0x10
    C4DVector   Scaling;                             // +0x20
};

namespace std {
struct __false_type { __false_type() { } };          // STLport _IS_POD_ITER result for a class element

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(unsigned int __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};

template vector<C3DKeyframe>::vector(unsigned int);
}