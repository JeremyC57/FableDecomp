// CEnginePrimitiveManagerAnimatedMeshesBase::CreateInternalPrimitive(long type) -- allocate and construct the internal primitive for an animated-mesh type: 9 = animated mesh, 0xb = morphed animated mesh, 0xd = animated mesh from file; null for any other type or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD6DF0, length 795 bytes (`ret 4`, `this` unused).
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long flags)
// @ 0x00B8FF00 ((9, 0xa), (0xb, 0xa), (0xd, 8)), CBase::CBase @ 0x0099A2D0 (the interpolators' root), malloc @ 0x00BFEA0E (the
// cache map's header node), CEngineInternalPrimitiveMorphInfo::CEngineInternalPrimitiveMorphInfo @ 0x00BAA250 (manifest
// label "CTCOpinionSource::CTCOpinionSource" is propagated; it is the morphed mesh's Morph member at +0x128).
// Vtables (rtti-vtables.tsv): 0x012A79B4 CEngineInternalPrimitiveAnimatedMesh, 0x01239840 C3DAnimationInterpolator,
// 0x01239838 C3DAnimationBlendState, 0x0123981C C3DPositionInterpolator, 0x01239824 C3DOrientationInterpolator,
// 0x012A7C04 CEngineInternalPrimitiveMorphedAnimatedMesh, 0x012A7B74 CEngineInternalPrimitiveAnimatedMeshFromFile.
// Layout: the model of the sibling factory 0x00BD6B90 (same chunk): the `__declspec(novtable)` mesh base of the landed
// static-mesh factory 0x00BA7820, then PDB CEngineInternalPrimitiveAnimatedMesh: Anim +0x70, WorldPos +0x84, Orientation
// +0xa0, Cache +0xd4 (STLport map, header node from malloc(0x20)), RenderInfo from +0xe0 (+0x120 = 0, +0x124 = -1);
// retail sizes 0x128 / 0x160 / 0x128 (PDB 0x12c for the plain mesh). The switch lowers to `sub 9 / sub 2 / sub 2` with
// the case-0xd body first and the case-9 body last.
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

namespace N_0099a2d0 { struct CBase {
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
}; } using N_0099a2d0::CBase;

class C3DAnimationBlendState : public CBase { public:            // vtable 0x01239838, 8 bytes
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;           // +0x04
    virtual ~C3DAnimationBlendState();
};

class C3DAnimationInterpolator : public CBase { public:          // vtable 0x01239840, 0x14 bytes
    C3DAnimationBlendState StartState;             // +0x04
    C3DAnimationBlendState EndState;               // +0x0C
    virtual ~C3DAnimationInterpolator();
};

class C3DVector { public: float X, Y, Z; };
class C3DPositionInterpolator : public CBase { public:           // vtable 0x0123981C, 0x1c bytes
    C3DVector StartPosition;                       // +0x04
    C3DVector EndPosition;                         // +0x10
    virtual ~C3DPositionInterpolator();
};

class CRightHandedSet { public: float m[6]; };
class C3DOrientationInterpolator : public CBase { public:        // vtable 0x01239824, 0x34 bytes
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

class CEngineInternalPrimitiveMorphInfo { public:
    unsigned char _body[0x38];
    CEngineInternalPrimitiveMorphInfo();           // 0x00BAA250
};

struct CEngineInternalPrimitiveMorphedAnimatedMesh : CEngineInternalPrimitiveAnimatedMesh {   // vtable 0x012A7C04, 0x160 bytes
    CEngineInternalPrimitiveMorphInfo Morph;       // +0x128
    virtual void v00();
    __forceinline CEngineInternalPrimitiveMorphedAnimatedMesh() : CEngineInternalPrimitiveAnimatedMesh(0xb, 0xa) { }
};

struct CEngineInternalPrimitiveAnimatedMeshFromFile : CEngineInternalPrimitiveAnimatedMesh {   // vtable 0x012A7B74, 0x128 bytes
    virtual void v00();
    __forceinline CEngineInternalPrimitiveAnimatedMeshFromFile() : CEngineInternalPrimitiveAnimatedMesh(0xd, 8) { }
};

struct CEnginePrimitiveManagerAnimatedMeshesBase {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerAnimatedMeshesBase::CreateInternalPrimitive(long type)
{
    switch (type) {
    case 9:
        return new CEngineInternalPrimitiveAnimatedMesh(9, 0xa);
    case 0xb:
        return new CEngineInternalPrimitiveMorphedAnimatedMesh;
    case 0xd:
        return new CEngineInternalPrimitiveAnimatedMeshFromFile;
    }
    return 0;
}