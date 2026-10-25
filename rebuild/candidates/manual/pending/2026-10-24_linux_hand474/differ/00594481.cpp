// std::_Rb_tree<CDateAndTime, pair<const CDateAndTime, CWideString>, ...>::insert_unique(iterator position, const value_type& v) -- the STLport hinted unique insert: inserts next to the hint when the key falls just before / after it, returns the hint for an equal key, otherwise falls back on the unhinted insert
// Grade: DIFFER(423v423) -- one register choice: the end() branch loads the rightmost node into ecx and adds 0x10 for the key (`mov ecx,[eax+0xc] ; add ecx,0x10`); ours loads it into eax and uses `lea ecx,[eax+0x10]`; every other byte matches
// Leaf: ?insert_unique@?$_Rb_tree@VCDateAndTime@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594481, 423 bytes (0x1A7, no padding: /Os TU), thiscall `ret 0Ch` (hidden iterator
//   result, the hint by value, the value by reference). One caller: the map's insert(iterator, const value_type&)
//   wrapper 0x0059483E.
// Identity: STLport 4.6 _tree.c `_Rb_tree::insert_unique(iterator, const _Value&)`, instantiated for a
//   map<CDateAndTime, CWideString> (the catalog's "CMemoryAllocatorVariableSize_TransferIn_DateAndTime" is wrong; the
//   neighbours 0x00593CE4 / 0x00593D76 are this tree's _M_insert / insert_unique(v)).
// Pragmas: `#pragma optimize("s", on)` + `#pragma optimize("y", off)` (frame pointer and `leave`, no alignment padding).
// Callees: CDateAndTime::operator< 0x009E9B80 (less<CDateAndTime>, thiscall on the left key), _Rb_global<bool>::
//   _M_increment 0x004292C0 / _M_decrement 0x004294E2 (cdecl), _Rb_tree::_M_insert 0x00593CE4 (hidden iterator result,
//   x, y, v, w = 0), _Rb_tree::insert_unique(const value_type&) 0x00593D76 (hidden pair result).
// Layout: STLport _Rb_tree: header node pointer +0x00, node count +0x04, key compare +0x08; nodes: left +0x08,
//   right +0x0c, value +0x10 (the CDateAndTime key first).
// Shape: the _tree.c body verbatim; VC7.1 homes `__comp_v_pos` in the dead high byte of the hint's argument slot and
//   `__after` in the slot itself (the hint lives in esi).
#pragma optimize("s", on)
#pragma optimize("y", off)

class CDateAndTime {
public:
    long Date;                                                                         // +0x00
    long Time;                                                                         // +0x04
    bool operator<(const CDateAndTime& other) const;                                   // 0x009E9B80
};

class CWideString {
public:
    void* PStringData;
};

namespace std {

template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _K, class _V> struct pair_kv {
    typedef _K first_type;
    _K first;
    _V second;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                                                // +0x00
    _Base_ptr _M_parent;                                                               // +0x04
    _Base_ptr _M_left;                                                                 // +0x08
    _Base_ptr _M_right;                                                                // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                             // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr);                                          // 0x004292C0
    static _Base_ptr _M_decrement(_Base_ptr);                                          // 0x004294E2
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
    void _M_decrement() { _M_node = _Rb_global<bool>::_M_decrement(_M_node); }
};

template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_increment(); return *this; }
    _Self& operator--() { _M_decrement(); return *this; }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Value value_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    size_type _M_node_count;                                                           // +0x04
    _Compare  _M_key_compare;                                                          // +0x08

    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return (_Base_ptr&)(__x->_M_right); }
    static _Value& _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(__x)); }
    size_type size() const { return _M_node_count; }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00593CE4
    pair<iterator, bool> insert_unique(const value_type& __v);                         // 0x00593D76
    iterator insert_unique(iterator __position, const value_type& __v);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const _Value& __v)
{
    if (__position._M_node == this->_M_header._M_data->_M_left) { // begin()

        // if the container is empty, fall back on insert_unique.
        if (size() <= 0)
            return insert_unique(__v).first;

        if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        // first argument just needs to be non-null
        else {
            bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));

            if (__comp_pos_v == false)  // compare > and compare < both false so compare equal
                return __position;
            //Below __comp_pos_v == true

            // Standard-conformance - does the insertion point fall immediately AFTER
            // the hint?
            iterator __after = __position;
            ++__after;

            // Check for only one member -- in that case, __position points to itself,
            // and attempting to increment will cause an infinite loop.
            if (__after._M_node == this->_M_header._M_data)
                // Check guarantees exactly one member, so comparison was already
                // performed and we know the result; skip repeating it in _M_insert
                // by specifying a non-zero fourth argument.
                return _M_insert(0, __position._M_node, __v, __position._M_node);

            // All other cases:

            // Optimization to catch insert-equivalent -- save comparison results,
            // and we get this for free.
            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else {
                return insert_unique(__v).first;
            }
        }

    }
    else if (__position._M_node == this->_M_header._M_data) { // end()
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            // pass along to _M_insert that it can skip comparing
            // v, Key ; since compared Key < v already, don't care about the result
            return _M_insert(0, _M_rightmost(), __v, __position._M_node); // first argument just needs to be non-null
        else
            return insert_unique(__v).first;
    }
    else {
        iterator __before = __position;
        --__before;

        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));

        if (__comp_v_pos
            && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {

            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node); // first argument just needs to be non-null
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
            // first argument just needs to be non-null
        }
        else {
            // Does the insertion point fall immediately AFTER the hint?
            iterator __after = __position;
            ++__after;

            // Optimization to catch equivalent cases and avoid unnecessary comparisons
            bool __comp_pos_v = !__comp_v_pos;  // Stored this result earlier
            // If the earlier comparison was true, this comparison doesn't need to be
            // performed because it must be false.  However, if the earlier comparison
            // was false, we need to perform this one because in the equal case, both will
            // be false.
            if (!__comp_v_pos) __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));

            if ((!__comp_v_pos) // comp_v_pos true implies comp_v_pos false
                && __comp_pos_v
                && (__after._M_node == this->_M_header._M_data ||
                    _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {

                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else {
                // Test for equivalent case
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}

template class _Rb_tree<CDateAndTime, pair_kv<const CDateAndTime, CWideString>,
                        _Select1st<pair_kv<const CDateAndTime, CWideString> >, less<CDateAndTime> >;

}
