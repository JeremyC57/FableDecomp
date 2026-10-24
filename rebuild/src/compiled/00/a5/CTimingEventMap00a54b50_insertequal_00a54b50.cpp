// CTimingEventMap_00a54b50::insert_equal(const value_type& v) -- STLport _Rb_tree::insert_equal of C3DAnimationStats' multimap<CCharString, float> TimingEvents: descend to the leaf where v sorts (equal keys go right), then link a new node there through _M_insert
// Leaf: insert_equal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A54B50, length 93 bytes (+ int3 padding), `ret 8` (hidden iterator result
//   + &v), the result pointer in eax.
// Callers: the landed C3DAnimationStats::InitialiseStats @ 0x00A4EBB0 (call 0x00A4ECF8, "CEventMap::insert @ 0x00A54B50"
//   there) and the copy path @ 0x00A54E50 (call 0x00A54ECB).
// Callees: _Rb_tree::_M_insert(x, y, v, w) @ 0x00A546D0 (thiscall, hidden iterator result first, `w = 0`);
//   CBasicString<char>::operator< @ 0x00429950 (the key compare, see below).
// Layout: Ego_r.pdb C3DAnimationStats::TimingEvents = multimap<CCharString, float>; STLport tree = { _M_header +0,
//   comparator, _M_node_count }, node base { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC }, key at +0x10.
// Shape: `y = header; x = root; while (x) { y = x; x = comp(key(v), key(x)) ? left(x) : right(x); } return
//   _M_insert(x, y, v);` -- `y = x` is scheduled between the data-pointer compare and its `je`; the hidden result
//   pointer is passed straight through to _M_insert (RVO) and returned. std::less<CCharString> is inline over the
//   out-of-line string compare, whose template body sits in the TU as in retail so that the key's data pointer is read
//   once before the loop (see 0x007ACD30).
// Label correction: the catalog's `_Tree<_Tmap_traits<CCharString,...>>::_Ubound` is propagated (this is insert_equal).
template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }   // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CTimingEventMap_00a54b50 {               // STLport _Rb_tree<CCharString, pair<const CCharString, float>, ...>
public:
    typedef std::pair<const CCharString, float> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_node_base* _Base_ptr;
    struct iterator {                          // _Rb_tree_iterator
        _Base_ptr _M_node;
        iterator(_Base_ptr x) : _M_node(x) { }
        iterator(const iterator& it) : _M_node(it._M_node) { }
    };
    _Node*                 _M_header;          // +0x0
    std::less<CCharString> _M_key_compare;
    unsigned long          _M_node_count;
    static _Base_ptr _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr _S_right(_Base_ptr x) { return x->_M_right; }
    static const CCharString& _S_key(_Base_ptr x) { return ((_Node*)x)->_M_value_field.first; }
    _Base_ptr _M_root() const { return _M_header->_M_parent; }
    iterator _M_insert(_Base_ptr x, _Base_ptr y, const value_type& v, _Base_ptr w = 0);   // 0x00A546D0
    iterator insert_equal(const value_type& v);
};

CTimingEventMap_00a54b50::iterator CTimingEventMap_00a54b50::insert_equal(const value_type& v)
{
    _Base_ptr y = _M_header;
    _Base_ptr x = _M_root();
    while (x != 0) {
        y = x;
        x = _M_key_compare(v.first, _S_key(x)) ? _S_left(x) : _S_right(x);
    }
    return _M_insert(x, y, v);
}