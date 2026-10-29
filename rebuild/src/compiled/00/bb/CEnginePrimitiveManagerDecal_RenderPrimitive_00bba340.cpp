// CEnginePrimitiveManagerDecal::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when enabled, wraps the decal in a pool-allocated sub-primitive and queues it in sort list 0 at its distance from the camera; returns 2 (rendered), else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBA340, length 142 bytes.
// Callees: 0x00BEB540 CEngineSubPrimitiveBase::operator new (`__fastcall`, size 0x18 in ecx: the sub-primitive pool),
// 0x0099A2F0 CBaseClassNonCopyable::CBaseClassNonCopyable(), 0x00B84720 CEngineSubPrimitiveRenderer::AddSubPrimitive(long list,
// CEngineSubPrimitiveBase*, float distance, CEnginePrimitiveManager*) on the global renderer @ 0x01436E74; the
// camera global @ 0x01436EA0 (world position at +0x54). Vtable 0x012A5ADC = CEngineSubPrimitiveDecal.
// Layout: CEnginePrimitiveManager::Enable at +8 (PDB); CEngineInternalPrimitiveDecal::WorldPos at +0x54 (PDB);
// CEngineSubPrimitiveBase (PDB, 0x18): Parent +0xc, NextSubPrimitive +0x10.
// Shape: the sub-primitive's constructor chain is inlined: the out-of-line CBase constructor, Parent and the null
// NextSubPrimitive from the `__declspec(novtable)` CEngineSubPrimitiveBase, then the decal's vtable; the distance
// is the camera-to-decal difference computed X, Y, Z into x87 registers and summed top-first (Z*Z + Y*Y + X*X).
#include <math.h>

class C3DVector { public: float X, Y, Z; };

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class __declspec(novtable) CEngineSubPrimitiveBase : public CBaseClassNonCopyable {   // PDB, 0x18 bytes
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540 (sub-primitive pool)
};

class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                    // +0x54 (CameraDesc +0x14, its WorldPosition +0x40)
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

class CEngineInternalPrimitiveDecal {               // CEngineInternalPrimitiveBase (0x48) + decal data (PDB)
public:
    unsigned char _base[0x54];
    C3DVector     WorldPos;                         // +0x54
};
class CEngineSubPrimitiveDecal : public CEngineSubPrimitiveBase {   // vtable 0x012A5ADC
public:
    CEngineSubPrimitiveDecal(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveDecal();
};
class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerDecal {
public:
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerDecal::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                        CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    CEngineSubPrimitiveDecal* subPrimitive = new CEngineSubPrimitiveDecal(primitive);
    const CEngineInternalPrimitiveDecal& decal = (const CEngineInternalPrimitiveDecal&)primitive;
    float dx = g_pEngineCamera->WorldPosition.X - decal.WorldPos.X;
    float dy = g_pEngineCamera->WorldPosition.Y - decal.WorldPos.Y;
    float dz = g_pEngineCamera->WorldPosition.Z - decal.WorldPos.Z;
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(0, subPrimitive, (float)sqrt(dx * dx + dy * dy + dz * dz),
                                                   (CEnginePrimitiveManager*)this);
    return RENDERED_SUB_PRIMITIVES;
}