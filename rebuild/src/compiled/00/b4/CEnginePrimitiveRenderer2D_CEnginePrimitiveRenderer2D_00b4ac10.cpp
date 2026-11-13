// CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D() -- build the 2D primitive renderer: CEngineComponent base, empty
//   PrimitiveManagers array and counted pointers, the five 2D primitive managers created over the function-static empty scene list
//   `empty_scene` and registered through AddPrimitiveManager, FOV2D from the engine def (FOV_2D degrees -> radians, pi/4 when not
//   positive), the light vector (normalised (1,1,1)), ambient/diffuse/back light colours and the 83-entry 2D layer distance table.
// Leaf: ??0CEnginePrimitiveRenderer2D@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4AC10, 826 bytes (body), thiscall, plain `ret`, no EH frame.
// Identity: Ego_r.pdb 0x009EB690 / FableWin.pdb ??0CEnginePrimitiveRenderer2D@@QAE@XZ; vtable 0x012A2308.
// Callees: CEngineComponent::CEngineComponent 0x00B59710; operator new 0x00BFEA1A / delete 0x00BFE9BC; atexit 0x004012BC (empty_scene
//   dtor _$E2 0x0122AD60); CEnginePrimitiveManager2DStaticMeshes ctor 0x00BA1AC0, CEnginePrimitiveManager2DAnimatedMeshes 0x00BD1F30,
//   CEnginePrimitiveManager2DSprites 0x00BAD040, CEnginePrimitiveManagerParticleGroup 0x00BBBF40 (+ inline 2D derived ctor, vtable
//   0x012A22D0), CEnginePrimitive2DViewportManager 0x00BE1E20; CCountedPointer<T>::Reset 0x00B4B360 / 0x00B4B410 / 0x00B4B4C0 /
//   0x00B4B620 / 0x00B4B570; AddPrimitiveManager 0x00B4ABB0; C3DVector::AccurateNormalise 0x00A14440.
// Shape: AddPrimitiveManager is defined just before the ctor in the same retail TU; its real body sits here out of line
//   (#pragma auto_inline(off)) so c2 keeps each converted counted-pointer temp's Info in a register across the call (dec/jne release).
//   FOV: `2pi * (FOV_2D * 1/360)` keeps the FOV_2D load first and both pooled multiplies; the light vector and the three colours are
//   written through references (retail: `lea ecx,[esi+7Ch]` then [ecx]/[ecx+4]/[ecx+8], and the xor of the loop index between
//   the diffuse and back colours).
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_MEMBERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CENGINEDEF_FOV_2D
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_MANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_REGISTER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEnginePrimitiveManager2DParticleGroup.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineDef.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"

// Real hierarchy (Ego_r.pdb ??0CEnginePrimitiveManagerStaticMeshesBase 0x00A55E60 / ??0CEnginePrimitiveManagerAnimatedMeshesBase
// 0x00A84830; RTTI base lists of vtables 0x012A5160 / 0x012A7964): each 2D mesh manager derives directly from its *MeshesBase,
// which derives from CEnginePrimitiveManagerMeshBase -> CEnginePrimitiveManagerOcclusionBase -> CEnginePrimitiveManager.
// Both 2D classes add no members (Ego_r sizeof 0x318 / 0x690 = their bases'). StaticMeshesBase comes from its shared header;
// CEnginePrimitiveManagerAnimatedMeshesBase has no shared header, so it is modelled locally (sizeof 0x690, Ego_r).
class CEnginePrimitiveManagerAnimatedMeshesBase : public CEnginePrimitiveManagerMeshBase {   // no shared header; Ego_r sizeof 0x690
public:
    CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>& scenes);     // 0x00A84830
    virtual ~CEnginePrimitiveManagerAnimatedMeshesBase();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
private:
    unsigned char _pad_0x20[0x690 - 0x20];
};
class CEnginePrimitiveManager2DStaticMeshes : public CEnginePrimitiveManagerStaticMeshesBase {     // RTTI vtable 0x012A5160
public:
    CEnginePrimitiveManager2DStaticMeshes(CArray<CEngineScene*>& scenes);         // 0x00BA1AC0
    virtual ~CEnginePrimitiveManager2DStaticMeshes();
};
class CEnginePrimitiveManager2DAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase {   // RTTI vtable 0x012A7964
public:
    CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>& scenes);       // 0x00BD1F30
    virtual ~CEnginePrimitiveManager2DAnimatedMeshes();
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManager2DStaticMeshes) == 0x318);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManager2DAnimatedMeshes) == 0x690);

extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                     // 0x01436E84
extern const CEngineDef* g_pEngineDef_01436e20;                                  // 0x01436E20 (descriptive)
extern const float g_TwoPi;                             // 0x0128F608

// Real body of the landed row rebuild/src/compiled/00/b4/CEnginePrimitiveRenderer2D_AddPrimitiveManager_00b4abb0.cpp
// (CEnginePrimitiveRenderer2D::AddPrimitiveManager 0x00B4ABB0, defined just before this ctor in the same retail TU), copied
// verbatim and kept out of line, as retail calls it.
#pragma auto_inline(off)
void CEnginePrimitiveRenderer2D::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager)
{
    PrimitiveManagers.push_back(manager);
    g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(manager.Get());
}
#pragma auto_inline(on)

CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D()
    : First2DPrim(0), First2DDisplacementPrim(0)
{
    static CArray<CEngineScene*> empty_scene;
    StaticMesh2DManager.Reset(new CEnginePrimitiveManager2DStaticMeshes(empty_scene));
    AnimatedMesh2DManager.Reset(new CEnginePrimitiveManager2DAnimatedMeshes(empty_scene));
    Sprite2DManager.Reset(new CEnginePrimitiveManager2DSprites(empty_scene));
    Particle2DManager.Reset(new CEnginePrimitiveManager2DParticleGroup(empty_scene));
    Viewport2DManager.Reset(new CEnginePrimitive2DViewportManager(empty_scene));
    AddPrimitiveManager(StaticMesh2DManager);
    AddPrimitiveManager(AnimatedMesh2DManager);
    AddPrimitiveManager(Sprite2DManager);
    AddPrimitiveManager(Particle2DManager);
    AddPrimitiveManager(Viewport2DManager);
    FOV2D = g_TwoPi * (g_pEngineDef_01436e20->FOV_2D * (1.0f / 360.0f));
    if (FOV2D <= 0.0f)
        FOV2D = 0.78539819f;
    C3DVector& light = LightVector;
    light.X = 1.0f;
    light.Y = 1.0f;
    light.Z = 1.0f;
    light.AccurateNormalise();
    float depth = 0.1f;
    CRGBFloatColour& ambient = AmbientLightColour;
    ambient.R = 0.5f;
    ambient.G = 0.5f;
    ambient.B = 0.5f;
    ambient.A = 1.0f;
    CRGBFloatColour& diffuse = DiffuseLightColour;
    diffuse.R = 0.5f;
    diffuse.G = 0.5f;
    diffuse.B = 0.5f;
    diffuse.A = 1.0f;
    CRGBFloatColour& back = BackLightColour;
    back.R = 0.0f;
    back.G = 0.0f;
    back.B = 0.0f;
    back.A = 1.0f;
    for (long i = 0; i < 83; ++i) {
        if (i == 72) {
            Layer2DDistances[i] = depth + 0.40499997f;   // 0x3ECF5C28 (pool 0x0139A3C4); 0.405f would be 0x3ECF5C29
            depth += 0.80999994f;                        // 0x3F4F5C28 (pool 0x0139A3BC); 0.81f would be 0x3F4F5C29
        } else {
            Layer2DDistances[i] = depth + 0.00054878066f;
            depth += 0.0010975613f;
        }
        Enable2DLayer[i] = true;
        Layer2DSortDistances[i] = (float)i;
    }
}