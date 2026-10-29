// CEnginePrimitiveManagerParticleGroup::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- when enabled, queue a 3D particle group as a pool-allocated sub-primitive at its distance from the camera: in list 1 (water-stage group without z-writes) or 0, or for a screen-displacement group (when the displacement pass is available) in lists 5 and 6 with a second displacement sub-primitive; returns 2 (rendered), 0 when disabled
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBBF70, length 307 bytes (`ret 0Ch`, virtual).
// Callees: CEngineSubPrimitiveBase::operator new @ 0x00BEB540 (`__fastcall`, size 0x18), CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0,
// CEngineSubPrimitiveParticleGroup::CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase&) @ 0x00BBBDB0 (second
// construction out of line), CEngineSubPrimitiveRenderer::AddSubPrimitive @ 0x00B84720 (renderer @ 0x01436E74); camera @ 0x01436EA0
// (WorldPosition +0x54), displacement-pass flag byte +0xef of the engine settings @ 0x01436E38.
// Layout: the 2D twin Render2DPrimitive 0x00BE0CD0 (parked) and the landed Decal RenderPrimitive 0x00BBA340; the group's WorldPos
// +0x5c, ScreenDisplacement +0xa0, WaterStage +0xa4, ZBufferWritable +0xa8 (retail offsets of the landed 2D Update 0x00BE0DE0).
// Shape: the 0x1c frame is `this` (spilled: ebp later carries the distance), the camera delta as a C3DVector LOCAL (its X spilled;
// three float locals give a 0x10 frame with dx in the argument slot) and a copy of WorldPos (its Z copied through eax); the
// distance is a named float homed in the dead `primitive` slot; the sort list is the `WaterStage && !ZBufferWritable ? 1 : 0`
// ternary; RenderFlags/Alpha are assigned after each `new`.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#include <math.h>
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
class CEngineSubPrimitiveParticleGroup : public CEngineSubPrimitiveBase {   // vtable 0x012A5BB0
public:
    CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }   // 0x00BBBDB0
    virtual ~CEngineSubPrimitiveParticleGroup();
};

class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                    // +0x54
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

class CEngineInternalPrimitiveParticleGroup {      // retail layout
public:
    unsigned char _pad00[0x5c];
    C3DVector     WorldPos;                         // +0x5c
    unsigned char _pad68[0x38];
    bool          ScreenDisplacement;               // +0xa0
    unsigned char _pada1[3];
    long          WaterStage;                       // +0xa4
    bool          ZBufferWritable;                  // +0xa8
};

struct CEngineSettings_01436e38 {
    unsigned char _pad[0xef];
    bool          DisplacementPassEnabled;          // +0xef (descriptive)
};
extern CEngineSettings_01436e38* g_pEngineSettings_01436e38;   // 0x01436E38

class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerParticleGroup {
public:
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerParticleGroup::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                                CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    CEngineInternalPrimitiveParticleGroup& group = (CEngineInternalPrimitiveParticleGroup&)primitive;
    CEngineSubPrimitiveParticleGroup* subPrimitive = new CEngineSubPrimitiveParticleGroup(primitive);
    subPrimitive->RenderFlags = 0;
    subPrimitive->Alpha = 0xff;
    C3DVector position = group.WorldPos;
    C3DVector delta(position.X - g_pEngineCamera->WorldPosition.X, position.Y - g_pEngineCamera->WorldPosition.Y,
                    position.Z - g_pEngineCamera->WorldPosition.Z);
    float distance = (float)sqrt(delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z);
    if (group.ScreenDisplacement) {
        if (g_pEngineSettings_01436e38->DisplacementPassEnabled) {
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(5, subPrimitive, distance, (CEnginePrimitiveManager*)this);
            CEngineSubPrimitiveParticleGroup* displacement = new CEngineSubPrimitiveParticleGroup(primitive);
            displacement->RenderFlags = 0;
            displacement->Alpha = 0xff;
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(6, displacement, distance, (CEnginePrimitiveManager*)this);
        }
    } else {
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(group.WaterStage != 0 && !group.ZBufferWritable ? 1 : 0, subPrimitive, distance,
                                                       (CEnginePrimitiveManager*)this);
    }
    return RENDERED_SUB_PRIMITIVES;
}