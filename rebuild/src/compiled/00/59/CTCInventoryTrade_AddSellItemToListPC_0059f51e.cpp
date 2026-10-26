// CTCInventoryTrade::AddSellItemToListPC(NUISystem::CComponent* owner, long defIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool addToTop) -- PC twin of 0x0059ED42: build the PC sell-menu entry for one item (branch template when the shop holds a default stock item for the def, else a cleared plain entry), record it in ItemsToSell, wire its seven actions (the select and view actions also carry the owner component) and add it to the list; returns whether it is a branch entry
// Leaf: AddSellItemToListPC@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059F51E, 1504 bytes, `ret 18h`, bool in al.
//   FableWin.pdb ?AddSellItemToListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJPAVCList@3@PAVCViewport@3@_N@Z (the catalog
//   label "AddSellItemToList" is propagated from 0x0059ED42). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CTCInventoryBase::GetTradingThing 0x005BC4C4 (catalog
//   "CWorld::DrawGetEnvironment" is propagated), std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline
//   GetInterface, type 0x53 = CTCShop), CTCShop::HasDefaultStockItem(long) 0x007723D0 (catalog "_atexit" is propagated; it is
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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CPresenceBits {
    bool test(long bit);                                // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                      // +0x00
    SEntry* last;                                       // +0x04
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
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
};
class CTCShop { public:                                        // interface type 0x53
    bool HasDefaultStockItem(long defIndex);            // 0x007723D0
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
template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);
    ~CCountedPointer();
    void Reset(T* object);
    T* operator->() const { return m_pObject; }
};
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
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
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
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CCountedPointer<CComponent> entry;
    CTCShop* shop = 0;
    GetTradingThing()->GetInterface(&shop, 0x53);
    if (shop->HasDefaultStockItem(defIndex)) {
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    } else {
        isBranch = false;
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        entry->Clear();
    }
    ItemsToSell.insert(SSellRecord(entry.m_pObject, SItemAmount(defIndex, count)));
    CComponent* comp = entry.m_pObject;
    CCountedPointer<CAction> select(new CAction);
    select->ActionType = 0x7d0;
    select->UIActionParams.push_back(new CActionParamLong(defIndex));
    select->AddParam(entry.m_pObject, select);
    CWideString model(L"PC_MODEL");
    select->UIActionParams.push_back(new CActionParamString(model));
    select->AddParam(owner, select);
    entry->AddSelectedAction(select);
    CCountedPointer<CAction> focus(new CAction);
    focus->ActionType = 0x7df;
    focus->AddParam(entry.m_pObject, focus);
    comp->AddFocusAction(focus);
    CCountedPointer<CAction> view(new CAction);
    view->ActionType = 0x7d7;
    view->UIActionParams.push_back(new CActionParamLong(defIndex));
    view->AddParam(viewport, view);
    view->AddParam(entry.m_pObject, view);
    view->AddParam(owner, view);
    entry->AddSelectedAction(view);
    CCountedPointer<CAction> focusView(new CAction);
    focusView->ActionType = 0x7e3;
    focusView->AddParam(entry.m_pObject, focusView);
    comp->AddFocusAction(focusView);
    CCountedPointer<CAction> sell(new CAction);
    sell->ActionType = 0x7d8;
    sell->UIActionParams.push_back(new CActionParamLong(defIndex));
    sell->AddParam(entry.m_pObject, sell);
    entry->AddSecondaryAction(sell);
    CCountedPointer<CAction> focusSell(new CAction);
    focusSell->ActionType = 0x7e4;
    focusSell->AddParam(entry.m_pObject, focusSell);
    comp->AddFocusAction(focusSell);
    CCountedPointer<CAction> unfocus(new CAction);
    unfocus->ActionType = 0x7d6;
    unfocus->UIActionParams.push_back(new CActionParamLong(defIndex));
    unfocus->AddParam(entry.m_pObject, unfocus);
    unfocus->UIActionParams.push_back(new CActionParamLong(count));
    comp->AddUnfocusAction(unfocus);
    comp->SetUserParam(entry.m_pObject);
    if (addToTop)
        list->AddEntryAtTop(entry);
    else
        list->AddEntry(entry);
    return isBranch;
}