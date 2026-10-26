// std::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const value_type&, _Rb_tree_node_base*) -- link a new node for the value under y (left or right by the key order) and rebalance
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@J@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004316CF, length 164 bytes, `ret 0x14` (__fastcall: hidden iterator result, x, y, v, w).
// Name: STLport 4.x `_Rb_tree::_M_insert(x, y, v, w)` of the map<CCharString, long> tree whose _M_create_node is the landed 0x00431773;
// called by that tree's insert_unique 0x00431606 (previous row, both exits). The catalog's "BinaryTree_InsertDigScriptNode" is a label.
// Callees: _M_create_node(const value_type&) @ 0x00431773 (landed, out of line), _Rb_global<bool>::_Rebalance(node, root&)
// @ 0x0042971D (landed, static __cdecl: two `pop ecx`), Compare(const char*, const char*) @ 0x00411570 (landed, __fastcall).
// Pragmas: #pragma optimize("y", off) + #pragma optimize("s", on) -- frame pointer; `pop ecx ; pop ecx` after _Rebalance.
// Shape: STLport verbatim -- `if (y == header || (w == 0 && (x != 0 || key_compare(KeyOfValue()(v), key(y)))))` left else right;
// VC7.1 tail-merges the left arm's `_M_rightmost() = z` into the right arm's identical store (`jmp 0x00431724`) and schedules the
// two zero stores of z's children before the parent store. The key compare is less<CCharString> -> CCharString::operator<,
// inlined: same data block -> false, null left -> true, null right -> false, else Compare(chars, chars) < 0 (`setl al`).
// Layout: STLport _Rb_tree_node (color +0, parent +4, left +8, right +0xc, value +0x10); tree {header +0, node count +4};
// CCharString is one CCharStringData* whose block starts with the C string (the landed 0x0099E900 model).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharStringData {
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    __forceinline bool operator<(const CCharString& other) const
    {
        bool result;
        if (other.PStringData == PStringData)
            result = false;
        else if (!PStringData)
            result = true;
        else if (!other.PStringData)
            result = false;
        else
            result = Compare(PStringData->Chars, other.PStringData->Chars) < 0;
        return result;
    }
};

namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class T> struct less {
    __forceinline bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    __forceinline const typename V::first_type& operator()(const V& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<V>*   _Link_type;
    typedef _Rb_tree_iterator<V> iterator;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    C         _M_key_compare;

    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr x) { return x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr x) { return x->_M_parent; }
    static const K& _S_key(_Base_ptr x) { return KoV()(((_Link_type)x)->_M_value_field); }

protected:
    _Link_type _M_create_node(const V& x);              // 0x00431773
private:
    iterator _M_insert(_Base_ptr x_, _Base_ptr y_, const V& v, _Base_ptr w_);
};

template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::iterator
_Rb_tree<K, V, KoV, C, A>::_M_insert(_Base_ptr x_, _Base_ptr y_, const V& v, _Base_ptr w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type x = (_Link_type)x_;
    _Link_type y = (_Link_type)y_;
    _Link_type z;

    if (y == _M_header ||
        (w == 0 &&
         (x != 0 || _M_key_compare(KoV()(v), _S_key(y))))) {
        z = _M_create_node(v);
        _S_left(y) = z;
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        _S_right(y) = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    _S_parent(z) = y;
    _S_left(z) = 0;
    _S_right(z) = 0;
    _Rb_global_inst::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

template class _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
                        less<CCharString>, allocator<pair<const CCharString, long> > >;
}