// CThingMap64::Insert -- STLport _Rb_tree<unsigned __int64, pair<const unsigned __int64, CIntelligentPointer<CThing>>>::_M_insert(x, y, v, w): create the node out of line, link it left/right of y (keeping root/leftmost/rightmost), rebalance and count
// Leaf: Insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00783F00, 161 bytes (+ int3 padding), `ret 14h` (hidden iterator result + 4).
// Callees: _M_create_node(const value_type&) 0x00783DD0 (__fastcall on the tree: malloc(0x20), copies the 64-bit key and
//   builds the CIntelligentPointer<CThing> at +8 -- vtable 0x01238C6C), _Rb_tree_rebalance(node, root&) 0x0042971D (__cdecl).
// Label: the manifest's "CCreatureAction_ScorpionChargeInto::InsertIntoTree" is a propagated name; the body is the STLport
//   tree insert of a map keyed by an unsigned 64-bit id (UID) whose 0x10-byte value is {key, CIntelligentPointer}.
// Shape: the landed CCombatantMap::Insert (0x006F4D00) with the node creation out of line: condition
//   `y == header || (w == 0 && (x != 0 || key(v) < key(y)))` with the unsigned 64-bit compare (high `jb/ja`, low `jb`).
struct CBaseObject;

struct CIntelligentPointerThing {                         // CIntelligentPointer<CThing>: vptr + item
    void*        m_vtable;
    CBaseObject* m_pItem;
};

struct SThingMapValue {                                   // pair<const unsigned __int64, CIntelligentPointer<CThing>>
    unsigned __int64         first;                       // +0x00
    CIntelligentPointerThing second;                      // +0x08
};

struct SRbNodeBase {
    long         _M_color;                                // +0x00
    SRbNodeBase* _M_parent;                               // +0x04
    SRbNodeBase* _M_left;                                 // +0x08
    SRbNodeBase* _M_right;                                // +0x0c
};

struct SRbNode : SRbNodeBase {
    SThingMapValue _M_value_field;                        // +0x10
};

void __cdecl _Rb_tree_rebalance(SRbNodeBase* x, SRbNodeBase*& root);   // 0x0042971D

struct CThingMap64 {
    SRbNodeBase*  _M_header;                              // +0x00
    unsigned long _M_node_count;                          // +0x04
    struct iterator {
        SRbNodeBase* _M_node;
        iterator(SRbNodeBase* node) : _M_node(node) { }
    };
    SRbNode* _M_create_node(const SThingMapValue& v);     // 0x00783DD0
    iterator Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingMapValue& v, SRbNodeBase* w_);
};

CThingMap64::iterator CThingMap64::Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingMapValue& v, SRbNodeBase* w_)
{
    SRbNode* w = (SRbNode*)w_;
    SRbNode* y = (SRbNode*)y_;
    SRbNode* x = (SRbNode*)x_;
    SRbNode* z;
    if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        }
        else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_tree_rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}