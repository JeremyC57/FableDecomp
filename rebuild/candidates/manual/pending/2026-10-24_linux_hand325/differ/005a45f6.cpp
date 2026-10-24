// CTCInventoryTrade::ConstructSellActionsList(NUISystem::CComponent* parent, long defIndex, unsigned long count, bool addAsChild) -- build the sell-actions sub-list for one item: a "sell" entry plus "sell 10", "sell 50" and "sell all (n)" entries as the count allows; when the count is non-zero and the shop holds a default stock item for the def, remember the list in ItemSellLists[parent], attach it to the parent and return true, else return false
// Grade: DIFFER(same length) -- scheduling only: after ItemSellLists[parent] retail loads `parent` and `&actionsList` before the store through the returned reference (mov ecx,[ebp+8] ; lea edx ; mov [eax],esi), ours stores first (the key escapes by address, so VC keeps the load after the store)
// Leaf: ConstructSellActionsList@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A45F6, 1767 bytes, `ret 10h`, bool in al (bl). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / dtor 0x004291DE, CPlayerManager::GetMainPlayer 0x00449960 (g_pMainGameComponent
//   0x013B86A0 +0x1C) + CPlayer::GetControlledCreature 0x00487DD0, CTCInventoryBase::GetTradingThing 0x005BC4C4, std::bitset::test
//   0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (inline GetInterface, type 0x53 = CTCShop), GetInventoryCategory 0x005D99D0,
//   CDataBank::GetTextBySymbol 0x009C95E0 (text bank at g_pMainGameComponent +0x14), IntToWideString 0x00405F00 (`__fastcall`),
//   CWideString operator+ 0x0099BE70 (`__fastcall`), CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   operator new 0x00BFEA1A, CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646
//   (same-TU noinline stand-in), CTCShop::HasDefaultStockItem 0x007723D0, ItemSellLists operator[] 0x005B3167.
// Shape: the AddSellItemToList 0x0059ED42 TU conventions; the hero's tab presence test is a bare statement (its result is
//   dropped, so only the bitset call survives); the shop pointer is zero-initialised in memory and the lookup key reuses its
//   slot; the three literal addresses of the repeated entry construction are CSE'd into ebx/edi/esi by VC; the item count is
//   an if/else chain of four SetItemCount calls that VC cross-jumps into one call site (a ?: chain becomes seta/inc);
//   the list object is read into a local before the map operator[] call; the two returns share the destructor tail through bl.
// Layout: retail CTCInventoryTrade ItemSellLists +0x20C (map<CComponent*, CComponent*>, PDB +0x258); CActionParamULONG
//   (vtable 0x0122F844, type 5) 12 bytes.
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
    CWideString(const CWideString& other);              // 0x0099B5F0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);                                 // 0x00405F00
CWideString __fastcall operator+(const CWideString& left, const CWideString& right); // 0x0099BE70
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
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
};
struct CTCShop {                                        // interface type 0x53
    bool HasDefaultStockItem(long defIndex);            // 0x007723D0
};
struct CPlayer { CThing* GetControlledCreature(); };  // 0x00487DD0
struct CPlayerManager { CPlayer* GetMainPlayer() const; };   // 0x00449960
struct CMainGameComponent {
    unsigned char   _pad00[0x14];
    CDataBank*      TextBank;                           // +0x14
    unsigned char   _pad18[0x1c - 0x18];
    CPlayerManager* PlayerManager;                      // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
long __fastcall GetInventoryCategory(long defIndex);  // 0x005D99D0
namespace NUISystem {
struct CActionParam {
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamULONG : CActionParam {               // vtable 0x0122F844
    unsigned long Value;                                // +0x08
    CActionParamULONG(unsigned long value) : CActionParam(5), Value(value) {}
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
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);
    ~CCountedPointer();
    void Reset(T* object);
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CAction {
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
    CAction();                                          // 0x0042BE50
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
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChildAtEnd(const CCountedPointer<CComponentBase>& child);   // slot 59 (+0xEC)
    virtual void AddChild(const CCountedPointer<CComponentBase>& child);        // slot 60 (+0xF0)
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void AddPressedAction(const CCountedPointer<CAction>& action);     // slot 66 (+0x108)
    virtual void v067(); virtual void v068(); virtual void v069();
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
    virtual void AddEntry(const CCountedPointer<CComponentBase>& entry);        // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                      // slot 156 (+0x270)
    virtual void v157();
    virtual void SetSelectedIndex(long index);                                  // slot 158 (+0x278)
    unsigned char _pad04[0x18 - 0x04];
};
struct CComponent : CComponentBase, CActionParam {};
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
struct CComponentMap {                                  // map<CComponent*, CComponent*> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    CComponentBase*& operator[](CComponent* const& key);   // 0x005B3167
};
struct CTCInventoryTrade {
    unsigned char _pad000[0x20c];
    CComponentMap ItemSellLists;                        // +0x20c
    CThing* GetTradingThing();                          // 0x005BC4C4 (CTCInventoryBase)
    bool ConstructSellActionsList(CComponent* parent, long defIndex, unsigned long count, bool addAsChild);
};
bool CTCInventoryTrade::ConstructSellActionsList(CComponent* parent, long defIndex, unsigned long count, bool addAsChild)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CCountedPointer<CComponentBase> actionsList(CFrontEndManager::GetInstance()->CreateComponent("UI_LIST_SELL_ACTIONS", false));
    CComponentBase* list = actionsList.m_pObject;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetControlledCreature();
    CDataBank* bank = g_pMainGameComponent->TextBank;
    CTCShop* shop = 0;
    GetTradingThing()->GetInterface(&shop, 0x53);
    hero->m_present.test(GetInventoryCategory(defIndex));
    CWideString sellText = bank->GetTextBySymbol("TEXT_GUI_MENU_SELL");
    CCountedPointer<CComponentBase> sellEntry(fe->CreateMenuEntryComponent(sellText, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<CAction> sellAction(new CAction);
    sellAction->ActionType = 0x25;
    sellEntry->AddPressedAction(sellAction);
    if (count > 50)
        list->SetItemCount(4);
    else if (count > 10)
        list->SetItemCount(3);
    else if (count > 1)
        list->SetItemCount(2);
    else
        list->SetItemCount(1);
    list->SetSelectedIndex(0);
    list->AddEntry(sellEntry);
    if (count > 10) {
        CWideString label = bank->GetTextBySymbol("TEXT_GUI_MENU_SELL") + CWideString(L" ") + IntToWideString(10);
        CCountedPointer<CComponentBase> entry(fe->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x52;
        action->UIActionParams.push_back(new CActionParamULONG(10));
        entry->AddPressedAction(action);
        list->AddEntry(entry);
    }
    if (count > 50) {
        CWideString label = bank->GetTextBySymbol("TEXT_GUI_MENU_SELL") + CWideString(L" ") + IntToWideString(50);
        CCountedPointer<CComponentBase> entry(fe->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x52;
        action->UIActionParams.push_back(new CActionParamULONG(50));
        entry->AddPressedAction(action);
        list->AddEntry(entry);
    }
    if (count > 1) {
        CWideString label = bank->GetTextBySymbol("TEXT_GUI_MENU_SELL_ALL") + CWideString(L" (") + IntToWideString(count) + CWideString(L")");
        CCountedPointer<CComponentBase> entry(fe->CreateMenuEntryComponent(label, 25.0f, 0.66666f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x52;
        action->UIActionParams.push_back(new CActionParamULONG(count));
        entry->AddPressedAction(action);
        list->AddEntry(entry);
    }
    if (count != 0 && shop->HasDefaultStockItem(defIndex)) {
        CComponentBase* listObject = actionsList.m_pObject;
        ItemSellLists[parent] = listObject;
        if (addAsChild)
            parent->AddChild(actionsList);
        else
            parent->AddChildAtEnd(actionsList);
        return true;
    }
    return false;
}
