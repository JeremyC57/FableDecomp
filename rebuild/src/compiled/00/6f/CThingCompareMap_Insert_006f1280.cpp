// CThingCompareMap::Insert(SRbNodeBase* x, SRbNodeBase* y, const SThingCompareValue& v, SRbNodeBase* w) -- STLport _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>>::_M_insert: allocate a node, copy the value in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance and count
// Leaf: Insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F1280, 215 bytes, `ret 14h` (hidden iterator result + 4 arguments).
// Callees: malloc 0x00BFEA0E (the 0x20-byte node: 0x10 links + the 16-byte, 8-aligned pair),
//   _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (__cdecl; the extern "C" spelling the landed
//   tree inserts use).
// Layout: the tree is {header node pointer, node count}; header parent = root (+4), left = leftmost (+8),
//   right = rightmost (+0xc); the value sits at node +0x10: the unsigned 64-bit key then the thing pointer
//   (only the three used words are copied). The key compare is the unsigned 64-bit `<` (high dword
//   `jb/ja`, then the low dword).
// Shape: STLport's _M_insert condition `y == header || (w == 0 && (x != 0 || key(v) < key(y)))`;
//   _M_create_node is the placement-new construct with its null test, duplicated in both branches; the
//   value's memberwise copy constructor gives the three edx word moves. Same shape as the landed
//   CCombatantMap::Insert 0x006F4D00.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;

struct SThingCompareValue {                                 // pair<const unsigned __int64, CThing*>
    unsigned __int64 first;                                 // +0x00
    CThing* second;                                         // +0x08
    SThingCompareValue(const SThingCompareValue& other) : first(other.first), second(other.second) { }
};

struct SRbNodeBase {
    long         _M_color;                                  // +0x00
    SRbNodeBase* _M_parent;                                 // +0x04
    SRbNodeBase* _M_left;                                   // +0x08
    SRbNodeBase* _M_right;                                  // +0x0c
};

struct SRbNode : SRbNodeBase {
    SThingCompareValue _M_value_field;                      // +0x10
};

extern "C" void __cdecl Rebalance_0042971d(SRbNodeBase* node, SRbNodeBase*& root);   // 0x0042971D

struct CThingCompareMap {
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    struct iterator {
        SRbNodeBase* _M_node;
        iterator(SRbNodeBase* node) : _M_node(node) { }
    };
    SRbNode* _M_create_node(const SThingCompareValue& v)
    {
        SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SThingCompareValue(v);
        return tmp;
    }
    iterator Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingCompareValue& v, SRbNodeBase* w_);
};

CThingCompareMap::iterator CThingCompareMap::Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingCompareValue& v, SRbNodeBase* w_)
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
    Rebalance_0042971d(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}