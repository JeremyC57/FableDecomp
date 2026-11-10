// CTCInventoryTrade::AddSellItemToListPC(NUISystem::CComponent* owner, long defIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool addToTop) -- PC twin of 0x0059ED42: build the PC sell-menu entry for one item (branch template when the shop holds a default stock item for the def, else a cleared plain entry), record it in ItemsToSell, wire its seven actions (the select and view actions also carry the owner component) and add it to the list; returns whether it is a branch entry
// Leaf: AddSellItemToListPC@CTCInventoryTrade
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059F51E, 1504 bytes, `ret 18h`, bool in al.
//   FableWin.pdb ?AddSellItemToListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJPAVCList@3@PAVCViewport@3@_N@Z (the catalog
//   label "AddSellItemToList" is propagated from 0x0059ED42). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CManager::GetInstance 0x0041E5F2, CTCInventoryBase::GetTradingThing 0x005BC4C4 (catalog
//   "CWorld::DrawGetEnvironment" is propagated), std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline
//   GetInterface, type 0x53 = CTCShop), CTCShop::IsValidStockItem(long) 0x007723D0 (catalog "_atexit" is propagated; it is
//   `FindDefaultStockItem(def) != 0`, 0x00772250), CManager::CreateMenuEntryComponent 0x00428494, CCountedPointer<CComponent>
//   Reset 0x00429C6F / dtor 0x004291DE, CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   ItemsToSell insert(const value_type&) 0x005B2872 (pair<iterator,bool> by hidden pointer), operator new 0x00BFEA1A,
//   CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU
//   noinline stand-in), CAction::AddParam 0x0053FDE0, CActionParamString 0x0054DAE0.
// Shape: the 0x0059ED42 body (this chunk) and the landed AddWantedItemToList 0x0059CB90 model (same TU): each `new CActionParamLong` is an rvalue bound to the
//   push_back const reference, so VC parks it in a dead frame word (the `this` spill, then the dead viewport / defIndex
//   argument slots); the vtable 0x012366F4 sits in ebx; the owner's CActionParam pointer stays in esi
//   from its first use to its last, after which esi takes the constant 3. The bool result is the byte [ebp-1] set before GetInstance.
// Layout: retail CTCInventoryTrade ItemsToSell +0x150 (map<CComponent*, pair<long, long> >, PDB +0x178); engine/CTCInventoryTrade.h
//   is the flattened PDB layout (16-byte containers), so the class is modelled locally (landed ctor 0x0059EC48 offsets).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
class CTCBase;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCShop { public:                                        // interface type 0x53
    bool IsValidStockItem(long defIndex) const;            // 0x007723D0
};
namespace NUISystem {
class CActionParam { public:
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
};
class CActionParamString : public CActionParam { public:
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
#include "engine/CCountedPointer.h"
namespace NUISystem {
class CAction { public:
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
    virtual void v068();
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
class CComponent : public CComponentBase, public CActionParam { public:};
class CViewport : public CComponent { public:};
class CList { public:
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
class CManager { public:
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}
using namespace NUISystem;
struct SItemAmount {                                    // pair<long, long>: def index and count
    long first;
    long second;
    SItemAmount(long a, long b) : first(a), second(b) {}
};
struct SSellRecord {                                    // pair<CComponent* const, pair<long, long> >
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
class CTCInventoryTrade { public:
    unsigned char     _pad000[0x150];
    CComponentPairMap ItemsToSell;                      // +0x150
    CThing* GetTradingThing();                          // 0x005BC4C4 (CTCInventoryBase)
    bool AddSellItemToListPC(CComponent* owner, long defIndex, long count, CList* list, CViewport* viewport, bool addToTop);
};
bool CTCInventoryTrade::AddSellItemToListPC(CComponent* owner, long defIndex, long count, CList* list, CViewport* viewport, bool addToTop)
{
    bool isBranch = true;
    CManager* fe = CManager::GetInstance();
    CCountedPointer<CComponent> entry;
    CTCShop* shop = 0;
    GetTradingThing()->GetTC(TCI_SHOP, &shop);
    if (shop->IsValidStockItem(defIndex)) {
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    } else {
        isBranch = false;
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        entry->Clear();
    }
    ItemsToSell.insert(SSellRecord(entry.Data, SItemAmount(defIndex, count)));
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
    view->ActionType = 0x7d7;
    view->UIActionParams.push_back(new CActionParamLong(defIndex));
    view->AddParam(viewport, view);
    view->AddParam(entry.Data, view);
    view->AddParam(owner, view);
    entry->AddSelectedAction(view);
    CCountedPointer<CAction> focusView(new CAction);
    focusView->ActionType = 0x7e3;
    focusView->AddParam(entry.Data, focusView);
    comp->AddFocusAction(focusView);
    CCountedPointer<CAction> sell(new CAction);
    sell->ActionType = 0x7d8;
    sell->UIActionParams.push_back(new CActionParamLong(defIndex));
    sell->AddParam(entry.Data, sell);
    entry->AddSecondaryAction(sell);
    CCountedPointer<CAction> focusSell(new CAction);
    focusSell->ActionType = 0x7e4;
    focusSell->AddParam(entry.Data, focusSell);
    comp->AddFocusAction(focusSell);
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