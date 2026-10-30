// CEnginePrimitiveMorphedAnimatingMesh::~CEnginePrimitiveMorphedAnimatingMesh() -- destroy the morph members (the ignored-bone-name set, the theme-channel vector, the texture-morph vector) and then the animating-mesh members (def pointer, animation interpolator, orientation and position interpolators)
// Leaf: ??1CEnginePrimitiveMorphedAnimatingMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00709B10, length 152 bytes.
// Label correction: catalogued as CTCAssassinRush::~CTCAssassinRush (propagated). Its three callers (0x00707853, 0x00727F55,
// 0x0077B60F) destroy a stack object built by 0x00709A50 = the CEnginePrimitiveMorphedAnimatingMesh constructor (15 arguments:
// texture morphs into +0x90, the CCharString set into +0xA8); no vtable store (engine primitive descriptors are plain classes).
// Callees: _Rb_tree<CCharString>::_M_erase @ 0x004ABA70 (catalog "LTextTreeWalkThrough::BuildTreeArray"), free @ 0x00BFEA14,
// the +0x9C vector dtor @ 0x007099E0, vector<CEngineTextureMorph>::~vector @ 0x005BC0D3, C3DAnimationInterpolator::
// ~C3DAnimationInterpolator @ 0x004C2E90, CBaseClass::~CBaseClass @ 0x0099A2E0 (the two novtable interpolators; the last a tail jump).
// Shape: the STLport set dtor inline: `if (_M_node_count) { _M_erase(root); leftmost = header; root = 0; rightmost = header;
// count = 0; }` with &set kept in esi and the header re-read for each store, then `if (header) free(header)`; the counted def
// release keeps its zero store inside the null test (inline ReleaseTemporary()).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct __declspec(novtable) C3DPositionInterpolator : CBaseClass {       // +0x1c, 0x1c bytes
    unsigned char _data[0x18];
};
struct __declspec(novtable) C3DOrientationInterpolator : CBaseClass {    // +0x38, 0x34 bytes
    unsigned char _data[0x30];
};
class C3DAnimationInterpolator { public:               // +0x6c, 0x14 bytes
    unsigned char _data[0x14];
    ~C3DAnimationInterpolator();                // 0x004C2E90
};
struct CCountedDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void ReleaseTemporary() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointer {
    CCountedDef* Data;
    ~CDefPointer()
    {
        if (Data) {
            Data->ReleaseTemporary();
            Data = 0;
        }
    }
};
struct CTextureMorphVector {                    // vector<CEngineTextureMorph>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CTextureMorphVector();                     // 0x005BC0D3
};
struct CThemeChannelVector {                    // the +0x9c vector
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CThemeChannelVector();                     // 0x007099E0
};
struct _Rb_tree_node_base {
    long _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct CCharStringSet {                         // STLport set<CCharString>
    _Rb_tree_node_base* _M_header;
    unsigned long _M_node_count;
    void _M_erase(_Rb_tree_node_base* x);       // 0x004ABA70
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
    ~CCharStringSet()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};

struct CEnginePrimitiveAnimatingMesh {
    unsigned char _pad00[0x1c];
    C3DPositionInterpolator PositionInterpolator;       // +0x1c
    C3DOrientationInterpolator OrientationInterpolator; // +0x38
    C3DAnimationInterpolator AnimationInterpolator;     // +0x6c
    CDefPointer PDef;                                   // +0x80
    unsigned char _pad84[0x90 - 0x84];
};
struct CEnginePrimitiveMorphedAnimatingMesh : CEnginePrimitiveAnimatingMesh {
    CTextureMorphVector TextureMorphs;                  // +0x90
    CThemeChannelVector ThemeChannels;                  // +0x9c
    CCharStringSet IgnoredBones;                        // +0xa8
    ~CEnginePrimitiveMorphedAnimatingMesh();
};

CEnginePrimitiveMorphedAnimatingMesh::~CEnginePrimitiveMorphedAnimatingMesh()
{
}