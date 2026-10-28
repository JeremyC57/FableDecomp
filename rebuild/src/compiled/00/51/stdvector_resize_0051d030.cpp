// std::vector::resize(size_type new_size, const value_type& x) -- (vector<set<unsigned long> >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@V?$allocator@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@2@@std@@QAEXIABV?$set@KU?$less@K@std@@V?$allocator@K@2@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051D030, 104 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<set<unsigned long> >::resize(unsigned int, const set<unsigned long>&); evidence: its only caller is the landed vector<set<unsigned long> >::resize(n) 0x0051D240 (FableWin.pdb ?resize@?$vector@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@...@std@@QAEXI@Z), which builds an empty set and passes it as x; its fill-insert 0x0051C920 grows through J085's vector<set<unsigned long> >::_M_insert_overflow 0x0051C470.
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J086: the
//   104-byte resize(n, x) of 12-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x005171B0 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x0051AC70 (bound only by the landed DestroyRange_Tree_0051ac70 shape name) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x0051C920 (insert(end(), n, x), thiscall, `ret 0Ch`).
// Shape: STLport 4.6 _vector.h `resize(n, x)` with erase(first, last) inline; the tag temporaries live in the dead new_size slot.
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { };
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
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
protected:
    public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // out of line
    protected:
};
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
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;   // +0x00
};
}
typedef std::set<unsigned long> TElement_0051d030;
template void std::vector<TElement_0051d030 >::resize(unsigned int, const TElement_0051d030&);