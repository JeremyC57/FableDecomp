// CTCHeroReceiveItems::GlobalUpdate(bool) -- per-frame update of the hero's "receive items" display (retail 0x007CACB0, 2974 bytes,
// vtable 0x0126EBFC slot 26; Ego_r.pdb 0x007C9230 / FableWin.pdb 0x02726CD0 ?GlobalUpdate@CTCHeroReceiveItems@@UAEX_N@Z).
// Leaf: ?GlobalUpdate@CTCHeroReceiveItems@@UAEX_N@Z
// Inline-budget: CEnginePrimitiveHandle::Clear is expanded at both sites in retail (0x007CAD10, 0x007CAEE4); VC7.1's c2 inline scorer
//   rates the plain inline Clear c1 52..54 at static frequency 1/4 and 1/8 (scores 74..108 against the threshold 74, c2 trace), so the
//   row takes the FABLE_FORCEINLINE spelling FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE (hdr overlay). A per-row /d2inlT<N>
//   cannot do it: the first IS_THING_ALIVE(PContainer) site (score 103) must stay a call (0x004CC340) while the second Clear scores 104+.
// What it does: when receiving items -- on ForceClear resets the display boxes, clears the item primitive, gives the current item to the
//   inventory and drops it from the list; if the list is then empty, sends MESSAGE_EVENT_HERO_REWARDED_FROM_CONTAINER (when asked),
//   disables the container's fishing / digging spot, clears the background primitive and stops. Otherwise, when the current item is
//   gone, purges dead items, takes the next one, plays the receive sound, builds its subtitle text (fish: description, length, weight;
//   gold: hero money / rent money / online score; inventory item: description) and its starting orientation, and shows the text.
//   While displaying: grows the item area towards the def's container-item rectangle (anchored bottom-right to the render target),
//   counts the full-size timer down, spins the item and marks the parent thing as rendered.
// Shape: `if (!IsReceivingItems()) return;` (shrink-wrapped prologue); ForceClear/Amount stores written before the box copies (retail
//   schedules them between the second copy's loads); the CSoundInit default ctor body is visible (FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR)
//   but c2 keeps it a call (0x0041C930): with the body visible PSoundManager is read once (edi) and the GainFaderBits store stays ahead
//   of the Get2DEmitter vtable load. GFLimit<float,float,float> and CRightHandedSet::Init are declaration-only (retail calls them).
// Callees: CPlayerGui::PeekGuiDef 0x00438570; CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/
//   0x00A01C10; AddItemToInventory 0x007CABD0; free 0x00BFEA14; IS_THING_ALIVE<CIntelligentPointer<CThing>> 0x004CC340; CTCBase::GetWorld
//   0x00686A70; CMessageEventManager::AddMessage 0x00687540; CVectorMap::LowerBound 0x0040F020 / const 0x004365B0; CSoundInit() 0x0041C930;
//   CASoundManager slot 9 / CASoundEmitter slot 4 PlaySoundEvent; CWideString ctor/dtor 0x0099AED0/0x0099B510, operator+ 0x0099BE70 /
//   0x0099BF30, operator+= 0x0099B8D0, operator= 0x0099B7D0; CCharString(const char*,long) 0x0099EBF0 / dtor 0x0099EAE0;
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0; GFIntToWideString 0x00405F00; __ftol2 0x00BFEA70; CTCInventoryItem::GetDescription
//   0x005D9070 / GetStartingRightHandedSet 0x005D92F0; CThingObject::GetProperties (slot 48); CTCBase::GetPlayerManager 0x00686D40;
//   CPlayerManager::GetMainPlayer 0x00449970; CTCGold::GetGold 0x007CBDC0; CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0;
//   CTCHeroStats::AddMoney 0x0057B327; NGameText::ConstructString 0x009C7B70; CRightHandedSet::Init 0x004BBE10 / Rotate 0x00A22F80;
//   CTCBase::PeekComponent 0x00686D60; CSubtitleRenderer::SetText 0x00492E4B; GFLimit<MMM> 0x0048DAE0;
//   CDisplayManager::GetRenderTargetDimensions 0x009BEDC0; CThing::SetAsRenderedThisFrame 0x004C7590.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INIT
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CONTAINERITEM
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_DESCRIPTION
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_RENTMONEY_B
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsI.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EMessageEventType.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CTCFish.h"
#include "engine/CTCFishingSpot.h"
#include "engine/CTCDiggingSpot.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCGold.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundEmitter.h"
#include "engine/CSoundInit.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/GFGetInterpolatedValue.h"


template <class T, class TMin, class TMax> void __fastcall GFLimit(T& value, const TMin& minimum, const TMax& maximum);   // 0x0048DAE0
CWideString __fastcall GFIntToWideString(long value);                    // 0x00405F00
extern C3DVector g_ItemUp;                                               // 0x01383A5C
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390

void CTCHeroReceiveItems::GlobalUpdate(bool)
{
    if (!IsReceivingItems())
        return;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    if (ForceClear) {
        ForceClear = false;
        AmountThroughForAreaResize = 0.0f;
        CurrentAreaItemTaking = InitialAreaItemTook;
        OldAreaItemTaking = InitialAreaItemTook;
        ItemPrimitiveHandle.Clear();
        DisplayingItems = false;
        if (IS_THING_ALIVE(PCurrentItem)) {
            AddItemToInventory(*PCurrentItem, PersistCurrentItem);
            Items.pop_front();
            PCurrentItem = CIntelligentPointer<CThingObject>(0);
        }
        if (Items.empty()) {
            DisplayingItems = false;
            if (SendMessageW && IS_THING_ALIVE(PContainer))
                GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_HERO_REWARDED_FROM_CONTAINER,
                    CMainGameComponent::GetConstantFPS(), C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, (long)PContainer.Get());
            if (IS_THING_ALIVE(PContainer)) {
                CTCFishingSpot* fishing_spot;
                if (PContainer->GetTC(&fishing_spot)) {
                    fishing_spot->SetAsEnabled(false);
                } else {
                    CTCDiggingSpot* digging_spot;
                    if (PContainer->GetTC(&digging_spot))
                        digging_spot->SetAsEnabled(false);
                }
            }
            BackgroundPrimitiveHandle.Clear();
            return;
        }
    }
    if (!IS_THING_ALIVE(PCurrentItem)) {
        for (std::list<std::pair<CIntelligentPointer<CThingObject>, bool> >::iterator it = Items.begin(); it != Items.end(); ) {
            CThingObject* item = it->first.Get();
            if (IS_THING_ALIVE(item))
                ++it;
            else
                it = Items.erase(it);
        }
        if (Items.empty()) {
            ReceivingItems = false;
            DisplayingItems = false;
            return;
        }
        PCurrentItem = Items.front().first;
        PersistCurrentItem = Items.front().second;
        if (PSoundManager) {
            CSoundInit init;
            init.GainFaderBits = 0x8010;
            PSoundManager->Get2DEmitter().PlaySoundEvent(CMainGameComponent::Get()->GetSampleBank(), def.ReceiveItemsSoundCriteria, true, init, false);
        }
        CWideString text;
        if (PCurrentItem->HasTC(TCI_FISH)) {
            const CTCFish* fish = NULL;
            PCurrentItem->PeekTC(&fish);
            const CTCInventoryItem* inventory_item = NULL;
            PCurrentItem->PeekTC(&inventory_item);
            CWideString length_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_FISH_LENGTH");
            CWideString length_units = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_FISH_LENGTH_UNITS");
            CWideString weight_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_FISH_WEIGHT");
            CWideString weight_units = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_FISH_WEIGHT_UNITS");
            text += inventory_item->GetDescription() + L".  " + length_text + L": " + GFIntToWideString((long)fish->GetLength()) + length_units
                  + L".  " + weight_text + L": " + GFIntToWideString((long)fish->GetWeight()) + weight_units + L".";
            OldItemRHSet = CurrentItemRHSet = inventory_item->GetStartingRightHandedSet();
        } else if (PCurrentItem->HasProperty(10) || PCurrentItem->HasTC(TCI_GOLD)) {
            long gold = 0;
            CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
            CTCHeroStats* stats;
            hero->GetTC(&stats);
            const CTCGold* gold_tc = NULL;
            PCurrentItem->PeekTC(&gold_tc);
            if (gold_tc) {
                gold = gold_tc->GetGold();
                CTCOwnedEntity* owned_entity;
                if (PCurrentItem->GetTC(&owned_entity)) {
                    CTCBuyableHouse* house;
                    if (owned_entity->GetPOwner()->GetTC(&house) && house->GetRented()) {
                        stats->AddRentMoney(gold);
                        CTCHeroOnlineScoreboard* scoreboard;
                        hero->GetTC(&scoreboard);
                        scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)16,
                                                           (float)stats->GetRentMoney(), false);
                    }
                }
            }
            stats->AddMoney(gold, true);
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TXT_OBJT_GOLD");
            NGameText::ConstructString(text, gold);
            OldItemRHSet.Init(g_ItemUp, C3DVector(0.0f, 0.0f, -1.0f));
            CurrentItemRHSet = OldItemRHSet;
        } else if (PCurrentItem->HasTC(TCI_INVENTORY_ITEM)) {
            const CTCInventoryItem* inventory_item = NULL;
            PCurrentItem->PeekTC(&inventory_item);
            text += inventory_item->GetDescription();
            OldItemRHSet = CurrentItemRHSet = inventory_item->GetStartingRightHandedSet();
        }
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetText(PeekComponent().PeekCutSceneMainFont(), text, 0x62, 0.0f, true, true);
    }
    if (IsDisplayingItems()) {
        OldAreaItemTaking = CurrentAreaItemTaking;
        if (AmountThroughForAreaResize < 1.0f) {
            AmountThroughForAreaResize += 1.0f / (CMainGameComponent::GetConstantFPS() * def.NumberOfSecondsForContainerItemToReachFullSize);
            GFLimit(AmountThroughForAreaResize, 0.0f, 1.0f);
            CurrentAreaItemTaking.TLX = GFGetInterpolatedValue(InitialAreaItemTook.TLX, def.ContainerItemDisplayAreaTLPos.X, AmountThroughForAreaResize);
            CurrentAreaItemTaking.TLY = GFGetInterpolatedValue(InitialAreaItemTook.TLY, def.ContainerItemDisplayAreaTLPos.Y, AmountThroughForAreaResize);
            CurrentAreaItemTaking.BRX = GFGetInterpolatedValue(InitialAreaItemTook.BRX,
                (float)GPDisplayManager->GetRenderTargetDimensions().X + def.ContainerItemDisplayAreaBRPos.X - 640.0f, AmountThroughForAreaResize);
            CurrentAreaItemTaking.BRY = GFGetInterpolatedValue(InitialAreaItemTook.BRY,
                (float)GPDisplayManager->GetRenderTargetDimensions().Y + def.ContainerItemDisplayAreaBRPos.Y - 480.0f, AmountThroughForAreaResize);
            FullSizeTimer = (long)(CMainGameComponent::GetConstantFPS() * def.NumberOfSecondsForContainerItemToStayAtFullSize + 1.0f);
        }
        --FullSizeTimer;
        OldItemRHSet = CurrentItemRHSet;
        CurrentItemRHSet.Rotate(g_ItemUp, -(1.0f / (CMainGameComponent::GetConstantFPS() * def.NumberOfSecondsForContainerItemToRotate)));
        ParentThing->SetAsRenderedThisFrame(4);
    }
}