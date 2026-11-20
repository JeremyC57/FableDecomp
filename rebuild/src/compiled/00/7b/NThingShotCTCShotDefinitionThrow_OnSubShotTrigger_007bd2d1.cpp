// NThingShot::CTCShotDefinitionThrow::OnSubShotTrigger() -- spawn the shot def's sub-shots: NumSubShots objects of the sub-shot def's object class, turned evenly across -SubShotMaxAngle..+SubShotMaxAngle about the line to the target, each given its own shot TC aimed along its turned direction
// Inline-budget: no flag; the TU-mode pragmas optimize("s", on) (the size-optimised shot TU, as the landed siblings 0x007BCF7F / 0x007BD845; without it DIFFER(625v598)) and optimize("y", off) (the siblings' ebp-frame pairing; byte-neutral for this row, RELOCATION_MATCH without it); the header opt-ins that decide expanded vs called are FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (the landed per-TU opt-in, 75 landed rows: retail expands CCharString::operator==(const char*) at 0x007BD3A6..0x007BD3D4 -- the null test, the intrinsic strcmp against "" (`repe cmpsb`), then CBasicString<char>::operator== 0x004115A0 -- while it still calls CRightHandedSet's compiler-generated copy 0x0049CF80 at 0x007BD3EC; plain inline DIFFER(567v598), both called; no /d2inlS value gives that pair: the copy expands from /d2inlS21..S24 up, the operator only from S25, so /d2inlS30 with the plain operator DIFFER(624v598)), FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (GetDef<CThingShotDef>'s noinline body visible: still called at 0x007BD303, but c2 then keeps pshot_def's pointee in esi and drops the CDefPointer null tests, 0x007BD4F2 / 0x007BD516; declaration only DIFFER(614v598)) and FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (the CDefString -> CCharString conversion 0x00415DD0 called at 0x007BD32E / 0x007BD44D; with the inline forwarder DIFFER(598v598)).
// Leaf: ?OnSubShotTrigger@CTCShotDefinitionThrow@NThingShot@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD2D1, 598 bytes, thiscall, ebp frame + `leave` (optimize("y", off) + ("s", on) TU, as the
//   landed siblings 0x007BCF7F MultiArrow / 0x007BD845 DrainLife InitMovement).
// Identity: Ego_r.pdb (0x007BC4A3) / FableWin.pdb ?OnSubShotTrigger@CTCShotDefinitionThrow@NThingShot@@MAEXXZ (protected virtual; the
//   Throw vtable 0x0123CDC4). Ego_d.pdb (debug length 1954): def_index; { main_target_pos, pshot_def (CDefPointer<const CThingShotDef>),
//   main_target_direction, nshots, class_type_name (const CCharString&), n, obj_def_index { rotate_angle, rotate_step { pobject
//   { rhs_to_use } { target_direction, target_pos, ptc_shot } } } } -- every name and block here is Ego_d's. main_target_pos is
//   unused (retail still calls GetShootAtPos into its slot, 0x007BD339).
// Callees: CMainGameComponent::PeekDefinitionManager 0x00415FBC (on PMainGameComponent 0x013B86A0), CDefinitionManager::GetDef
//   <CThingShotDef> 0x0078B230, GetDefClassTypeNameFromGlobalIndex 0x009ACCE0, CDefString::operator CCharString 0x00415DD0,
//   CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0 / GetPWeapon 0x007BEFF0 / Init (C3DVector overload) 0x007BF790, CThing::GetPos
//   0x004C73D0 / GetPlayer 0x004C7A30 / AddTC 0x004C9D60, CTCPhysicsBase::GetRHSet (virtual, +0x120), CRightHandedSet's
//   compiler-generated copy 0x0049CF80 (Ego_d: `public compiler-generated`), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThingObject::Create 0x00703210 (__fastcall), C3DVector::RotateXY
//   0x00A14040, CBasicString<char>::operator== 0x004115A0, CCharString(const char*) 0x0099EBF0 / ~CCharString 0x0099EAE0, _ftol2.
// Shape: the parent thing through the FableWin public inline GetParentThing() (with ParentThing-> the ParentThing load at
//   0x007BD414 is scheduled after the Create argument pushes, DIFFER(598v598)); the Init floats are the protected members read in
//   place (FableWin's inline accessors GetDamageMultiplier / GetShotBuildupTime copy them into frame temporaries ahead of the
//   GetPItem call, DIFFER(613v598)); the class name is Ego_d's const reference (the conversion builds it in place).
// Inferred: the Init floats DamageMultiplier / ShotBuildupTime as member reads (FableWin's GetDamageMultiplier / GetShotBuildupTime accessors,
//   measured with a diagnostic overlay, DIFFER(613v598); PShooter.Get() / PShotAbilityDef and FableWin's GetPShooter() /
//   GetCombatAbilityDef() are byte-neutral, and the shared header has neither accessor, so the members stay).
// Inferred: GetParentThing() at the Create argument (ParentThing-> there DIFFER(598v598); neutral at the two other sites).
// Inferred: rotate_angle as `SubShotMaxAngle * -1.0f` (retail `fmul` by the -1.0 constant; unary minus gives `fchs`, DIFFER(594v598)).
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CThingShotDef.h"
#include "engine/CThingObject.h"
#include "engine/NThingShot_CTCShotDefinitionThrowPredictive.h"
namespace NThingShot {
void CTCShotDefinitionThrow::OnSubShotTrigger()
{
    long def_index = GetShotDef().SubShotType;
    if (def_index >= 0) {
        CDefPointer<const CThingShotDef> pshot_def;
        CMainGameComponent::Get()->PeekDefinitionManager().GetDef(def_index, &pshot_def);
        long obj_def_index = pshot_def->ObjectDef;
        const CCharString& class_type_name = CMainGameComponent::Get()->PeekDefinitionManager().GetDefClassTypeNameFromGlobalIndex(obj_def_index);
        C3DVector main_target_pos = GetShootAtPos();
        C3DVector main_target_direction = GetShootAtPos() - GetParentThing().GetPos();
        long nshots = GetShotDef().NumSubShots;
        if (nshots > 0) {
            float rotate_angle = GetShotDef().SubShotMaxAngle * -1.0f;
            float rotate_step = 2.0f * GetShotDef().SubShotMaxAngle / (nshots - 1);
            for (long n = 0; n < nshots; ++n) {
                if (class_type_name == "OBJECT") {
                    CThingObject* pobject;
                    {
                        CRightHandedSet rhs_to_use = GetParentThing().GetPhysicsTC()->GetRHSet();
                        pobject = CThingObject::Create(obj_def_index, GetParentThing().GetPos(), PShooter->GetPlayer().GetNumber(), &rhs_to_use, 0, "");
                    }
                    if (pobject) {
                        CTCShotDefinitionBase* ptc_shot = (CTCShotDefinitionBase*)pobject->AddTC(pshot_def->ShotDefinitionClassName, true, 0);
                        C3DVector target_direction = main_target_direction;
                        target_direction.RotateXY(rotate_angle);
                        C3DVector target_pos = target_direction + GetParentThing().GetPos();
                        ptc_shot->Init(pshot_def, PShooter.Get(), GetPWeapon(), target_pos, (float)GetShotAccuracyPercentage(), DamageMultiplier, ShotBuildupTime, PShotAbilityDef);
                    }
                }
                rotate_angle += rotate_step;
            }
        }
    }
}
}