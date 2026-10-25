// std::_Rb_tree::_M_insert(_Base_ptr x, _Base_ptr y, const value_type& v, _Base_ptr w) -- link a new node of the screen-filter bypass map under y (left when y is the header, w is null and x is set or v sorts first), fix the header links, rebalance, count
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008843F0, length 153 bytes (`ret 14h`: the hidden iterator + four arguments).
// Identity: `map<CThing*, CThingGraphicByPassFlagState>` = NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry
//   ::reset (Ego_r.pdb); nine callers in the insert_unique / hinted insert bodies 0x00884560.. (landed 0x00884560 calls
//   it as `Insert(&j, x, y, value, 0)`). The catalog label "RBTree_Insert" is descriptive.
// Callees: _M_create_node @ 0x008840D0 (this chunk, out of line), `_Rb_global<bool>::_Rebalance(node, root&)` @
//   0x0042971D (`__cdecl`, manifest label "BalanceTree" is propagated).
// Shape: STLport 4.x `_M_insert` verbatim: the left arm tests `y == header || (w == 0 && (x != 0 || v.first <
//   key(y)))` (unsigned pointer compare: `jb`), each arm creates its node; the children are zeroed with immediates, the
//   count is bumped through eax, and the iterator is written through the hidden result pointer.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (the STLport malloc allocator)
inline void* operator new(unsigned int, void* where) { return where; }

class CBaseObject;
class CThing;

#pragma pack(push, 1)                                   // retail packs CIntelligentPointer (CThingGraphicArgPasser is 9 bytes)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    void* PData;                                        // +4
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CScreenFilterEntry {
        class CThingGraphicByPassFlagState {            // Ego_r.pdb, 9 bytes
        public:
            CIntelligentPointer<CThing> thing;          // +0
            bool bypassReset;                           // +8
            CThingGraphicByPassFlagState() : bypassReset(false) { }
        };
    };
};
}
#pragma pack(pop)

typedef NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::CThingGraphicByPassFlagState CByPassState;

namespace std {
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp>
inline void _Construct(_Tp* __p, const _Tp& __val) { new (__p) _Tp(__val); }

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    _Color_type _M_color;                               // +0
    _Rb_tree_node_base* _M_parent;                      // +4
    _Rb_tree_node_base* _M_left;                        // +8
    _Rb_tree_node_base* _M_right;                       // +0xC
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy>
class _Rb_global {
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value>
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Value value_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _Node* _M_header;                                   // +0 (the alloc proxy's data: the header node)
    unsigned int _M_node_count;                         // +4
    _Compare _M_key_compare;                            // +8

    _Node* _M_allocate() { return (_Node*)malloc(sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& __x);
    _Link_type _M_clone_node(_Link_type __x);
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);

    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
};

template <class _Pair>
struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp>
struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}


namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v, _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {

        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header) {
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
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
template class _Rb_tree<CThing*, pair<CThing* const, CByPassState>, _Select1st<pair<CThing* const, CByPassState> >, less<CThing*> >;
}