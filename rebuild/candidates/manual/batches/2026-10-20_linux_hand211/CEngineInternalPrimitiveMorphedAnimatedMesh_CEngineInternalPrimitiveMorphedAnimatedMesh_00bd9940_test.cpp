// Behaviour test for CEngineInternalPrimitiveMorphedAnimatedMesh() (0x00BD9940): the primitive base gets the type/flags pair, five CBaseClass-rooted
// interpolator parts are built, the mesh base's pointers and theme channel are cleared, the cache map is an empty tree whose
// header links to itself, and the render info has no bone transforms and frame -1.
#include <cstdio>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#endif
#include <stddef.h>
#include <cstdlib>
static int g_bases = 0, g_morphs = 0; static long type = 0; static unsigned long g_flags = 0;
class CBaseClass {                                      // vtable 0x01231710
public:
    virtual void v000() { }
    CBaseClass() { ++g_bases; }
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
    virtual void v001() { }
    C3DAnimationBlendState() { }
};
struct C3DAnimationInterpolator : CBaseClass {         // vtable 0x01239840
    C3DAnimationBlendState StartState;                  // +0x04
    C3DAnimationBlendState EndState;                    // +0x0c
    virtual void v001() { }
    C3DAnimationInterpolator() { }
};
struct C3DPositionInterpolator : CBaseClass {          // vtable 0x0123981C
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
    virtual void v001() { }
    C3DPositionInterpolator() { }
};
struct C3DOrientationInterpolator : CBaseClass {       // vtable 0x01239824
    CRightHandedSet StartOrientation;                   // +0x04
    CRightHandedSet EndOrientation;                     // +0x1c
    virtual void v001() { }
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
    virtual void v00() { }
    unsigned char _body[0x44];                          // +0x04 (Ego_r.pdb: flags, fade, grid, alpha, render layer, links)
    CEngineInternalPrimitiveBase(long t, unsigned long flags) { type = t; g_flags = flags; }
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
    virtual void v00() { }
    CEngineInternalPrimitiveAnimatedMesh();
    __forceinline CEngineInternalPrimitiveAnimatedMesh(long type, unsigned long flags) : CEngineInternalPrimitiveMeshBase(type, flags) { }
};
struct CEngineInternalPrimitiveMorphInfo {
    unsigned char _body[0x40];                          // decal / composite-surface vector maps, hidden material set and array
    CEngineInternalPrimitiveMorphInfo() { ++g_morphs; }
};
struct CEngineInternalPrimitiveMorphedAnimatedMesh : CEngineInternalPrimitiveAnimatedMesh {   // vtable 0x012A7C04
    CEngineInternalPrimitiveMorphInfo Morph;            // +0x128
    virtual void v00() { }
    CEngineInternalPrimitiveMorphedAnimatedMesh();
};

CEngineInternalPrimitiveMorphedAnimatedMesh::CEngineInternalPrimitiveMorphedAnimatedMesh()
    : CEngineInternalPrimitiveAnimatedMesh(0xb, 0xa)
{
}

int main()
{
    int bad = 0;
    CEngineInternalPrimitiveMorphedAnimatedMesh* m = new CEngineInternalPrimitiveMorphedAnimatedMesh();
    if (!(type == 0xb && g_flags == 0xa && g_morphs == 1)) bad |= 1;
    if (g_bases != 5) bad |= 2;
    if (m->m_unnamed48 || m->m_unnamed4c || m->MeshInfo || m->SimulationInfo.Object || m->MeshEffect.Object) bad |= 4;
    if (m->ThemeChannelDesc.ChannelIndex0 || m->ThemeChannelDesc.ChannelIndex1 || m->ThemeChannelDesc.ChannelBlendFactor) bad |= 8;
    if (m->Anim.StartState.Data.Object || m->Anim.EndState.Data.Object) bad |= 16;
    SCacheNode* h = m->Cache.Cache._M_header.m_data;
    if (!h || h->_M_color != 0 || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h || m->Cache.Cache._M_node_count != 0) bad |= 32;
    if (m->RenderInfo.BoneTransforms != 0 || m->RenderInfo.RenderFrame != -1) bad |= 64;
    if (bad) { std::printf("00bd9940_TEST FAIL\n"); return 1; }
    std::printf("00bd9940_TEST PASS\n");
    return 0;
}
