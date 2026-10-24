// CTCBoastingCrowdControl::CTCBoastingCrowdControl(CThing& thing) -- the boasting-crowd control component: an empty hero -> crowd-thing map, inactive, crowd sizes not suppressed
// Leaf: ??0CTCBoastingCrowdControl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007837C0, 112 bytes, __fastcall `ret 4`.
// Identity: the vtable it stores, 0x0126B3AC, is CTCBoastingCrowdControl (rtti-vtables.tsv); Ego_r.pdb
//   `??0CTCBoastingCrowdControl@@QAE@AAVCThing@@@Z`. The catalog's "CEngineSubPrimitive2DClockSprite_Constructor" is
//   propagated.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, malloc @ 0x00BFEA0E (the 0x20-byte tree header node),
//   STLport `_Rb_tree::_M_erase(root)` @ 0x00782FE0 (thiscall on the map).
// Layout: PDB CTCBoastingCrowdControl with the retail 12-byte STLport map: Active +0x0c, HeroCrowdThings +0x10 (header
//   pointer, node count, empty comparator), CrowdSizesSuppressed +0x1c (PDB +0x20).
// Shape: the inlined STLport map constructor -- `_Rb_tree_base(allocator)`: header proxy zeroed, then `allocate(1)`
//   (malloc 0x20) stored; `_M_node_count(0)`; `_M_empty_initialize()` (red colour byte, root 0, leftmost = rightmost =
//   header, each re-reading the header word); the malloc's `add esp,4` lands after the member stores -- followed by the
//   body: `HeroCrowdThings.clear()` (the inlined STLport clear: `if (_M_node_count != 0) { _M_erase(root); ... }`) and
//   the two flag clears (body assignments: an initialiser for Active, declared before the map, would precede it).
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E

class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class K, class V> struct pair {
    K first;
    V second;
};

struct CHeroCrowdMap {                                   // STLport map<unsigned __int64, CIntelligentPointer<CThing>>
    typedef pair<const unsigned __int64, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10 (node 0x20 bytes)
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;                  // empty comparator, padded

    CHeroCrowdMap() : _M_header(0)
    {
        _M_header = (_Node*)malloc(sizeof(_Node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
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

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    CTCBase(CThing& thing);                              // 0x00686800
};

class CTCBoastingCrowdControl : public CTCBase {
public:
    bool          Active;                                // +0x0c
    CHeroCrowdMap HeroCrowdThings;                       // +0x10
    bool          CrowdSizesSuppressed;                  // +0x1c

    CTCBoastingCrowdControl(CThing& thing);
};

CTCBoastingCrowdControl::CTCBoastingCrowdControl(CThing& thing) : CTCBase(thing)
{
    HeroCrowdThings.clear();
    Active = false;
    CrowdSizesSuppressed = false;
}