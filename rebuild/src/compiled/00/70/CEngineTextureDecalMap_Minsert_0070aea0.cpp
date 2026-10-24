// CEngineTextureDecalMap::_M_insert(SRbNodeBase* x, SRbNodeBase* y, const SDecalMapValue& v, SRbNodeBase* w) -- STLport _Rb_tree<long, pair<const long, CEngineTextureDecal>>::_M_insert: allocate a node, copy the value in (deep-copying the blend list), link it left/right of y, rebalance and count
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070AEA0, length 226 bytes (+ int3 padding). __fastcall (this in ecx),
//         `ret 14h` (hidden iterator result + 4 arguments).
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E (the 0x28-byte node: 0x10 links + the 0x18-byte pair, both branches),
//          vector<CCompositeBlendDesc>::vector(const vector&) @ 0x005BB346 (manifest "std::vector::operator=" is
//          propagated), _Rb_global<bool>::_Rebalance(node, root&) @ 0x0042971D (__cdecl, the extern "C" spelling the
//          landed tree inserts use).
// Identity: the manifest label "BinaryTree_Node_Insert" is descriptive. Both callers sit in 0x0070AFE0 (insert_unique),
//   which CTCTextureDecal::PeekEngineDecals (0x0070A570) uses to group its decals by texture into a local
//   map<long, CEngineTextureDecal> before copying them into EngineDecals. Value: the long key, then the 20-byte retail
//   CEngineTextureDecal {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends}.
// Shape: the landed CCameraModeMap::_M_insert (0x0069D520) form: STLport's condition `y == header || (w == 0 && (x != 0 ||
//   key(v) < key(y)))` (signed `jl`), _M_create_node duplicated in both branches (placement copy with its null test:
//   pair's memberwise copy -> three words and the out-of-line blend-list copy), header parent/left/right fix-ups,
//   child links cleared, the __cdecl rebalance, `++_M_node_count`, and the iterator returned through the hidden pointer.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

struct CCompositeBlendDesc;
struct CCompositeBlendDescVector {                          // vector<CCompositeBlendDesc> (retail STLport, 12 bytes)
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    CCompositeBlendDescVector(const CCompositeBlendDescVector& __x);   // 0x005BB346
};

struct CEngineTextureDecal {                                // Ego_r.pdb; 20 bytes retail
    long OriginalTextureIndex;                              // +0x00
    long DecalID;                                           // +0x04
    CCompositeBlendDescVector OverlaidBlends;               // +0x08
};

struct SDecalMapValue {                                     // pair<const long, CEngineTextureDecal> (24 bytes)
    const long first;                                       // +0x00
    CEngineTextureDecal second;                             // +0x04
    SDecalMapValue(const SDecalMapValue& other) : first(other.first), second(other.second) { }
};

struct SRbNodeBase {
    long         _M_color;                                  // +0x00
    SRbNodeBase* _M_parent;                                 // +0x04
    SRbNodeBase* _M_left;                                   // +0x08
    SRbNodeBase* _M_right;                                  // +0x0c
};

struct SRbNode : SRbNodeBase {
    SDecalMapValue _M_value_field;                          // +0x10
};

extern "C" void __cdecl Rebalance_0042971d(SRbNodeBase* node, SRbNodeBase*& root);   // 0x0042971D

struct CEngineTextureDecalMap {                             // map<long, CEngineTextureDecal> (retail 12 bytes)
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    struct iterator {
        SRbNodeBase* _M_node;
        iterator(SRbNodeBase* node) : _M_node(node) { }
    };
    SRbNode* _M_create_node(const SDecalMapValue& v)
    {
        SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SDecalMapValue(v);
        return tmp;
    }
    iterator _M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const SDecalMapValue& v, SRbNodeBase* w_);
};

CEngineTextureDecalMap::iterator CEngineTextureDecalMap::_M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const SDecalMapValue& v, SRbNodeBase* w_)
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