// NFire::CFirePoint::SetAsSmoldering() -- unless a live singe effect is already attached, look up the parent def's "SINGE" special effect and, when the effects sub-def has it, create the point's own effect (EffectIndex) at the parent's position, attach its particle emitter to the parent's helper point and keep it in PSingeEffect
// Leaf: ?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007805D0, 299 bytes, thiscall, `ret`. Caller: NFire::CFirePoint::AddFireDamage 0x00780800 (landed).
// Identity: Ego_r.pdb ?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ (private; Ego_r 0x00594710, tc_flammable.obj between GetClosestFirePointTo
//   and AddFireDamage); Ego_d.pdb NFire::CFirePoint::SetAsSmoldering (debug length 743) locals pspecial_fx (CDefPointer<const
//   CSpecialEffectsDef>), fx_index, peffect, tcd_particle_emitter; the landed SetAsBurning 0x00780540 is its twin (Ego_d 41..55).
// Callees: CBaseIntelligentPointer GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10; GetSubDef<CSpecialEffectsDef>
//   0x00669580 and CSpecialEffectsDef::GetEffectFromString 0x00669D30 (both out of line, their bodies visible in the TU: the shared-header
//   noinline opt-ins); CCharString 0x0099EBF0 ("SINGE" 0x0126B184) / ~CCharString 0x0099EAE0; CThing::GetPos 0x004C73D0;
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880 and AttachToThing 0x006E0BE0; vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Ego_d lines: 60 (22 B) `if (!IS_THING_ALIVE(PSingeEffect))` (the Ego_d IS_THING_ALIVE<CIntelligentPointer<CThing> > instance: fastcall, movzx,
//   test, jcc); 64 (17 B) the pspecial_fx declaration (ENullDef ctor + EH state); 65 (148 B) the GetSubDef && GetEffectFromString("SINGE")
//   test; 67 (29 B) Create, = SetAsBurning 46; 68 (19 B) `if (IS_THING_ALIVE(peffect))`, = SetAsBurning 47; 70 (7 B) tcd_particle_emitter
//   = NULL; 72 (34 B) AttachToThing, = SetAsBurning 51; 73 (43 B) the PSingeEffect assignment from a temporary; 76 (15 B) pspecial_fx's
//   destructor at the block end.
// Retail creates EffectIndex, not the looked-up fx_index (`mov ecx,[esi+8]` at 0x00780673; fx_index is written and never read).
// Inferred: Ego_d line 71 is 358 B (SetAsBurning's twin line 50: 361 B), far more than the plain GetTCDriver call (~12 B at /Od); the
//   release bytes are the plain call (`xor ecx,ecx ; test ah,8 ; je ; mov ecx,[edi+5Ch]`, 0x0078068C), so the debug line carries a
//   debug-only check around it. Written as the plain call, as the landed 0x00836260 / 0x008C4810 / 0x00750DD0 do.
// Header: FABLE_RETAIL_NFIRE_CFIREPOINT_SETASSMOLDERING (new, add-only) declares the private member in engine/NFire_CFirePoint.h.
// Inline-budget: GetSubDef<CSpecialEffectsDef> stays out of line because retail calls its COMDAT 0x00669580 (the shared opt-in
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the landed 0x0066BC10 idiom); no other inline control.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
#define FABLE_RETAIL_NFIRE_CFIREPOINT_SETASSMOLDERING
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/NFire_CFirePoint.h"

namespace NFire {
void CFirePoint::SetAsSmoldering()
{
    if (!IS_THING_ALIVE(PSingeEffect)) {
        CDefPointer<const CSpecialEffectsDef> pspecial_fx;
        long fx_index;
        if (PParentThing->PeekBaseDef().GetSubDef(&pspecial_fx) && pspecial_fx->GetEffectFromString("SINGE", fx_index)) {
            CThing* peffect = CTCDParticleEmitter::Create(EffectIndex, PParentThing->GetPos(), false);
            if (IS_THING_ALIVE(peffect)) {
                CTCDParticleEmitter* tcd_particle_emitter = NULL;
                peffect->GetTCDriver(&tcd_particle_emitter);
                tcd_particle_emitter->AttachToThing(*PParentThing, 0xc, HelperName, HelperIndex, 0.0f);
                PSingeEffect = CIntelligentPointer<CThing>(peffect);
            }
        }
    }
}
}