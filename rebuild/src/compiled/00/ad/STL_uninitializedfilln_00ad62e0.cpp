// _STL::__uninitialized_fill_n(CUniqueVertex* first, unsigned int n, const CUniqueVertex& x, const __false_type&) -- copy-constructs n unique vertices from x into raw storage and returns the end
// Leaf: ??$__uninitialized_fill_n@PAVCUniqueVertex@QuadricOptimiserInternals@@IV12@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD62E0, 65 bytes, __fastcall (ecx = first, edx = n), `ret 8` (x, the tag).
// Identity: STLport 4.x `__uninitialized_fill_n` (non-POD path) over QuadricOptimiserInternals::CUniqueVertex (0x1C
//   bytes); callers 0x00AD6BAA, 0x00AD7D1A, 0x00AD7F4D (vector<CUniqueVertex> fill / insert paths).
// Callees: the set<unsigned long> (_Rb_tree) copy constructor 0x00AD3A10 (twice per element, the two CIndexList
//   members; shared COMDAT of every set<unsigned long> copy in the image).
// Layout: CUniqueVertex (Ego_r.pdb member order, 12-byte STLport sets): bool Exists +0, CIndexList VertexIndices +4,
//   CIndexList FaceList +0x10 (CIndexList = { set<unsigned long> Indices }).
// Shape: STLport verbatim `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return __cur;` (exceptions off):
//   `test edx,edx ; jbe` for the unsigned count, the placement new's null test, then CUniqueVertex's implicit copy
//   constructor (the Exists byte, then the two set copies; edi = cur + 0x10 with the first member at edi - 0xC).
// Label correction: catalogued as `vector<char>::_Ufill` (propagated); the body is
//   _STL::__uninitialized_fill_n<CUniqueVertex*, unsigned int, CUniqueVertex>.
inline void* operator new(unsigned int, void* __p) { return __p; }

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp> struct less { };
template <class _Tp> struct _Identity { };
template <class _Tp> class allocator {
public:
    allocator() { }
};

struct _Rb_tree_node_base;

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    _Rb_tree_node_base* _M_header;                                  // +0x00
    unsigned int        _M_node_count;                              // +0x04
    _Compare            _M_key_compare;                             // +0x08
    _Rb_tree(const _Rb_tree& __x);                                  // 0x00AD3A10 for set<unsigned long>
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    set(const set& __x) : _M_t(__x._M_t) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    _STL::set<unsigned long> Indices;                               // +0x00
};

class CUniqueVertex {
public:
    bool       Exists;                                              // +0x00
    CIndexList VertexIndices;                                       // +0x04
    CIndexList FaceList;                                            // +0x10
};
}

template QuadricOptimiserInternals::CUniqueVertex* __fastcall
_STL::__uninitialized_fill_n<QuadricOptimiserInternals::CUniqueVertex*, unsigned int, QuadricOptimiserInternals::CUniqueVertex>(
    QuadricOptimiserInternals::CUniqueVertex*, unsigned int, const QuadricOptimiserInternals::CUniqueVertex&,
    const _STL::__false_type&);