// CLongToRegionMap::_M_create_node(const SLongRegionPair&) -- STLport _Rb_tree node allocation for map<long, CRegion>: malloc the node and copy-construct the value in place
// Leaf: _M_create_node
// Retail: Fable.exe (TLC, VC7.1) @ 0x006199A0, 46 bytes, `ret 4`. Callees: malloc 0x00BFEA0E (the 0x6C-byte node:
// 0x10 links + the long key + a 0x58-byte CRegion), CRegion::CRegion(const CRegion&) 0x0051A900.
// Unreferenced in the image (recovered from a mis-cut catalog row); `this` (the tree) is not read.
// Shape: malloc, then the placement-new construct under VC's null test of `&tmp->_M_value_field`; the pair's
// memberwise copy constructor reads the key, forms `&v.second`, then stores the key before the CRegion copy.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CRegion { public:                                            // 0x58 bytes
    unsigned char _data[0x58];
    CRegion(const CRegion& other);                          // 0x0051A900
};

struct SLongRegionPair {                                    // pair<const long, CRegion>
    long    first;                                          // +0x00
    CRegion second;                                         // +0x04
    SLongRegionPair(const SLongRegionPair& other) : first(other.first), second(other.second) { }
};

struct SRbNodeBase {
    unsigned char _M_color;                                 // +0x00
    SRbNodeBase*  _M_parent;                                // +0x04
    SRbNodeBase*  _M_left;                                  // +0x08
    SRbNodeBase*  _M_right;                                 // +0x0c
};

struct SRbNode : SRbNodeBase {
    SLongRegionPair _M_value_field;                         // +0x10
};

struct CLongToRegionMap {
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    SRbNode* _M_create_node(const SLongRegionPair& v);
};

SRbNode* CLongToRegionMap::_M_create_node(const SLongRegionPair& v)
{
    SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
    new (&tmp->_M_value_field) SLongRegionPair(v);
    return tmp;
}