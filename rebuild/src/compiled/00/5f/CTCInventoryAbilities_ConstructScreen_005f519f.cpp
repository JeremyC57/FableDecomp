// CTCInventoryAbilities::ConstructScreen(CComponent*) -- build the magic and expressions sections of the skills screen.
// Same-TU callees: complete magic and expressions builders keep the output counts local.
// Leaf: ?ConstructScreen@CTCInventoryAbilities@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Flags:
// Retail: 005f519f,2088 bytes,thiscall.
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
NUISystem::CComponent* CTCInventoryAbilities::ConstructScreen(NUISystem::CComponent* parent)
{
    const NGameText::CDataBank& textBank=PeekComponent().GetTextBank();
    NUISystem::CManager::GetInstance()->SetAbilitiesInventory(this);
    std::list<CWideString> titles;
    titles.push_back(textBank.GetTextBySymbol("TEXT_GUI_MAGIC"));
    titles.push_back(textBank.GetTextBySymbol("TEXT_GUI_EXPRESSIONS"));
    long expressionCount=0;
    long magicCount=0;
    NUISystem::CManager& ui=*NUISystem::CManager::GetInstance();
    NUISystem::CList* list=static_cast<NUISystem::CList*>(ui.CreateComponent("UI_SUB_LIST",false));
    CCountedPointer<NUISystem::CComponent> magic(GetMagicListUIComponent(magicCount,parent));
    CCountedPointer<NUISystem::CComponent> expressions(GetExpressionsListUIComponent(expressionCount,parent));
    CCountedPointer<NUISystem::CComponent> back(ui.CreateComponent("UI_BACK_FOR_LIST",false));
    CCountedPointer<NUISystem::CComponent> select(ui.CreateComponent("UI_SELECT_FOR_LIST",false));
    list->AddNonScrollingChild(back);
    list->AddNonScrollingChild(select);
    CCountedPointer<NUISystem::CComponent> title(ui.CreateComponent("UI_SKILLS_TITLE",false));
    CCharString titleName("SKILLS TITLE");
    title->SetName(titleName);
    list->AddNonScrollingChild(title);
    unsigned long itemCount=0;
    if(!magic.IsNull()) ++itemCount;
    if(!expressions.IsNull()) ++itemCount;
    list->InitialiseOffsets(itemCount);
    CWideString caption=textBank.GetTextBySymbol("TEXT_GUI_MENU_MAGIC");
    CCountedPointer<NUISystem::CComponent> magicEntry;
    if(magicCount>0) {
        caption+=CTCInventoryStats::GetXString();
        caption+=GFIntToWideString(magicCount);
    }
    if(!magic.IsNull()) {
        magicEntry.Reset(ui.CreateMenuEntryComponent(caption,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        magicEntry->AddChild(magic);
    } else {
        magicEntry.Reset(ui.CreateMenuEntryComponent(caption,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        magicEntry->MakeUnavailable();
    }
    CCountedPointer<NUISystem::CComponent> magicDescription(ui.CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF",false));
    NUISystem::CText* magicText=static_cast<NUISystem::CText*>(magicDescription.Get());
    magicText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol("TEXT_GUI_MENUGUIDE_SKILLS_MAGIC"),true,true);
    magicEntry->AddChild(magicDescription);
    NUISystem::CViewport* viewport=static_cast<NUISystem::CViewport*>(parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    viewport->AddFakeChild(magicDescription.Get());
    CCountedPointer<NUISystem::CComponent> subtitle(ui.CreateComponent("UI_ONE_OFF_SUBTITLE",false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption,true,true);
    magicEntry->AddChild(subtitle);
    list->AddScrollingChildAndSetOffsets(magicEntry);
    CCountedPointer<NUISystem::CComponent> magicRing(ui.CreateComponent("UI_RING_PIC_SKILLS_MAGIC",false));
    magicEntry->AddChild(magicRing);
    caption=textBank.GetTextBySymbol("TEXT_GUI_MENU_EXPRESSIONS");
    if(expressionCount>0) {
        caption+=CTCInventoryStats::GetXString();
        caption+=GFIntToWideString(expressionCount);
    }
    CCountedPointer<NUISystem::CComponent> expressionEntry;
    if(!expressions.IsNull()) {
        expressionEntry.Reset(ui.CreateMenuEntryComponent(caption,25.0f,0.6666667f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        expressionEntry->AddChild(expressions);
    } else {
        expressionEntry.Reset(ui.CreateMenuEntryComponent(caption,25.0f,0.6666667f,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        expressionEntry->MakeUnavailable();
    }
    CCountedPointer<NUISystem::CComponent> expressionDescription(ui.CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF",false));
    NUISystem::CText* expressionText=static_cast<NUISystem::CText*>(expressionDescription.Get());
    expressionText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol("TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS"),true,true);
    expressionEntry->AddChild(expressionDescription);
    viewport->AddFakeChild(expressionDescription.Get());
    subtitle.Reset(ui.CreateComponent("UI_ONE_OFF_SUBTITLE",false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption,true,true);
    expressionEntry->AddChild(subtitle);
    list->AddScrollingChildAndSetOffsets(expressionEntry);
    CCountedPointer<NUISystem::CComponent> expressionRing(ui.CreateComponent("UI_RING_PIC_SKILLS_EXPRESSION",false));
    expressionEntry->AddChild(expressionRing);
    CCountedPointer<NUISystem::CComponent> button(ui.CreateComponent("UI_BACK_FOR_LIST",false));
    list->AddNonScrollingChild(button);
    button.Reset(ui.CreateComponent("UI_SELECT_FOR_LIST",false));
    list->AddNonScrollingChild(button);
    return list;
}

// Same-TU body reconstructed from the earlier 005f4c8f draft; the output count never escapes.
NUISystem::CComponent* CTCInventoryAbilities::GetExpressionsListUIComponent(long& count,NUISystem::CComponent* owner)
{
    NUISystem::CManager* ui=NUISystem::CManager::GetInstance();
    CTCInventory* inventory;
    if(!ParentThing->GetTC(&inventory)) return 0;
    inventory->ItemsMenuEntries.clear();
    inventory->AssignedIcons.clear();
    typedef std::vector<std::pair<long,long> > ItemVector;
    ItemVector allItems,social,renown,alignment,stealth;
    inventory->GetAllItems(allItems);
    count=0;
    for(unsigned long n=0;n<allItems.size();++n) {
        CDefPointer<const CDefClassBase> def;
        GetDefinitionManager().GetDef(allItems[n].first,&def);
        CDefPointer<const CExpressionSubDef> subDef;
        CDefPointer<CExpressionDef> expression;
        if(def->GetSubDef(&subDef)) {
            if(const_cast<CGameDefinitionManager&>(GetDefinitionManager()).GetDef(subDef->ExpressionDef,&expression)) {
                switch(expression->ExpressionType) {
                case EXPRESSION_INVENTORY_TYPE_RENOWN:renown.push_back(allItems[n]);break;
                case EXPRESSION_INVENTORY_TYPE_ALIGNMENT:alignment.push_back(allItems[n]);break;
                case EXPRESSION_INVENTORY_TYPE_STEALTH:stealth.push_back(allItems[n]);break;
                default:social.push_back(allItems[n]);break;
                }
                ++count;
            }
        }
    }
    NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST",false));
    CCountedPointer<NUISystem::CComponent> back(ui->CreateComponent("UI_BACK_FOR_LIST",false));
    CCountedPointer<NUISystem::CComponent> select(ui->CreateComponent("UI_SELECT_FOR_LIST",false));
    list->AddNonScrollingChild(back);list->AddNonScrollingChild(select);
    CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_EXPRESSIONS_TITLE",false));
    title->SetName("EXPRESSIONS TITLE");
    list->AddNonScrollingChild(title);list->InitialiseOffsets(4);
    const NGameText::CDataBank& bank=PeekComponent().GetTextBank();
    CWideString caption;
    caption=bank.GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
    ConstructExpressionCategoryList(caption,list,social,owner,"UI_RING_PIC_EXPR_SOCIAL","TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
    caption=bank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
    ConstructExpressionCategoryList(caption,list,renown,owner,"UI_RING_PIC_EXPR_RENOWN","TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
    caption=bank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
    ConstructExpressionCategoryList(caption,list,alignment,owner,"UI_RING_PIC_EXPR_ALIGNMENT","TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
    caption=bank.GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
    ConstructExpressionCategoryList(caption,list,stealth,owner,"UI_RING_PIC_EXPR_STEALTH","TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    return list;
}

// Same-TU magic builder: complete retail phases, with original data types and shared UI/container models.
NUISystem::CComponent* CTCInventoryAbilities::GetMagicListUIComponent(long& count,NUISystem::CComponent* owner)
{
    const unsigned short emptyWide[]={0};
    const unsigned short spaceWide[]={0x20,0};
    const unsigned short levelPrefix[]={0x20,0x2d,0x20,0};
    const NGameText::CDataBank& bank=PeekComponent().GetTextBank();
    std::list<EHeroAbility> owned;
    NUISystem::CManager* ui=NUISystem::CManager::GetInstance();
    const CDefPointer<const NUISystem::CUIMiscThingsDef> misc=GetMiscThingsDef();
    for(long n=1;n<20;++n)
        if(Abilities[n].first>=0) owned.push_back(static_cast<EHeroAbility>(n));
    AssignedAbilities.clear();
    AssignedAbilitiesSave.clear();
    unsigned long received=std::min<unsigned long>(ReceivedAbilityOrder.size(),misc->TotalSpellsInPalettes);
    for(unsigned long n=0;n<received;++n) {
        AssignedAbilities.push_back(ReceivedAbilityOrder[n]);
        AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);
    }
    unsigned long assigned=AssignedAbilities.size();
    if(assigned<misc->TotalSpellsInPalettes) {
        EHeroAbility empty=static_cast<EHeroAbility>(0);
        EHeroAbility savedEmpty=static_cast<EHeroAbility>(0);
        for(unsigned long n=0;n<misc->TotalSpellsInPalettes-assigned;++n) {
            AssignedAbilities.push_back(empty);
            AssignedAbilitiesSave.push_back(savedEmpty);
        }
    }
    for(unsigned long n=0;n<AssignedAbilities.size() && n<AssignedAbilitiesSave.size();++n) {
        if(n%misc->TotalSpellsInContainer==1 && n<AssignedAbilities.size()-1 && n<AssignedAbilitiesSave.size()-1) {
            EHeroAbility swapped=AssignedAbilities[n];
            AssignedAbilities[n]=AssignedAbilities[n+1];
            AssignedAbilities[n+1]=swapped;
            swapped=AssignedAbilitiesSave[n];
            AssignedAbilitiesSave[n]=AssignedAbilitiesSave[n+1];
            AssignedAbilitiesSave[n+1]=swapped;
        }
    }
    CCountedPointer<NUISystem::CComponent> palette(ui->CreateComponent("UI_SPELL_CONTAINER_LIST_TEMPLATE",false));
    CCountedPointer<NUISystem::CComponent> container(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE",false));
    CCountedPointer<NUISystem::CComponent> selection(ui->CreateComponent("UI_SPELL_SELECTION_TEMPLATE",false));
    CCountedPointer<NUISystem::CComponent> indication(ui->CreateComponent("UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST",false));
    NUISystem::CSpellContainerList* paletteList=static_cast<NUISystem::CSpellContainerList*>(palette.Get());
    paletteList->InitialiseOffsets(AssignedAbilities.size()/misc->TotalSpellsInContainer+1);
    paletteList->AddSpellSelectionChild(selection);
    paletteList->AddSpellIndicationChild(indication);
    static_cast<NUISystem::CSpellContainer*>(container.Get())->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
    std::vector<EHeroAbility> reordered(AssignedAbilities.size());
    for(unsigned long n=0;n<AssignedAbilities.size();++n) {
        unsigned long row=n/misc->TotalSpellsInContainer;
        unsigned long column=n%misc->TotalSpellsInContainer;
        reordered[AssignedAbilities.size()-(row+1)*misc->TotalSpellsInContainer+column]=AssignedAbilities[n];
    }
    unsigned long paletteSize=reordered.size();
    for(unsigned long n=0;n<paletteSize;++n) {
        if(n%misc->TotalSpellsInContainer==0 && n!=0) {
            paletteList->AddChildAndSetOffsets(container);
            container.Reset(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE",false));
            static_cast<NUISystem::CSpellContainer*>(container.Get())->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
        }
        CCountedPointer<NUISystem::CComponent> spell;
        if(reordered[n]!=static_cast<EHeroAbility>(0)) {
            spell.Reset(ui->CreateComponent("UI_SPELL_TEMPLATE",false));
            static_cast<NUISystem::CSprite*>(spell.Get())->SetGraphicIndex(Abilities[reordered[n]].second->IconGraphicIndex);
        } else spell.Reset(ui->CreateComponent("UI_SPELL_EMPTY_TEMPLATE",false));
        static_cast<NUISystem::CSpellContainer*>(container.Get())->AddChildAndSetOffsets(spell);
    }
    paletteList->AddChildAndSetOffsets(container);
    CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS",false));
    NUISystem::CComponent* assignment=actions->FindChild("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN");
    CCountedPointer<NUISystem::CAction> exclusive(new NUISystem::CAction);
    exclusive->ActionType=NUISystem::UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT;
    CCountedPointer<NUISystem::CAction> show(new NUISystem::CAction);
    show->ActionType=NUISystem::UI_ACTION_TYPE_SHOW_SPELL_SELECTION;
    CCountedPointer<NUISystem::CAction> set(new NUISystem::CAction);
    set->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN;
    assignment->AddAction(exclusive);
    assignment->AddAction(show);
    assignment->AddAction(set);
    CategoriesLists.clear();
    unsigned long attackCount=0,physicalCount=0,surroundCount=0;
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        EUISpellCategory category=GetCategory(*it);
        if(category==SPELL_CATEGORY_SURROUND) ++surroundCount;
        if(category==SPELL_CATEGORY_PHYSICAL) ++physicalCount;
        if(category==SPELL_CATEGORY_ATTACK) ++attackCount;
    }
    const EUISpellCategory categoryOrder[3]={SPELL_CATEGORY_SURROUND,SPELL_CATEGORY_ATTACK,SPELL_CATEGORY_PHYSICAL};
    const unsigned long categoryCounts[3]={surroundCount,attackCount,physicalCount};
    const char* categoryTitles[3]={"UI_SURROUND_CATEGORIES_TITLE","UI_ATTACK_CATEGORIES_TITLE","UI_PHYSICAL_CATEGORIES_TITLE"};
    for(unsigned long n=0;n<3;++n) {
        if(categoryCounts[n]>0) {
            NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST",false));
            list->InitialiseOffsets(categoryCounts[n]);
            CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT",false));
            list->AddNonScrollingChild(ring);
            CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent(categoryTitles[n],false));
            list->AddNonScrollingChild(title);
            CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST",false));
            list->AddNonScrollingChild(button);
            button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST",false));
            list->AddNonScrollingChild(button);
            CategoriesLists[categoryOrder[n]]=list;
        }
    }
    count=physicalCount+attackCount+surroundCount;
    NUISystem::CComponent* root=ui->CreateComponent("UI_MAGIC_CATEGORIES",false);
    NUISystem::CViewport* viewport=static_cast<NUISystem::CViewport*>(owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if(viewport) {
        NUISystem::CComponent* description=root->FindChild("UI_ATTACK_SPELLS_DESC");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("UI_PHYSICAL_SPELLS_DESC");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("UI_SURROUND_SPELLS_DESC");
        if(description) viewport->AddFakeChild(description);
    }
    UnassignedIcons.clear();
    AbilitiesMenuEntries.clear();
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        CWideString caption,displayName;
        CTCInventoryExperience* experience;
        if(ParentThing->GetTC(static_cast<ETCInterfaceType>(22),&experience)) {
            displayName=experience->GetExperienceAbilityName(*it);
            caption=displayName;
            caption+=misc->SpaceSeparator;
            caption+=GFIntToWideString(Abilities[*it].first+1);
            displayName+=CWideString(levelPrefix)+bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL")+CWideString(spaceWide);
            displayName+=GFIntToWideString(Abilities[*it].first+1);
        }
        CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        AbilitiesMenuEntries[*it]=entry.Get();
        bool isAssigned=false;
        for(std::vector<EHeroAbility>::iterator a=AssignedAbilities.begin();a!=AssignedAbilities.end();++a)
            if(*a==*it) { isAssigned=true;break; }
        if(!isAssigned) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_UNASSIGNED_SPELL_ICON",false));
            entry->AddChild(icon);
            UnassignedIcons[*it]=icon.Get();
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        entry->AddChild(palette);
        menuEntry->AddUIActionParam(new NUISystem::CActionParamSLONG(*it));
        menuEntry->AddUIActionParam(new NUISystem::CActionParamULONG(Abilities[*it].second->IconGraphicIndex));
        menuEntry->AddUIActionParam(palette.Get());
        CCountedPointer<NUISystem::CComponent> name(ui->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW",false));
        CCountedPointer<NUISystem::CText> text;
        text.SetStaticCast(name);
        text->SetValue(displayName,true,true);
        entry->AddChild(name);
        CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ICON_TRADE_TEMPLATE",false));
        if(icon->GetType()==static_cast<NUISystem::EType>(0))
            static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(icon);
        entry->AddChild(actions);
        CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW",false));
        static_cast<NUISystem::CText*>(description.Get())->SetValue(GetAbilityPDef(*it)->LevDescription[Abilities[*it].first],CWideString(emptyWide),true);
        entry->AddChild(description);
        if(viewport) viewport->AddFakeChild(description.Get());
        static_cast<NUISystem::CList*>(CategoriesLists[GetCategory(*it)])->AddScrollingChildAndSetOffsets(entry);
    }
    const char* subtitles[3]={"UI_ATTACK_SUBTITLE","UI_SURROUND_SUBTITLE","UI_PHYSICAL_SUBTITLE"};
    const unsigned long counts[3]={attackCount,surroundCount,physicalCount};
    for(long n=0;n<3;++n) {
        EUISpellCategory category=static_cast<EUISpellCategory>(n);
        std::map<EUISpellCategory,NUISystem::CComponent*>::iterator found=CategoriesLists.find(category);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* entry=GetMenuEntryFromCategory(category,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(counts[n]),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild(subtitles[n]));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(counts[n]),true,true);
        } else {
            NUISystem::CComponent* entry=GetMenuEntryFromCategory(category,root);
            entry->MakeUnavailable();
            NUISystem::CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return root;
}