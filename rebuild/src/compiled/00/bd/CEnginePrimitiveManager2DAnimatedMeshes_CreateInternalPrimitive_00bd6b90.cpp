// CEnginePrimitiveManager2DAnimatedMeshes::CreateInternalPrimitive(long type) -- allocate and construct the internal primitive for a 2D animated-mesh type: 0x25 = 2D animated mesh, 0x26 = 2D morphed animated mesh; null for any other type or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD6B90, length 601 bytes (`ret 4`, `this` unused).
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long flags)
// @ 0x00B8FF00 ((0x25|0x26, 0x80000000)), CBaseClass::CBaseClass @ 0x0099A2D0 (the interpolators' root), malloc @ 0x00BFEA0E (the
// cache map's header node), CEngineInternalPrimitiveMorphInfo::CEngineInternalPrimitiveMorphInfo @ 0x00BAA250 (manifest
// label "CTCOpinionSource::CTCOpinionSource" is propagated; it is the morphed mesh's Morph member at +0x150).
// Vtables (rtti-vtables.tsv): 0x012A79B4 CEngineInternalPrimitiveAnimatedMesh, 0x01239840 C3DAnimationInterpolator,
// 0x01239838 C3DAnimationBlendState, 0x0123981C C3DPositionInterpolator, 0x01239824 C3DOrientationInterpolator,
// 0x012A7A44 CEngineInternalPrimitive2DAnimatedMesh, 0x012A7ADC CEngineInternalPrimitive2DMorphedAnimatedMesh.
// Layout: the `__declspec(novtable)` mesh base of the landed static-mesh factory 0x00BA7820 (zero words +0x48..+0x50,
// counted pointers +0x64/+0x68, theme-channel bytes +0x6c), then PDB CEngineInternalPrimitiveAnimatedMesh: Anim +0x70
// (CBase root + two blend states, each vptr + counted pointer), WorldPos +0x84, Orientation +0xa0, Cache +0xd4 (a
// 12-byte STLport map: header node from malloc(0x20), count), RenderInfo from +0xe0 (two words initialised: +0x120 = 0,
// +0x124 = -1), then the 2D part: bounding sphere +0x140; sizes 0x150 / 0x188 (PDB 0x154 / 0x19c: retail is narrower).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

template <class T> class CIVCountedPointer { public:     // intrusive counted pointer (one word)
    T* m_p;
    __forceinline CIVCountedPointer() : m_p(0) { }
};
class CEnginePrimitiveSimulationInfo;
class CEngineMeshEffect;

class CEnginePrimitiveThemeChannelDesc { public:          // three flag bytes
    bool m_a, m_b, m_c;
    __forceinline CEnginePrimitiveThemeChannelDesc() : m_a(false), m_b(false), m_c(false) { }
};

struct __declspec(novtable) CEngineInternalPrimitiveMeshBase : CEngineInternalPrimitiveBase {   // intermediate base
    long          m_mesh;                          // +0x48
    long          m_material;                      // +0x4C
    long          m_animation;                     // +0x50
    unsigned char _pad54[0x10];
    CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;   // +0x64
    CIVCountedPointer<CEngineMeshEffect>              MeshEffect;       // +0x68
    CEnginePrimitiveThemeChannelDesc                  ThemeChannelDesc; // +0x6C
    __forceinline CEngineInternalPrimitiveMeshBase(long type, unsigned long flags)
        : CEngineInternalPrimitiveBase(type, flags), m_mesh(0), m_material(0), m_animation(0) { }
};

class CBaseClass {
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();
};

class C3DAnimationBlendState : public CBaseClass { public:            // vtable 0x01239838, 8 bytes
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;           // +0x04
    virtual ~C3DAnimationBlendState();
};

class C3DAnimationInterpolator : public CBaseClass { public:          // vtable 0x01239840, 0x14 bytes
    C3DAnimationBlendState StartState;             // +0x04
    C3DAnimationBlendState EndState;               // +0x0C
    virtual ~C3DAnimationInterpolator();
};

class C3DVector { public: float X, Y, Z; };
class C3DPositionInterpolator : public CBaseClass { public:           // vtable 0x0123981C, 0x1c bytes
    C3DVector StartPosition;                       // +0x04
    C3DVector EndPosition;                         // +0x10
    virtual ~C3DPositionInterpolator();
};

class CRightHandedSet { public: float m[6]; };
class C3DOrientationInterpolator : public CBaseClass { public:        // vtable 0x01239824, 0x34 bytes
    CRightHandedSet StartOrientation;              // +0x04
    CRightHandedSet EndOrientation;                // +0x1C
    virtual ~C3DOrientationInterpolator();
};

struct CRetailMap {                                // STLport map: header node pointer, node count, empty compare
    struct SNode { unsigned char m_color; SNode* m_parent; SNode* m_left; SNode* m_right; long m_value[4]; };
    SNode*        m_header;
    unsigned long m_count;
    unsigned char m_compare;
    __forceinline CRetailMap() : m_header(0)
    {
        m_header = (SNode*)malloc(sizeof(SNode));
        m_count = 0;
        m_header->m_color = 0;
        m_header->m_parent = 0;
        m_header->m_left = m_header;
        m_header->m_right = m_header;
    }
};

class CEngineRenderInfoAnimatedMesh { public:             // +0xe0 in the mesh; only two words initialised
    unsigned char _pad00[0x40];
    long          m_count;                         // +0x40 (+0x120)
    long          m_index;                         // +0x44 (+0x124)
    __forceinline CEngineRenderInfoAnimatedMesh() : m_count(0), m_index(-1) { }
};

class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { public:   // vtable 0x012A79B4
    C3DAnimationInterpolator      Anim;            // +0x70
    C3DPositionInterpolator       WorldPos;        // +0x84
    C3DOrientationInterpolator    Orientation;     // +0xA0
    CRetailMap                    Cache;           // +0xD4
    CEngineRenderInfoAnimatedMesh RenderInfo;      // +0xE0
    virtual void v00();
    __forceinline CEngineInternalPrimitiveAnimatedMesh(long type, unsigned long flags)
        : CEngineInternalPrimitiveMeshBase(type, flags) { }
};

class C3DBoundingSphere { public:
    long m_centreX, m_centreY, m_centreZ, m_radius;
    __forceinline C3DBoundingSphere() : m_centreX(0), m_centreY(0), m_centreZ(0), m_radius(0) { }
};

class CEngineInternalPrimitive2DAnimatedMesh : public CEngineInternalPrimitiveAnimatedMesh { public:   // vtable 0x012A7A44, 0x150 bytes
    unsigned char     _pad128[0x18];
    C3DBoundingSphere ObjectBoundingSphere;        // +0x140
    virtual void v00();
    __forceinline CEngineInternalPrimitive2DAnimatedMesh(long type)
        : CEngineInternalPrimitiveAnimatedMesh(type, 0x80000000) { }
};

class CEngineInternalPrimitiveMorphInfo { public:
    unsigned char _body[0x38];
    CEngineInternalPrimitiveMorphInfo();           // 0x00BAA250
};

struct CEngineInternalPrimitive2DMorphedAnimatedMesh : CEngineInternalPrimitive2DAnimatedMesh {   // vtable 0x012A7ADC, 0x188 bytes
    CEngineInternalPrimitiveMorphInfo Morph;       // +0x150
    virtual void v00();
    __forceinline CEngineInternalPrimitive2DMorphedAnimatedMesh() : CEngineInternalPrimitive2DAnimatedMesh(0x26) { }
};

class CEnginePrimitiveManager2DAnimatedMeshes { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManager2DAnimatedMeshes::CreateInternalPrimitive(long type)
{
    switch (type) {
    case 0x25:
        return new CEngineInternalPrimitive2DAnimatedMesh(0x25);
    case 0x26:
        return new CEngineInternalPrimitive2DMorphedAnimatedMesh;
    }
    return 0;
}