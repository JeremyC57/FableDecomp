// Grade: DIFFER(188v188) -- scheduling/registers only: retail loads the head entry's Z into ecx before the camera pointer and pushes esi after it, and loads the distance (homed in the dead `primitive` slot) into edx with the renderer global into ecx BEFORE the argument pushes; ours pushes esi first, copies Z through eax and fetches the renderer after the pushes (tried inline dx/dy/dz, a C3DVector delta with GetLength, a renderer local, the whole expression in the call)
// CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when enabled and the trail has entries, queue a pool-allocated sprite-trail sub-primitive at the head entry's distance from the camera (alpha 0xff, list 1); returns 2 (rendered sub-primitives), or 0 when disabled
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA6B0, 188 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?RenderPrimitive@CEnginePrimitiveManagerSpriteTrails@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z.
// Callees: CEngineSubPrimitiveBase::operator new 0x00BEB540 (`__fastcall`, size 0x18 in ecx: the sub-primitive pool),
//   CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CEngineSubPrimitiveRenderer::AddSubPrimitive(sub, distance,
//   alpha, list, manager) 0x00B846E0 on the global renderer 0x01436E74; the camera global 0x01436EA0 (WorldPosition +0x54).
//   Vtable 0x012A69D4 = CEngineSubPrimitiveSpriteTrail.
// Layout (Ego_r.pdb, retail agrees): CEnginePrimitiveManager::Enable +8; CEngineInternalPrimitiveSpriteTrail::Trail +0x68
//   (12-byte vector of CSpriteTrailEntry, position first); CEngineSubPrimitiveBase (0x18): Parent +0xc, NextSubPrimitive +0x10.
// Shape: the landed CEnginePrimitiveManagerDecal::RenderPrimitive (0x00BBA340) model with the distance computed BEFORE the
//   allocation into a named float local (homed in the dead `primitive` slot): the head entry's position copied into a C3DVector
//   local (memberwise copy ctor), the delta through an inline C3DVector operator- and GetLength (sqrt of the squared sum).
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                           // 0x0099A2F0
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class __declspec(novtable) CEngineSubPrimitiveBase : public CBaseClassNonCopyable {   // PDB, 0x18 bytes
public:
    unsigned long                 RenderFlags;         // +0x04
    unsigned char                 Alpha;               // +0x08
    CEngineInternalPrimitiveBase* Parent;              // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive;    // +0x10
    CEnginePrimitiveManager*      Manager;             // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540 (sub-primitive pool)
};
class CEngineSubPrimitiveSpriteTrail : public CEngineSubPrimitiveBase {   // vtable 0x012A69D4
public:
    CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveSpriteTrail();
};

struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                       // +0x54
};
extern CEngineCamera* g_pEngineCamera;                 // 0x01436EA0

class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned long alpha, long list,
                         CEnginePrimitiveManager* manager);   // 0x00B846E0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

struct CSpriteTrailEntry {                             // CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry
    C3DVector Position;                                // +0x00
};
class CEngineInternalPrimitiveSpriteTrail {            // CEngineInternalPrimitiveBase (0x48) + sprite-trail data (PDB)
public:
    unsigned char      _base[0x68];
    CSpriteTrailEntry* TrailFirst;                     // +0x68 (Trail)
    CSpriteTrailEntry* TrailLast;                      // +0x6c
    CSpriteTrailEntry* TrailEnd;                       // +0x70
};

enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerSpriteTrails {
public:
    void* SceneList;                                   // +0x04
    bool  Enable;                                      // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                               CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    const CEngineInternalPrimitiveSpriteTrail& trail = (const CEngineInternalPrimitiveSpriteTrail&)primitive;
    if (trail.TrailFirst != trail.TrailLast) {
        C3DVector head = trail.TrailFirst->Position;
        C3DVector delta = head - g_pEngineCamera->WorldPosition;
        float distance = delta.GetLength();
        CEngineSubPrimitiveSpriteTrail* subPrimitive = new CEngineSubPrimitiveSpriteTrail(primitive);
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(subPrimitive, distance, 0xff, 1, (CEnginePrimitiveManager*)this);
    }
    return RENDERED_SUB_PRIMITIVES;
}
