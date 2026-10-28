// std::_Rb_tree::insert_unique(iterator position, const value_type& v) -- map<CWideString, CChecksumCacheFile::CEntry> hinted unique insert (STLport 4.6 _tree.c): use the hint when v sorts right before/after it, else fall back on insert_unique(v)
// Grade: DIFFER(634v634) -- one register choice in the end() arm: retail `mov ecx,[eax+0Ch] ; add ecx,10h` for &_S_key(_M_rightmost()), ours `mov eax,[eax+0Ch] ; lea ecx,[eax+10h]`; everything else (634 bytes) identical
// Leaf: ?insert_unique@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@U?$_Select1st@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@3@U?$less@VCWideString@@@3@V?$allocator@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@2@@2@U32@ABU?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A614B0, 634 bytes, `ret 0Ch` (hidden iterator result, position by value, v).
// Identity: the only caller is std::map<CWideString, CChecksumCacheFile::CEntry>::operator[] @ 0x00A617E0 (landed,
//   `i = insert(i, value_type(k, Tp()))`); Ego_r.pdb member CChecksumCacheFile::Entries is that map. The body is the
//   STLport 4.6 _tree.c `insert_unique(iterator, const value_type&)` line for line: begin() / end() / middle arms,
//   `size() <= 0` (ja), the hint iterator copies homed in the dead position / v argument slots.
// Callees: CWideString::operator< @ 0x0099AF70 (less<CWideString> inline), _Rb_tree_iterator::operator++ @ 0x00A60780,
//   _Rb_tree_iterator::operator-- @ 0x00A60890, _Rb_tree::insert_unique(const value_type&) @ 0x00A61360,
//   _Rb_tree::_M_insert @ 0x00A61270 (all four sit in this TU in retail: same-TU noinline bodies below, copied from
//   their landed byte-exact sources, so VC7.1 sees the hint copies do not escape).
// Label: the manifest's `BinaryTree_Insert_RBTree` is a placeholder.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
class CChecksumCacheFile {
public:
    struct CEntry {                                          // 0x20 bytes: CDateAndTime TimeStamp + ulong Checksum
        unsigned long Data[8];
    };
};

#pragma auto_inline(off)

namespace std {

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> class allocator { };

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp> struct _Nonconst_traits { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);  // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    __declspec(noinline) _Self& operator++();               // 0x00A60780
    __declspec(noinline) _Self& operator--();               // 0x00A60890
};

template <class _Value, class _Traits>
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator++()
{
    _Base_ptr __x = _M_node;
    if (__x->_M_right != 0) {
        __x = __x->_M_right;
        while (__x->_M_left != 0)
            __x = __x->_M_left;
    }
    else {
        _Base_ptr __y = __x->_M_parent;
        while (__x == __y->_M_right) {
            __x = __y;
            __y = __y->_M_parent;
        }
        if (__x->_M_right != __y)
            __x = __y;
    }
    _M_node = __x;
    return *this;
}

template <class _Value, class _Traits>
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator--()
{
    _Base_ptr __x = _M_node;
    if (__x->_M_color == false && __x->_M_parent->_M_parent == __x)
        __x = __x->_M_right;
    else if (__x->_M_left != 0) {
        _Base_ptr __y = __x->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        __x = __y;
    }
    else {
        _Base_ptr __y = __x->_M_parent;
        while (__x == __y->_M_left) {
            __x = __y;
            __y = __y->_M_parent;
        }
        __x = __y;
    }
    _M_node = __x;
    return *this;
}

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Value value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef size_t size_type;
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = (_Link_type)malloc(sizeof(_Node));
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
    size_type _M_node_count;                                // +0x04
    _Compare _M_key_compare;                                // +0x08

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static value_type& _S_value(_Link_type __x) { return __x->_M_value_field; }
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(_S_value(__x)); }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static value_type& _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(_Link_type(__x))); }
public:
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
private:
    __declspec(noinline) iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
public:
    iterator begin() { return iterator(_M_leftmost()); }
    size_type size() const { return _M_node_count; }
    __declspec(noinline) pair<iterator, bool> insert_unique(const value_type& __x);   // 0x00A61360
    iterator insert_unique(iterator __position, const value_type& __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v,
                                                                 _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;
    if (__y == this->_M_header._M_data || (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x */ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}

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
            } else {
                return insert_unique(__v).first;
            }
        }

    } else if (__position._M_node == this->_M_header._M_data) { // end()
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            // pass along to _M_insert that it can skip comparing
            // v, Key ; since compare Key, v was true, compare v, Key must be false.
            return _M_insert(0, _M_rightmost(), __v, __position._M_node); // Last argument only needs to be non-null
        else
            return insert_unique(__v).first;
    } else {
        iterator __before = __position;
        --__before;

        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));

        if (__comp_v_pos
            && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {

            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node); // Last argument only needs to be non-null
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
            // first argument just needs to be non-null
        } else {
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
            } else {
                // Test for equivalent case
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}

}   // namespace std

#pragma auto_inline(on)

typedef std::pair<const CWideString, CChecksumCacheFile::CEntry> CChecksumEntryValue;

template class std::_Rb_tree<CWideString, CChecksumEntryValue, std::_Select1st<CChecksumEntryValue>,
    std::less<CWideString>, std::allocator<CChecksumEntryValue> >;
