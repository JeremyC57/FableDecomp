// CTCHeroCentre::FrameUpdate() -- while waiting for the training tutorial: once no subtitle text is shown and the world is not paused, stop waiting and, for a hero with the experience inventory, queue the open-training-screen game event; otherwise, when the quest card list is new or the quest manager asks for an update: kill the old cards, then for every available quest card type reuse the matching card object already in the world or create one at the next card position (a feat card carries the type's feat index and names), list it, and kill the leftover card objects
// Leaf: ?FrameUpdate@CTCHeroCentre@@UAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077D5B0, 1491 bytes, plain `ret` (/GS cookie 0x0139C8A8, `and esp,-8` frame); CTCBase slot 9.
// Identity: Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?FrameUpdate@CTCHeroCentre@@UAEXXZ; locals as Ego_d.pdb lists them (phero, game_event;
//   rhset, quest_card_types, existing_quest_cards, position_index; it, pquest_card (CIntelligentPointer<CThing>&); it, pquest_card
//   (CThingObject*), def_name, def_index; c1; c1, ptc_existing_quest_card; ptc_quest_card; ptc_quest_card; c1), statements as its
//   line table (lines 89..236; the 132..517-byte entries are debug ASSERT lines with no retail code).
// Callees: CTCBase::PeekWorld 0x00686A80 / PeekPlayerManager 0x00686D50 / PeekThingSearchTools 0x00686D90, CPlayerManager::GetMainPlayer
//   0x00449960 / GetMainPlayerNumber 0x00449700, CPlayer::GetPControlledCreature 0x00487DC0, CGameEvent::AddData 0x009F1760,
//   CMainGameComponent::AddGameEvent 0x0041649C, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10, CThing::Kill 0x004C9B80, vector<CIntelligentPointer<CThing>>::erase 0x004AC0F0 / reserve 0x0077E580 /
//   _M_insert_overflow 0x004AC940, GetAllLeafThings<CThingObject, vector, CThingFilter_HasTC> 0x0077EC20, CTCPhysicsBase slot 72 GetRHSet,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CCharString(const char*, long)
//   0x0099EBF0 / dtor 0x0099EAE0 / operator= 0x0099EFB0, CThingObject::Create 0x00703210, the interface map LowerBound 0x0040F020 /
//   0x004365B0, CBasicString<char>::Compare 0x00411570, std::__find 0x006C4060, CWorld::GetFrame 0x0049D870.
// Inferred: (1) the two string arguments are implicit conversions (`Create(.., "")`, `GetDefGlobalIndexFromName("OBJECT_FEAT_CARD_DUMMY")`):
//   retail pushes `lea edx,[temp]` and loads QuestCardPositions' start after the string constructor; an explicit CCharString("..")
//   pushes the constructor's eax and loads the start first (1474 bytes with it, before the copy-constructor opt-in). (2) GetTC / PeekTC and
//   the CIntelligentPointer copy constructor in their FableWin text (FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN,
//   FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN): with the EXPANDED GetTC the ScriptName compare at 0x0077D8E2 has its two
//   data pointers in swapped registers, with the plain copy constructor the push_back at 0x0077DA2C has esi / edi swapped (84 and 73
//   instructions off; same bytes otherwise). (3) AddULONG: FableWin has AddULONG and AddSLONG (same bytes here); the landed
//   OpenTrainingScreen 0x0077D4B0 spells this event AddULONG.
// Fidelity: the TU instantiates GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CThingFilter_HasTC> (retail 0x0077EC20, 136
//   bytes, compiled next to this row) from engine/CThingSearchTools.h's visible noinline body. With FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG
//   and FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE (the define pair of the landed shared-header instance 0x00910A90) that instance is
//   RELOCATION_MATCH against 0x0077EC20 from this file (tools/gother.py; 126 bytes without the pair), and this row is unchanged. The row
//   needs the visible body (the size of existing_quest_cards is cached across calls in the second c1 loop, 0x0077D886).
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDULONG
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_BOASTMADE
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
#define FABLE_RETAIL_CQUESTMANAGER_FAILUREBOOKKEEP
#define FABLE_RETAIL_CQUESTMANAGER_QUESTCARDTYPES
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_QUESTNAME
#define FABLE_RETAIL_CTCQUESTCARD_SCRIPTQUESTNAME
#define FABLE_RETAIL_CTCQUESTCARD_FEATACCESSORS
#define FABLE_RETAIL_CTCHEROCENTRE_CLASS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CQuestManager.h"
#include "engine/CTCQuestCard.h"
#include "engine/CTCHeroCentre.h"
#include "engine/ThingAlive.h"

void CTCHeroCentre::FrameUpdate()
{
    if (WaitingForTutorialToFinish)
    {
        if (!CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText())
        {
            if (!PeekWorld().IsPaused())
            {
                WaitingForTutorialToFinish = false;

                CThingPlayerCreature* phero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();

                if (phero->HasTC(TCI_INVENTORY_EXPERIENCE))
                {
                    CGameEvent game_event(0x1a);
                    game_event.AddULONG(0xb);
                    game_event.SetPlayer(PeekPlayerManager().GetMainPlayerNumber());
                    CMainGameComponent::Get()->AddGameEvent(game_event);
                }
            }
        }
    }
    else if (FrameQuestCardListFrom == 0 || CQuestManager::Get().GetQuestCardsNeedUpdating())
    {
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = QuestCards.begin(); it != QuestCards.end(); ++it)
        {
            CIntelligentPointer<CThing>& pquest_card = *it;

            if (IS_THING_ALIVE(pquest_card))
            {
                pquest_card->Kill(false);
            }
        }

        QuestCards.clear();

        const std::list<CQuestManager::CQuestCard>& quest_card_types = CQuestManager::Get().GetAvailableQuestCardTypes();

        QuestCards.reserve(quest_card_types.size());

        std::vector<CThingObject*> existing_quest_cards;
        PeekThingSearchTools().GetAllLeafThings<CThingObject>(CThingFilter_HasTC(TCI_QUEST_CARD), existing_quest_cards);

        CRightHandedSet rhset = GetParentThing().GetPhysicsTC()->GetRHSet();
        long position_index = 0;

        if (!QuestCardPositions.empty())
        {
            for (std::list<CQuestManager::CQuestCard>::const_iterator it = quest_card_types.begin(); it != quest_card_types.end(); ++it)
            {
                CThingObject* pquest_card = NULL;
                const CCharString& def_name = it->DefName;
                long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(def_name);

                if (def_index > 0)
                {
                    for (long c1 = 0; c1 < existing_quest_cards.size(); c1++)
                    {
                        if (existing_quest_cards[c1]->GetDefGlobalIndex() == def_index)
                        {
                            pquest_card = existing_quest_cards[c1];
                            break;
                        }
                    }

                    if (!pquest_card)
                    {
                        pquest_card = CThingObject::Create(def_index, QuestCardPositions[position_index], 5, &rhset, 0, "");
                    }
                }
                else
                {
                    def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_FEAT_CARD_DUMMY");

                    if (def_index > 0)
                    {
                        for (long c1 = 0; c1 < existing_quest_cards.size(); c1++)
                        {
                            if (existing_quest_cards[c1]->GetDefGlobalIndex() == def_index)
                            {
                                const CTCQuestCard* ptc_existing_quest_card = NULL;
                                existing_quest_cards[c1]->PeekTC(&ptc_existing_quest_card);

                                if (ptc_existing_quest_card->GetFeatIndex() == it->FeatIndex && ptc_existing_quest_card->GetScriptQuestName() == it->ScriptName)
                                {
                                    pquest_card = existing_quest_cards[c1];
                                    break;
                                }
                            }
                        }

                        if (!pquest_card)
                        {
                            pquest_card = CThingObject::Create(def_index, QuestCardPositions[position_index], 5, &rhset, 0, "");

                            CTCQuestCard* ptc_quest_card = NULL;
                            pquest_card->GetTC(&ptc_quest_card);

                            if (ptc_quest_card)
                            {
                                ptc_quest_card->SetFeatIndex(it->FeatIndex);
                                ptc_quest_card->SetQuestName(def_name);
                                ptc_quest_card->SetScriptQuestName(it->ScriptName);
                            }
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

                if (pquest_card)
                {
                    CTCQuestCard* ptc_quest_card = NULL;
                    pquest_card->GetTC(&ptc_quest_card);

                    if (ptc_quest_card->GetFeatIndex() == -1)
                    {
                        ptc_quest_card->SetScriptQuestName(it->ScriptName);
                    }

                    QuestCards.push_back(CIntelligentPointer<CThing>(pquest_card));

                    if (position_index < QuestCardPositions.size() - 1)
                    {
                        position_index++;
                    }
                }
            }
        }

        for (long c1 = 0; c1 < existing_quest_cards.size(); c1++)
        {
            if (std::find(QuestCards.begin(), QuestCards.end(), CIntelligentPointer<CThing>(existing_quest_cards[c1])) == QuestCards.end())
            {
                existing_quest_cards[c1]->Kill(false);
            }
        }

        FrameQuestCardListFrom = PeekWorld().GetFrame();
        CQuestManager::Get().SetQuestCardsNeedUpdating(false);
    }
}
