// CTCInventory::ConstructItemEntry(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner, NUISystem::CCountedPointer<NUISystem::CComponent>* actionsList) -- build the weapon-menu entry for one inventory item (branch template for items with an inventory def), remember it in ItemsMenuEntries, add the "select" marker for def items, the quick-access slot icon when the item is assigned to one, wire its select / focus / view / unfocus actions, add it to the list and hang the actions list under def items
// Grade: DIFFER(1626v1629) -- ItemsMenuEntries[defIndex] = entry: retail passes &defIndex itself, reads entry before the operator[] call and loads defIndex for the next call BEFORE the store through the returned reference; with the by-value inline Set the key is copied to [ebp-0xC] and defIndex goes to edi; with a direct `map[defIndex] = local` ours stores first and swaps ebx/edi (tried: direct + named locals, Set(long,..) [best], direct key + Set for the icons, Set(const long&,..))
// Leaf: ConstructItemEntry@CTCInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058A0C7, 1629 bytes, `ret 14h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / dtor 0x004291DE, LookUpInventoryItemDef 0x005D9710
//   (`__fastcall`), the long-keyed component map operator[] 0x005949C3 (ItemsMenuEntries / AssignedIcons), CWideString
//   0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator new 0x00BFEA1A, CAction 0x0042BE50,
//   CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU noinline stand-in),
//   CAction::AddParam 0x0053FDE0, CActionParamString 0x0054DAE0; component slots 3 (+0x0C), 59 (+0xEC), 67 (+0x10C),
//   150 (+0x258), 152/153 (+0x260/+0x264).
// Shape: the landed ConstructStyleEntry 0x00582ACE model (same TU); the four quick-access arms each build their icon,
//   attach it and record it under their slot number, and VC cross-jumps the identical tails (the slot literal lands in a
//   frame word, the icon dtor is shared).
// Layout: retail CTCInventory (12-byte containers): QuickAccessItems +0x148 (PDB +0x168), ItemsMenuEntries +0x1FC
//   (PDB +0x240), AssignedIcons +0x208 (PDB +0x250). engine/CTCInventory.h is the PDB layout, so the class is modelled locally.
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
long __fastcall LookUpInventoryItemDef(long defIndex);  // 0x005D9710
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
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);
    ~CCountedPointer();
    void Reset(T* object);
    T* operator->() const { return m_pObject; }
};
struct CAction {
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
    CAction();                                          // 0x0042BE50
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);   // 0x0053FDE0
};
struct CComponent;
struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                                // slot 3 (+0x0C)
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
    virtual void AddChildAtEnd(const CCountedPointer<CComponent>& child);          // slot 59 (+0xEC)
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066();
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action);        // slot 67 (+0x10C)
    virtual void v068(); virtual void v069();
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
    virtual void SetUserValue(long value);                                         // slot 150 (+0x258)
    virtual void v151();
    virtual void AddFocusAction(const CCountedPointer<CAction>& action);           // slot 152 (+0x260)
    virtual void AddUnfocusAction(const CCountedPointer<CAction>& action);         // slot 153 (+0x264)
    unsigned char _pad04[0x18 - 0x04];
};
struct CComponent : CComponentBase, CActionParam {};
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
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);               // slot 152 (+0x260)
};
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
struct CLongComponentMap {                              // map<long, CComponent*> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    CComponent*& operator[](const long& key);           // 0x005949C3
    __forceinline void Set(long key, CComponent* value) { (*this)[key] = value; }
};
struct CLongVector {                                    // vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
struct CTCInventory {
    unsigned char     _pad000[0x148];
    CLongVector       QuickAccessItems;                 // +0x148
    unsigned char     _pad154[0x1fc - 0x154];
    CLongComponentMap ItemsMenuEntries;                 // +0x1fc
    CLongComponentMap AssignedIcons;                    // +0x208
    void ConstructItemEntry(long defIndex, long count, CList* list, CComponent* owner, CCountedPointer<CComponent>* actionsList);
};
void CTCInventory::ConstructItemEntry(long defIndex, long count, CList* list, CComponent* owner, CCountedPointer<CComponent>* actionsList)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CCountedPointer<CComponent> selectMarker;
    CCountedPointer<CComponent> entry;
    if (LookUpInventoryItemDef(defIndex) == 0)
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                 "PC_BUTTON_BASTARD"));
    else
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 0.0f,
                                                 "PC_BUTTON_BASTARD"));
    ItemsMenuEntries.Set(defIndex, entry.m_pObject);
    if (LookUpInventoryItemDef(defIndex)) {
        if (!selectMarker.m_pObject)
            selectMarker.Reset(fe->CreateComponent("UI_SELECT_FOR_MENU_ENTRY", false));
        entry->AddChildAtEnd(selectMarker);
    }
    if (QuickAccessItems._M_start[2] == defIndex) {
        CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
        entry->AddChildAtEnd(icon);
        AssignedIcons.Set(2, icon.m_pObject);
    } else if (QuickAccessItems._M_start[3] == defIndex) {
        CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
        entry->AddChildAtEnd(icon);
        AssignedIcons.Set(3, icon.m_pObject);
    } else if (QuickAccessItems._M_start[0] == defIndex) {
        CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
        entry->AddChildAtEnd(icon);
        AssignedIcons.Set(0, icon.m_pObject);
    } else if (QuickAccessItems._M_start[1] == defIndex) {
        CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
        entry->AddChildAtEnd(icon);
        AssignedIcons.Set(1, icon.m_pObject);
    }
    CComponent* comp = entry.m_pObject;
    CCountedPointer<CAction> select(new CAction);
    select->ActionType = 0x7d0;
    select->UIActionParams.push_back(new CActionParamLong(defIndex));
    select->AddParam(entry.m_pObject, select);
    CWideString model(L"UI_ITEM_MODEL_TEMPLATE_NEW");
    select->UIActionParams.push_back(new CActionParamString(model));
    entry->AddSelectedAction(select);
    CCountedPointer<CAction> focus(new CAction);
    focus->ActionType = 0x7df;
    focus->AddParam(entry.m_pObject, focus);
    comp->AddFocusAction(focus);
    CCountedPointer<CAction> view(new CAction);
    view->ActionType = 0x7d3;
    view->UIActionParams.push_back(new CActionParamLong(defIndex));
    view->AddParam(owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"), view);
    view->AddParam(entry.m_pObject, view);
    view->UIActionParams.push_back(new CActionParamLong(count));
    entry->AddSelectedAction(view);
    CCountedPointer<CAction> focusView(new CAction);
    focusView->ActionType = 0x7e2;
    focusView->AddParam(entry.m_pObject, focusView);
    comp->AddFocusAction(focusView);
    CCountedPointer<CAction> unfocus(new CAction);
    unfocus->ActionType = 0x7d6;
    unfocus->UIActionParams.push_back(new CActionParamLong(defIndex));
    unfocus->AddParam(entry.m_pObject, unfocus);
    unfocus->UIActionParams.push_back(new CActionParamLong(count));
    comp->AddUnfocusAction(unfocus);
    comp->SetUserValue(defIndex);
    list->AddEntry(entry);
    if (LookUpInventoryItemDef(defIndex))
        entry->AddChildAtEnd(*actionsList);
}
