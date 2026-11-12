// CThingVillage::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- after the physical base, submit the village's current graphic as a static mesh at the interpolated position and orientation
// Grade: DIFFER(ours v retail) -- 308v309 as-is (regraded once, unchanged draft); local model, not migrated to the shared headers in this batch; residue: one byte short: retail keeps the interpolant in x87 (fld [info+4] after the tint, fmul st(2)/st(1)) and adds/stores the interpolated position X, Y, then Z; ours copies the by-value float parameter to a frame word (fmul [mem]) and adds Z, Y, X; frame 0x90 v 0x80, UniqueID low preloaded into ebx. A named float t keeps x87 but hoists its load (296 bytes); operator+ via += (overlay test) fixed the multiply phase only.
// Leaf: ?DrawPreparePrimitivesForRendering@CThingVillage@@QAE_NABUCEnginePrimitivePrepareInfo@@@Z
// Earlier grade note: DIFFER(308v309) -- one byte short: retail keeps the interpolant in x87 (fld [info+4] after the tint, fmul st(2)/st(1)) and adds/stores the interpolated position X, Y, then Z; ours copies the by-value float parameter to a frame word (fmul [mem]) and adds Z, Y, X; frame 0x90 v 0x80, UniqueID low preloaded into ebx. A named float t keeps x87 but hoists its load (296 bytes); operator+ via += (overlay test) fixed the multiply phase only.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083B6B0, length 309 bytes (+ int3 padding), `ret 4` (vtable slot 36).
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0, CThingVillage::GetCurrentGraphic
// @ 0x0083B520, the physics component's virtual slot 71 (+0x11c, the interpolated right-handed set by hidden pointer),
// CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh @ 0x0077BD70 (landed; 13 stack words), the engine's virtual slot 20
// (+0x50, add primitive), the mesh effect's slot 1 (destroy) when the temporary's reference drops to zero.
// Layout: CEnginePrimitivePrepareInfo (Ego_r.pdb, as PreparePrimitivesForEngine 0x0051EAB0): Current.WFInterpolate +4,
// Engine +0x34; CThing UniqueID +0x18, PhysicsTC +0x60 (Position +0x0c, LastPosition +0x18); CThingPhysical Scale +0x9c;
// the engine graphic's mesh index +0 and size +8. CEnginePrimitiveStaticMesh as the landed ctor (0x4c bytes, mesh effect
// pointer +0x3c).
// Shape: the primitive is a temporary handed straight to the engine (the ctor's `this` is pushed), its tint a by-value
// CRGBColour built in the argument slot, its theme-channel argument an uninitialised default; the position is
// `(Position - LastPosition) * t + LastPosition`.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
enum EEngineInterpolationSet { INTERPOLATION_DEFAULT = 0 };
struct CEngineMeshEffect {
    virtual void v00();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long m_refCount;                                           // +0x04
    void Release() { --m_refCount; if (m_refCount == 0) Destroy(); }
};
struct CEnginePrimitiveStaticMesh {                            // 0x4c bytes
    unsigned char      _pad00[0x3c];
    CEngineMeshEffect* MeshEffect;                             // +0x3c
    unsigned char      _pad40[0x4c - 0x40];
    CEnginePrimitiveStaticMesh(unsigned __int64 uid, unsigned long mesh, const C3DVector& pos, const CRightHandedSet& rhs,
                               float size, bool lit, unsigned char alpha, CRGBColour tint, float fade,
                               EEngineInterpolationSet interp, const CEnginePrimitiveThemeChannelDesc& channel, bool bullet);   // 0x0077BD70
    ~CEnginePrimitiveStaticMesh() { if (MeshEffect) MeshEffect->Release(); }
};
class CIEngine {
public:
    virtual void e000(); virtual void e001(); virtual void e002(); virtual void e003(); virtual void e004(); virtual void e005();
    virtual void e006(); virtual void e007(); virtual void e008(); virtual void e009(); virtual void e010(); virtual void e011();
    virtual void e012(); virtual void e013(); virtual void e014(); virtual void e015(); virtual void e016(); virtual void e017();
    virtual void e018(); virtual void e019();
    virtual void AddPrimitive(const CEnginePrimitiveStaticMesh& primitive);   // slot 20 (+0x50)
};
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
struct CEnginePrimitivePrepareInfo {
    CInterpolationInfoSet Current;                             // +0x00
    unsigned char         _pad08[0x34 - 0x08];
    CIEngine*             Engine;                              // +0x34
};
class CTCPhysicsBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual CRightHandedSet GetInterpolatedRHSet(float interpolate) const;   // slot 71 (+0x11c)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                                    // +0x0c
    C3DVector     LastPosition;                                // +0x18
};
struct CEngineGraphic {
    unsigned long MeshIndex;                                   // +0x00
    unsigned char _pad04[4];
    float         Size;                                        // +0x08
};
#pragma pack(push, 4)
class CThingPhysical {
public:
    virtual void v00();
    unsigned char    _pad04[0x18 - 0x04];
    unsigned __int64 UniqueID;                                 // +0x18
    unsigned char    _pad20[0x60 - 0x20];
    CTCPhysicsBase*  PhysicsTC;                                // +0x60
    unsigned char    _pad64[0x9c - 0x64];
    float            Scale;                                    // +0x9c
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
};
#pragma pack(pop)
class CThingVillage : public CThingPhysical {
public:
    CEngineGraphic* GetCurrentGraphic();                       // 0x0083B520
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingVillage::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        CEngineGraphic* graphic = GetCurrentGraphic();
        info.Engine->AddPrimitive(CEnginePrimitiveStaticMesh(UniqueID, graphic->MeshIndex,
            (PhysicsTC->Position - PhysicsTC->LastPosition) * info.Current.WFInterpolate + PhysicsTC->LastPosition,
            PhysicsTC->GetInterpolatedRHSet(info.Current.WFInterpolate), graphic->Size * Scale, true, 0xff,
            CRGBColour(0xff, 0xff, 0xff, 0xff), 0.0f, INTERPOLATION_DEFAULT, CEnginePrimitiveThemeChannelDesc(), true));
        return true;
    }
    return false;
}
