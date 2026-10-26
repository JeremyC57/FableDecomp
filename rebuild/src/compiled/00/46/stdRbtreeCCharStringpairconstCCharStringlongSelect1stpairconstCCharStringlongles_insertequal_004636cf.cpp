// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >, less<CCharString> >::insert_equal(const value_type& v) -- STLport multimap insert: walk from the root to a leaf by key (equal keys go right), then link the new node there
// Leaf: ?insert_equal@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004636CF, 99 bytes, `ret 8` (the hidden iterator return slot + the value); y-off + size TU
//         (frame pointer, `this` spilled to [ebp-4], `leave`).
// Callees: _Rb_tree::_M_insert(_Base_ptr x, _Base_ptr y, const value_type& v, _Base_ptr w = 0) @ 0x004316CF (catalogued as the
//          behavioural BinaryTree_InsertDigScriptNode; it returns the iterator through the hidden slot), the CBasicString compare
//          Compare(const char*, const char*) @ 0x00411570 (__fastcall, landed) inside the inline CCharString::operator<.
// Identity: both callers, 0x004634DB and 0x004637F7 (the CStringParser module around the landed 0x0046381B copy constructor),
//   build a pair {CCharString (copy ctor 0x0099EC30), long} on the stack, pass it here and destroy the string after: the value
//   is pair<const CCharString, long> and the walk is insert_equal's (no predecessor check), so the container is a
//   multimap<CCharString, long>. The catalog label BinarySearchTree_FindOrInsert is behavioural.
// Layout: STLport tree {_M_header (node*), _M_node_count, _M_key_compare}; node {color, parent +4, left +8, right +0xc,
//   value +0x10}; CCharString {CCharStringData* PStringData}, data block {const char* Chars, Length} (the landed 0x0099E900).
// Shape: STLport 4.x insert_equal verbatim; less<CCharString> -> CCharString::operator< inline: same block -> false, own block
//   null -> true, other null -> false, else Compare(...) < 0 (`setl` then the bool re-tested; the equality test is written
//   other-first for `cmp eax(node key),ebx`); `_M_insert(x, y, v)` pushes the default w = 0. Compare's body (the landed
//   0x00411570) must be visible in the TU: VC7.1 then knows the call only reads memory and hoists the value key's data
//   pointer into ebx for the whole walk (saved inside the `x != 0` guard); with a bare declaration it is re-read per node.
//   The size TU keeps operator< / less::operator() out of line unless they are __forceinline.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData { public:
    const char*   Chars;                // +0x00
    unsigned long Length;               // +0x04
};
inline long __fastcall Compare(const char* a, const char* b)   // 0x00411570
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

class CCharString {
public:
    CCharStringData* PStringData;       // +0x00
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return Compare(PStringData->Chars, other.PStringData->Chars) < 0;
    }
};

namespace std {

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    __forceinline bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp> class allocator {};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                 // +0x00
    _Base_ptr _M_parent;                // +0x04
    _Base_ptr _M_left;                  // +0x08
    _Base_ptr _M_right;                 // +0x0c
};

template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;              // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class _Value> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node_base*     _Base_ptr;
    typedef _Rb_tree_node<_Value>*  _Link_type;
    typedef _Value                  value_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _Link_type    _M_header;            // +0x00 (_STLP_alloc_proxy over the header node pointer)
    unsigned long _M_node_count;        // +0x04
    _Compare      _M_key_compare;       // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header->_M_parent; }
    static _Link_type& _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x004316CF
public:

    iterator insert_equal(const value_type& __v)
    {
        _Link_type __y = this->_M_header;
        _Link_type __x = _M_root();
        while (__x != 0) {
            __y = __x;
            __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x) : _S_right(__x);
        }
        return _M_insert(__x, __y, __v);
    }
};

} // namespace std

typedef std::pair<const CCharString, long> CNameCountPair;
template class std::_Rb_tree<CCharString, CNameCountPair, std::_Select1st<CNameCountPair>, std::less<CCharString> >;