// NUISystem::CUIDef::~CUIDef() -- destroy the UI def's members in reverse order (shape children, swapping times/states, movie name, action order, the two action maps, the four child/separation vectors, the sprite map, the text value, the state vector), then the inlined def bases free the SubDefInfoMap buffer and tail-call ~CResource
// Leaf: ??1CUIDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x006301A0, length 432 bytes (428 + int3 padding), tail `jmp`, no EH frame.
// Label correction: the catalog's "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated; the body stores
//   vtable 0x01259F8C, which rtti-vtables.tsv names NUISystem::CUIDef, and frees exactly the PDB CUIDef containers.
// Callees: free 0x00BFEA14, CWideString::~CWideString 0x0099B510, the STLport _Rb_tree::_M_erase instantiations
//   0x00630A10 (Sprites), 0x00630D90 (ActionMap), 0x0042FE07 (ActionMapAliases) (manifest "BuildTreeArray" labels are
//   propagated), CResource::~CResource 0x009FC550 (tail; manifest "~CMemoryBlock" is propagated). Vtables: CUIDef 0x01259F8C,
//   CDefPointeeBase 0x01230BA0.
// Layout (retail; Ego_r.pdb CUIDef with the 0x39-byte packed CParentDefClassBase and 12-byte STLport vectors/maps):
//   States +0x40 (0x7c-byte CUIStateDef, virtual dtor), TextValue +0x54, Sprites +0x64, Children +0x70, Horizontal/Vertical/
//   NonScrolling separations +0x7c/+0x88/+0x94, ActionMap +0x1a8, ActionMapAliases +0x1b4, ActionOrder +0x1c0, MovieFilename
//   +0x1d8, SwappingStates +0x1e0, SwappingTimes +0x1ec, ShapeChildren +0x214; the base's SubDefInfoMap buffer at +0x29.
// Shape: the def-dtor model of the landed CUIStateDef::~CUIStateDef 0x00624DC0 (empty user dtor on CDefPointeeBase, the
//   intermediate def bases with generated dtors store no vtable); STLport containers with inline destructors: vector =
//   destroy range + `if (start) free(start)`, map = clear() (erase root, reset header links, zero count) + header free.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CResource { public:                                    // engine/CResource.h: the 0x1c-byte root
    virtual void v00();
    virtual ~CResource();                                     // 0x009FC550
};
class CDefPointeeBase : public CResource { public:                  // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> struct CRetailVector {                    // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct CRbTreeNode {
    long         _M_color;                                  // +0x00
    CRbTreeNode* _M_parent;                                 // +0x04
    CRbTreeNode* _M_left;                                   // +0x08
    CRbTreeNode* _M_right;                                  // +0x0c
};
template<int Kind> struct CRbTree {                         // STLport map (12 bytes): header node, count, empty compare
    CRbTreeNode*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    unsigned char _M_key_compare;                           // +0x08
    void _M_erase(CRbTreeNode* x);                          // 0x00630A10 / 0x00630D90 / 0x0042FE07
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
    ~CRbTree()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};

class CWideString { public:
    void* m_pData;
    ~CWideString();                                         // 0x0099B510
};

namespace NUISystem {
class CUIStateDef { public:                                        // 0x7c bytes, virtual destructor in slot 0
    virtual ~CUIStateDef();
    unsigned char _body[0x78];
};

#pragma pack(push, 1)                                       // def classes are byte packed in retail
struct CParentDefClassBase : CDefPointeeBase {              // 0x39 bytes
    unsigned char                _pad04[0x29 - 0x04];
    CRetailVector<unsigned long> SubDefInfoMap;             // +0x29
    unsigned char                _pad35[0x39 - 0x35];
};
class CUIDef : public CParentDefClassBase { public:                       // vtable 0x01259F8C
    unsigned char              _pad39[3];
    long                       Type;                        // +0x3c
    CRetailVector<CUIStateDef> States;                      // +0x40
    unsigned long              MeshIndex;                   // +0x4c
    long                       Font;                        // +0x50 CDefString
    CWideString                TextValue;                   // +0x54
    float                      Height;                      // +0x58
    float                      Width;                       // +0x5c
    long                       ExpansionType;               // +0x60
    CRbTree<0>                 Sprites;                     // +0x64 map<ETableSprites, long>
    CRetailVector<long>        Children;                    // +0x70
    CRetailVector<unsigned long> HorizontalSeparations;     // +0x7c
    CRetailVector<unsigned long> VerticalSeparations;       // +0x88
    CRetailVector<unsigned long> NonScrollingChildren;      // +0x94
    unsigned char              _padA0[0x1a8 - 0xa0];        // MeshType .. Redefiner
    CRbTree<1>                 ActionMap;                   // +0x1a8 map<unsigned long, CCharString>
    CRbTree<2>                 ActionMapAliases;            // +0x1b4 map<unsigned long, unsigned long>
    CRetailVector<unsigned long> ActionOrder;               // +0x1c0
    unsigned char              _pad1cc[0x1d8 - 0x1cc];      // edit-box settings
    CWideString                MovieFilename;               // +0x1d8
    unsigned char              _pad1dc[4];                  // LayerIndependant
    CRetailVector<unsigned long> SwappingStates;            // +0x1e0
    CRetailVector<float>       SwappingTimes;               // +0x1ec
    unsigned char              _pad1f8[0x214 - 0x1f8];      // BastardChild .. RightClickedState
    CRetailVector<unsigned long> ShapeChildren;             // +0x214
    unsigned char              _pad220[0x22c - 0x220];      // UseViewArea, PCStyle, Sprite2DFlag
    virtual void v00();
    ~CUIDef();
};
#pragma pack(pop)

CUIDef::~CUIDef()
{
}
}