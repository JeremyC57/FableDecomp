// CTCBoastingCrowdControl::Shutdown() -- kill every live crowd thing recorded in the hero -> crowd map, clear the map and mark the control inactive
// Leaf: ?Shutdown@CTCBoastingCrowdControl@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007832B0, 196 bytes (+ int3 padding to 208), __fastcall, plain `ret`.
// Identity: Ego_r.pdb `?Shutdown@CTCBoastingCrowdControl@@QAEXXZ`; vtable 0x0126B3AC slots 2 and 14 are `jmp` stubs to it
//   and OnSetInLimbo(true) (landed 0x00783A10) calls it (that source calls it KillAllCrowdThings).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3: the alive test reads it twice), CThing::Kill(bool) @
//   0x004C9B80, STLport `_Rb_tree::_M_erase(root)` @ 0x00782FE0 (thiscall on the map; catalogued as the propagated
//   LTextTreeWalkThrough::BuildTreeArray).
// Layout: PDB CTCBoastingCrowdControl: Active +0x0c, HeroCrowdThings +0x10 (STLport map<unsigned __int64,
//   CIntelligentPointer<CThing>>: header pointer, node count); node value at +0x10 (key) / +0x18 (pointer); retail
//   CThing Dead = bit 0 of +0x91.
// Shape: `for (it = begin(); it != end; ++it) if (IS_THING_ALIVE(it->second.GetPItem())) it->second.GetPItem()->
//   Kill(true);` with end() in a local (ebx) and `++it` the inlined `_Rb_global<bool>::_M_increment`; `this` is parked
//   in the `push ecx` frame word while edi holds the node's pointer member; then STLport `clear()` inlined (`if
//   (_M_node_count != 0) { _M_erase(root); leftmost = header; root = 0; rightmost = header; _M_node_count = 0; }`) and
//   `Active = false`.
// Label correction: catalogued as `LTextTree_KillAllThings`; the body is CTCBoastingCrowdControl::Shutdown.
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                               // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool removeFromWorld);                     // 0x004C9B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                            // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct _Rb_global {
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

template <class K, class V> struct pair {
    K first;
    V second;
};

struct CHeroCrowdMap {                                   // STLport map<unsigned __int64, CIntelligentPointer<CThing>>
    typedef pair<const unsigned __int64, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;

    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void _M_erase(_Rb_tree_node_base* x);                // 0x00782FE0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

#define IS_THING_ALIVE(t) ((t) && !(t)->IsDead())

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};

class CTCBoastingCrowdControl : public CTCBase {
public:
    bool          Active;                                // +0x0c
    CHeroCrowdMap HeroCrowdThings;                       // +0x10

    void Shutdown();
};

void CTCBoastingCrowdControl::Shutdown()
{
    CHeroCrowdMap::iterator end = HeroCrowdThings.end();
    for (CHeroCrowdMap::iterator it = HeroCrowdThings.begin(); it != end; ++it) {
        if (IS_THING_ALIVE((*it).second.GetPItem()))
            (*it).second.GetPItem()->Kill(true);
    }
    HeroCrowdThings.clear();
    Active = false;
}