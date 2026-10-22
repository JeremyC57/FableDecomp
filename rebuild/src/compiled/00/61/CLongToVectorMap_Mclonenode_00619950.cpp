// CLongToVectorMap::_M_clone_node(SRbNode*) -- STLport _Rb_tree node clone for map<long, vector<?>>: allocate a node, copy-construct the value, copy the colour and clear both children
// Leaf: _M_clone_node
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619950, 67 bytes, `ret 4`. Callees: malloc 0x00BFEA0E (the 0x20-byte node:
// 0x10 links + the long key + a 12-byte STLport vector), the vector copy constructor 0x0047BB90.
// Unreferenced in the image (recovered from a mis-cut catalog row); `this` (the tree) is not read.
// Shape: STLport `_M_clone_node(x) { tmp = _M_create_node(x->_M_value_field); tmp->_M_color = x->_M_color;
// tmp->_M_left = 0; tmp->_M_right = 0; return tmp; }` -- _M_create_node is malloc plus the placement-new
// construct under its null test; the pair's memberwise copy ctor stores the key before the vector copy.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CRetailVector12 {                                    // 12-byte STLport vector (element type unknown)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CRetailVector12(const CRetailVector12& other);         // 0x0047BB90
};

struct SLongVectorPair {                                    // pair<const long, vector<?>>
    long            first;                                  // +0x00
    CRetailVector12 second;                                 // +0x04
    SLongVectorPair(const SLongVectorPair& other) : first(other.first), second(other.second) { }
};

struct SRbNodeBase {
    unsigned char _M_color;                                 // +0x00
    SRbNodeBase*  _M_parent;                                // +0x04
    SRbNodeBase*  _M_left;                                  // +0x08
    SRbNodeBase*  _M_right;                                 // +0x0c
};

struct SRbNode : SRbNodeBase {
    SLongVectorPair _M_value_field;                         // +0x10
};

struct CLongToVectorMap {
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    SRbNode* _M_create_node(const SLongVectorPair& v)
    {
        SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SLongVectorPair(v);
        return tmp;
    }
    SRbNode* _M_clone_node(SRbNode* x);
};

SRbNode* CLongToVectorMap::_M_clone_node(SRbNode* x)
{
    SRbNode* tmp = _M_create_node(x->_M_value_field);
    tmp->_M_color = x->_M_color;
    tmp->_M_left = 0;
    tmp->_M_right = 0;
    return tmp;
}