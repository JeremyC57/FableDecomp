// CCreatureAction_TrollWhackGroundBase::WhackGround() -- the troll's ground whack: when the creature's def has a CSpecialEffectsDef
//   sub-def, the action names an effect (virtual GetEffectName), the sub-def maps that name to an effect index and the action has a
//   strike-event helper name, create the effect at that helper point of the troll's graphic, facing along the troll's last movement
//   (Position - OldPosition, normalised; the zero vector when it has not moved); in every case start a medium, short rumble at the
//   troll's position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C61C0, length 457 bytes, thiscall, plain `ret`, frame 0x4C, no EH frame.
// Leaf: ?WhackGround@CCreatureAction_TrollWhackGroundBase@@AAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?WhackGround@CCreatureAction_TrollWhackGroundBase@@AAEXXZ (private, non-virtual); the only
//   four call sites (xref): the class's own PerformAction 0x008C6390 (`jmp 008C61C0`, vtable 0x0128E0EC slot 3) and calls at
//   0x008C63A3 (CCreatureAction_TrollAngryGroundPound::PerformAction 0x008C63A0, vtable 0x0128E23C slot 3), 0x008C63C4
//   (CCreatureAction_TrollTunnel::PerformAction 0x008C63C0, vtable 0x0128E504 slot 3) and 0x008C6438
//   (CCreatureAction_TrollProjectileAttack::PerformAction 0x008C6430, vtable 0x0128E3AC slot 3): each derived PerformAction
//   starts with the base PerformAction, which VC7.1 inlined to this call.
// Callees: CCharString::CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0; CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature->,
//   six times); CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 (out-of-line COMDAT; its body is visible in the TU through
//   engine/CDefClassBase_GetSubDef.h, noinline); virtual GetEffectName slot 82 (+0x148); CSpecialEffectsDef::GetEffectFromString
//   0x00669D30; virtual IsActionWithStrikeEventHelperName slot 22 (+0x58); virtual GetStrikeEventHelperName slot 23 (+0x5C, CCharString
//   by value); NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint 0x004C0560; NCombatActionHelpers::CreateEffect 0x008F7F50
//   (__fastcall); std::vector<CCharString>::~vector 0x00414EA0; CThing::GetPos 0x004C73D0; CTCDRumble::Create 0x007F1B70 (__fastcall);
//   the CDefPointer release (`dec [ebx+4]` / `call [vtbl+4]`) is inline.
// Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C (1e-4f, read from memory by `fcom`); 1.0f literal 0x0122DED8.
// Layout: PCreature +0x08 (CIntelligentPointer<CThingCreatureBase>); CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64, PBaseDef
//   +0x70; CTCPhysicsBase Position +0x0C, OldPosition +0x18.
// Shape: retail has three 12-byte vector slots -- the named movement difference (+0x20), the conditional's result temporary (+0x2C,
//   addressed through eax on both arms) and the named direction (+0x38, copied from the temporary through ecx/edx/eax) -- so the
//   normalise is written out in the row (magnitude, then `magnitude > DELTA_NOTIONAL_ZERO ? movement * (1 / magnitude) : zero`), not
//   a helper call. The unused std::vector<CCharString> local (+0x44, three zero stores, out-of-line dtor) is declared first in the
//   inner block. The def pointer is kept in ebx from the GetEffectFromString call to its release because the GetSubDef<T> body is
//   visible (the compiler then knows only that callee writes the CDefPointer), and reloaded at 0x008C6348 on the two early exits.
// Headers: overlay opt-ins FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS (the polymorphic class on CCombatActionBase) and
//   FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS (the base's eleven own virtuals, slots 70..80, so GetEffectName is slot 82).
// Local declarations (no shared header exists): NCombatActionHelpers::CreateEffect, CTCDRumble::Create (PDB spellings).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS
#define FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NCreatureActionEvents.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TrollWhackGroundBase.h"

namespace NCombatActionHelpers {
void __fastcall CreateEffect(const CThingCreatureBase& creature, long effect, const C3DVector& direction, const C3DVector& pos);   // 0x008F7F50
}
class CTCDRumble { public:
    static CThing* __fastcall Create(const C3DVector& pos, EQuakeStrength strength, EQuakeLength length, bool b);   // 0x007F1B70
};
void CCreatureAction_TrollWhackGroundBase::WhackGround()
{
    CDefPointer<const CSpecialEffectsDef> def;
    CCharString effectName;
    if (PCreature->PeekBaseDef().GetSubDef(&def) && GetEffectName(effectName)) {
        long effect;
        if (def->GetEffectFromString(effectName, effect) && IsActionWithStrikeEventHelperName()) {
            std::vector<CCharString> names;
            C3DVector movement = PCreature->GetPhysicsTC()->GetPosition() - PCreature->GetPhysicsTC()->GetLastPosition();
            float magnitude = movement.GetAccurateMagnitude();
            C3DVector direction = magnitude > DELTA_NOTIONAL_ZERO ? movement * (1.0f / magnitude) : C3DVector(0.0f, 0.0f, 0.0f);
            C3DVector pos;
            PCreature->GetTCGraphicAppearance()->GetHelperPoint(GetStrikeEventHelperName(), 0, 0, pos, 1.0f);
            NCombatActionHelpers::CreateEffect(*PCreature, effect, direction, pos);
        }
    }
    CTCDRumble::Create(PCreature->GetPos(), QUAKE_STRENGTH_MEDIUM, QUAKE_LENGTH_SHORT, false);
}