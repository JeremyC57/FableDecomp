// NCameraModes::CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative(CIntelligentPointer<CThing> thing, const C3DVector& lookAt, const C3DVector& from, const C3DVector& to, float fovFraction, long frames, bool shortenOnOcclusion) -- constructor: FOV zoom base, keep the thing, and record the start offset from the look-at relative to the thing's facing (Mag, Angle about Axis)
// Grade: DIFFER(ours v retail) -- DIFFER(447v445) measured 2026-11-11 with FABLE_HDR_OVERLAY=agentQ13/hdr (no overlay header used): local model of classes that have or need shared headers -- the migration is the next step. Earlier diagnosis: x87 scheduling in the inline fast Normalise: VC keeps the Newton-step r on the stack (fst r; fmul st(2); fld r; fmul st(1); two fstp st(0)) where retail spills it (fstp r; fxch; fmul [r]); and the cross product's 2nd/3rd components load facing.X before offset.Z/offset.Y (operand order is canonical, source order has no effect)
// Q13 (4 grades + probes, this file is the shared-C3DVector migration probe): the magnitude must be the explicit `(float)sqrt(offset.X*offset.X + offset.Y*offset.Y + offset.Z*offset.Z)`; the Normalise squared sum as running statements (`squared=x*x; squared+=y*y; squared+=z*z;`) gives retail's second sum. Left: the Newton r stays on the x87 stack (`fst r ; fmul st(2)`) where retail spills it (`fstp r ; fxch st(1) ; fmul [r]`, 0x00852F0D..0x00852F13) and consumes x/y in place; every r spelling (X*=r, X=x*r, r*x, NEWTON_GROUPED, address-taken r via `*(unsigned long*)&r = estimate`) is identical. Header debt: needs engine headers NCameraModes::CScriptModuleBase (0x4c), CScriptModuleFOVZoom (0x84, ctor 0x00852560), CScriptModuleFOVZoomStayRelative (0xa0; vtable 0x01275A94).
// Leaf: ??0CScriptModuleFOVZoomStayRelative
#include <math.h>
#pragma intrinsic(acos)
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS
#include "engine/C3DVector.h"

FABLE_FORCEINLINE void ProbeNormalise(C3DVector& v){float x=v.X;float y=v.Y;float z=v.Z;float squared=x*x;squared+=y*y;squared+=z*z;unsigned long bits=*(unsigned long*)&squared;unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*(float*)&estimate;r=(3.0f-r*r*squared)*r*0.5f;v.X*=r;v.Y*=r;v.Z*=r;}
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
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacing();                   // slot 70 (+0x118)
};
struct CBaseObject;
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                       // +0x60 (retail)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
    ~CBaseIntelligentPointer();                      // 0x00A01C10
    CBaseObject* GetPItemBase();
    void SetPItem(CBaseObject* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing(CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CThing* GetPItem();                              // 0x00A01B50
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
};

namespace NCameraModes {

struct CScriptModuleFOVZoom : CScriptModuleBase {
    unsigned char _fov[0x84 - 0x4c];
    virtual void v00();
    CScriptModuleFOVZoom(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to,
                         float fovFraction, long frames, bool shortenOnOcclusion);   // 0x00852560
};

struct CScriptModuleFOVZoomStayRelative : CScriptModuleFOVZoom {   // vtable 0x01275A94
    CIntelligentPointerThing Thing;                  // +0x84
    C3DVector                Axis;                   // +0x8c
    float                    Angle;                  // +0x98
    float                    Mag;                    // +0x9c
    virtual void v00();
    CScriptModuleFOVZoomStayRelative(CIntelligentPointerThing thing, const C3DVector& lookAt, const C3DVector& from,
                                     const C3DVector& to, float fovFraction, long frames, bool shortenOnOcclusion);
};

CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative(CIntelligentPointerThing thing, const C3DVector& lookAt,
        const C3DVector& from, const C3DVector& to, float fovFraction, long frames, bool shortenOnOcclusion)
    : CScriptModuleFOVZoom(lookAt, from, to, fovFraction, frames, shortenOnOcclusion), Thing(thing)
{
    C3DVector facing = thing.GetPItem()->GetPhysicsTC()->GetFacing();
    C3DVector offset = to - lookAt;
    Mag = (float)sqrt(offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z);
    ProbeNormalise(offset);
    Angle = (float)acos(facing.Z * offset.Z + facing.Y * offset.Y + facing.X * offset.X) * 0.15915494f;
    Axis = C3DVector(facing.Z * offset.Y - facing.Y * offset.Z,
                     offset.Z * facing.X - facing.Z * offset.X,
                     facing.Y * offset.X - offset.Y * facing.X);
}

}
