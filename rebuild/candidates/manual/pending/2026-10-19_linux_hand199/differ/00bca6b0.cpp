// CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when the manager is enabled and the trail has points, wrap the trail in a pool-allocated sub-primitive and queue it on the sub-primitive renderer at the camera distance of its head point; returns 2 (rendered) when enabled, else 0
// Grade: DIFFER(188v188) -- scheduling only: retail pushes esi after the X subtraction and loads the distance into edx with the renderer before the AddSubPrimitive pushes; ours pushes esi earlier and loads the renderer after the pushes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCA6B0, length 192 bytes (ret 0Ch, virtual).
// Callees: CEngineSubPrimitiveBase::operator new 0x00BEB540 (`__fastcall`, size 0x18 in ecx: the sub-primitive pool),
// CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase*,
// float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager*) 0x00B846E0 on the renderer @ 0x01436E74;
// the camera global @ 0x01436EA0 (world position +0x54). Vtable 0x012A69D4 = CEngineSubPrimitiveSpriteTrail (RTTI).
// Layout: CEnginePrimitiveManager::Enable +8; CEngineInternalPrimitiveSpriteTrail::Trail +0x68 (PDB, CArray of entries whose
// first member is the C3DVector position); CEngineSubPrimitiveBase (PDB 0x18): Parent +0xC, NextSubPrimitive +0x10.
// Shape: the landed Decal body (0x00BBA340) with the distance computed first: the head point is copied into a C3DVector
// local (X/Y forwarded to the FPU, Z through a register into the frame), the camera is subtracted with the inline
// operator-, and the magnitude (Z*Z + Y*Y + X*X top-first) is homed in the dead layer-argument slot across the new.
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                       // 0x0099A2F0
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

struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                    // +0x54
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

struct CSpriteTrailEntry {                          // CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry
    C3DVector Position;                             // +0x00
};
class CEngineInternalPrimitiveSpriteTrail {
public:
    unsigned char      _base[0x68];
    CSpriteTrailEntry* Trail_first;                 // +0x68 CArray<CSpriteTrailEntry>
    CSpriteTrailEntry* Trail_last;                  // +0x6c
    CSpriteTrailEntry* Trail_end;                   // +0x70
};
class CEngineSubPrimitiveSpriteTrail : public CEngineSubPrimitiveBase {   // vtable 0x012A69D4
public:
    CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveSpriteTrail();
};
class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags,
                         CEnginePrimitiveManager* manager);   // 0x00B846E0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerSpriteTrails {
public:
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                               CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    const CEngineInternalPrimitiveSpriteTrail& trail = (const CEngineInternalPrimitiveSpriteTrail&)primitive;
    if (trail.Trail_first != trail.Trail_last) {
        C3DVector head = trail.Trail_first[0].Position;
        float distance = (head - g_pEngineCamera->WorldPosition).GetMagnitude();
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(new CEngineSubPrimitiveSpriteTrail(primitive), distance, 0xff, 1,
                                                       (CEnginePrimitiveManager*)this);
    }
    return RENDERED_SUB_PRIMITIVES;
}
