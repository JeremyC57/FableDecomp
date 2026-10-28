// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (map<pair<long,long>, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> >) STLport 4.6 node insert: create a node holding a copy of v (the counted pointer's reference count is bumped), link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@U?$pair@JJ@std@@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@2@U?$_Select1st@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@U?$less@U?$pair@JJ@std@@@2@V?$allocator@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD4620, 241 bytes, thiscall `ret 14h` (hidden iterator result, x, y, &v, w).
// Identity (proven): STLport 4.6 _tree.c `_Rb_tree::_M_insert` (private in _tree.h, hence AAE): the 0x20-byte node
//   (base 0x10 + value 0x10), left/right/root/leftmost/rightmost fix-ups and the cdecl _Rb_global<bool>::_Rebalance 0x0042971D
//   are the text verbatim. The value type is proven by its only caller, insert_unique 0x00AD5630, whose own callers
//   (0x00AD6094 / 0x00AD6799, the QuadricOptimiser contraction map) build a pair<long,long> key and a
//   CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> value (Ego_r.pdb names the class and that
//   counted-pointer template argument); the key compare is pair<long,long> operator< (signed jg / jl), the value copy
//   bumps CCPPointerInfo::RefCount. The catalog's "BTree_InsertPair" is descriptive.
// Callees: malloc 0x00BFEA0E (inline node allocate), _Rb_global<bool>::_Rebalance 0x0042971D (__cdecl, landed spelling).
// Model: the landed set<void*> twin 0x00835270 with a map's pair value (placement copy null-checked).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                                // +0
    void (__fastcall* DeleteFunc)(void* data);             // +4
    void* Data;                                            // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    CCountedPointer(const CCountedPointer<T>& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
namespace QuadricOptimiserInternals { class CVertexPairContraction; }

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
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
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> _Self;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Rb_tree(const _Self& __x);                                    // out of line where used (not here)
    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
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

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}
}   // namespace std

typedef std::pair<long, long> TKey;
typedef CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> TMapped;
typedef std::pair<const TKey, TMapped> TValue;
typedef std::_Rb_tree<TKey, TValue, std::_Select1st<TValue>, std::less<TKey>, std::allocator<TValue> > TTree;
template TTree::iterator TTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const TValue&,
                                          std::_Rb_tree_node_base*);