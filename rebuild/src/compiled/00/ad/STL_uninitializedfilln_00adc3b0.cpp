// std::__uninitialized_fill_n(CVertexGroupBlock* first, unsigned int n, const CVertexGroupBlock& x, const __false_type&) -- copy-construct n copies of one vertex-group block (group vector + group -> index map) into raw storage
// Flags: /Gr
// Leaf: ??$__uninitialized_fill_n@PAVCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC3B0, 0x89 bytes (+ int3 padding), __fastcall (first ecx, n edx), `ret 8`
// (x, the __false_type reference), returns the end of the constructed run.
// Identity: same element and copy constructor as the sibling __uninitialized_copy pair 0x00ADC330 / 0x00ADC440 --
// Ego_r.pdb CVertexGroupBlock (Groups vector<unsigned long> +0, GroupToIndexMap map<unsigned long, long> +0xC) --
// the fill_len != 1 path of vector<CVertexGroupBlock>::_M_insert_overflow. Catalog "Vector_Copy_UninitRange" is
// descriptive.
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6, _Rb_tree<unsigned long, pair<const unsigned long, long>,
// ...>::_Rb_tree(const _Rb_tree&) @ 0x00A96E40.
// Shape: STLport `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` with an unsigned count (`jbe`, homed in
// the `push ecx` frame word, decremented in memory) and ebp as the zero register for the placement-new test;
// CVertexGroupBlock's implicit copy constructor inline (vector copy with __copy_trivial, out-of-line tree copy).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class T> class allocator {                        // STLport allocator<T>: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    _Vector_base(unsigned int n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> class vector : public _Vector_base<T> {  // used here for a POD element only (__true_type copy)
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = (T*)__copy_trivial(x._M_start, x._M_finish, this->_M_start);
    }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct _Rb_tree_node_base;

template <class K, class V, class Compare> class _Rb_tree {
public:
    _Rb_tree_node_base* _M_header;                          // +0x00 (alloc proxy)
    unsigned int        _M_node_count;                      // +0x04
    Compare             _M_key_compare;                     // +0x08
    _Rb_tree(const _Rb_tree& __x);                          // 0x00A96E40
};

template <class K, class T, class Compare = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    _Rb_tree<K, value_type, Compare> _M_t;
    map(const map& __x) : _M_t(__x._M_t) { }
};

} // namespace std

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    std::vector<unsigned long>    Groups;                  // +0x00
    std::map<unsigned long, long> GroupToIndexMap;         // +0x0C
};

namespace std {
template CVertexGroupBlock* __uninitialized_fill_n<CVertexGroupBlock*, unsigned int, CVertexGroupBlock>(
    CVertexGroupBlock*, unsigned int, const CVertexGroupBlock&, const __false_type&);
} // namespace std