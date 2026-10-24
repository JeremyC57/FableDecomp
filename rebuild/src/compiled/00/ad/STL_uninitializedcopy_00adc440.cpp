// _STL::__uninitialized_copy(const CVertexGroupBlock* first, const CVertexGroupBlock* last, CVertexGroupBlock* result, const __false_type&) -- copy-construct a run of vertex-group blocks (group vector + group -> index map) into raw storage
// Flags: /Gr
// Leaf: ??$__uninitialized_copy@PBVCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC440, 0x7D bytes, __fastcall (first ecx, last edx),
// `ret 8` (result, the __false_type reference), returns the end of the constructed run.
// Identity: the 0x18-byte element is Ego_r.pdb CVertexGroupBlock (Groups vector<unsigned long> +0, GroupToIndexMap
// map<unsigned long, long> +0xC). Callers: vector<CVertexGroupBlock>::operator= 0x00ADCA30 (twice: _M_allocate_and_copy
// and the tail copy, both over `(const_pointer)__x._M_start` ranges) and the functions at 0x00ADC5DC / 0x00ADC818,
// so this is the <const T*, T*> instantiation; the byte-identical 0x00ADC330 is the <T*, T*> one used by reserve and
// _M_insert_overflow. Catalog "Vector_CopyConstructor_6Way_List" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(n)), memmove @ 0x00BFEAE6 (__copy_trivial of the group words),
// _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_Rb_tree(const _Rb_tree&) @ 0x00A96E40.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` (placement-new null test) with
// CVertexGroupBlock's implicit copy constructor: the vector copy constructor inline (`_Vector_base(x.size(), a)` zeroes
// the three words, allocate (null when empty), `__copy_trivial` = `last == first ? result : memmove(...) + n`), then
// the map copy constructor forwarding to the out-of-line tree copy constructor.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace _STL {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
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

} // namespace _STL

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    _STL::vector<unsigned long>    Groups;                  // +0x00
    _STL::map<unsigned long, long> GroupToIndexMap;         // +0x0C
};

namespace _STL {
template CVertexGroupBlock* __uninitialized_copy<const CVertexGroupBlock*, CVertexGroupBlock*>(
    const CVertexGroupBlock*, const CVertexGroupBlock*, CVertexGroupBlock*, const __false_type&);
} // namespace _STL