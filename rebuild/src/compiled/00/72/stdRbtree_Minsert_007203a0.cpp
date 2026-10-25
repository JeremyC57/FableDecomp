// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const pair<const CCharString, pair<long,CDateAndTime> >& v, _Rb_tree_node_base* w) -- link a new node for v under y (left or right by key order) and rebalance; returns the new node's iterator
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007203A0, 263 bytes, thiscall `ret 14h` (hidden iterator result, x, y, v, w).
// Label correction: catalog "RBTree_Insert" -> STLport 4.x _Rb_tree<CCharString, pair<const CCharString,
//   pair<long, CDateAndTime> >, ...>::_M_insert of CTCHeroMorph::CustomToBankIndex (the landed map::operator[]
//   0x00721640 is the same map; node 0x34 = 0x10 header + CCharString + long + 28-byte CDateAndTime).
// Callees: malloc 0x00BFEA0E (_M_create_node's allocate(1), inlined), CCharString copy constructor 0x0099EC30,
//   CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator<), _Rb_global<bool>::_Rebalance
//   0x0042971D (static __cdecl).
// Shape: STLport verbatim (see 0x00756150): the inlined _M_create_node copies the pair memberwise -- the key's copy
//   constructor, the long, then CDateAndTime's implicit 7-dword `rep movsd` (STLport's user pair copy constructor
//   keeps the long and the date apart).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

template <class T> class CBasicString {
public:
    T* m_pData;
    bool operator<(const CBasicString& other) const;     // 0x00429950
};
class CCharStringData : public CBasicString<char> {
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    bool operator<(const CCharString& other) const
    {
        bool result;
        if (other.PStringData == PStringData)
            result = false;
        else if (!PStringData)
            result = true;
        else if (!other.PStringData)
            result = false;
        else
            result = *PStringData < *other.PStringData;
        return result;
    }
};

class CDateAndTime {                                     // Ego_r.pdb, 28 bytes
public:
    long          Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
};

namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {
public:
    allocator() {}
};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
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
    _Base_ptr    _M_header;                              // +0x00
    unsigned int _M_node_count;                          // +0x04
    C            _M_key_compare;

    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr x) { return x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr x) { return x->_M_parent; }
    static const K& _S_key(_Base_ptr x) { return KoV()(((_Link_type)x)->_M_value_field); }

    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
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

typedef pair<const CCharString, pair<long, CDateAndTime> > SCustomBankEntry;
template class _Rb_tree<CCharString, SCustomBankEntry, _Select1st<SCustomBankEntry>, less<CCharString>, allocator<SCustomBankEntry> >;

}