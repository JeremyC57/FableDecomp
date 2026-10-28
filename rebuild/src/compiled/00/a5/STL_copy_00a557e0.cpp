// std::copy(set<CCharString>::const_iterator first, set<CCharString>::const_iterator last, back_insert_iterator<vector<CCharString> > result) -- append every string of a set range to a vector (push_back per element), return the output iterator
// Grade: RELOCATION_MATCH (agent902, 1st grade of this pass) -- the set iterator is STLport 4.6's const _Rb_tree_iterator<_Value, _Const_traits<_Value> >, whose COPY constructor is implicit (its only user ctor converts from the _Nonconst_traits iterator); the user copy ctor of the parked model kept VC7.1 from shrink-wrapping the `push edi` into the non-empty path.
// Leaf: ??$copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A557E0, length 154 bytes (+6 int3 pad), `ret 0Ch`.
// Caller: 0x00A4EFB0. Callees: CCharString copy ctor 0x0099EC30 (push_back's placement copy), 0x00433530
//   vector<CCharString>::_M_insert_overflow (the full-storage arm).
// Identity: catalogued as `BinaryTreeTraversal_Insert`. The source nodes hold a CCharString at +0x10 (STLport set node:
//   color +0, parent +4, left +8, right +0xC, value +0x10) and the destination elements are 4-byte CCharStrings: STLport
//   `copy(first, last, back_inserter(v))` with `__copy(..., input_iterator_tag(), (ptrdiff_t*)0)` inlined (no tag
//   arguments, `ret 0Ch` = the three by-value iterators).
// Shape: /Gr TU `__fastcall` template returning the 4-byte back_insert_iterator (user ctor) through the hidden pointer
//   in ecx (ebp), the three iterators on the stack (first in esi, last in ebx, the container pointer in edi);
//   `for (; first != last; ++result, ++first) *result = *first;` with push_back inlined (finish compared with the end
//   of storage in memory, placement copy with its null test, `add [edi+4],4`; the overflow arm's empty tag homed in the
//   dead `result` slot) and the tree iterator's _M_increment inlined (right subtree's leftmost, else climb while a right
//   child, then the header guard); the empty range returns `result` straight from its slot.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
};

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary
struct input_iterator_tag { input_iterator_tag() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00433530
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Container>
class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

struct _Rb_tree_node_base {
    char                _M_color;                    // +0x00
    _Rb_tree_node_base* _M_parent;                   // +0x04
    _Rb_tree_node_base* _M_left;                     // +0x08
    _Rb_tree_node_base* _M_right;                    // +0x0C
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};

template <class _Dummy>
class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node);
};

template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Rb_tree_node_base* __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};

template <class _Tp> struct _Const_traits {
    typedef _Tp        value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp        value_type;
    typedef _Tp&       reference;
    typedef _Tp*       pointer;
};

// STLport 4.6 tree iterator: set<T>::iterator and ::const_iterator are both the _Const_traits instantiation, so its
// copy constructor is the IMPLICIT one (the user constructor below converts from the _Nonconst_traits iterator).
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _InputIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&, _Distance*)
{
    for ( ; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, input_iterator_tag(), (int*)0);
}

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}

template back_insert_iterator<vector<CCharString> > __fastcall copy(_Rb_tree_iterator<CCharString, _Const_traits<CCharString> >,
                                                                     _Rb_tree_iterator<CCharString, _Const_traits<CCharString> >,
                                                                     back_insert_iterator<vector<CCharString> >);
}