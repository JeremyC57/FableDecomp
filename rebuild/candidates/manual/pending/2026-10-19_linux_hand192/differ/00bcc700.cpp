// Grade: DIFFER(190v192) -- twin of 0x00BCA6B0: retail keeps the inner array's first pointer in ecx and copies it to eax (`mov eax,ecx`), loads Z into edx before the camera and pushes esi after it, and fetches the renderer before the argument pushes; ours differs in those registers/schedule
// CEnginePrimitiveManagerWeaponTrails::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when enabled and the first trail has points, queue a pool-allocated weapon-trail sub-primitive at its first point's distance from the camera (alpha 0xff, list 1); returns 2 (rendered sub-primitives), or 0 when disabled
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCC700, 192 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?RenderPrimitive@CEnginePrimitiveManagerWeaponTrails@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z.
// Callees: CEngineSubPrimitiveBase::operator new 0x00BEB540 (`__fastcall`, size 0x18 in ecx: the sub-primitive pool),
//   CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CEngineSubPrimitiveRenderer::AddSubPrimitive(sub, distance,
//   alpha, list, manager) 0x00B846E0 on the global renderer 0x01436E74; the camera global 0x01436EA0 (WorldPosition +0x54).
//   Vtable 0x012A6AD0 = CEngineSubPrimitiveWeaponTrail.
// Layout (Ego_r.pdb, retail agrees): CEnginePrimitiveManager::Enable +8; CEngineInternalPrimitiveWeaponTrail::Trail +0x5c
//   (CArray<CArray<C3DVector>>, the first inner array tested and its first point used); CEngineSubPrimitiveBase (0x18): Parent +0xc, NextSubPrimitive +0x10.
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
class CEngineSubPrimitiveWeaponTrail : public CEngineSubPrimitiveBase {   // vtable 0x012A6AD0
public:
    CEngineSubPrimitiveWeaponTrail(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveWeaponTrail();
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

struct CPointArray {                                   // CArray<C3DVector> (retail 12 bytes)
    C3DVector* First;
    C3DVector* Last;
    C3DVector* End;
};
class CEngineInternalPrimitiveWeaponTrail {            // CEngineInternalPrimitiveBase (0x48) + weapon-trail data (PDB)
public:
    unsigned char _base[0x5c];
    CPointArray*  TrailFirst;                          // +0x5c (Trail: CArray<CArray<C3DVector>>)
    CPointArray*  TrailLast;                           // +0x60
    CPointArray*  TrailEnd;                            // +0x64
};

enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerWeaponTrails {
public:
    void* SceneList;                                   // +0x04
    bool  Enable;                                      // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerWeaponTrails::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                               CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    const CEngineInternalPrimitiveWeaponTrail& trail = (const CEngineInternalPrimitiveWeaponTrail&)primitive;
    const CPointArray& points = *trail.TrailFirst;
    if (points.First != points.Last) {
        C3DVector head = *points.First;
        C3DVector delta = head - g_pEngineCamera->WorldPosition;
        float distance = delta.GetLength();
        CEngineSubPrimitiveWeaponTrail* subPrimitive = new CEngineSubPrimitiveWeaponTrail(primitive);
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(subPrimitive, distance, 0xff, 1, (CEnginePrimitiveManager*)this);
    }
    return RENDERED_SUB_PRIMITIVES;
}
