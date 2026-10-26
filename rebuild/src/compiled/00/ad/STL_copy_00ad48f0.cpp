// std::__copy(_InputIter first, _InputIter last, _OutputIter result, const input_iterator_tag&, _Distance*) -- STLport input-iterator copy of a set<unsigned long> range through an insert_iterator into another set: assign each element to the inserter, step the source iterator, return the inserter
// Leaf: ??$__copy@U?$_Rb_tree_iterator@KU?$_Const_traits@K@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD48F0, 111 bytes (+int3), /Gr __fastcall `ret 14h`: the
//   insert_iterator result through the hidden pointer in ecx (it has a user constructor), the tag reference in edx,
//   then first, last, the 8-byte inserter by value and the distance pointer on the stack.
// Identity: callers 0x00AD5BAF (__copy_aux(first, last, result, const __false_type&) 0x00AD5B90, `ret 10h`, passing
//   a null distance pointer) and 0x00AD63DF, 0x00AD6D3F, 0x00AD6D5C, 0x00AD6EDA, 0x00AD6EF7. The loop body calls the
//   set<unsigned long> inserter's operator= 0x00AD4810 on the by-value result slot with &node->value (node +0x10).
//   Catalog label "TreeWalk_Destroy" is descriptive and wrong.
// Shape: STLport `for ( ; first != last; ++result, ++first) *result = *first; return result;` -- the inserter's
//   operator= stays out of line (same-TU noinline body), ++result is empty, ++first inlines
//   `_Rb_global<bool>::_M_increment` on the enregistered node; the inserter is copied out of its argument slot.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo { public:
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace QuadricOptimiserInternals {
class CVertexPairContraction;
class CContractionReference {
public:
    CCountedPointer<CVertexPairContraction> Contraction;   // +0
    unsigned long                           SubIndex;      // +8
};
}

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool __fastcall operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                         // 0x0042971D
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   // 0x0042A34E
    static _Base_ptr __cdecl _M_increment(_Base_ptr _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    void _M_deallocate_node(_Link_type __p) { if (__p != 0) free(__p); }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }

    iterator insert_unique(iterator __position, const value_type& __x);
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    pair<iterator, bool> insert_unique(const value_type& __x);
    _Base_ptr _M_lower_bound(const key_type& __k) const;
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        __y->_M_value_field.~value_type();
        _M_deallocate_node(__y);
        --_M_node_count;
    }
};


template <class _Key, class _Compare, class _Alloc> class set {
public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Identity<value_type>, key_compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;                                     // +0x00
    iterator insert(iterator __position, const value_type& __x)
    {
        typedef typename _Rep_type::iterator _Rep_iterator;
        return _M_t.insert_unique((_Rep_iterator&)__position, __x);
    }
};

struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };

template <class _Container> class insert_iterator {
protected:
    _Container* container;                              // +0x00
    typename _Container::iterator iter;                 // +0x04
public:
    typedef _Container container_type;
    typedef output_iterator_tag iterator_category;
    insert_iterator(_Container& __x, typename _Container::iterator __i) : container(&__x), iter(__i) { }
    __declspec(noinline) insert_iterator<_Container>& operator=(const typename _Container::value_type& __val)
    {
        iter = container->insert(iter, __val);
        ++iter;
        return *this;
    }
    insert_iterator<_Container>& operator*() { return *this; }
    insert_iterator<_Container>& operator++() { return *this; }
    insert_iterator<_Container>& operator++(int) { return *this; }
};

}   // namespace std

namespace std {
template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result,
                              const input_iterator_tag&, _Distance*)
{
    for ( ; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}
}   // namespace std

typedef std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > TIndexSet;
template std::insert_iterator<TIndexSet> __fastcall std::__copy(TIndexSet::const_iterator, TIndexSet::const_iterator,
                                                                  std::insert_iterator<TIndexSet>,
                                                                  const std::input_iterator_tag&, int*);