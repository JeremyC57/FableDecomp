// CEnginePrimitiveManagerWeaponTrails::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when the manager is enabled and the first trail strip has points, wrap the trail in a pool-allocated sub-primitive and queue it on the sub-primitive renderer at the camera distance of its head point; returns 2 (rendered) when enabled, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCC700, length 192 bytes (ret 0Ch, virtual).
// Grade: DIFFER(190v192) -- the twin of 0x00BCA6B0 plus the strip-0 load (retail eax=[edi+5Ch], ecx=strip.first, `mov eax,ecx`);
//        scheduling of `push esi` and of the distance/renderer loads before the
//        AddSubPrimitive pushes (retail edx/ecx, ours ecx then a late renderer load)
// Callees: CEngineSubPrimitiveBase::operator new 0x00BEB540 (`__fastcall`, size 0x18 in ecx: the sub-primitive pool),
// CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase*,
// float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager*) 0x00B846E0 on the renderer @ 0x01436E74;
// the camera global @ 0x01436EA0 (world position +0x54). Vtable 0x012A6AD0 = CEngineSubPrimitiveWeaponTrail (RTTI).
// Layout: CEnginePrimitiveManager::Enable +8; CEngineInternalPrimitiveWeaponTrail::Trail +0x5C (PDB, CArray of CArray<C3DVector>;
// only strip 0 is looked at, without an emptiness test of the outer array); CEngineSubPrimitiveBase (PDB 0x18): Parent +0xC, NextSubPrimitive +0x10.
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

struct CTrailPointArray {                           // CArray<C3DVector>
    C3DVector* first;
    C3DVector* last;
    C3DVector* end;
};
class CEngineInternalPrimitiveWeaponTrail {
public:
    unsigned char     _base[0x5c];
    CTrailPointArray* Trail_first;                  // +0x5c CArray<CArray<C3DVector> >
    CTrailPointArray* Trail_last;                   // +0x60
    CTrailPointArray* Trail_end;                    // +0x64
};
class CEngineSubPrimitiveWeaponTrail : public CEngineSubPrimitiveBase {   // vtable 0x012A6AD0
public:
    CEngineSubPrimitiveWeaponTrail(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveWeaponTrail();
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

class CEnginePrimitiveManagerWeaponTrails {
public:
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerWeaponTrails::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                               CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    const CEngineInternalPrimitiveWeaponTrail& trail = (const CEngineInternalPrimitiveWeaponTrail&)primitive;
    const CTrailPointArray& points = trail.Trail_first[0];
    if (points.first != points.last) {
        C3DVector head = points.first[0];
        float distance = (head - g_pEngineCamera->WorldPosition).GetMagnitude();
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(new CEngineSubPrimitiveWeaponTrail(primitive), distance, 0xff, 1,
                                                       (CEnginePrimitiveManager*)this);
    }
    return RENDERED_SUB_PRIMITIVES;
}
