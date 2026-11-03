// CTCTavernGame::FrameUpdate() -- per-frame tavern game driver: while played, quit when the game master dies, keep the hero in the tavern-game player mode, quit when the owner's village is hostile or the hero was hit; then run the state machine (greeting, options, instructions, betting, new game / round, reaction, payout, farewell, timed wait), count the state timer down and mark the thing rendered
// Leaf: ?FrameUpdate@CTCTavernGame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB9E0, 1472 bytes: 1414 bytes of code, 2 pad bytes, the 14-entry jump table 0x005EBF68..0x005EBFA0.
//   Vtable 0x012560BC slot 9. Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCTavernGame@@UAEXXZ.
// Callees (all 58 retail call targets paired one-to-one with this object's relocations; data: PClass<CPlayerGui> 0x013B8790,
//   0x013BAE1C, ConstantFPS 0x01375550, "\n\n\n" 0x012560B0, "TAVERN_GAME_NEW_GAME" 0x01256098, 0.0f 0x0122DEDC, 1.0f 0x0122DED8):
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetTC<T> via CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020,
//   CTCVillageMember::GetPVillage 0x007EE540, CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetCurrentMode 0x00633BE0 / AddMode 0x006345C0, CTCTavernGame::PlayerHasBeenHit 0x005EB510, SetState 0x005E9B20,
//   GetSubDef<CTavernGameDef> 0x005ED4A0, GetText 0x005E9E70, GetTextWithNarrator 0x005EB3A0, SetStateAfterOK 0x005EB080,
//   SetStateAfterYesNoAlt 0x005EADC0, SetStateAfterInstructions 0x005EB180, SetStateAfterYesNo 0x005EAF30,
//   CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 / ToWideString 0x0099E480, operator+(CWideString,CWideString) 0x0099BE70,
//   ~CWideString 0x0099B510, PlaySoundW 0x005EA950, CThing::SetAsRenderedThisFrame 0x004C7590; virtuals LeaveGame [+0x78] (inlined
//   SetState(47)), NewGame [+0x7C], NewRound [+0x80], EndRound [+0x84], QuitMidGame [+0x88], UpdateValues [+0xA0].
// Layout (shared retail models): CTCTavernGame (FABLE_RETAIL_CTCTAVERNGAME, sizeof 0xF0, Ego_r offsets: PGameMaster +0xD0),
//   CTavernGameDef (sizeof 0x1A4, Ego_r members), CTCOwnedEntity (POwner at the packed +0x0D: FABLE_RETAIL_CINTELLIGENTPOINTER),
//   CTCVillageMember, CTCVillage (LaggyLimbo +0xB3, IsEnemyBecauseOfCrime +0x7C), CPlayerGui (SubtitleRenderer +0x18,
//   DisplayingText at gui +0xFD), CMainGameComponent::ConstantFPS.
// Shape: SetState (0x005E9B20, not landed) is defined here as in retail's TU; /O2 inlines it at the constant-state sites, and
//   its quit-request branch maps the state to 47 (`state = 47`), which gives the retail `if (g) g = 0; LeaveGame()` tails.
//   The game-master check is an if/else (retail lays the dead-master quit after the hostile-village / hit quit). GetSubDef
//   <CTavernGameDef> is the landed 0x005ED4A0 body defined in this TU, so VC7.1 sees the def slot is not captured and reuses
//   it; the case 5 sound name is a named block local (GOTCHAS 313).
// Names: IS_THING_ALIVE<T> is the Ego_r template (`thing && !thing->IsDead()`, landed 0x008C6DB0 / 0x00836AD0). The state
//   numbers (1..14, 47) are the jump table's and the call sites' constants; their ETavernGameStates enumerator names are not
//   in the PDBs we have, so they stay numeric. EPlayerMode 26 likewise (no enumerator name proven). g_bTavernGameQuitRequested
//   is the descriptive spelling six landed rows already bind to 0x013BAE1C (no PDB data name).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CTavernGameDef.h"
#include "engine/CDefinitionManager.h"
// GetSubDef<CTavernGameDef> 0x005ED4A0 (landed, same body): the member template is only declared in CDefClassBase.h, so its
// instantiation is defined here; VC7.1 keeps it out of line (its own retail row) and sees it does not capture the def slot.
template <>
bool CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#include "engine/CTCTavernGame.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "fable_game.h"

extern bool g_bTavernGameQuitRequested;                  // 0x013BAE1C

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing.Get() && !thing->IsDead();
}

void CTCTavernGame::SetState(ETavernGameStates state)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        state = (ETavernGameStates)47;
    }
    if (state == 5)
        GamePlayed = true;
    else if (state == 47) {
        LeaveGame();
        return;
    }
    PreviousTavernGameState = TavernGameState;
    TavernGameState = state;
}

void CTCTavernGame::FrameUpdate()
{
    if (IsBeingPlayed) {
        if (IS_THING_ALIVE(PGameMaster)) {
            CTCVillageMember* game_master_member;
            if (PGameMaster->GetTC(&game_master_member) && game_master_member)
                game_master_member->GetPVillage();
            CPlayer& player = GetPlayerManager().GetMainPlayer();
            if (!CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText() && player.GetCurrentMode() != (EPlayerMode)26)
                player.AddMode((EPlayerMode)26, false);
            CTCOwnedEntity* owned_entity;
            CTCVillageMember* village_member;
            if ((ParentThing->GetTC(&owned_entity) && owned_entity->GetPOwner() &&
                 owned_entity->GetPOwner()->GetTC(&village_member) && village_member->GetPVillage() &&
                 (village_member->GetPVillage()->LaggyLimbo || village_member->GetPVillage()->IsEnemyBecauseOfCrime)) ||
                PlayerHasBeenHit()) {
                SetState((ETavernGameStates)47);
                return;
            }
        } else {
            SetState((ETavernGameStates)47);
            return;
        }
    }
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    switch (TavernGameState) {
    case 1:
        SetStateAfterOK(GetTextWithNarrator(def->Greeting), GetText(def->GreetingForward), (ETavernGameStates)2);
        break;
    case 2:
        if (BaseBet > CurrentMoney)
            BaseBet = CurrentMoney == 0 ? def->MinBet : CurrentMoney;
        if (CurrentMoney > 0)
            SetStateAfterYesNoAlt(GetTextWithNarrator(GamePlayed ? def->OptionsSubsequent : def->OptionsInitial),
                                  GetText(def->OptionsForward), (ETavernGameStates)4, GetText(def->OptionsBack), (ETavernGameStates)10,
                                  GetText(def->OptionsAlternative), (ETavernGameStates)3);
        else
            SetStateAfterOK(GetTextWithNarrator(def->NoMoney), GetText(def->OptionsBack), (ETavernGameStates)10);
        break;
    case 3:
        SetStateAfterInstructions(def->InstructionsPC, GetText(def->Continue), (ETavernGameStates)2);
        break;
    case 4:
        SetStateAfterYesNo(GetTextWithNarrator(def->Betting) + CCharString("\n\n\n").ToWideString(), GetText(def->BettingForward),
                           (ETavernGameStates)5, GetText(def->BettingBack), (ETavernGameStates)2);
        break;
    case 5:
        {
            CCharString sound("TAVERN_GAME_NEW_GAME");
            PlaySoundW(sound);
        }
        NewGame();
        break;
    case 7:
        NewRound();
        break;
    case 6:
        EndRound();
        break;
    case 8: {
        unsigned long reaction = Draw ? def->ReactionDraw
                               : GameWon ? (NewBestScore ? def->ReactionWinNewBestScore : def->ReactionWin)
                               : def->ReactionLose;
        SetStateAfterOK(GetTextWithNarrator(reaction), GetText(def->ReactionForward), (ETavernGameStates)9);
        break;
    }
    case 9: {
        bool prize_given = HasPrizeBeenGiven;
        if (UpdateValues()) {
            if (prize_given != HasPrizeBeenGiven)
                SetStateAfterOK(GetText(def->PrizeGiven), GetText(def->ReactionForward), (ETavernGameStates)12);
            else
                SetStateAfterOK(GetText(def->ClickToContinue), GetText(def->ReactionForward), (ETavernGameStates)2);
        } else
            SetState((ETavernGameStates)2);
        break;
    }
    case 10:
        SetStateAfterOK(GetTextWithNarrator(GamePlayed ? def->FarewellSubsequent : def->FarewellInitial), GetText(def->FarewellForward),
                        (ETavernGameStates)47);
        break;
    case 14:
        if (StateTimer <= 0.0f)
            SetState(NextTavernGameState);
        break;
    case 11:
        QuitMidGame();
        break;
    case 12:
        SetState((ETavernGameStates)2);
        break;
    }
    if (StateTimer > 0.0f)
        StateTimer -= 1.0f / CMainGameComponent::GetConstantFPS();
    ParentThing->SetAsRenderedThisFrame(4);
}