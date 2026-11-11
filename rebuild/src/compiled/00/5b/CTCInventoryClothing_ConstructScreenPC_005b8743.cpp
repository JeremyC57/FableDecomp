// CTCInventoryClothing::ConstructScreenPC(NUISystem::CComponent* parent) -- builds the PC clothing screen: dresses the mannequin and hands the screen and the hero to the UI manager, sorts the hero's clothing into head / hands / torso / legs / feet lists by the appearance modifier's suit part (hero suits go to a suits list when one of their parts is owned and is not a "no clothing" placeholder, or the suit is HERO_SUIT_NAKED), then per non-empty part (head, hands, torso, suits, legs, feet) the part name, the PC guide description (placed in the PC_UI_FRAME) and the part id; the PC_SUB_LIST gets the "CLOTHING TITLE" title and one menu entry per part ("<part> x<count>", back = close clothing, the description, a subtitle and a change-child-state action), the fourth entry dimmed and made the default child, and each entry's sub-list from ConstructClothingListPC (flagged when the hero wears a non-removable modifier on that part) or the entry made unavailable; ClothingList = the returned list
// Leaf: ?ConstructScreenPC@CTCInventoryClothing@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B8743, 3,838 bytes, thiscall `ret 4`, no EH frame, biased frame (`push ebp ; lea ebp,[esp-74h] ;
//   sub esp,124h`): optimize("s"). Identity: Ego_r.pdb donor 0x005641A2 / FableWin.pdb 0x01E2AFC8 ?ConstructScreenPC@CTCInventoryClothing@@QAE..
//   (non-virtual: not in the CTCInventoryClothing vtable 0x0124356C); sole caller NUISystem::CManager 0x0041FBDD. The console twin
//   ConstructScreen 0x005B76D1 (vtable slot 77) has the same body shape.
// Shape devices (agentCLO, RELOCATION_MATCH), all plain C++:
//   - the def lookups GetSubDef<T> / GetDef<T> have same-TU bodies under auto_inline(off) (as the matched CTCInventoryWeapons screens
//     0x005C8459 / 0x005C9A95): the CDefPointer locals stay uncaptured (retail's in-place `dec dword ptr [ecx+4] ; jne` releases);
//   - STLport's _List_base(const allocator&) body is visible (still called, 0x0042AB58): VC7.1 then homes the list's empty allocator
//     temporary on the dead high byte of the `parent` slot ([ebp+7Fh], 0x005B9038) instead of opening the suit-check flag's slot;
//   - the per-part item count reads `partLists[i]` (no reference local): retail keeps the counter (`inc ecx ; cmp ecx,eax ; jb`) and
//     re-reads the size after the entry test (0x005B917A..0x005B919D); a reference local gives a count-down loop;
//   - the ConstructClothingListPC call is written in both arms of the worn-part test (retail pushes the literal `1` / `ebx` in each
//     arm and tail-merges the call, 0x005B94D5 / 0x005B94D9); a bool expression argument is materialised in al.
// Header: engine/CTCInventoryClothing.h FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN (NHeroInformationScreens::CBase at +0x130, the screen
//   methods, typed ClothingItemMap / ClothingList), engine/CHeroSuitDef.h / engine/CAppearanceModifierDef.h _STATGETTYPENAME opt-ins.
// Name fix (agentNF7 2026-11-10): 0x0042AF3C is CDefinitionManager::GetDef<CThingBaseDef> (FableWin.pdb; NF5/NF6 STATUS): the out handle retyped CDefPointer<const CThingBaseDef> (shared engine/CThingBaseDef.h), so the template binds that instantiation.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CACTIONPARAMSTATE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CACTIONPARAMSTRING_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CActionParamState.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroSuitDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CThingBaseDef.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CThingBaseDef> 0x0042AF3C, GetDef<CThingObjectDef>
// 0x0042B040, GetSubDef<CHeroSuitDef> 0x005935C4, GetSubDef<CAppearanceModifierDef> 0x0057FF92).
#pragma auto_inline(off)
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
// STLport 4.6 _list.h _List_base constructor / destructor bodies (retail keeps them out of line: 0x0042AB58 / 0x0042AB73).
template <class _Tp, class _Alloc>
std::_List_base<_Tp, _Alloc>::_List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(), 0)
{
    _List_node<_Tp>* n = _M_node.allocate(1);
    n->_M_next = n;
    n->_M_prev = n;
    _M_node._M_data = n;
}
#pragma auto_inline(on)

NUISystem::CComponent* CTCInventoryClothing::ConstructScreenPC(NUISystem::CComponent* parent)
{
    InitialiseMannequin();
    UseMannequin = true;
    SetUpSelectedClothes();
    NUISystem::CManager::GetInstance()->SetInventoryClothing(this);
    NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing);
    NUISystem::CComponent* frame = parent->FindChild("PC_UI_FRAME");

    // sort the hero's clothing into the suit-part lists
    std::vector<std::pair<long, long> > items;
    GetAllItems(items);
    std::vector<std::pair<long, long> > headItems;
    std::vector<std::pair<long, long> > handsItems;
    std::vector<std::pair<long, long> > bodyItems;
    std::vector<std::pair<long, long> > legsItems;
    std::vector<std::pair<long, long> > feetItems;
    std::vector<std::pair<long, long> > suitItems;
    for (unsigned long i = 0; i < items.size(); ++i) {
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(items[i].first, &def);
        CDefPointer<const CHeroSuitDef> suit;
        if (def->GetSubDef(&suit)) {
            for (std::map<EClothingSuitPart, long>::const_iterator it = suit->SuitParts.begin(); it != suit->SuitParts.end(); ++it) {
                if (GetNumberOfItemsOfTypeInInventory(it->second) > 0) {
                    if (items[i].first == GetDefinitionManager().GetDefGlobalIndexFromName("HERO_SUIT_NAKED") ||
                        (it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT") &&
                         it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT") &&
                         it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES") &&
                         it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS") &&
                         it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS"))) {
                        suitItems.push_back(items[i]);
                        break;
                    }
                }
            }
        } else {
            CDefPointer<const CAppearanceModifierDef> modifier;
            if (def->GetSubDef(&modifier)) {
                switch (modifier->SuitPart) {
                case CLOTHING_SUIT_HEAD:  headItems.push_back(items[i]); break;
                case CLOTHING_SUIT_BODY:  bodyItems.push_back(items[i]); break;
                case CLOTHING_SUIT_HANDS: handsItems.push_back(items[i]); break;
                case CLOTHING_SUIT_LEGS:  legsItems.push_back(items[i]); break;
                case CLOTHING_SUIT_FEET:  feetItems.push_back(items[i]); break;
                }
            }
        }
    }

    // one menu entry per non-empty part
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    std::vector<std::vector<std::pair<long, long> > > partLists;
    std::vector<EClothingSuitPart> parts;
    std::vector<CWideString> partNames;
    std::vector<CCountedPointer<NUISystem::CComponent> > descriptions;
    if (!headItems.empty()) {
        partLists.push_back(headItems);
        partNames.push_back(textBank.GetTextBySymbol("TEXT_GUI_MENU_HEAD"));
        descriptions.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_HEAD"));
        if (frame)
            frame->AddPositionChild(descriptions.back().Get());
        parts.push_back(CLOTHING_SUIT_HEAD);
    }
    if (!handsItems.empty()) {
        partLists.push_back(handsItems);
        partNames.push_back(textBank.GetTextBySymbol("TEXT_GUI_MENU_HANDS"));
        descriptions.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_HANDS"));
        if (frame)
            frame->AddPositionChild(descriptions.back().Get());
        parts.push_back(CLOTHING_SUIT_HANDS);
    }
    if (!bodyItems.empty()) {
        partLists.push_back(bodyItems);
        partNames.push_back(textBank.GetTextBySymbol("TEXT_GUI_MENU_TORSO"));
        descriptions.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_TORSO"));
        if (frame)
            frame->AddPositionChild(descriptions.back().Get());
        parts.push_back(CLOTHING_SUIT_BODY);
    }
    if (!suitItems.empty()) {
        partLists.push_back(suitItems);
        partNames.push_back(textBank.GetTextBySymbol("TEXT_GUI_MENU_SUITS"));
        descriptions.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_SUITS"));
        if (frame)
            frame->AddPositionChild(descriptions.back().Get());
        parts.push_back(CLOTHING_SUIT_NULL);
    }
    if (!legsItems.empty()) {
        partLists.push_back(legsItems);
        partNames.push_back(textBank.GetTextBySymbol("TEXT_GUI_MENU_LEGS"));
        descriptions.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_LEGS"));
        if (frame)
            frame->AddPositionChild(descriptions.back().Get());
        parts.push_back(CLOTHING_SUIT_LEGS);
    }
    if (!feetItems.empty()) {
        partLists.push_back(feetItems);
        partNames.push_back(textBank.GetTextBySymbol("TEXT_GUI_MENU_FEET"));
        descriptions.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_FEET"));
        if (frame)
            frame->AddPositionChild(descriptions.back().Get());
        parts.push_back(CLOTHING_SUIT_FEET);
    }

    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<NUISystem::CComponent> title(manager->CreateComponent("PC_TITLE", false));
    CCountedPointer<NUISystem::CText> titleText;
    title->SetName("CLOTHING TITLE");
    titleText.SetStaticCast(title);
    CWideString titleString;
    ConstructTitleString(titleString);
    titleText->SetValue(titleString, true, true);
    list->AddNonScrollingChild(title);
    if (frame)
        frame->AddPositionChild(title.Get());
    unsigned long partCount = partLists.size();
    list->InitialiseOffsets(partCount);

    // the parts the hero wears that cannot be removed
    CTCHeroAttachableAppearanceModifiers* modifiers = 0;
    ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &modifiers);
    std::list<long> wornModifiers;
    modifiers->GetAppearanceModifierList(wornModifiers, APPEARANCE_CLOTHING);
    std::map<EClothingSuitPart, bool> fixedParts;
    for (std::list<long>::iterator it = wornModifiers.begin(); it != wornModifiers.end(); ++it) {
        CDefPointer<const CThingObjectDef> def;
        CDefPointer<const CAppearanceModifierDef> modifier;
        if (GetDefinitionManager().GetDef(*it, &def)) {
            if (def->GetSubDef(&modifier) && !modifier->IsRemoveable)
                fixedParts[modifier->SuitPart] = true;
        }
    }

    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
    frame->AddPositionChild(viewport);
    ClothingItemMap.clear();
    long defaultChild = 0;
    bool dimButton = false;
    for (unsigned long i = 0; i < partCount; ++i) {
        CWideString text(partNames[i]);
        if (i == 3) {
            defaultChild = 3;
            dimButton = true;
        }
        if (!partLists[i].empty()) {
            text += CTCInventoryStats::GetXString();
            long count = 0;
            for (unsigned long j = 0; j < partLists[i].size(); ++j)
                count += partLists[i][j].second;
            text += GFIntToWideString(count);
        }
        CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f,
            "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        if (dimButton) {
            NUISystem::CComponent* button = entry->FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
            if (button) {
                std::map<unsigned long, NUISystem::CUIState>& states = button->GetStates();
                for (std::map<unsigned long, NUISystem::CUIState>::iterator it = states.begin(); it != states.end(); ++it) {
                    it->second.Colour.R = (unsigned char)(it->second.Colour.R * 0.9f);
                    it->second.Colour.B = (unsigned char)(it->second.Colour.B * 0.9f);
                }
            }
            dimButton = false;
        }
        CCountedPointer<NUISystem::CAction> backAction(new NUISystem::CAction);
        backAction->ActionType = NUISystem::UI_ACTION_TYPE_CLOSE_CLOTHING;
        entry->AddActionOnBack(backAction);
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        menuEntry->AddUIActionParam(entry.Get());
        entry->AddChild(descriptions[i]);
        viewport->AddFakeChild(descriptions[i].Get());
        CCountedPointer<NUISystem::CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE", false));
        CCountedPointer<NUISystem::CText> subtitleText;
        subtitleText.SetStaticCast(subtitle);
        subtitleText->SetValue(text, true, true);
        subtitleText->SetName(text.ToCharString());
        CCountedPointer<NUISystem::CAction> stateAction(new NUISystem::CAction);
        stateAction->ActionType = NUISystem::UI_ACTION_TYPE_CHANGE_CHILD_STATE;
        stateAction->UIActionParams.push_back(new NUISystem::CActionParamState(NUISystem::UI_STATE_INACTIVE));
        stateAction->UIActionParams.push_back(new NUISystem::CActionParamString(text));
        stateAction->UIActionParams.push_back(new NUISystem::CActionParamSLONG(0));
        entry->AddAction(stateAction);
        entry->AddChild(subtitle);
        frame->AddPositionChild(subtitle.Get());
        list->AddScrollingChildAndSetOffsets(entry);
        CCountedPointer<NUISystem::CComponent> subList;
        if (parts[i] != CLOTHING_SUIT_NULL && fixedParts[parts[i]])
            subList.Reset(ConstructClothingListPC(partLists[i], viewport, frame, parts[i], true));
        else
            subList.Reset(ConstructClothingListPC(partLists[i], viewport, frame, parts[i], false));
        if (!subList.IsNull())
            entry->AddChild(subList);
        else
            entry->MakeUnavailable();
    }
    list->SetDefaultChild(defaultChild);
    ClothingList = list;
    return list;
}