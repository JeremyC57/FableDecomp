// CTCFireballSpell::FinishSpell() -- adopts the combat target (non-creatures and enemies, or a friendly creature while the player
// z-targets), then unless in one-anim cast mode starts the finish-fireball action at the target when it is a valid ranged target
// (else the untargeted one), passing on the removed item; resets the will timer if the action is refused, else makes a noise.
// Leaf: ?FinishSpell@CTCFireballSpell@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C9180, 549 bytes, `ret`.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (PeekTC<CTCTargetingBase>), CTCTargetingBase slot 33 GetPTargetedThing (+0x84),
//          CTCEnemy::AreThingsEnemies @ 0x0076D2F0, CThing::GetPlayer @ 0x004C7A30 (+0x20A CPlayer::IsZTargeting, inline),
//          CIntelligentPointer<CThing> ctor(T*) @ 0x004A96F0 / operator= @ 0x004A9760 / copy ctor @ 0x004A9730 (the out-of-line copies),
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
//          CThingCreatureBase::FinishCurrentAction @ 0x00662650, NCreatureMode::IsValidRangedWeaponTargetForShooter @ 0x0085F8D0,
//          CCreatureAction_FinishFireballSpell ctors @ 0x008BCE50 / 0x008BCE10, CCreatureActionBase::SetPItemRemoved @ 0x0072D360,
//          CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900 (the action's implicit destructor),
//          CCharString(const char*, long) @ 0x0099EBF0, CThingCreatureBase::MakeNoise @ 0x00662110, ~CCharString @ 0x0099EAE0.
//          CIntelligentPointer<CThing> vtable 0x01238C6C; "SIMPLE_NOISE" @ 0x01268A5C.
// NF200 (2026-11-20, q4411 / q6405): port onto the shared headers. The 13 row-local models (CBaseIntelligentPointer, CIntelligentPointer,
//   CCharString, CVectorMap, CPlayer, CThing, N_0062e900::CCreatureActionBase, CCreatureAction_FinishFireballSpell, CThingCreatureBase,
//   CTCBase, CTCTargeting, CTCEnemy, CSpecialAbilitiesFireballSpellDef, CWillButtonHelperTimer, CTCFireballSpell) are gone: the class is
//   engine/CTCFireballSpell.h (NF200 opt-ins _MEMBERS / _FINISHSPELL), the action engine/CCreatureAction_FinishFireballSpell.h (NF200
//   opt-in _CLASS on CCombatActionBase; RTTI CCombatActionBase@0), the +0x2C store CCreatureActionBase::SetPItemRemoved (FableWin.pdb
//   0x01DD0230 / Ego_r.pdb 0x00525E80; body visible per TU, NF98 opt-in _SETPITEMREMOVED_BODY: out of line at the first site, expanded at the
//   second), the interface-8 component CTCTargetingBase (slot 33 GetPTargetedThing) and the +0x20A byte CPlayer::IsZTargeting, as the
//   landed sibling 0x0072C790. Own name: the row was catalogued CTCFireballSpell::FrameUpdate (QAE, non-virtual), but the virtual
//   FrameUpdate is 0x007C93B0; this body is ?FinishSpell@CTCFireballSpell@@QAEXXZ (Ego_r.pdb 0x007C7F80 + FableWin.pdb 0x02721680, public
//   non-virtual void(); Ego_r order OnKill / OnRemove / FinishSpell / FrameUpdate = retail 0x007C9160 / .. / 0x007C9180 / 0x007C93B0;
//   it starts the CCreatureAction_FinishFireballSpell). Its only caller 0x007C93B0 bound the descriptive ?UpdateButtonHeld@..; respelled.
// Inferred: the noise string in an inner block (as the HEAD row had it): retail shares its frame slot with the intelligent-pointer
//   temporary at [esp+0x10] (frame 0x11C); at function scope it gets its own slot (frame 0x120, graded) and with a temporary
//   argument the ctor result is pushed from eax (graded).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED_BODY
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_FINISHFIREBALLSPELL_CLASS
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CSPECIALABILITIESFIREBALLSPELLDEF_MEMBERS
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETSTATE_INLINE
#define FABLE_RETAIL_CTCFIREBALLSPELL
#define FABLE_RETAIL_CTCFIREBALLSPELL_MEMBERS
#define FABLE_RETAIL_CTCFIREBALLSPELL_FINISHSPELL
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCEnemy.h"
#include "engine/NCreatureMode.h"
#include "engine/CSpecialAbilitiesFireballSpellDef.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_FinishFireballSpell.h"
#include "engine/CTCFireballSpell.h"

void CTCFireballSpell::FinishSpell()
{
    CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(ParentThing);
    const CTCTargetingBase* targeting;
    if (creature->PeekTC(&targeting)) {
        CThing* target = targeting->GetPTargetedThing();
        if (target && !target->IsDead()) {
            if (!(target->GetTypeFlags() & 8) || CTCEnemy::AreThingsEnemies(*creature, *target))
                PTarget = target;
            else if (creature->GetPlayer().IsZTargeting())
                PTarget = target;
        }
    }
    if (!OneAnimCastMode) {
        creature->FinishCurrentAction();
        bool started;
        if (PTarget.Get() && !PTarget.Get()->IsDead()
            && NCreatureMode::IsValidRangedWeaponTargetForShooter(*creature, *PTarget.Get(), PDef->TargettingArcAngle)) {
            CCreatureAction_FinishFireballSpell action(*creature, *PTarget.Get());
            if (PItemRemoved.Get() && !PItemRemoved.Get()->IsDead())
                action.SetPItemRemoved(PItemRemoved);
            started = creature->SetCurrentAction(action);
        } else {
            CCreatureAction_FinishFireballSpell action(*creature);
            if (PItemRemoved.Get() && !PItemRemoved.Get()->IsDead())
                action.SetPItemRemoved(PItemRemoved);
            started = creature->SetCurrentAction(action);
        }
        if (!started) {
            Timer.SetCurrentButtonHeldState(CWillButtonHelperTimer::BEING_HELD_DOWN);
            return;
        }
    }
    {
        CCharString noise("SIMPLE_NOISE");
        creature->MakeNoise(noise);
    }
}
