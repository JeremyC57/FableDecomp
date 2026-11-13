// CThingPlayerCreature::OnCollisionWithObject(CThing* thing) -- the player creature touched an object: gold goes to the hero's money (and to his rent money and online score when the gold belongs to a rented house), a quest-activating container hands out its items, a pickable item is auto-picked up or shown; then the pickup sound plays and a collected gold / container thing is killed
// Leaf: ?OnCollisionWithObject@CThingPlayerCreature@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006ACA30, 2026 bytes (`ret 4`; ends 0x006AD219, int3 padding to 0x006AD220).
// Identity: Ego_r.pdb 0x00549FF0 / FableWin.pdb 0x01DF10E0 ?OnCollisionWithObject@CThingPlayerCreature@@QAEXPAVCThing@@@Z (public);
//   its only caller is CThingPlayerCreature::OnCollision 0x006AD580 (landed) for an object thing (ThingType 5).
// Callees: CThingObject::GetProperties (virtual slot 48, +0xC0, on the thing); CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound
//   0x0040F020 (non-const: GetTC) / 0x004365B0 (const: PeekTC); CCharString() 0x0099E4B0, (const char*, long) 0x0099EBF0,
//   ~ 0x0099EAE0, operator= 0x0099EFB0; CBasicString<char>::Compare 0x00411570; CThing::PeekDefinitionManager 0x004C7990,
//   PeekGlobalDefinition 0x004C79A0, GetPos 0x004C73D0, Kill 0x004C9B80; CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040;
//   CDefClassBase::GetSubDef<CGoldDef> 0x006AE320 / <CInventoryItemDef> 0x005BADE1; CTCHeroStats::AddMoney 0x0057B327;
//   CTCHeroReceiveItems::HasPickedUpItem 0x007CA800, SetItemAsPickedUp 0x007CB850, GiveItem 0x007CAB30; CThingObject::Create
//   0x00703210; CThingPlayerCreature::CreateAndAddObjectToInventory 0x006AC200; CPlayerGui::AddAutoPickup 0x00437980 (on the
//   singleton 0x013B8790); CTCInventoryItem::GetInventoryType 0x005D9030, AddToInventory 0x005D8D50; the STLport bitset range
//   check (puts 0x00BFEAF2 "bitset", abort 0x00BFEAEC); CTCGold::GetGold 0x007CBDC0; CBaseIntelligentPointer::GetPItem 0x00A01B50
//   (CTCOwnedEntity::GetPOwner); CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0; CTCSoundPlayer::PlayAttachedCriteria
//   0x0066BD00.
// Interfaces (ETCInterfaceType): HERO_STATS 4, ON_ACTION_USE 13, SOUND_PLAYER 16, INVENTORY_ITEM 26, OWNED_ENTITY 51, CONTAINER 60,
//   STOCK_ITEM 84, HERO_RECEIVE_ITEMS 203, GOLD 204, BUYABLE_HOUSE 213, HERO_ONLINE_SCOREBOARD 269 (bitset<274> range-checked for
//   the item's variable inventory type). Object properties (EObjectProperty, enumerator names not in the name sets): 0x400 gold,
//   0x2000000 pickable object; CThingObjectDef::Property +0x90 tests the same gold bit.
// Shape (each measured; NOTES.md grade log):
//   - No early returns: the exits that skip the sound and the kill are switch `break`s with a `collected` flag that c2 threads
//     away. Five `return`s each add a ~CCharString call to the sound's frame-packer weight (12 v the container pointer's 7) and
//     swap their frame words; the flag form gives the retail order (container [esp+18h], sound [esp+1Ch]).
//   - CThing::HasTC<CTCActionUseActivateQuest>() (FableWin / Ego_r member template; body proven against retail's unreferenced
//     COMDAT copy 0x006AE680, forced inline as GetTC / PeekTC: plain-inline cost 175) gives the class-name test with both string
//     temporaries destroyed unconditionally.
//   - Def lookups through the shared bodies (GETDEF_GLOBAL_BODY / GETSUBDEF_BODY): the out handles do not escape, so the
//     releases are `dec [reg+4] ; jne` without re-tests as in retail.
//   - Container items: `if (!give_item) { auto pickup } else { create and give }` lays the auto-pickup block after the loop as
//     retail does (the then/else order of the plain if puts it inside the loop).
//   - Object pickup: an `added` flag (one shared def release; a `break` out of the def's scope adds a second release).
//   - The rent score reads GetRentMoney() at the UpdateScoreForActivity call: c2 forwards the stored sum (`mov [esp+2Ch],ecx`, no
//     copy); a named `long` local adds an allocator candidate that shifts the stats / gold colouring and the scratch rotation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_CLASSTYPE
#define FABLE_RETAIL_CTHING_PEEKGLOBALDEFINITION
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_CREATEANDADD
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CINVENTORYITEMDEF_STATGETTYPENAME
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CLOTHING_DEFS
#define FABLE_RETAIL_CGOLDDEF
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_RENTMONEY
#define FABLE_RETAIL_CTCSTOCKITEM
#define FABLE_RETAIL_CTCCONTAINER_ITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUP
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_AUTOPICKUP
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CGoldDef.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCStockItem.h"
#include "engine/CTCActionUseActivateQuest.h"
#include "engine/CTCContainer.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CTCGold.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CPlayerGui.h"


void CThingPlayerCreature::OnCollisionWithObject(CThing* thing)
{
    bool collected = false;
    bool kill_thing = false;
    bool call_on_kill = true;
    CCharString sound;
    switch (static_cast<CThingPhysical*>(thing)->GetProperties()) {
    case 0x2000000: {                                   // a pickable object (inventory item, quest container)
        const CTCStockItem* stock_item;
        if (thing->PeekTC(&stock_item) && stock_item->IsForSale())
            break;
        if (thing->HasTC<CTCActionUseActivateQuest>()) {
            const CTCContainer* container;
            if (!thing->PeekTC(&container))
                break;
            const std::vector<std::pair<long, CCharString> >& items = container->GetContainerItems();
            if (items.empty())
                break;
            for (unsigned long i = 0; i < items.size(); ++i) {
                CDefPointer<const CThingObjectDef> def;
                PeekDefinitionManager().GetDef(items[i].first, &def);
                if (!def.IsNull()) {
                    if (def->Property & 0x400) {                    // the gold property bit, as above
                        CTCHeroStats* stats;
                        if (GetTC(&stats)) {
                            CDefPointer<const CGoldDef> gold_def;
                            def->GetSubDef(&gold_def);
                            if (!gold_def.IsNull()) {
                                stats->AddMoney(gold_def->Gold, true);
                                sound = PeekGlobalDefinition().PickupGoldSoundCriteria;
                            }
                        }
                    } else {
                        CDefPointer<const CInventoryItemDef> item_def;
                        def->GetSubDef(&item_def);
                        bool give_item = !item_def.IsNull() && item_def->DoNotAutoPickUp;
                        if (!give_item) {
                            CTCHeroReceiveItems* receive_items = NULL;
                            GetTC(&receive_items);
                            if (receive_items && !receive_items->HasPickedUpItem(items[i].first)) {
                                receive_items->SetItemAsPickedUp(items[i].first);
                                give_item = true;
                            }
                        }
                        if (!give_item) {
                            if (CreateAndAddObjectToInventory(items[i].first, -1, true, items[i].second)) {
                                CPlayerGui::Get().AddAutoPickup(items[i].first);
                                sound = PeekGlobalDefinition().PickupItemSoundCriteria;
                            }
                        } else {
                            CThingObject* object = CThingObject::Create(items[i].first, GetPos(), GetPlayerNumber(), NULL, 0, items[i].second);
                            if (object) {
                                CTCHeroReceiveItems* receiver;
                                GetTC(&receiver);
                                receiver->GiveItem(*object, true);
                            }
                        }
                    }
                }
            }
            kill_thing = true;
            call_on_kill = false;
            collected = true;
        } else {
            bool already_picked_up = false;
            CDefPointer<const CThingObjectDef> def;
            PeekDefinitionManager().GetDef(thing->GetDefGlobalIndex(), &def);
            if (!def.IsNull()) {
                CDefPointer<const CInventoryItemDef> item_def;
                def->GetSubDef(&item_def);
                if (!item_def.IsNull()) {
                    CTCHeroReceiveItems* receive_items = NULL;
                    if (item_def->AutoPickUpAfterFirstPickUp && GetTC(&receive_items) && receive_items) {
                        if (receive_items->HasPickedUpItem(thing->GetDefGlobalIndex()))
                            already_picked_up = true;
                        else
                            receive_items->SetItemAsPickedUp(thing->GetDefGlobalIndex());
                    }
                }
            }
            bool added = false;
            CTCInventoryItem* inventory_item;
            if (already_picked_up && thing->GetTC(&inventory_item)) {
                NInventory::CTCInventoryBase* inventory = NULL;
                GetTC(inventory_item->GetInventoryType(), &inventory);
                if (inventory_item->AddToInventory(*inventory, false, true, 0, true, true)) {
                    CPlayerGui::Get().AddAutoPickup(thing->GetDefGlobalIndex());
                    sound = PeekGlobalDefinition().PickupItemSoundCriteria;
                    added = true;
                }
            }
            if (!added) {
                CTCHeroReceiveItems* receiver;
                GetTC(&receiver);
                receiver->GiveItem(static_cast<CThingObject&>(*thing), true);
            }
            collected = true;
        }
        break;
    }
    case 0x400: {                                       // gold
        CTCHeroStats* stats;
        if (!GetTC(&stats))
            break;
        collected = true;
        kill_thing = true;
        const CTCGold* gold = NULL;
        thing->PeekTC(&gold);
        if (gold) {
            stats->AddMoney(gold->GetGold(), true);
            sound = PeekGlobalDefinition().PickupGoldSoundCriteria;
            CTCOwnedEntity* owned_entity;
            if (thing->GetTC(&owned_entity)) {
                CTCBuyableHouse* house;
                if (owned_entity->GetPOwner()->GetTC(&house) && house->GetRented()) {
                    stats->AddRentMoney(gold->GetGold());
                    CTCHeroOnlineScoreboard* scoreboard;
                    GetTC(&scoreboard);
                    scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)16,   // the rent activity (enumerator names not in the PDB name sets)
                                                          (float)stats->GetRentMoney(), false);
                }
            }
        }
        break;
    }
    }
    if (collected) {
        CTCSoundPlayer* sound_player;
        if (GetTC(&sound_player))
            sound_player->PlayAttachedCriteria(sound, true, false, false, -1);
        if (kill_thing)
            thing->Kill(call_on_kill);
    }
}