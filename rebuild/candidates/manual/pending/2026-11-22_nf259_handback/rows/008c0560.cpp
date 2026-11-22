// CCombatAction_WillHealInto::Initialise() -- names this action's animation "ST_HEAL_INTO" then runs the base Initialise
// Leaf: ?Initialise@CCombatAction_WillHealInto@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0560, 51 bytes; vftable 0x012948AC slot 12.
// Retail bytes:  push ecx ; push esi ; push -1 ; mov esi,ecx ; push offset "ST_HEAL_INTO" ; lea ecx,[esp+0Ch] ; call CCharString::CCharString(const char*,long) ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call SetCombatAnimation ; lea ecx,[esp+4] ; call CCharString::~CCharString ; mov ecx,esi ; call CCombatActionBase::Initialise ; pop esi ; pop ecx ; ret
// Identity: Ego_r.pdb 0x008F8380 + FableWin.pdb 0x02A113C0 ?Initialise@CCombatAction_WillHealInto@@UAEXXZ (public virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (fable_string.h), CCombatActionBase::SetCombatAnimation
//   (const CCharString&) 0x008574D0 and CCombatActionBase::Initialise 0x00857520 (direct base call; Ego_r.pdb spellings, the MELEE branch's
//   _INITIALISE opt-in).
// NF259 (2026-11-22, q12731, nf257 port debt): the row had no Leaf line and modelled CCombatActionBase (with a placeholder virtual) and
//   CCombatAction_WillHealInto locally; moved onto engine/CCombatActionBase.h (MELEE branch, INITIALISE) and
//   engine/CCombatAction_WillHealInto.h (nf257 WILLHEALINTO_OVERRIDES). Body unchanged.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTION_WILLHEALINTO_OVERRIDES
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_WillHealInto.h"

void CCombatAction_WillHealInto::Initialise()
{
    {
        CCharString name("ST_HEAL_INTO", -1);       // a named local: a bare temporary would reuse the ctor's eax
        SetCombatAnimation(name);            // instead of re-deriving `lea eax,[esp+4]` as retail does
    }
    CCombatActionBase::Initialise();
}
