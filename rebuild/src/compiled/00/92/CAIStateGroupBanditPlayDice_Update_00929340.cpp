// CAIStateGroup_BanditPlayDice::Update() -- a bandit joins the camp dice game: claim the free CTCExclusiveUse position (give up if
// someone already uses it), walk to it, count itself into the CTCDiceGame, face along the position's forward, sheathe a carried
// weapon if PutWeaponAway() says so, play (Act), then leave: count itself out, clear the current player if it was us, free the
// position, forget the game and draw the weapon again; every exit is Terminate().
// Retail 0x00929340, 507 bytes (`sub esp,0x120`, no EH frame; one 0x118 slot at [esp+0x18] shared by the Sheathe/Unsheathe
// temporaries, conditional-temporary flag word at [esp+0x10]). Vtable 0x0128AD54 slot 12; Ego_r.pdb / FableWin.pdb
// ?Update@CAIStateGroup_BanditPlayDice@@MAEXXZ (protected virtual). PutWeaponAway (slot 16) and Act (slot 17) are called virtually.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCExclusiveUse::SetUser 0x00916640, CThing::GetPos 0x004C73D0,
// CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00, CTCPhysicsBase::GetRHSet (vslot 72, +0x120), AIAction_LookAtPos 0x0097D580,
// CTCCarrying::IsThingCarryingWeapon 0x006A1540, CCreatureAction_SheatheWeapons ctor 0x008CC370, AIAction_PerformActionAndWait
// 0x0097DB70, ~CCombatActionBase 0x0062E900, CCreatureAction_UnsheatheWeapons ctor 0x008B8590, CTCDiceGame::SetCurrentPlayer
// 0x00929550, CAIStateGroupBase::Terminate 0x008FCF40.
// Shape notes: the entry test reads DiceGamePosition into a local first (retail `mov eax,[esi+14h] ; lea ecx,[eax+0Ch]`); the
// orientation is fetched before the position (named reference), the sum is GetPos() + Forward; ++players goes through the FableWin
// Get/SetNumberOfPlayers accessors (load/inc/store), --players is direct (`dec [eax+0Ch]`); the Unsheathe failure path is an early
// `Terminate(); return;` like the Sheathe one (shared tail). CTCExclusiveUse / CTCDiceGame / CCreatureAction_SheatheWeapons have no
// engine header and are modelled here from Ego_r.pdb.
// Leaf: ?Update@CAIStateGroup_BanditPlayDice@@MAEXXZ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_BanditPlayDice.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCombatActionBase.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"

class CTCExclusiveUse : public CTCBase {                 // Ego_r.pdb sizeof 0x14, no engine header
public:
    CIntelligentPointer<CThing> User;                   // +0x0c
    void SetUser(CThing* user);                         // 0x00916640
};
class CTCDiceGame : public CTCBase {                     // Ego_r.pdb sizeof 0x18, no engine header
public:
    long NumberOfPlayers;                               // +0x0c
    CIntelligentPointer<CThing> CurrentPlayer;          // +0x10
    long GetNumberOfPlayers() { return NumberOfPlayers; }
    void SetNumberOfPlayers(long number) { NumberOfPlayers = number; }
    CThing* GetCurrentPlayer() { return CurrentPlayer.Get(); }
    void SetCurrentPlayer(CThing* player);              // 0x00929550
};
class CCreatureAction_SheatheWeapons : public CCombatActionBase {   // no engine header
public:
    CCreatureAction_SheatheWeapons(CThingCreatureBase& creature);   // 0x008CC370
    bool PlayedSound;                                   // +0x114
};
void CAIStateGroup_BanditPlayDice::Update()
{
    WasCarryingWeapon = false;
    CTCExclusiveUse* position = DiceGamePosition;
    if (position->User.Get()) {
        DiceGame = 0;
        Terminate();
        return;
    }
    DiceGamePosition->SetUser(&GetCreature());
    bool arrived;
    if (AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, DiceGamePosition->GetParentThing().GetPos(), arrived, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f) != AI_ACTION_FINISHED) {
        DiceGamePosition->SetUser(0);
        DiceGame = 0;
        Terminate();
        return;
    }
    DiceGame->SetNumberOfPlayers(DiceGame->GetNumberOfPlayers() + 1);
    const CRightHandedSet& rh = DiceGamePosition->GetParentThing().GetPhysicsTC()->GetRHSet();
    AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, DiceGamePosition->GetParentThing().GetPos() + rh.GetForward(), 0x11, 0.0027777f);
    WasCarryingWeapon = PutWeaponAway() && CTCCarrying::IsThingCarryingWeapon(GetCreature());
    if (WasCarryingWeapon && AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()), false) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    Act();
    --DiceGame->NumberOfPlayers;
    if (DiceGame->GetCurrentPlayer() == &GetCreature())
        DiceGame->SetCurrentPlayer(0);
    DiceGamePosition->SetUser(0);
    DiceGame = 0;
    if (WasCarryingWeapon && AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()), false) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    WasCarryingWeapon = false;
    Terminate();
}