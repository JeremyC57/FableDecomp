// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<set<unsigned long> >) STLport growth insert: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times) and the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@V?$allocator@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@2@@std@@IAEXPAV?$set@KU?$less@K@std@@V?$allocator@K@2@@2@ABV32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051C470, 286 bytes (+ int3 padding), thiscall, `ret 14h` (five arguments).
// Identity (proven): std::vector<set<unsigned long> >::_M_insert_overflow; evidence: its only caller is vector<set<unsigned long> >::_M_fill_insert 0x0051C920 (0x0051CA3F), the fill-insert of the landed vector<set<unsigned long> >::resize(n, x) 0x0051D030 / resize(n) 0x0051D240 chain; the element copy 0x00513700 is the set copy constructor (a 0x14-byte header node, _M_copy 0x005114F0) and 0x0051AC70 the 12-byte tree range destroy; both PDB name sets hold a vector of sets only as vector<set<unsigned long> > (FableWin.pdb ?resize@?$vector@V?$set@KU?$less@K@std@@...), never set<long>.
// Label: the manifest's "std::list_Cons_node_ReallocateAndCopy" is propagated from a byte-identical body elsewhere (family J085: the 286-byte growth inserts
//   of 12/20-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14; the element copy std::set<unsigned long>::set(const set&) 0x00513700 (FableWin.pdb ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z) (thiscall, `ret 4`; head, x and fill copies);
//   __uninitialized_copy<set<unsigned long>*, set<unsigned long>*>(first, last, result, const __false_type&) 0x00518F30 (the tail, __fastcall, `ret 8`); __destroy_aux<set<unsigned long>*>(first, last, const __false_type&) 0x0051AC70 (_M_clear's _Destroy, __fastcall, `ret 4`).
// Shape: STLport 4.6 vector.c verbatim (no EH): the head copy and the fill are inline loops around the out-of-line element copy
//   (placement new's null test), the tail copy stays out of line, and every __false_type tag shares the live atend slot (an
//   empty class with a user constructor).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    vector(const vector<_Tp, _Alloc>& __x);               // out of line
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
} // namespace std
namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity;
template <class _Value> struct _Rb_tree_node;
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {                                           // 12 bytes
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;  // +0x00
    unsigned int _M_node_count;                            // +0x04
    _Compare _M_key_compare;                               // +0x08
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    set(const set<_Key, _Compare, _Alloc>& __x);           // 0x00513700 for unsigned long (out of line)
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;   // +0x00
};
}
typedef std::set<unsigned long> TElement_0051c470;
template void std::vector<TElement_0051c470 >::_M_insert_overflow(TElement_0051c470*, const TElement_0051c470&, const std::__false_type&,
                                                               unsigned int, bool);