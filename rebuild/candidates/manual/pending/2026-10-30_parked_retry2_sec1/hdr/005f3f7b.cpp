// CTCInventoryAbilities::ConstructExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long,long> >& items, NUISystem::CComponent* owner, const CCharString& templateName, const CCharString& descriptionSymbol) -- console expressions category: add a branch entry captioned "title x n" to the list with a one-off description (shown in the owner's scrolling viewport), a subtitle and (when named) an extra template component; an empty category clears the entry and resets its right selection arrow, otherwise it gets a sub-list (back / select headers plus the template's "_FOR_LIST" header) titled "<title>: <name>", an items-actions list with the same headers and title, and ContructItemLists fills them
// Grade: DIFFER(1438v1438) -- frame slots only: retail homes the description text temporary at [ebp-0xC] (ours reuses the dead descriptionSymbol slot [ebp+0x1C]) and the optional template component at [ebp-0x3C] (sharing the later sub-list slot; ours [ebp-0x34]) (tried: base [best], extra as a temporary + named block-local text)
// Leaf: ConstructExpressionCategoryList@CTCInventoryAbilities
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F3F7B, 1438 bytes, `ret 18h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   FableWin.pdb ?ConstructExpressionCategoryList@CTCInventoryAbilities@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@...
//   (the catalog label "ConstructPCExpressionCategoryList" is propagated from the PC twin 0x005F45B5).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / dtor 0x004291DE, CTCBase::GetGameSubsystem 0x00686D60 (text bank +0x14),
//   CDataBank::GetTextBySymbol 0x009C95E0, GetXString 0x005CC772, IntToWideString 0x00405F00, CWideString copy 0x0099B720 /
//   += 0x0099B8D0 / += (const wchar*) 0x0099B940 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / != 0x0099E960 /
//   operator+(const CCharString&, const char*) 0x0099F600, NUISystem::CAction() 0x0042BED4 / copy 0x0042CD84 / its param-list
//   dtor 0x0042ABCA, CTCInventoryAbilities::ContructItemLists 0x005F077F; component slots 3 (+0x0C FindChild by value), 59 (+0xEC),
//   62 (+0xF8, a CAction by value), 83 (+0x14C SetName), 92 (+0x170 Clear), 142 (+0x238: GetText, or the viewport's SetContent),
//   144 (+0x240 SetText), 145 (+0x244 SetHeader), 152 (+0x260 list AddEntry), 156 (+0x270), 158 (+0x278). The global name
//   0x013B8C60 is CCharString("EXPRESSIONS TITLE").
// Shape: the PC twin 0x005F45B5 (this chunk) without the PC frame registration and with the console back / select list headers;
//   the literal addresses and -1 lengths are CSE'd into registers by VC; dead argument slots carry the later temporaries.
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout; no data member is touched here, so `this` is a local
//   method-only model. The console CAction is the 8-byte ActionType + param-list record (landed 0x0059DB82 model).
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator+=(const CWideString& other);        // 0x0099B8D0
    const CWideString& operator+=(const unsigned short* text);      // 0x0099B940
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CGameSubsystem {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CCharString g_ExpressionsTitleName;             // 0x013B8C60 ("EXPRESSIONS TITLE")

namespace NUISystem {
struct CActionParamList {                               // STLport list<CActionParam*> (one node pointer)
    void* _M_node;
    ~CActionParamList();                                // 0x0042ABCA
};
struct CAction {                                        // console action record, 8 bytes
    long             ActionType;
    CActionParamList UIActionParams;
    CAction();                                          // 0x0042BED4
    CAction(const CAction& other);                      // 0x0042CD84
};
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                       // slot 3 (+0x0C)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChildAtEnd(const CCountedPointer<CComponent>& child);  // slot 59 (+0xEC)
    virtual void v060(); virtual void v061();
    virtual void SetAction(CAction action);                                    // slot 62 (+0xF8)
    virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082();
    virtual void SetName(const CCharString& name);                         // slot 83 (+0x14C)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091();
    virtual void Clear();                                                  // slot 92 (+0x170)
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123();
    virtual void AddFrameMember(CComponent* member);                       // slot 124 (+0x1F0)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual const CWideString& GetText();                                  // slot 142 (+0x238)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);         // slot 144 (+0x240)
    virtual void SetHeader(const CCountedPointer<CComponent>& header);     // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);       // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                 // slot 156 (+0x270)
    virtual void v157();
    virtual void SetSelectedIndex(long index);                             // slot 158 (+0x278)
};
struct CViewport {                                      // the description viewport (its slot 142 takes the content)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual void SetContent(CComponent* content);                          // slot 142 (+0x238)
};
struct CList : CComponent {};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
struct CTCInventoryAbilities {
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60 (CTCBase)
    void ContructItemLists(CItemVector& items, CList* subList, CComponent* owner, CCountedPointer<CComponent>& actionsList);   // 0x005F077F
    void ConstructExpressionCategoryList(CWideString& title, CList* list, CItemVector& items, CComponent* owner,
                                         const CCharString& templateName, const CCharString& descriptionSymbol);
};
void CTCInventoryAbilities::ConstructExpressionCategoryList(CWideString& title, CList* list, CItemVector& items, CComponent* owner,
                                                            const CCharString& templateName, const CCharString& descriptionSymbol)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CViewport* viewport = (CViewport*)owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    CWideString caption(title);
    if (!items.empty()) {
        caption += GetXString();
        caption += IntToWideString(items.size());
    }
    CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                   0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    list->AddEntry(entry);
    CCountedPointer<CComponent> description(fe->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    CComponent* descriptionText = description.Data;
    CDataBank* bank = GetGameSubsystem()->TextBank;
    descriptionText->SetText(bank->GetTextBySymbol(descriptionSymbol), true, true);
    entry->AddChildAtEnd(description);
    viewport->SetContent(description.Data);
    CCountedPointer<CComponent> subtitle(fe->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    subtitle->SetText(caption, true, true);
    entry->AddChildAtEnd(subtitle);
    if (templateName != "") {
        CCountedPointer<CComponent> extra(fe->CreateComponent(templateName, false));
        entry->AddChildAtEnd(extra);
    }
    CCountedPointer<CComponent> subList(fe->CreateComponent("UI_SUB_LIST", false));
    CComponent* sub = subList.Data;
    CCountedPointer<CComponent> back(fe->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> select(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
    sub->SetHeader(back);
    sub->SetHeader(select);
    if (templateName != "") {
        CCountedPointer<CComponent> header(fe->CreateComponent(templateName + "_FOR_LIST", false));
        sub->SetHeader(header);
    }
    if (items.empty()) {
        entry->Clear();
        CComponent* entryObject = entry.Data;
        CComponent* arrow = entryObject->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CAction reset;
            reset.ActionType = 1;
            arrow->SetAction(reset);
        }
    } else {
        entry->AddChildAtEnd(subList);
        sub->SetItemCount(items.size());
        CCountedPointer<CComponent> titleComp(fe->CreateComponent("UI_EXPRESSIONS_TITLE", false));
        CComponent* titleObject = titleComp.Data;
        CWideString titleText(titleObject->GetText());
        titleText += L":";
        titleText += title;
        titleObject->SetText(titleText, true, true);
        titleComp->SetName(g_ExpressionsTitleName);
        sub->SetHeader(titleComp);
        CCountedPointer<CComponent> actions(CFrontEndManager::GetInstance()->CreateComponent("UI_LIST_ITEMS_ACTIONS", false));
        CComponent* actionsList = actions.Data;
        actionsList->SetSelectedIndex(0);
        CCountedPointer<CComponent> actionsBack(fe->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<CComponent> actionsSelect(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
        actionsList->SetHeader(actionsBack);
        actionsList->SetHeader(actionsSelect);
        CCountedPointer<CComponent> actionsTitle(fe->CreateComponent("UI_EXPRESSIONS_TITLE", false));
        actionsTitle->SetText(titleText, true, true);
        actionsTitle->SetName(g_ExpressionsTitleName);
        actionsList->SetHeader(actionsTitle);
        ContructItemLists(items, (CList*)sub, owner, actions);
    }
}
