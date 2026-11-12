// CTCBoss::KillHelpers() -- every live helper creature either dies outright (CThing::Kill(true)) when its base def's
// CSpecialEffectsDef sub-def knows the effect "EXPLODE_ON_KILLING_BLOW", or has its health driven past zero
// (ModifyHealth(-(Health + 1), true)) and is knocked down with an upward hit direction; then every helper slot is cleared.
// Leaf: ?KillHelpers@CTCBoss@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AF490, 550 bytes, thiscall, `ret`. Ego_r.pdb ?KillHelpers@CTCBoss@@QAEXXZ (0x007B0E60, CTCBoss
// block between SetHelperCreature and AddEffect, same order as retail 0x007AF310 / 0x007AF490 / 0x007AF6C0).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / ~ 0x00A01C10,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::Kill 0x004C9B80, CThingPhysical::ModifyHealth (vtable +0xEC,
//   0x006A68F0), CThing::GetPos 0x004C73D0, CCombatAction_GenericStrikeResponseKnockdown ctor 0x008679C0,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900. 1.0f @ 0x0122DED8;
//   CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0; string "EXPLODE_ON_KILLING_BLOW" @ 0x0125AB88.
// Shape: the `GetSubDef(&def) && GetEffectFromString("..", effect)` condition makes the classic VC conditional temporary (flag word
//   bit 0 + materialised bool); the knockdown action is a temporary built from `*HelperCreatures[i]` and a C3DVector(0,0,1) temporary
//   and destroyed by its implicit inline dtor (member dtors then ~CCombatActionBase); the slot is cleared by assigning a
//   CIntelligentPointer<CThingAICreature>(0) temporary. The 8-aligned frame comes from the knockdown temporary.
// Headers: overlay opt-ins FABLE_RETAIL_CTCBOSS_KILLHELPERS (declaration) and
//   FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_IMPLICIT_DTOR (real AnimName / PThingThatHit types, implicit dtor).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_IMPLICIT_DTOR
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
// GETEFFECT_BODY: same-TU noinline copy of landed row 0x00669D30 CSpecialEffectsDef::GetEffectFromString (real body, called out of
// line). Retail needs it here: the `effect` long at [esp+0x24] (lea edx,[esp+0x24] at 0x7af530) is treated as uncaptured, so its dead
// slot is reused for the SetCurrentAction object spill (0x7af5f6 mov [esp+0x24],eax) and the def pointer stays at [esp+0x18] for the
// ebx loads at 0x7af529 / 0x7af550; without it effect gets its own slot [esp+0x40], frame 0x170 (retail sub esp,0x168), 23 hunks.
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTCBOSS_KILLHELPERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
#include "engine/CTCBoss.h"
// Local IS_THING_ALIVE (as landed 0x0095C0B0): shared engine/ThingAlive.h spells it `thing!=0`, which does not compile for
// CIntelligentPointer<T> (C2676, even with FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT); retail tests GetPItem() then the Dead bit.
template<class T> inline bool IS_THING_ALIVE(const T& thing) { return thing.Get() && !thing->IsDead(); }
void CTCBoss::KillHelpers()
{
    for (unsigned long i = 0; i < HelperCreatures.size(); ++i) {
        if (IS_THING_ALIVE(HelperCreatures[i])) {
            CDefPointer<const CSpecialEffectsDef> def;
            long effect;
            if (HelperCreatures[i]->PeekBaseDef().GetSubDef(&def) && def->GetEffectFromString("EXPLODE_ON_KILLING_BLOW", effect)) {
                HelperCreatures[i]->Kill(true);
            } else {
                HelperCreatures[i]->ModifyHealth(-(HelperCreatures[i]->GetHealth() + 1.0f), true);
                HelperCreatures[i]->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(*HelperCreatures[i], 0, HelperCreatures[i]->GetPos(), C3DVector(0.0f, 0.0f, 1.0f)));
            }
        }
        HelperCreatures[i] = CIntelligentPointer<CThingAICreature>(0);
    }
}