// QuadricOptimiserInternals::CIndexList::GetIntersection(const CIndexList& other) const -- the indices present in both lists: STLport set_intersection of the two sets into an inserter over a fresh list, returned by value
// Leaf: ?GetIntersection@CIndexList@QuadricOptimiserInternals@@QBE
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC8960, 196 bytes, thiscall `ret 8` (hidden CIndexList result + other).
// Identity: Ego_r.pdb `?GetIntersection@CIndexList@QuadricOptimiserInternals@@QBE?AV12@ABV12@@Z` (with GetUnion and
//   GetSymmetricDifference, the class's only by-value set operations). The algorithm called is the 7-dword /Gr
//   `__set_intersection(first1, last1, first2, last2, insert_iterator, less<unsigned long>)` 0x00AD59D0 -- it makes a
//   single inserter call (0x00AD5A91 -> insert_iterator::operator= 0x00AD4810) and has no copy tail, unlike the
//   union 0x00AD6BC0 (three inserter calls + two __copy tails, used by 0x00ACA2B0) and the symmetric difference
//   0x00AD6D70 (two + two, used by 0x00ACA380). Caller 0x00AC9B10. Catalog label "BinaryTree_Copy_Simple" is
//   descriptive and wrong.
// Callees: malloc @ 0x00BFEA0E (the local set's header), __set_intersection @ 0x00AD59D0, the set<unsigned long> tree
//   copy constructor @ 0x00AD3A10 (no NRVO: the local is copied into the hidden result), its _M_erase @ 0x004CF810 and
//   free @ 0x00BFEA14 (the local's destructor: clear() then the header deallocate).
// Layout: CIndexList {set<unsigned long> Indices +0} = STLport _Rb_tree {header +0, count +4, less +8}, 12 bytes;
//   frame: the discarded inserter result at +0, the local list at +8.
// Shape: `CIndexList result; set_intersection(Indices.begin(), Indices.end(), other.Indices.begin(),
//   other.Indices.end(), inserter(result.Indices, result.Indices.begin())); return result;` with STLport's
//   set_intersection inlined down to `__set_intersection(..., __less(value_type*))`; exceptions off.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline less<_Tp> __less(_Tp*) { return less<_Tp>(); }
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
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

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Value value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }

    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0) { _M_empty_initialize(); }
    _Rb_tree(const _Rb_tree& __x);                      // 0x00AD3A10 for set<unsigned long>
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // red: tells the header from the root in operator++
        _M_root() = 0;
        _M_leftmost() = (_Link_type)this->_M_header._M_data;
        _M_rightmost() = (_Link_type)this->_M_header._M_data;
    }
    void _M_erase(_Link_type __x);                      // 0x004CF810 for set<unsigned long>
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = (_Link_type)this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = (_Link_type)this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    iterator begin() { return iterator(_M_leftmost()); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    iterator end() { return iterator((_Link_type)this->_M_header._M_data); }
    const_iterator end() const { return const_iterator((_Link_type)this->_M_header._M_data); }
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
    set() : _M_t() { }
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
};

struct output_iterator_tag { output_iterator_tag() { } };

template <class _Container> class insert_iterator {
protected:
    _Container* container;                              // +0x00
    typename _Container::iterator iter;                 // +0x04
public:
    typedef _Container container_type;
    typedef output_iterator_tag iterator_category;
    insert_iterator(_Container& __x, typename _Container::iterator __i) : container(&__x), iter(__i) { }
    insert_iterator<_Container>& operator=(const typename _Container::value_type& __val);   // 0x00AD4810
};

template <class _Container, class _Iterator>
inline insert_iterator<_Container> __fastcall inserter(_Container& __x, _Iterator __i)
{
    typedef typename _Container::iterator __iter;
    return insert_iterator<_Container>(__x, __iter(__i));
}

}   // namespace std

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > Indices;   // +0x00
    CIndexList GetUnion(const CIndexList& other) const;
    CIndexList GetIntersection(const CIndexList& other) const;
    CIndexList GetSymmetricDifference(const CIndexList& other) const;
};
}

namespace std {
template <class _InputIter1, class _InputIter2, class _OutputIter, class _Compare>
_OutputIter __fastcall __set_intersection(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2,
                                          _InputIter2 __last2, _OutputIter __result, _Compare __comp);   // 0x00AD59D0
template <class _InputIter1, class _InputIter2, class _OutputIter>
inline _OutputIter __fastcall set_intersection(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2,
                                               _InputIter2 __last2, _OutputIter __result)
{
    return __set_intersection(__first1, __last1, __first2, __last2, __result,
                              __less((typename _InputIter1::value_type*)0));
}
}   // namespace std

namespace QuadricOptimiserInternals {
CIndexList CIndexList::GetIntersection(const CIndexList& other) const
{
    CIndexList result;
    std::set_intersection(Indices.begin(), Indices.end(), other.Indices.begin(), other.Indices.end(),
                           std::inserter(result.Indices, result.Indices.begin()));
    return result;
}
}