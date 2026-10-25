// std::map::operator[](const key_type& k) -- the bypass state stored for a thing in the screen-filter bypass map, inserting a default (null pointer, false) entry when the thing is not there yet
// Leaf: ??A?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00884C70, length 207 bytes (`ret 4`).
// Identity: Ego_r.pdb `map<CThing*, CThingGraphicByPassFlagState>::operator[]` -- the `reset` map of
//   NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry; callers 0x00881C18 / 0x00881D86. The catalog label
//   "RBTree_FindNode_or_insert_null" is descriptive.
// Callees: the hinted insert_unique @ 0x00884830 (hidden iterator result), CBaseIntelligentPointer ctor @ 0x00A01B10,
//   GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10; vtable 0x01238C6C.
// Shape: STLport 4.x `operator[]`: inline lower_bound (`jb` = unsigned pointer keys), then `insert(i, value_type(k,
//   _Tp()))`: the packed 9-byte `_Tp()` temporary at an odd frame offset (pointer reset through SetPItem(0), the bool
//   cleared), the 16-byte pair built from it by the memberwise copies, the by-value iterator built in its argument
//   slot, the returned iterator homed in the dead `k` slot; both temporaries destroyed through the base destructor.
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
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
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
    iterator insert_unique(iterator __position, const value_type& __x);   // 0x00884830
    iterator end() { return iterator(this->_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const key_type& __k)
    {
        _Link_type __y = this->_M_header;               /* Last node which is not less than __k. */
        _Link_type __x = (_Link_type)_M_root();         /* Current node. */
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = (_Link_type)_S_left(__x);
            else
                __x = (_Link_type)_S_right(__x);
        return iterator(__y);
    }

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
template <class _Key, class _Tp, class _Compare>
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;

    _Rep_type _M_t;                                     // +0 (12 bytes in retail)

    iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }
    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }

    _Tp& operator[](const key_type& __k)
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};
template class map<CThing*, CByPassState, less<CThing*> >;
}