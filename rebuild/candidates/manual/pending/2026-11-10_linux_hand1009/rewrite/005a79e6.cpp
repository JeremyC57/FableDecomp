// CTCInventoryTrade::ConstructBuyActionsListPC(NUISystem::CComponent* parent, long defIndex, long count, bool addAsChild) -- the PC twin of ConstructBuyActionsList: build the buy-actions sub-list for one shop item (PC_LIST_BUY_ACTIONS, PC menu-entry template): the buyable quantity is the stock count capped by the room in the hero's inventory and stepped down until affordable; a "buy" entry plus "buy 10", "buy 50" and "buy all (n)" entries as that quantity allows (never in a barber's, tattooist's or title shop); when the item is affordable, fits (or is a special case) and in stock, remember the list in ItemBuyLists[parent], attach it to the parent and return true, else return false
// Leaf: ?ConstructBuyActionsListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A79E6, 1935 bytes, thiscall `ret 10h`, bool in al (bl). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructBuyActionsListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z (the catalog's
//   "ConstructBuyActionsList" is propagated from the console twin 0x005A3E47): its strings are the PC ones ("PC_LIST_BUY_ACTIONS",
//   "PC_MENU_ENTRY_TEMPLATE", the 0.0f / 1.0f entry scales of every landed PC row), it follows ConstructSellActionsListPC 0x005A731F as
//   FableWin.pdb orders the pair, and its only caller is 0x00425B33.
// Shape: the console twin ConstructBuyActionsList 0x005A3E47 (matched in this chunk) with the PC strings and scales, as the
//   landed ConstructSellActionsListPC 0x005A731F is ConstructSellActionsList 0x005A45F6; i.e. the landed 0x005A45F6 source (same TU): the visible map operator[] stand-in (its lever), the
//   cross-jumped SetItemCount chain, the shared literal regs; plus the hero's inventory component through the const PeekTC
//   (LowerBound const 0x004365B0), the capped and stepped-down quantity, the shop-type tests and the wchar operator+ (0x0099BF30).
// Callees: as 0x005A45F6, plus NInventory::CTCInventoryBase::GetAmountOfItemsContainable 0x005BF30D / IsSpaceInInventoryForItem
//   0x005BF2ED (Ego_r.pdb QBEJJ@Z / QBE_NJ@Z, a pair 0x26 apart), CTCInventoryItem::GetInventoryType 0x005D99D0, CTCShop::
//   IsBarberShop 0x00772490 / IsTattooShop 0x00772510 / IsTitleShop 0x00772590, IsItemAffordable 0x0059DA10, IsSpecialCase
//   0x0059C983, CPlayer::PeekPControlledCreature 0x00487DD0, CWideString operator+(const CWideString&, const wchar_t*) 0x0099BF30,
//   ItemBuyLists operator[] 0x005B3167.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
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
    CWideString(const CWideString& other);              // 0x0099B5F0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall GFIntToWideString(long value);                               // 0x00405F00 (Ego_r.pdb)
CWideString __fastcall operator+(const CWideString& left, const CWideString& right); // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
class CTCBase;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                            // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            CInterfaceMap::value_type* found = (it == map->end() || wanted < it->first) ? map->end() : it;
            *out = (T*)found->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool PeekTC(ETCInterfaceType id, T** ptc) const
    {
        if (m_present.test(id)) {
            ETCInterfaceType type = id;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ptc = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CTCShop { public:                                        // interface type 0x53
    bool IsBarberShop() const;                          // 0x00772490
    bool IsTattooShop() const;                          // 0x00772510
    bool IsTitleShop() const;                           // 0x00772590
};
namespace NInventory {
class CTCInventoryBase { public:
    bool IsSpaceInInventoryForItem(long itemDefIndex) const;     // 0x005BF2ED
    long GetAmountOfItemsContainable(long itemDefIndex) const;   // 0x005BF30D
};
}
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };  // 0x00487DD0
class CPlayerManager { public: CPlayer* GetMainPlayer() const; };   // 0x00449960
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x14];
    CDataBank*      TextBank;                           // +0x14
    unsigned char   _pad18[0x1c - 0x18];
    CPlayerManager* PlayerManager;                      // +0x1c
};
class CTCInventoryItem { public:
    static ETCInterfaceType __fastcall GetInventoryType(long defIndex);   // 0x005D99D0 (FableWin.pdb ?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z)
};
namespace NUISystem {
class CActionParam { public:
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
class CActionParamULONG : public CActionParam { public:               // vtable 0x0122F844
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

namespace NUISystem {
class CAction { public:
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
    CAction();                                          // 0x0042BE50
};
class CComponent;                                       // the counted pointers hold NUISystem::CComponent (Ego_r.pdb)
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
    virtual void AddChildAtEnd(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void AddChild(const CCountedPointer<CComponent>& child);        // slot 60 (+0xF0)
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
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);        // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                      // slot 156 (+0x270)
    virtual void v157();
    virtual void SetSelectedIndex(long index);                                  // slot 158 (+0x278)
    unsigned char _pad04[0x18 - 0x04];
};
class CComponent : public CComponentBase, public CActionParam { public:};
class CManager {
public:
    static CManager* __fastcall GetInstance();  // 0x0041E5F2
 public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
};
}
using namespace NUISystem;
struct CComponentMap {                                  // map<CComponent*, CComponent*> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    CComponentBase*& operator[](CComponent* const& key);   // 0x005B3167 (same-TU noinline stand-in body below)
};
CComponentBase** FindOrInsertBuyList_5b3167(CComponentMap* map, CComponent* key);
#pragma auto_inline(off)
__declspec(noinline) CComponentBase*& CComponentMap::operator[](CComponent* const& key) { return *FindOrInsertBuyList_5b3167(this, key); }
#pragma auto_inline(on)
class CTCInventoryTrade { public:
    unsigned char _pad000[0x200];
    CComponentMap ItemBuyLists;                         // +0x200
    CThing* GetTradingThing();                          // 0x005BC4C4 (CTCInventoryBase)
    bool IsItemAffordable(long itemType, long count);   // 0x0059DA10
    bool IsSpecialCase(long itemDefIndex);              // 0x0059C983
    bool ConstructBuyActionsListPC(CComponent* parent, long defIndex, long count, bool addAsChild);
};
bool CTCInventoryTrade::ConstructBuyActionsListPC(CComponent* parent, long defIndex, long count, bool addAsChild)
{
    CManager* fe = CManager::GetInstance();
    CCountedPointer<CComponent> actionsList(CManager::GetInstance()->CreateComponent("PC_LIST_BUY_ACTIONS", false));
    CComponentBase* list = actionsList.Data;
    const NInventory::CTCInventoryBase* inventory = 0;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer()->PeekPControlledCreature();
    CTCShop* shop = 0;
    hero->PeekTC(CTCInventoryItem::GetInventoryType(defIndex), &inventory);
    GetTradingThing()->GetInterface(&shop, 0x53);
    CDataBank* bank = CMainGameComponent::Get()->TextBank;
    CWideString buyText = bank->GetTextBySymbol("TEXT_GUI_MENU_BUY");
    CCountedPointer<CComponent> buyEntry(fe->CreateMenuEntryComponent(buyText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                         0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<CAction> buyAction(new CAction);
    buyAction->ActionType = 0x24;
    buyEntry->AddPressedAction(buyAction);
    long maxBuyable = inventory->GetAmountOfItemsContainable(defIndex);
    long quantity = count;
    if (maxBuyable <= count)
        quantity = maxBuyable;
    for (; quantity >= 1; --quantity)
        if (IsItemAffordable(defIndex, quantity))
            break;
    if (quantity > 50)
        list->SetItemCount(4);
    else if (quantity > 10)
        list->SetItemCount(3);
    else if (quantity > 1)
        list->SetItemCount(2);
    else
        list->SetItemCount(1);
    list->SetSelectedIndex(0);
    list->AddEntry(buyEntry);
    if (quantity > 10 && !shop->IsBarberShop() && !shop->IsTattooShop() && !shop->IsTitleShop()) {
        CWideString label = bank->GetTextBySymbol("TEXT_GUI_MENU_BUY") + L" " + GFIntToWideString(10);
        CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(label, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x51;
        action->UIActionParams.push_back(new CActionParamULONG(10));
        entry->AddPressedAction(action);
        list->AddEntry(entry);
    }
    if (quantity > 50 && !shop->IsBarberShop() && !shop->IsTattooShop() && !shop->IsTitleShop()) {
        CWideString label = bank->GetTextBySymbol("TEXT_GUI_MENU_BUY") + L" " + GFIntToWideString(50);
        CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(label, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x51;
        action->UIActionParams.push_back(new CActionParamULONG(50));
        entry->AddPressedAction(action);
        list->AddEntry(entry);
    }
    if (quantity > 1 && !shop->IsBarberShop() && !shop->IsTattooShop() && !shop->IsTitleShop()) {
        CWideString label = bank->GetTextBySymbol("TEXT_GUI_MENU_BUY_ALL") + L" (" + GFIntToWideString(quantity) + L")";
        CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(label, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0x51;
        action->UIActionParams.push_back(new CActionParamULONG(quantity));
        entry->AddPressedAction(action);
        list->AddEntry(entry);
    }
    if (IsItemAffordable(defIndex, 1) && (inventory->IsSpaceInInventoryForItem(defIndex) || IsSpecialCase(defIndex)) && count != 0) {
        CComponentBase* listObject = actionsList.Data;
        ItemBuyLists[parent] = listObject;
        if (addAsChild)
            parent->AddChild(actionsList);
        else
            parent->AddChildAtEnd(actionsList);
        return true;
    }
    return false;
}
