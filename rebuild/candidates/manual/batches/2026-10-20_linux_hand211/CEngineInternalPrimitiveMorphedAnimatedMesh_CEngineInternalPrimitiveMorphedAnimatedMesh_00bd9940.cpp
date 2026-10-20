// CEngineInternalPrimitiveMorphedAnimatedMesh::CEngineInternalPrimitiveMorphedAnimatedMesh() -- the animated-mesh primitive built as type 0xb with flags 0xa, then this class's vtable and its morph info
// Leaf: ??0CEngineInternalPrimitiveMorphedAnimatedMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9940, 237 bytes (plain `ret`); vtable 0x012A7C04 (after the inlined
// CEngineInternalPrimitiveAnimatedMesh constructor's 0x012A79B4).
// Label: the catalog says CTCShotDefinitionBase::CTCShotDefinitionBase (propagated); the vtable store is
// CEngineInternalPrimitiveMorphedAnimatedMesh's (rtti-vtables.tsv), Morph is its Ego_r.pdb member.
// Callees: CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long, unsigned long) @ 0x00B8FF00, CBaseClass ctor
// @ 0x0099A2D0 (x5), malloc @ 0x00BFEA0E (the cache map's header node), CEngineInternalPrimitiveMorphInfo ctor @ 0x00BAA250.
// Layout: Morph (CEngineInternalPrimitiveMorphInfo) +0x128 (PDB +0x12c less the map's 4 bytes); the rest as 0x00BD92E0.
// Shape: as CEngineInternalPrimitiveAnimatedMesh's own constructor 0x00BD92E0 (same TU model) through its inline (type, flags)
// constructor; the derived vtable store and the out-of-line member constructor follow.
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
struct C3DVector { float X, Y, Z; C3DVector() { } };
struct CRightHandedSet { C3DVector Right, Up; CRightHandedSet() { } };
struct C3DAnimationBlendState : CBaseClass {           // vtable 0x01239838
    struct CSharedData;
    CIVCountedPointer<CSharedData> Data;                // +0x04
    virtual void v001();
    C3DAnimationBlendState() { }
};
struct C3DAnimationInterpolator : CBaseClass {         // vtable 0x01239840
    C3DAnimationBlendState StartState;                  // +0x04
    C3DAnimationBlendState EndState;                    // +0x0c
    virtual void v001();
    C3DAnimationInterpolator() { }
};
struct C3DPositionInterpolator : CBaseClass {          // vtable 0x0123981C
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
    virtual void v001();
    C3DPositionInterpolator() { }
};
struct C3DOrientationInterpolator : CBaseClass {       // vtable 0x01239824
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
struct CEngineRenderInfoMeshBase {
    struct CEngineLocalLightArray* Lights;              // +0x00
    struct C3DMesh2* Mesh;                              // +0x04
    unsigned long LOD;                                  // +0x08
};
struct CMatrix3x4 { float M[12]; };
struct CMatrix4x4;
struct CEngineRenderInfoAnimatedMesh : CEngineRenderInfoMeshBase {
    CMatrix3x4 ObjectTransform;                         // +0x0c
    float Distance;                                     // +0x3c
    CMatrix4x4* BoneTransforms;                         // +0x40
    long RenderFrame;                                   // +0x44
    CEngineRenderInfoAnimatedMesh() : BoneTransforms(0), RenderFrame(-1) { }
};
struct CEngineInternalPrimitiveBase {
    virtual void v00();
    unsigned char _body[0x44];                          // +0x04 (Ego_r.pdb: flags, fade, grid, alpha, render layer, links)
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};
struct C3DMeshInfo;
struct CEnginePrimitiveSimulationInfo;
struct CEngineMeshEffect;
struct CRGBColour { unsigned char B, G, R, A; CRGBColour() { } };
struct CEnginePrimitiveThemeChannelDesc {
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
struct CEngineInternalPrimitiveAnimatedMesh : CEngineInternalPrimitiveMeshBase {   // vtable 0x012A79B4
    C3DAnimationInterpolator Anim;                      // +0x70
    C3DPositionInterpolator WorldPos;                   // +0x84
    C3DOrientationInterpolator Orientation;             // +0xa0
    CCache Cache;                                       // +0xd4
    CEngineRenderInfoAnimatedMesh RenderInfo;           // +0xe0
    virtual void v00();
    CEngineInternalPrimitiveAnimatedMesh();
    __forceinline CEngineInternalPrimitiveAnimatedMesh(long type, unsigned long flags) : CEngineInternalPrimitiveMeshBase(type, flags) { }
};
struct CEngineInternalPrimitiveMorphInfo {
    unsigned char _body[0x40];                          // decal / composite-surface vector maps, hidden material set and array
    CEngineInternalPrimitiveMorphInfo();                // 0x00BAA250
};
struct CEngineInternalPrimitiveMorphedAnimatedMesh : CEngineInternalPrimitiveAnimatedMesh {   // vtable 0x012A7C04
    CEngineInternalPrimitiveMorphInfo Morph;            // +0x128
    virtual void v00();
    CEngineInternalPrimitiveMorphedAnimatedMesh();
};

CEngineInternalPrimitiveMorphedAnimatedMesh::CEngineInternalPrimitiveMorphedAnimatedMesh()
    : CEngineInternalPrimitiveAnimatedMesh(0xb, 0xa)
{
}
