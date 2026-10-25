// std::_Rb_tree::_M_copy(_Rb_tree_node* x, _Rb_tree_node* p) -- STLport structural copy of a CCharString-keyed red-black subtree (the map<CCharString, 4-byte value> instantiation): clone the root under p, recurse down each right child, walk the left spine iteratively
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D2D00, 207 bytes (+ int3 padding), thiscall `ret 8`.
// Callers: itself (0x009D2D56, 0x009D2DA8) and the tree's operator= 0x009D3420 (landed CRetailTree009d2d00_operator_009d3420,
// which calls this row "Copy"). Catalogued as "BankEntry_CloneTree" (descriptive).
// Callees: MSVCR71 malloc 0x00BFEA0E (the node allocator's allocate(1): no n != 0 test for the constant),
// CCharString::CCharString(const CCharString&) 0x0099EC30.
// Layout: 0x18-byte node = _Rb_tree_node_base {bool colour +0, parent +4, left +8, right +0xC} + the value at +0x10:
// pair<const CCharString, 4-byte mapped value> (the second word is copied raw, so a scalar: descriptive `unsigned long`).
// The tree is {header proxy +0, node count +4} as in the landed operator=.
// Shape: STLport 4.6 _M_copy with _M_clone_node / _M_create_node / _Construct inlined (no /EHsc: _STLP_TRY/_STLP_UNWIND
// vanish); placement new tests the value address for null (`lea esi,[ebp+10h] ; test esi,esi ; je`); `this` is
// spilled to the push-ecx slot for the recursive calls; the clone stores colour, left = 0, right = 0 before linking.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString
{
public:
    void* PData;                                       // +0x00 shared string data
    CCharString(const CCharString& other);             // 0x0099EC30
};

namespace std
{
    template <class _T1, class _T2>
    struct pair
    {
        _T1 first;
        _T2 second;
    };

    template <class _T1, class _T2>
    inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

    template <class _Tp>
    class allocator
    {
    public:
        _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    };

    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc
    {
    public:
        _Value _M_data;
    };

    template <class _Tp>
    struct less { };

    template <class _Pair>
    struct _Select1st { };

    typedef bool _Rb_tree_Color_type;

    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_Color_type _Color_type;
        typedef _Rb_tree_node_base* _Base_ptr;
        _Color_type _M_color;                          // +0x00
        _Base_ptr   _M_parent;                         // +0x04
        _Base_ptr   _M_left;                           // +0x08
        _Base_ptr   _M_right;                          // +0x0C
    };

    template <class _Value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        _Value _M_value_field;                         // +0x10
    };

    template <class _Tp, class _Alloc>
    class _Rb_tree_base
    {
    public:
        typedef _Rb_tree_node<_Tp> _Node;
        _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;   // +0x00
    };

    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
    class _Rb_tree : public _Rb_tree_base<_Value, _Alloc>
    {
    public:
        typedef _Rb_tree_node_base* _Base_ptr;
        typedef _Rb_tree_node<_Value> _Node;
        typedef _Node* _Link_type;
        typedef _Rb_tree_Color_type _Color_type;

        unsigned int _M_node_count;                    // +0x04
        _Compare     _M_key_compare;

        static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
        static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
        static _Color_type& _S_color(_Base_ptr __x) { return __x->_M_color; }

        _Link_type _M_create_node(const _Value& __x)
        {
            _Link_type __tmp = this->_M_header.allocate(1);
            _Construct(&__tmp->_M_value_field, __x);
            return __tmp;
        }

        _Link_type _M_clone_node(_Link_type __x)
        {
            _Link_type __tmp = _M_create_node(__x->_M_value_field);
            _S_color(__tmp) = _S_color(__x);
            _S_left(__tmp) = 0;
            _S_right(__tmp) = 0;
            return __tmp;
        }

        _Link_type _M_copy(_Link_type __x, _Link_type __p);
    };

    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_copy(_Link_type __x, _Link_type __p)
    {
        // structural copy.  __x and __p must be non-null.
        _Link_type __top = _M_clone_node(__x);
        __top->_M_parent = __p;
        if (__x->_M_right)
            __top->_M_right = _M_copy(_S_right(__x), __top);
        __p = __top;
        __x = _S_left(__x);
        while (__x != 0) {
            _Link_type __y = _M_clone_node(__x);
            __p->_M_left = __y;
            __y->_M_parent = __p;
            if (__x->_M_right)
                __y->_M_right = _M_copy(_S_right(__x), __y);
            __p = __y;
            __x = _S_left(__x);
        }
        return __top;
    }

    template class _Rb_tree<CCharString, pair<const CCharString, unsigned long>,
                            _Select1st<pair<const CCharString, unsigned long> >, less<CCharString> >;
}