// CEngineInternalPrimitive2DAnimatedMesh::CEngineInternalPrimitive2DAnimatedMesh() -- the 2D animated-mesh primitive: the animated-mesh primitive built as type 0x25 with flags 0x80000000, then this class's vtable and a zeroed object bounding sphere
// Leaf: ??0CEngineInternalPrimitive2DAnimatedMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD94E0, 253 bytes (plain `ret`); vtable 0x012A7A44 (stored after the inlined
//   CEngineInternalPrimitiveAnimatedMesh constructor's 0x012A79B4; rtti-vtables.tsv slot 0 0x00BD96F0).
// Label: the catalog says CTCShotDefinitionBase::CTCShotDefinitionBase (propagated); the vtable is
//   CEngineInternalPrimitive2DAnimatedMesh's (Ego_r.pdb 340 bytes: Initialised +0x12C, Layer, ClipBox, ObjectBoundingSphere +0x144).
// Callees: CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long, unsigned long) @ 0x00B8FF00, CBaseClass ctor
//   @ 0x0099A2D0 (x5), malloc @ 0x00BFEA0E (the cache map's header node).
// Layout and shape: the sibling CEngineInternalPrimitiveAnimatedMeshFromFile ctor 0x00BD9830 (chunk 211 model) through the
//   inline (type, flags) constructor; retail sits 4 below the PDB from the animated mesh's RenderInfo on (the 2D members at
//   +0x128); the bounding sphere's inline constructor zeroes its four words after the vtable store (integer zeroes through
//   the shared ebx).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);          // 0x00BFEA0E
class CBaseClass {                                      // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                       // 0x0099A2D0
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
};
class C3DVector { public: float X, Y, Z; C3DVector() { } };
class CRightHandedSet { public: C3DVector Right, Up; CRightHandedSet() { } };
class C3DAnimationBlendState : public CBaseClass { public:           // vtable 0x01239838
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;                // +0x04
    virtual void v001();
    C3DAnimationBlendState() { }
};
class C3DAnimationInterpolator : public CBaseClass { public:         // vtable 0x01239840
    C3DAnimationBlendState StartState;                  // +0x04
    C3DAnimationBlendState EndState;                    // +0x0c
    virtual void v001();
    C3DAnimationInterpolator() { }
};
class C3DPositionInterpolator : public CBaseClass { public:          // vtable 0x0123981C
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
    virtual void v001();
    C3DPositionInterpolator() { }
};
class C3DOrientationInterpolator : public CBaseClass { public:       // vtable 0x01239824
    CRightHandedSet StartOrientation;                   // +0x04
    CRightHandedSet EndOrientation;                     // +0x1c
    virtual void v001();
    C3DOrientationInterpolator() { }
};
template <class T> struct CAllocator {                  // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> { // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
struct SCacheNode {                                     // _Rb_tree_node<pair<const unsigned long, CArray<C3DVector> > >
    unsigned char _M_color;                             // +0x00
    SCacheNode* _M_parent;                              // +0x04
    SCacheNode* _M_left;                                // +0x08
    SCacheNode* _M_right;                               // +0x0c
    unsigned char _M_value_field[0x10];                 // +0x10
};
struct CCacheTreeBase {                                 // STLport _Rb_tree_base
    CAllocProxy<SCacheNode> _M_header;
    CCacheTreeBase(const CAllocator<SCacheNode>& alloc) : _M_header(alloc, 0)
    {
        _M_header.m_data = (SCacheNode*)malloc(sizeof(SCacheNode));
    }
};
struct CLessULong { CLessULong() { } };
struct CCacheTree : CCacheTreeBase {                    // STLport _Rb_tree (map<unsigned long, CArray<C3DVector> >)
    unsigned long _M_node_count;
    CLessULong _M_key_compare;
    CCacheTree() : CCacheTreeBase(CAllocator<SCacheNode>()), _M_node_count(0)
    {
        _M_header.m_data->_M_color = 0;                 // _S_rb_tree_red
        _M_header.m_data->_M_parent = 0;
        _M_header.m_data->_M_left = _M_header.m_data;
        _M_header.m_data->_M_right = _M_header.m_data;
    }
};
struct CCache {
    CCacheTree Cache;                                   // map<unsigned long, CArray<C3DVector> >
};
class CEngineRenderInfoMeshBase { public:
    class CEngineLocalLightArray* Lights;              // +0x00
    class C3DMesh2* Mesh;                              // +0x04
    unsigned long LOD;                                  // +0x08
};
class CMatrix3x4 { public: float M[12]; };
class CMatrix4x4;
class CEngineRenderInfoAnimatedMesh : public CEngineRenderInfoMeshBase { public:
    CMatrix3x4 ObjectTransform;                         // +0x0c
    float Distance;                                     // +0x3c
    CMatrix4x4* BoneTransforms;                         // +0x40
    long RenderFrame;                                   // +0x44
    CEngineRenderInfoAnimatedMesh() : BoneTransforms(0), RenderFrame(-1) { }
};
class CEngineInternalPrimitiveBase { public:
    virtual void v00();
    unsigned char _body[0x44];                          // +0x04 (Ego_r.pdb: flags, fade, grid, alpha, render layer, links)
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};
class C3DMeshInfo;
class CEnginePrimitiveSimulationInfo;
class CEngineMeshEffect;
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour() { } };
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;                        // +0x00
    unsigned char ChannelIndex1;                        // +0x01
    unsigned char ChannelBlendFactor;                   // +0x02
    unsigned char Pad;                                  // +0x03
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct __declspec(novtable) CEngineInternalPrimitiveMeshBase : CEngineInternalPrimitiveBase {
    unsigned long m_unnamed48;                          // +0x48 (unnamed in Ego_r.pdb)
    unsigned long m_unnamed4c;                          // +0x4c (unnamed in Ego_r.pdb)
    C3DMeshInfo* MeshInfo;                              // +0x50
    C3DMeshInfo* ShadowMeshInfo;                        // +0x54
    float Scale;                                        // +0x58
    CRGBColour TintColour;                              // +0x5c
    bool BulletTimeAffectsCloth;                        // +0x60
    CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;   // +0x64
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;    // +0x68
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;  // +0x6c
    CEngineInternalPrimitiveMeshBase(long type, unsigned long flags)
        : CEngineInternalPrimitiveBase(type, flags), m_unnamed48(0), m_unnamed4c(0), MeshInfo(0) { }
};
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { public:   // vtable 0x012A79B4
    C3DAnimationInterpolator Anim;                      // +0x70
    C3DPositionInterpolator WorldPos;                   // +0x84
    C3DOrientationInterpolator Orientation;             // +0xa0
    CCache Cache;                                       // +0xd4
    CEngineRenderInfoAnimatedMesh RenderInfo;           // +0xe0
    virtual void v00();
    __forceinline CEngineInternalPrimitiveAnimatedMesh(long type, unsigned long flags) : CEngineInternalPrimitiveMeshBase(type, flags) { }
};
struct C2DVectorF { float X, Y; C2DVectorF() { } };
class C2DBoxF { public: C2DVectorF TopLeft, BottomRight; C2DBoxF() { } };
class C3DBoundingSphere { public:                              // 16 bytes, zeroed by its inline constructor
    long Centre[3];                                     // C3DVector (zero words)
    long Radius;
    C3DBoundingSphere() { Centre[0] = 0; Centre[1] = 0; Centre[2] = 0; Radius = 0; }
};
class CEngineInternalPrimitive2DAnimatedMesh : public CEngineInternalPrimitiveAnimatedMesh { public:   // vtable 0x012A7A44
    bool          Initialised;                          // +0x128 (PDB 0x12C)
    long          Layer;                                // +0x12c EEngine2DLayer
    C2DBoxF       ClipBox;                              // +0x130
    C3DBoundingSphere ObjectBoundingSphere;             // +0x140
    virtual void v00();
    CEngineInternalPrimitive2DAnimatedMesh();
    __forceinline CEngineInternalPrimitive2DAnimatedMesh(long type, unsigned long flags) : CEngineInternalPrimitiveAnimatedMesh(type, flags) { }
};

CEngineInternalPrimitive2DAnimatedMesh::CEngineInternalPrimitive2DAnimatedMesh()
    : CEngineInternalPrimitiveAnimatedMesh(0x25, 0x80000000)
{
}