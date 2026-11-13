// CTCInventoryAbilities::GetMagicListUIComponent(long& count, NUISystem::CComponent* parent) -- the console skills screen's magic section
// Grade: DIFFER(5619v5619) -- every instruction right (sd.py: 0 hunks beyond 2 masked vtable relocations); frame residue only: two frame words swapped, retail `received` [ebp+68h] / received-loop n [ebp+64h] (0x005EF0AE..0x005EF0F3) v ours [ebp+64h] / [ebp+68h] (+1 word [ebp+68h] v [ebp-0Ch]); D 49 DF 2 DR 2
// Leaf: ?GetMagicListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EEFF2, 5619 bytes, `ret 8`. Biased frame (lea ebp,[esp-70h]): optimize("s",on) alone.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CSPELLCONTAINERLIST_VIRTUALS
#define FABLE_RETAIL_CSPELLCONTAINERLIST_OFFSETS
#define FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CEXPRESSIONSUBDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSION_NONCONST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SCREEN
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_algorithm.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CExpressionSubDef.h"
#include "engine/CExpressionDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CSpellContainerList.h"
#include "engine/NUISystem_CSpellContainer.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CHeroAbilityDef.h"
CWideString FABLE_FASTCALL GFIntToWideString(long value);

// Same-TU bodies of the helpers the row calls out of line (retail emits them earlier in this TU: GetCategory 0x005EDE37,
// GetAbilityPDef 0x005EDFD9, GetMiscThingsDef 0x005EE3B3); only the row is graded.
#pragma auto_inline(off)
// STLport _List_base<EHeroAbility> constructor / destructor COMDATs (0x005F88ED / 0x005F8908): out of line in retail, bodies visible.
namespace std {
template<class _Tp,class _Alloc> _List_base<_Tp,_Alloc>::_List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
    _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
}
template<class _Tp,class _Alloc> vector<_Tp,_Alloc>::vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
    this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
}
template<class _Tp,class _Alloc> _List_base<_Tp,_Alloc>::~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
}
CTCInventoryAbilities::EUISpellCategory CTCInventoryAbilities::GetCategory(EHeroAbility ability)
{
    switch (ability) {
    case 1: case 2: case 5: case 6: case 10:
        return SPELL_CATEGORY_SURROUND;
    case 3: case 9: case 11: case 12: case 16:
        return SPELL_CATEGORY_ATTACK;
    case 4: case 8: case 13: case 14: case 15: case 17:
        return SPELL_CATEGORY_PHYSICAL;
    default:
        return SPELL_CATEGORY_ATTACK;
    }
}
CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    return Abilities[ability].second;
}
const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF");
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    {
        CDefPointer<const NUISystem::CUIMiscThingsDef> def;
        CGameDefinitionManager::Get().GetDef(index, &def);
        return def;
    }
}
#pragma auto_inline(on)
long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const
{
    return Abilities[ability].first;
}

NUISystem::CComponent* CTCInventoryAbilities::GetMagicListUIComponent(long& count, NUISystem::CComponent* parent)
{
    const NGameText::CDataBank& bank = PeekComponent().GetTextBank();
    std::list<EHeroAbility> owned;
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    const CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();
    for (long n = 1; n < 20; ++n)
        if (Abilities[n].first >= 0)
            owned.push_back(static_cast<EHeroAbility>(n));
    AssignedAbilities.clear();
    AssignedAbilitiesSave.clear();
    unsigned long n;
    unsigned long received = ReceivedAbilityOrder.size() <= misc->TotalSpellsInPalettes ? ReceivedAbilityOrder.size() : misc->TotalSpellsInPalettes;
    for (unsigned long n = 0; n < received; ++n) {
        AssignedAbilities.push_back(ReceivedAbilityOrder[n]);
        AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);
    }
    unsigned long assigned = AssignedAbilities.size();
    if (assigned < misc->TotalSpellsInPalettes) {
        for (n = 0; n < misc->TotalSpellsInPalettes - assigned; ++n) {
            EHeroAbility empty = static_cast<EHeroAbility>(0);
            EHeroAbility savedEmpty = static_cast<EHeroAbility>(0);
            AssignedAbilities.push_back(empty);
            AssignedAbilitiesSave.push_back(savedEmpty);
        }
    }
    for (n = 0; n < AssignedAbilities.size() && n < AssignedAbilitiesSave.size(); ++n) {
        if (n % misc->TotalSpellsInContainer == 1 && n < AssignedAbilities.size() - 1 && n < AssignedAbilitiesSave.size() - 1) {
            EHeroAbility swapped = AssignedAbilities[n];
            AssignedAbilities[n] = AssignedAbilities[n + 1];
            AssignedAbilities[n + 1] = swapped;
            swapped = AssignedAbilitiesSave[n];
            AssignedAbilitiesSave[n] = AssignedAbilitiesSave[n + 1];
            AssignedAbilitiesSave[n + 1] = swapped;
        }
    }
    CCountedPointer<NUISystem::CComponent> palette(ui->CreateComponent("UI_SPELL_CONTAINER_LIST_TEMPLATE", false));
    CCountedPointer<NUISystem::CComponent> container(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE", false));
    CCountedPointer<NUISystem::CComponent> selection(ui->CreateComponent("UI_SPELL_SELECTION_TEMPLATE", false));
    CCountedPointer<NUISystem::CComponent> indication(ui->CreateComponent("UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST", false));
    NUISystem::CSpellContainerList* paletteList = static_cast<NUISystem::CSpellContainerList*>(palette.Get());
    paletteList->InitialiseOffsets(AssignedAbilities.size() / misc->TotalSpellsInContainer + 1);
    paletteList->AddSpellSelectionChild(selection);
    paletteList->AddSpellIndicationChild(indication);
    NUISystem::CSpellContainer* spellContainer = static_cast<NUISystem::CSpellContainer*>(container.Get());
    spellContainer->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
    std::vector<EHeroAbility> reordered(AssignedAbilities.size());
    for (unsigned long n = 0; n < AssignedAbilities.size(); ++n)
        reordered[AssignedAbilities.size() - (n / misc->TotalSpellsInContainer + 1) * misc->TotalSpellsInContainer + n % misc->TotalSpellsInContainer] = AssignedAbilities[n];
    for (unsigned long n = 0; n < reordered.size(); ++n) {
        if (n % misc->TotalSpellsInContainer == 0 && n != 0) {
            paletteList->AddChildAndSetOffsets(container);
            container.Reset(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE", false));
            spellContainer = static_cast<NUISystem::CSpellContainer*>(container.Get());
            spellContainer->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
        }
        CCountedPointer<NUISystem::CComponent> spell;
        if (reordered[n] != static_cast<EHeroAbility>(0)) {
            spell.Reset(ui->CreateComponent("UI_SPELL_TEMPLATE", false));
            static_cast<NUISystem::CSprite*>(spell.Get())->SetGraphicIndex(Abilities[reordered[n]].second->IconGraphicIndex);
        } else
            spell.Reset(ui->CreateComponent("UI_SPELL_EMPTY_TEMPLATE", false));
        static_cast<NUISystem::CSpellContainer*>(container.Get())->AddChildAndSetOffsets(spell);
    }
    paletteList->AddChildAndSetOffsets(container);
    CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS", false));
    NUISystem::CComponent* assignment = actions->FindChild("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN");
    CCountedPointer<NUISystem::CAction> exclusive(new NUISystem::CAction);
    exclusive->ActionType = NUISystem::UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT;
    CCountedPointer<NUISystem::CAction> show(new NUISystem::CAction);
    show->ActionType = NUISystem::UI_ACTION_TYPE_SHOW_SPELL_SELECTION;
    CCountedPointer<NUISystem::CAction> set(new NUISystem::CAction);
    set->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN;
    assignment->AddAction(exclusive);
    assignment->AddAction(show);
    assignment->AddAction(set);
    CategoriesLists.clear();
    unsigned long surroundCount = 0, physicalCount = 0, attackCount = 0;
    for (std::list<EHeroAbility>::iterator it = owned.begin(); it != owned.end(); ++it) {
        EUISpellCategory category = GetCategory(*it);
        if (category == SPELL_CATEGORY_SURROUND) ++surroundCount;
        if (category == SPELL_CATEGORY_PHYSICAL) ++physicalCount;
        if (category == SPELL_CATEGORY_ATTACK) ++attackCount;
    }
    if (surroundCount) {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
        list->InitialiseOffsets(surroundCount);
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));
        list->AddNonScrollingChild(ring);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_SURROUND_CATEGORIES_TITLE", false));
        list->AddNonScrollingChild(title);
        CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        CategoriesLists[SPELL_CATEGORY_SURROUND] = list;
    }
    if (attackCount) {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
        list->InitialiseOffsets(attackCount);
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));
        list->AddNonScrollingChild(ring);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_ATTACK_CATEGORIES_TITLE", false));
        list->AddNonScrollingChild(title);
        CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        CategoriesLists[SPELL_CATEGORY_ATTACK] = list;
    }
    if (physicalCount) {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
        list->InitialiseOffsets(physicalCount);
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));
        list->AddNonScrollingChild(ring);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_PHYSICAL_CATEGORIES_TITLE", false));
        list->AddNonScrollingChild(title);
        CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        CategoriesLists[SPELL_CATEGORY_PHYSICAL] = list;
    }
    count = attackCount + physicalCount + surroundCount;
    NUISystem::CComponent* root = ui->CreateComponent("UI_MAGIC_CATEGORIES", false);
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if (viewport) {
        if (NUISystem::CComponent* description = root->FindChild("UI_ATTACK_SPELLS_DESC"))
            viewport->AddFakeChild(description);
        if (NUISystem::CComponent* description = root->FindChild("UI_PHYSICAL_SPELLS_DESC"))
            viewport->AddFakeChild(description);
        if (NUISystem::CComponent* description = root->FindChild("UI_SURROUND_SPELLS_DESC"))
            viewport->AddFakeChild(description);
    }
    UnassignedIcons.clear();
    AbilitiesMenuEntries.clear();
    for (std::list<EHeroAbility>::iterator it = owned.begin(); it != owned.end(); ++it) {
        CWideString caption;
        CWideString displayName;
        CTCInventoryExperience* experience;
        if (ParentThing->GetTC(static_cast<ETCInterfaceType>(22), &experience)) {
            caption = displayName = experience->GetExperienceAbilityName(*it);
            caption += misc->SpaceSeparator;
            caption += GFIntToWideString(GetAbilityLevel(*it) + 1);
            displayName += CWideString(L" - ") + bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL") + CWideString(L" ");
            displayName += GFIntToWideString(GetAbilityLevel(*it) + 1);
        }
        CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        AbilitiesMenuEntries[*it] = entry.Get();
        if (std::find(AssignedAbilities.begin(), AssignedAbilities.end(), *it) == AssignedAbilities.end()) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_UNASSIGNED_SPELL_ICON", false));
            entry->AddChild(icon);
            UnassignedIcons[*it] = icon.Get();
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        entry->AddChild(palette);
        menuEntry->AddUIActionParam(new NUISystem::CActionParamSLONG(*it));
        menuEntry->AddUIActionParam(new NUISystem::CActionParamULONG(Abilities[*it].second->IconGraphicIndex));
        menuEntry->AddUIActionParam(palette.Get());
        CCountedPointer<NUISystem::CComponent> name(ui->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
        CCountedPointer<NUISystem::CText> text;
        text.SetStaticCast(name);
        text->SetValue(displayName, true, true);
        entry->AddChild(name);
        CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
        if (icon->GetType() == static_cast<NUISystem::EType>(0))
            static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(icon);
        entry->AddChild(actions);
        CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
        NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
        descriptionText->SetValue(GetAbilityPDef(*it)->LevDescription[GetAbilityLevel(*it)], L"", true);
        entry->AddChild(description);
        if (viewport)
            viewport->AddFakeChild(description.Get());
        static_cast<NUISystem::CList*>(CategoriesLists[GetCategory(*it)])->AddScrollingChildAndSetOffsets(entry);
    }
    {
        std::map<EUISpellCategory, NUISystem::CComponent*>::iterator found = CategoriesLists.find(SPELL_CATEGORY_ATTACK);
        if (found != CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK, root);
            menu->AddChild(children);
            NUISystem::CText* title = static_cast<NUISystem::CText*>(menu->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if (title)
                title->SetValue(title->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attackCount), true, true);
            NUISystem::CText* subtitle = static_cast<NUISystem::CText*>(menu->FindChild("UI_ATTACK_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(subtitle->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attackCount), true, true);
        } else {
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK, root);
            menu->MakeUnavailable();
            NUISystem::CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory, NUISystem::CComponent*>::iterator found = CategoriesLists.find(SPELL_CATEGORY_SURROUND);
        if (found != CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND, root);
            menu->AddChild(children);
            NUISystem::CText* title = static_cast<NUISystem::CText*>(menu->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if (title)
                title->SetValue(title->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(surroundCount), true, true);
            NUISystem::CText* subtitle = static_cast<NUISystem::CText*>(menu->FindChild("UI_SURROUND_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(subtitle->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(surroundCount), true, true);
        } else {
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND, root);
            menu->MakeUnavailable();
            NUISystem::CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory, NUISystem::CComponent*>::iterator found = CategoriesLists.find(SPELL_CATEGORY_PHYSICAL);
        if (found != CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL, root);
            menu->AddChild(children);
            NUISystem::CText* title = static_cast<NUISystem::CText*>(menu->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if (title)
                title->SetValue(title->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(physicalCount), true, true);
            NUISystem::CText* subtitle = static_cast<NUISystem::CText*>(menu->FindChild("UI_PHYSICAL_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(subtitle->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(physicalCount), true, true);
        } else {
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL, root);
            menu->MakeUnavailable();
            NUISystem::CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return root;
}
