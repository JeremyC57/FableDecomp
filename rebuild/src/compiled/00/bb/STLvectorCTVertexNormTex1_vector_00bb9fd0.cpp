// _STL::vector<CTVertexNormTex1>::vector(size_type n) -- STLport sized constructor for 32-byte decal vertices: allocate n elements and copy-construct a default (uninitialised) vertex into each
// Leaf: ??0?$vector@VCTVertexNormTex1@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB9FD0, 117 bytes, thiscall, `ret 4`, returns this.
// Identity: callers CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup 0x00BB8C00,
//   CEnginePrimitiveManagerDecal::RenderLandscapeDecal 0x00BBB0F4 and 0x00BBA0B8: the landscape-decal vertex buffers.
//   The 32-byte element with an empty default constructor is taken to be Ego_r.pdb CTVertexNormTex1 (position, normal,
//   uv; the only 32-byte vertex type of the decal renderers) -- element name descriptive. Catalog
//   "std_vector_initialize_with_fill" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(n)).
// Shape: STLport 4.x `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }` -- the base constructor's three zero stores, `allocate(n)` (malloc or
//   null), start/finish/end; the __false_type fill loop inline (`rep movsd` of the uninitialised 32-byte frame
//   temporary, placement-new null test).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CTVertexNormTex1 {                               // 32 bytes: position, normal, texture coordinates
public:
    float X, Y, Z;
    float NX, NY, NZ;
    float U, V;
    CTVertexNormTex1() { }
};

namespace _STL {

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

template vector<CTVertexNormTex1>::vector(size_type);

}   // namespace _STL