// CTCInventoryTrade::AddBuyItemToListPC(NUISystem::CComponent* owner, long defIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool addToTop) -- PC twin of AddBuyItemToList 0x0059FEE3: build the PC buy-menu entry (branch template when affordable, accepted by the hero's tab or a special case, in stock and not already owned; else a cleared plain entry that, for a special case, gets a type-0x12E action), record it in ItemsToBuy, wire its seven actions (select and view also carry the owner) and add it to the list; returns whether it is a branch entry
// Grade: DIFFER(1681v1662) -- same register-allocation residue as 0x0059FEE3: retail spills `this` and keeps the hero tab in ebx; ours keeps `this` in ebx, the tab round-trips through memory (tried: base, tab lookup in an inline helper)
// Leaf: AddBuyItemToListPC@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A6CA1, 1662 bytes, `ret 18h`, bool in al. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CPlayerManager::GetMainPlayer 0x00449960 (g_pMainGameComponent 0x013B86A0
//   +0x1C), CPlayer::GetControlledCreature 0x00487DD0, GetInventoryCategory(long) 0x005D99D0 (`__fastcall`), std::bitset::test
//   0x00410DE0 + const CVectorMap::LowerBound 0x004365B0 (the const inline GetInterface keyed by the category), the tab's
//   IsItemAllowed(long) 0x005BF2ED (catalog "CTCInventoryBase::GetItemDescription" is propagated; bool in al),
//   CTCInventoryTrade::AlreadyHaveSpecial 0x0059FC17 / IsItemAffordable 0x0059DA10 / IsSpecialCase 0x0059C983,
//   CManager::CreateMenuEntryComponent 0x00428494, CCountedPointer<CComponent> Reset 0x00429C6F / dtor 0x004291DE,
//   CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, ItemsToBuy insert 0x005B2872,
//   operator new 0x00BFEA1A, CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back
//   0x0042B646 (same-TU noinline stand-in), CAction::AddParam 0x0053FDE0, CActionParamString 0x0054DAE0.
// Shape: the AddSellItemToList 0x0059ED42 body (this chunk; landed AddWantedItemToList 0x0059CB90 model) with the buy
//   condition in front: `alreadyHave` is a named bool (frame byte [ebp-2]) read last in the && chain, so its test sits
//   after `count > 0` with the shared `push -1` of both CCharString constructions hoisted above the branch.
// Layout: retail CTCInventoryTrade ItemsToBuy +0x144 (map<CComponent*, pair<long, long> >, PDB +0x168).
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CPresenceBits {
    bool test(long bit) const;                          // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                      // +0x00
    SEntry* last;                                       // +0x04
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInventoryTab {                                // the hero's inventory component for an item category
    bool IsItemAllowed(long defIndex);                  // 0x005BF2ED
};
struct CPlayer { CThing* GetControlledCreature(); };  // 0x00487DD0
struct CPlayerManager { CPlayer* GetMainPlayer() const; };   // 0x00449960
struct CMainGameComponent { unsigned char _pad[0x1c]; CPlayerManager* PlayerManager; };   // +0x1c
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
long __fastcall GetInventoryCategory(long defIndex);  // 0x005D99D0
namespace NUISystem {
struct CActionParam {
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
};
struct CActionParamString : CActionParam {
    CWideString Value;                                  // +0x08
    CActionParamString(CWideString& text);              // 0x0054DAE0
};
struct CActionParamList {
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
};
}
void InsertParamNode(void* node, NUISystem::CActionParam* value);
#pragma auto_inline(off)
__declspec(noinline) void NUISystem::CActionParamList::push_back(NUISystem::CActionParam* const& value)
{
    InsertParamNode(_M_node, value);
}
#pragma auto_inline(on)

namespace NUISystem {
struct CAction {
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
    CAction();                                          // 0x0042BE50
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);   // 0x0053FDE0
};
struct CComponentBase {
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
    virtual void v065(); virtual void v066();
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action);   // slot 67 (+0x10C)
    virtual void AddSpecialAction(const CCountedPointer<CAction>& action);    // slot 68 (+0x110)
    virtual void AddSecondaryAction(const CCountedPointer<CAction>& action);  // slot 69 (+0x114)
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091();
    virtual void Clear();                                                      // slot 92 (+0x170)
    virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150();
    virtual void SetUserParam(CActionParam* param);                            // slot 151 (+0x25C)
    virtual void AddFocusAction(const CCountedPointer<CAction>& action);      // slot 152 (+0x260)
    virtual void AddUnfocusAction(const CCountedPointer<CAction>& action);    // slot 153 (+0x264)
    unsigned char _pad04[0x18 - 0x04];
};
struct CComponent : CComponentBase, CActionParam {};
struct CViewport : CComponent {};
struct CList {
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
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);         // slot 152 (+0x260)
    virtual void AddEntryAtTop(const CCountedPointer<CComponent>& entry);    // slot 153 (+0x264)
};
struct CManager {
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;
struct SItemAmount {                                    // pair<long, long>: def index and count
    long first;
    long second;
    SItemAmount(long a, long b) : first(a), second(b) {}
};
struct SSellRecord {                                    // pair<CComponent* const, pair<long, long> > (ItemsToBuy/ItemsToSell value)
    CComponent* first;
    SItemAmount second;
    SSellRecord(CComponent* a, const SItemAmount& b) : first(a), second(b) {}
};
struct CComponentPairMap {                              // map<CComponent*, pair<long, long> > (12 bytes)
    struct iterator { void* _M_node; };
    struct insert_result {                              // pair<iterator, bool>
        iterator first;
        bool     second;
        insert_result(const iterator& a, bool b) : first(a), second(b) {}
    };
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    insert_result insert(const SSellRecord& value);     // 0x005B2872
};
struct CTCInventoryTrade {
    unsigned char     _pad000[0x144];
    CComponentPairMap ItemsToBuy;                       // +0x144
    bool IsSpecialCase(long defIndex);                  // 0x0059C983
    bool IsItemAffordable(long defIndex, long count);   // 0x0059DA10
    bool AlreadyHaveSpecial(long defIndex);             // 0x0059FC17
    bool AddBuyItemToListPC(CComponent* owner, long defIndex, long count, CList* list, CViewport* viewport, bool addToTop);
};
bool CTCInventoryTrade::AddBuyItemToListPC(CComponent* owner, long defIndex, long count, CList* list, CViewport* viewport, bool addToTop)
{
    bool isBranch = true;
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CCountedPointer<CComponent> entry;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetControlledCreature();
    long category = GetInventoryCategory(defIndex);
    CTCInventoryTab* tab = 0;
    static_cast<const CThing*>(hero)->GetInterface(&tab, category);
    bool alreadyHave = AlreadyHaveSpecial(defIndex);
    if (IsItemAffordable(defIndex, 1) && (tab->IsItemAllowed(defIndex) || IsSpecialCase(defIndex)) && count > 0 && !alreadyHave) {
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    } else {
        isBranch = false;
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        entry->Clear();
        if (IsSpecialCase(defIndex)) {
            CCountedPointer<CAction> special(new CAction);
            special->ActionType = 0x12e;
            entry->AddSpecialAction(special);
        }
    }
    ItemsToBuy.insert(SSellRecord(entry.Data, SItemAmount(defIndex, count)));
    CComponent* comp = entry.Data;
    CCountedPointer<CAction> select(new CAction);
    select->ActionType = 0x7d0;
    select->UIActionParams.push_back(new CActionParamLong(defIndex));
    select->AddParam(entry.Data, select);
    CWideString model(L"PC_MODEL");
    select->UIActionParams.push_back(new CActionParamString(model));
    select->AddParam(owner, select);
    entry->AddSelectedAction(select);
    CCountedPointer<CAction> focus(new CAction);
    focus->ActionType = 0x7df;
    focus->AddParam(entry.Data, focus);
    comp->AddFocusAction(focus);
    CCountedPointer<CAction> view(new CAction);
    view->ActionType = 0x7d9;
    view->UIActionParams.push_back(new CActionParamLong(defIndex));
    view->AddParam(viewport, view);
    view->AddParam(entry.Data, view);
    view->AddParam(owner, view);
    entry->AddSelectedAction(view);
    CCountedPointer<CAction> focusView(new CAction);
    focusView->ActionType = 0x7e5;
    focusView->AddParam(entry.Data, focusView);
    comp->AddFocusAction(focusView);
    CCountedPointer<CAction> buy(new CAction);
    buy->ActionType = 0x7da;
    buy->UIActionParams.push_back(new CActionParamLong(defIndex));
    buy->AddParam(entry.Data, buy);
    entry->AddSecondaryAction(buy);
    CCountedPointer<CAction> focusBuy(new CAction);
    focusBuy->ActionType = 0x7e6;
    focusBuy->AddParam(entry.Data, focusBuy);
    comp->AddFocusAction(focusBuy);
    CCountedPointer<CAction> unfocus(new CAction);
    unfocus->ActionType = 0x7d6;
    unfocus->UIActionParams.push_back(new CActionParamLong(defIndex));
    unfocus->AddParam(entry.Data, unfocus);
    unfocus->UIActionParams.push_back(new CActionParamLong(count));
    comp->AddUnfocusAction(unfocus);
    comp->SetUserParam(entry.Data);
    if (addToTop)
        list->AddEntryAtTop(entry);
    else
        list->AddEntry(entry);
    return isBranch;
}
