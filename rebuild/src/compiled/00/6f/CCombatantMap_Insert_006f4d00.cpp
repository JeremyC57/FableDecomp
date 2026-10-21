// CCombatantMap::Insert -- STLport _Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo>>::_M_insert(x, y, v, w): allocate a node, copy the value in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance and count
// Leaf: Insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4D00, 230 bytes (240 with padding), `ret 14h` (hidden iterator result + 4).
// Callees: malloc 0x00BFEA0E (the 0x88-byte node: 0x10 links + 8-byte key + 0x70-byte CCombatantInfo), the CCombatantInfo
//   copy constructor 0x006F4990, _Rb_tree_rebalance(node, root&) 0x0042971D (__cdecl).
// Layout: the tree is {header node pointer, node count}; header parent = root (+4), left = leftmost (+8), right = rightmost
//   (+0xc); the key compare is the unsigned 64-bit `<` (high dword `jb/ja`, then low dword).
// Shape: STLport's _M_insert condition `y == header || (w == 0 && (x != 0 || key(v) < key(y)))`; _M_create_node is the
//   placement-new construct with its null test (`lea eax,[esi+10h] ; test eax,eax`), duplicated in both branches.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CCombatantInfo {                                     // retail 0x70 bytes
    unsigned char _data[0x70];
    CCombatantInfo(const CCombatantInfo& other);            // 0x006F4990
};

struct SCombatantValue {                                    // pair<const unsigned __int64, CCombatantInfo>
    unsigned __int64 first;                                 // +0x00
    CCombatantInfo second;                                  // +0x08
    SCombatantValue(const SCombatantValue& other) : first(other.first), second(other.second) { }
};

struct SRbNodeBase {
    long         _M_color;                                  // +0x00
    SRbNodeBase* _M_parent;                                 // +0x04
    SRbNodeBase* _M_left;                                   // +0x08
    SRbNodeBase* _M_right;                                  // +0x0c
};

struct SRbNode : SRbNodeBase {
    SCombatantValue _M_value_field;                         // +0x10
};

void __cdecl _Rb_tree_rebalance(SRbNodeBase* x, SRbNodeBase*& root);   // 0x0042971D

struct CCombatantMap {
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    struct iterator {
        SRbNodeBase* _M_node;
        iterator(SRbNodeBase* node) : _M_node(node) { }
    };
    SRbNode* _M_create_node(const SCombatantValue& v)
    {
        SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SCombatantValue(v);
        return tmp;
    }
    iterator Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SCombatantValue& v, SRbNodeBase* w_);
};

CCombatantMap::iterator CCombatantMap::Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SCombatantValue& v, SRbNodeBase* w_)
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