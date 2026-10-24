// vector<CRegion>::vector(unsigned int n) -- STLport sized constructor: allocate n world-map regions and copy-construct each from one default-constructed CRegion temporary
// Leaf: ??0?$vector@VCRegion@@
//
// Retail 0x0051BEC0, 122 bytes (then int3 padding), thiscall `ret 4`; 0x58-byte frame for the CRegion temporary.
// Callees: 0x00BFEA0E malloc, 0x006BC410 CRegion::CRegion() (the temporary), 0x0051A900 CRegion::CRegion(const CRegion&)
//   (per element, after the placement-new null test), 0x0051A110 CRegion::~CRegion() (the temporary).
// Callers: none by direct call (an explicit instantiation kept by the linker; the landed 0x006BC410 notes it).
// Element: CRegion is 0x58 bytes in retail (Ego_r.pdb's 100 minus the Dinkumware vector/map overhead; see the landed
//   0x006BC410 / 0x0051A110 / 0x0051BA50 layouts), so the PDB engine header does not apply; the local model only needs
//   its size and its three out-of-line special members.
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish = uninitialized_fill_n(
//   _M_start, n, _Tp()); }`: _Vector_base zeroes the triple (one xor), allocates `n ? malloc(n * 0x58) : 0` and sets
//   end of storage; the CRegion temporary is constructed after the allocation and destroyed after the fill. Unlike the
//   CRGBColour instance (0x005179C0) the fill is inline here: `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` (the
//   unsigned `jbe`), with the temporary's address (the constructor's eax) held in ebp.
// Label: catalog "StdVector_AllocateConstructCRegion" (descriptive).
extern "C" void* __cdecl malloc(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CRegion {
public:
    unsigned char Body[0x58];                   // Maps, VisMaps, names, PDef, mini-map data, exit-text map, offsets, flags
    CRegion();                                  // 0x006BC410
    CRegion(const CRegion& other);              // 0x0051A900
    ~CRegion();                                 // 0x0051A110
};

namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T> class vector : public _Vector_base<T> {
public:
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
}

template std::vector<CRegion>::vector(unsigned int);