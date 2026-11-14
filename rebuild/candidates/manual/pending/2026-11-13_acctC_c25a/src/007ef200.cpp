// CTCCarriedActionUseExpression::OnActionUse(CThing& user) -- use an expression item: run the base action-use; a live user that is not
// berserk performs the item's expression: a game-event expression posts its CGameEvent (player number of the item's player) when that
// player is a character; a script expression activates its quest and, when the def says so, still performs the expression; otherwise
// the user performs the expression (CCreatureAction_PerformExpression at the targeted creature / object, if any). Finally the item
// is returned to the inventory.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EF200, 734 bytes, thiscall `ret 4`, /GS frame 0x100.
// Leaf: ?OnActionUse@CTCCarriedActionUseExpression@@UAEXAAVCThing@@@Z
// Flags: /GS
// Flags evidence: the security cookie 0x0139C8A8 stored at entry and __security_check_cookie 0x00BFE9F9 before `ret 4`.
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (PeekTC
//   <CTCTargetingBase> 8 / <CTCExpression> 0x8F) and non-const 0x0040F020 (GetTC<CTCInventoryItem> 0x1A), CTCTargetingBase::
//   IsTargetedThingOfType 0x007F5A70 / IsTargetedThingInRange (vslot 38, +0x98) / GetPTargetedThing (vslot 33, +0x84),
//   CThing::PeekPlayer 0x004C7A10, CMainGameComponent::AddGameEvent 0x0041649C, CDefString::operator!= 0x005FA740 / GetString
//   0x00415DD0, CQuestManager::ActivateQuest 0x004B4A10 (singleton 0x013B89FC), ~CCharString 0x0099EAE0,
//   CCreatureAction_PerformExpression ctor 0x008430B0 / dtor 0x00718D50, CThingCreatureBase::SetCurrentAction 0x006644F0,
//   CTCInventoryItem::ReturnToInventory 0x005D8E50; the def's slot 1 on the last release.
// Shape: the targeted-thing test is an if / else-if with the same GetPTargetedThing assignment in both arms (VC7.1 cross-jumps
//   them; the `||` form gives other allocation choices all the way down); the expression TC out pointer is not tested (retail
//   reads the uninitialised slot on a miss); the item def index argument goes through GetParentThing() (the ParentThing load
//   is scheduled before the GetPDef copy, the movzx after it); both action statements are written out (retail keeps two copies).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISPLAYERCHARACTER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCarriedActionUseExpression.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCExpression.h"
#include "engine/CExpressionDef.h"
#include "engine/CPlayer.h"
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CQuestManager.h"
#include "engine/CCreatureAction_PerformExpression.h"
#include "engine/CTCInventoryItem.h"

void CTCCarriedActionUseExpression::OnActionUse(CThing& user)
{
    CTCCarriedActionUseBase::OnActionUse(user);
    if (&user != 0 && !user.IsDead() && !user.HasTC(TCI_BERSERK)) {
        CThing* target = 0;
        const CTCTargetingBase* targeting;
        if (user.PeekTC(&targeting)) {
            if (targeting->IsTargetedThingOfType((ETargetingType)2) && targeting->IsTargetedThingInRange(2))
                target = targeting->GetPTargetedThing();
            else if (targeting->IsTargetedThingOfType((ETargetingType)1) && targeting->IsTargetedThingInRange(1))
                target = targeting->GetPTargetedThing();
        }
        const CTCExpression* expression;
        ParentThing->PeekTC(&expression);
        if (expression->HasDef()) {
            const CExpressionDef& def = expression->PeekDef();
            if (def.GameEventType != 0) {
                const CPlayer& player = ParentThing->PeekPlayer();
                if (player.IsPlayerCharacter()) {
                    CGameEvent event(def.GameEventType);
                    event.SetPlayer(player.GetNumber());
                    CMainGameComponent::Get()->AddGameEvent(event);
                }
            } else if (def.ScriptName != "") {
                CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateQuest(def.ScriptName, false, def.ScriptLoadResources);
                if (def.PerformAfterScript)
                    static_cast<CThingCreatureBase&>(user).SetCurrentAction(CCreatureAction_PerformExpression(static_cast<CThingCreatureBase&>(user),
                        target, expression->GetPDef(), GetParentThing().GetDefGlobalIndex(), -1, -1, -1));
            } else
                static_cast<CThingCreatureBase&>(user).SetCurrentAction(CCreatureAction_PerformExpression(static_cast<CThingCreatureBase&>(user),
                    target, expression->GetPDef(), GetParentThing().GetDefGlobalIndex(), -1, -1, -1));
        }
    }
    CTCInventoryItem* item;
    if (ParentThing->GetTC(&item))
        item->ReturnToInventory();
}
