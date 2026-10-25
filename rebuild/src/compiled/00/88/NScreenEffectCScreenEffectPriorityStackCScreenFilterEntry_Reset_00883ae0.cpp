// NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::Reset() -- give every live thing in the entry's bypass map its saved colour-filter bypass state back
// Leaf: ?Reset@CScreenFilterEntry@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883AE0, length 142 bytes (`ret`).
// Identity: Ego_r.pdb `?Reset@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ`; the entry's `reset`
//   map<CThing*, CThingGraphicByPassFlagState> sits at +0x30 (PDB +0x30: nothing STL before it); called from
//   CScreenFilterProcess::OnErase (vtable 0x01278028 slot 2, landed 0x00880F50). The catalog label
//   "CEnvironmentSpellEffectInterface_UpdateAllEffects" is descriptive.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times per entry), CTCGraphicAppearance::
//   SetByPassColorFilter(bool) @ 0x004BFB00.
// Shape: STLport map walk with end() taken into a local first (held in ebp; `it != reset.end()` re-reads the member
//   after the call) and `_Rb_global<bool>::_M_increment` inlined on the node copy;
//   the body is the landed 0x00883AA0 idiom (IS_THING_ALIVE nested ifs re-fetching GetPItem, the appearance at +0x64
//   through a named thing local, the bool pushed zero-extended).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (the STLport malloc allocator)
inline void* operator new(unsigned int, void* where) { return where; }

class CBaseObject;
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void SetByPassColorFilter(bool byPass);             // 0x004BFB00
};
}
class CThing {                                          // retail offsets (landed 0x00883AA0)
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    unsigned char _pad68[0x29];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
};
namespace std {
template <class _Tp> struct less;
template <class _Key, class _Tp, class _Compare> class map;
}

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

#pragma pack(pop)

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
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value>
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
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
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header); }

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
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}
#pragma pack(push, 1)
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
        unsigned char _base[0x10];                      // +0x00 CStackEntry base (vptr ...)
        float param[8];                                 // +0x10 CScreenFilterParams (Alpha .. Tint)
        std::map<CThing*, CThingGraphicByPassFlagState, std::less<CThing*> > reset;   // +0x30 (12 bytes in retail)
        void Reset();
    };
};
}

#pragma pack(pop)

namespace NScreenEffect {
void CScreenEffectPriorityStack::CScreenFilterEntry::Reset()
{
    typedef std::map<CThing*, CThingGraphicByPassFlagState, std::less<CThing*> > CResetMap;
    CResetMap::iterator end = reset.end();
    for (CResetMap::iterator it = reset.begin(); it != end; ++it) {
        CThingGraphicByPassFlagState& state = (*it).second;
        if (state.thing.GetPItem() != 0) {
            if (!(((CThing*)state.thing.GetPItem())->m_flags91 & 1)) {
                CThing* thing = (CThing*)state.thing.GetPItem();
                if (thing->m_pGraphicAppearance)
                    thing->m_pGraphicAppearance->SetByPassColorFilter(state.bypassReset);
            }
        }
    }
}
}