// CTCInventoryTrade::ApplyTattooCard(long tattoo_card_def_index) -- apply a bought tattoo card to the hero: remember the permanent tattoos the hero wears, add the card's tattoo to the hero morph, then either (the card removes all tattoos) clear the texture morphs and put the permanent tattoos back, re-uploading their texture layers, or upload the card's one texture layer
// Leaf: ?ApplyTattooCard@CTCInventoryTrade@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) 0x005AD305, 620 bytes, thiscall `ret 4`, size TU with frame pointer (the TU pragmas of the landed
// Inline-budget: the existing shared opt-in FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY makes GetSubDef<CTattooDef> visible but
//   __declspec(noinline) (retail calls 0x005B1EFC out of line; visibility is the lever); it grades RELOCATION_MATCH at 0x005B1EFC from this file.
//   same-TU row CTCInventoryTrade::BuyItem 0x005AF64E, whose opt-in / include set this row starts from).
// Names: Ego_r.pdb / FableWin.pdb ?ApplyTattooCard@CTCInventoryTrade@@QAEXJ@Z; Ego_d tc_inventory_trade.obj: parameter
//   tattoo_card_def_index, function-scope ptc_morph_const / ptc_morph / ptc_hero_morph / tattoos_backup / tattoos, the first loop's
//   block {count, {ptattoo_def}} and the morph block {ptattoo_def, Textures, {count, {ptattoo_backup_def}}} -- written as named.
// Callees: bitset test 0x00410DE0 + const / non-const CVectorMap::LowerBound 0x004365B0 / 0x0040F020 (inline PeekTC / GetTC,
//   TCI_APPEARANCE_MORPH 3), CTCHeroMorph::GetTattooThings 0x0071DE20, CDefClassBase::GetSubDef<CTattooDef> 0x005B1EFC,
//   vector<CDefPointer<const CThingBaseDef> >::push_back 0x005B3325, CTCHeroMorph::AddTattoo(long) 0x0071DA70 and
//   AddTattoo(CDefPointer<const CThingBaseDef>&) 0x0071D940 (Ego_r.pdb names, same order; new hdr opt-in
//   FABLE_RETAIL_CTCHEROMORPH_ADDTATTOO), CTCBase virtual slot 9 FrameUpdate (+0x24), CTCInventoryItem::GetTattooCardDef 0x005D9AA0,
//   set<long> ctor 0x005157F0 / insert 0x00514230, CTCAppearanceMorphBase::UploadTextureMorphsToEngine 0x006CA7F0,
//   CMainGameComponent::PMainGameComponent 0x013B86A0 (inline GetDisplayEngine / Get3DEngine).
// Shape: the shared GetSubDef body is visible (existing FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, noinline COMDAT): c2 then keeps
//   each tattoo def handle in a callee-saved (ebx) for its Permanent test and release (`mov ebx,[ebp-4]` once, `dec [ebx+4]`) and the zero
//   constant out of ebx -- the "three-way allocation cycle" the local-model draft was parked on. The second loop is the same
//   `count < tattoos_backup.size()` index loop; VC7.1 makes it the count-down pointer walk retail has.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_ISSELLABLE   // NF34: 0x005D9B30 is IsSellable (Ego_r + FableWin; def +0x5C)
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS
#define FABLE_RETAIL_CTCHEROMORPH_ADDTATTOO
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_MEMBERS
#define FABLE_RETAIL_CTATTOODEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CTCAppearanceMorphBase.h"
#include "engine/CDisplayEngine.h"
#include "stlport/fable_map.h"

void CTCInventoryTrade::ApplyTattooCard(long tattoo_card_def_index)
{
    std::vector<CDefPointer<const CThingBaseDef> > tattoos;
    std::vector<CDefPointer<const CThingBaseDef> > tattoos_backup;
    const CTCHeroMorph* ptc_morph_const;
    if (GetParentThing().PeekTC(&ptc_morph_const))
        ptc_morph_const->GetTattooThings(tattoos);
    for (unsigned long count = 0; count < tattoos.size(); count++) {
        CDefPointer<const CTattooDef> ptattoo_def;
        if (tattoos[count]->GetSubDef(&ptattoo_def) && ptattoo_def->Permanent)
            tattoos_backup.push_back(tattoos[count]);
    }
    CTCHeroMorph* ptc_hero_morph = 0;
    if (GetParentThing().GetTC(&ptc_hero_morph)) {
        ptc_hero_morph->AddTattoo(tattoo_card_def_index);
        ptc_hero_morph->FrameUpdate();
    }
    CTCAppearanceMorphBase* ptc_morph;
    if (GetParentThing().GetTC(&ptc_morph)) {
        CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(tattoo_card_def_index);
        std::set<long> Textures;
        if (ptattoo_def->RemovesAllTattoos) {
            ptc_morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), 0, false, 0.0f);
            for (unsigned long count = 0; count < tattoos_backup.size(); count++) {
                if (ptc_hero_morph) {
                    ptc_hero_morph->AddTattoo(tattoos_backup[count]);
                    ptc_hero_morph->FrameUpdate();
                }
                CDefPointer<const CTattooDef> ptattoo_backup_def;
                if (tattoos_backup[count]->GetSubDef(&ptattoo_backup_def))
                    Textures.insert(ptattoo_backup_def->ReplacingTextureIndex);
            }
            if (!tattoos_backup.empty())
                ptc_morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), &Textures, false, 0.0f);
        } else {
            Textures.insert(ptattoo_def->ReplacingTextureIndex);
            ptc_morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), &Textures, false, 0.0f);
        }
    }
}