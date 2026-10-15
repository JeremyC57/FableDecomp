// NUISystem::CMorphingSprite::CMorphingSprite(void* def) -- the CSprite base (0x0041B800 with the def), the three vtables of this class, RenderCount = 0, an empty Handles list and GraphicIndices map, then GetDataFromDef()
// Leaf: ??0CMorphingSprite
// Retail: Fable.exe (TLC, VC7.1) @ 0x005545D0, 137 bytes (`ret 4`, returns `this`).
// Label: the catalog's "CMorphingSprite::Initialize" is the constructor (eax = this, three vtable stores
// 0x0124A49C / 0x0124A474 / 0x0124A46C = NUISystem::CMorphingSprite in rtti-vtables.tsv).
// Callees: CSprite::CSprite(void*) @ 0x0041B800, malloc @ 0x00BFEA0E (list node 0x0c, tree header 0x18; one `add esp,8`),
// CMorphingSprite::GetDataFromDef @ 0x005542F0.
// Layout: engine/CMorphingSprite.h (Ego_r.pdb, 0x1d8) has RenderCount +0x1b4, Handles +0x1b8, GraphicIndices +0x1c4;
// retail CSprite is 0x30 bytes smaller and the STLport list is one word, so `this` is modelled locally:
// RenderCount +0x184, Handles +0x188 (node pointer), GraphicIndices +0x18c (header, node count, compare).
// Shape: the CSprite model of the landed NUISystem::CMouseCursor ctor (0x0055C650); the list ctor builds its
// sentinel through a local, the tree ctor re-reads the header member for each store.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct IPrimary_005545d0 { virtual void v00(); };
struct CObserver {                                 // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct ISecondaryB_005545d0 { virtual void b00(); };
struct CSprite : IPrimary_005545d0, CObserver, ISecondaryB_005545d0 {   // retail 0x184 bytes
    unsigned char _body[0x184 - 0x1c];
    CSprite(void* def);                            // 0x0041B800
};

struct CEnginePrimitiveHandle;
struct SListNode {
    SListNode*              _M_next;
    SListNode*              _M_prev;
    CEnginePrimitiveHandle* _M_data;
};
struct CHandleList {                               // list<CEnginePrimitiveHandle*> (retail: one word)
    SListNode* _M_node;
    CHandleList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

struct _Rb_tree_node_base {
    char                _M_color;                  // +0
    _Rb_tree_node_base* _M_parent;                 // +4
    _Rb_tree_node_base* _M_left;                   // +8
    _Rb_tree_node_base* _M_right;                  // +0xc
};
struct SIndexNode : _Rb_tree_node_base {           // 0x18 bytes
    unsigned long first;
    unsigned long second;
};
struct CIndexMap {                                 // map<unsigned long, unsigned long>
    SIndexNode*   _M_header;                       // +0
    unsigned long _M_node_count;                   // +4
    unsigned char _M_key_compare;                  // +8
    CIndexMap()
        : _M_header(0)
    {
        _M_header = (SIndexNode*)malloc(sizeof(SIndexNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

namespace NUISystem {
struct CMorphingSprite : CSprite {                 // vtables 0x0124A49C / 0x0124A474 / 0x0124A46C
    long          RenderCount;                     // +0x184
    CHandleList   Handles;                         // +0x188
    CIndexMap     GraphicIndices;                  // +0x18c
    unsigned long SpriteGraphicIndex;              // +0x198
    virtual void v00();
    virtual void a00();
    virtual void b00();
    CMorphingSprite(void* def);
    void GetDataFromDef();                         // 0x005542F0
};

CMorphingSprite::CMorphingSprite(void* def)
    : CSprite(def)
    , RenderCount(0)
{
    GetDataFromDef();
}
}