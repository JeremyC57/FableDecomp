// std::_Rb_tree::_M_create_node(const value_type& x) -- allocate a 0x20-byte node of the screen-filter bypass map and copy-construct its {CThing*, CThingGraphicByPassFlagState} value
// Leaf: ?_M_create_node@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008840D0, length 78 bytes (`ret 4`).
// Identity: the tree is NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::reset, a
//   `map<CThing*, CThingGraphicByPassFlagState>` (Ego_r.pdb; its CScreenFilterEntry::Reset is 0x00883AE0); callers are
//   the two node-creating arms of _M_insert 0x008843F0. The catalog label "RBTree_AllocNode_with_intelligent_pointer"
//   is descriptive.
// Callees: malloc @ 0x00BFEA0E (STLport malloc allocator), CBaseIntelligentPointer ctor @ 0x00A01B10, GetPItem @
//   0x00A01B50, SetPItem @ 0x00A01B90; vtable CIntelligentPointer<CThing> 0x01238C6C.
// Shape: STLport `_M_create_node` = allocate(1) + `_Construct(&node->_M_value_field, x)` (placement new: the null test on
//   `node + 0x10`); the pair's explicit memberwise copy ctor copies the key, then the bypass state's implicit copy ctor
//   runs CIntelligentPointer's copy ctor (base ctor, vtable, SetPItem(other.GetPItem())) and copies the bool. Retail
//   CIntelligentPointer is packed (Ego_r.pdb CThingGraphicByPassFlagState = 9 bytes).
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
protected:
    _Link_type _M_create_node(const value_type& __x);
protected:
    _Link_type _M_clone_node(_Link_type __x);
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w);
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
public:

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
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_M_create_node(const value_type& __x)
{
    _Link_type __tmp = _M_allocate();
    _Construct(&__tmp->_M_value_field, __x);
    return __tmp;
}
template class _Rb_tree<CThing*, pair<CThing* const, CByPassState>, _Select1st<pair<CThing* const, CByPassState> >, less<CThing*> >;
}