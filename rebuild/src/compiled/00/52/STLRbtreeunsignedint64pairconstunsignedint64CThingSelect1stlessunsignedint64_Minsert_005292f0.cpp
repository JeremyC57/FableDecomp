// _STL::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, _Select1st, less<unsigned __int64> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of CThingManager::UIDToThingMap: allocate a node, copy the value in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x005292F0, 215 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x20-byte node: 0x10 links + the 16-byte, 8-aligned pair),
//   _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (static, __cdecl).
// Callers: the tree's insert_unique pair at 0x0052A25B / 0x0052A29D (CThingManager TU, 0x0052xxxx).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4 }; the header's parent is the root
//   (+4), its left the leftmost (+8), its right the rightmost (+0xc); the value sits at node +0x10: the unsigned
//   64-bit UID then the CThing pointer (only the three used words are copied). The PDB names the map
//   CThingManager::UIDToThingMap (map<unsigned __int64, CThing*>).
// Shape: byte twin of the landed CThingCompareMap::Insert 0x006F1280 (same instantiation compiled in another TU):
//   STLport's _M_insert condition `y == header || (w == 0 && (x != 0 || key(v) < key(y)))`; _M_create_node is the
//   placement-new construct with its null test, duplicated in both branches; the pair's memberwise copy
//   constructor gives the three edx word moves; the unsigned 64-bit `<` is `jb/ja` on the high word then the low.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;

struct SUIDThingPair {                                      // pair<const unsigned __int64, CThing*>
    unsigned __int64 first;                                 // +0x00
    CThing* second;                                         // +0x08
    SUIDThingPair(const SUIDThingPair& other) : first(other.first), second(other.second) { }
};

struct _Rb_tree_node_base {
    long                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

struct SUIDThingNode : _Rb_tree_node_base {
    SUIDThingPair _M_value_field;                           // +0x10
};

extern "C" void __cdecl Rebalance_0042971d(_Rb_tree_node_base* node, _Rb_tree_node_base*& root);   // 0x0042971D

struct CUIDToThingTree {                                    // _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, ...>
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* node) : _M_node(node) { }
    };
    SUIDThingNode* _M_create_node(const SUIDThingPair& v)
    {
        SUIDThingNode* tmp = (SUIDThingNode*)malloc(sizeof(SUIDThingNode));
        new (&tmp->_M_value_field) SUIDThingPair(v);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SUIDThingPair& v, _Rb_tree_node_base* w_);
};

CUIDToThingTree::iterator CUIDToThingTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SUIDThingPair& v, _Rb_tree_node_base* w_)
{
    SUIDThingNode* w = (SUIDThingNode*)w_;
    SUIDThingNode* y = (SUIDThingNode*)y_;
    SUIDThingNode* x = (SUIDThingNode*)x_;
    SUIDThingNode* z;
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