// CTCInventory::GetItemsListUIComponent(NUISystem::CComponent* owner) -- console items screen: reset the menu-entry and icon maps, drop the books from the hero's items, sort every other non-expression item that is neither a hair card nor a tattoo into the trophies / potions / gifts / produce / other lists by its UI inventory category, then build the items sub-list (back / select prompts, "ITEMS TITLE" header, five rows) and fill one category row per list through ConstructItemCategoryList; returns the sub-list
// Grade: DIFFER(1382v1393) -- register/slot permutation plus jump threading: retail keeps this in edi, zero in esi and the late-saved index in ebx (fe at [ebp-0x14], this [ebp-0x10], bank/hair [ebp-0xC]); ours uses esi/ebx/edi and other slots; retail also hoists the tattoo pointer load above the hair test and threads both early exits into the tattoo release (tried: base, `case 4:` split from `default:` [kept: retail's dead `dec eax`], the def release through a `long&` count)
// Leaf: GetItemsListUIComponent@CTCInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x00590235, 1393 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>(T*) 0x00429C15 /
//   dtor 0x004291DE, the component-map clear 0x00593235, CTCInventoryBase::GetAllItems 0x005BF482, vector<pair<long,long> >
//   erase 0x00593139 / push_back 0x005947B6, free 0x00BFEA14, CTCInventoryItem::IsBook 0x005D9970 / IsExpression 0x005D9830 /
//   GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0 / GetUIInventoryCategory 0x005D96F0 (all `__fastcall`; the two
//   Is* tests were propagated as "IsAugmentation": their sub-def lookups are CCarriedReadableDef 0x005DA3C0 and
//   CExpressionSubDef 0x005DA240), CDataBank::GetTextBySymbol 0x009C95E0, CWideString() 0x0099AED0 / operator= 0x0099B7D0 /
//   ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CTCInventory::ConstructItemCategoryList 0x0058F016 (this chunk).
// Shape: the GetExpressionsListUIComponent 0x005F4C8F builder with an erase loop in front; the two by-value def pointers are
//   released (and nulled) at the end of the item scope, and categories 4 and above all go to the "other" list.
// Layout: retail CTCInventory ItemsMenuEntries / AssignedIcons +0x1FC / +0x208; engine/CTCInventory.h is the Ego_r.pdb
//   layout, so the class is modelled locally.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
extern CCharString g_ItemsTitleName;                   // 0x013B8B4C ("ITEMS TITLE")
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x00429C15
    ~CCountedPointer();                                 // 0x004291DE
    T* operator->() const { return m_pObject; }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefBasePointer {              // counted def pointer returned by value
    T* Object;
    __forceinline ~CDefBasePointer()
    {
        if (Object) {
            T* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
};
struct CHairCardDef : CDefBase { };
struct CTattooDef : CDefBase { };
bool __fastcall IsExpression(long defIndex);            // 0x005D9830 (CTCInventoryItem, CExpressionSubDef test)
bool __fastcall IsBook(long defIndex);                  // 0x005D9970 (CTCInventoryItem, CCarriedReadableDef test)
long __fastcall GetUIInventoryCategory(long defIndex);  // 0x005D96F0 (CTCInventoryItem, def +0x6C, 4 without a def)
CDefBasePointer<CHairCardDef> __fastcall GetHairCardDef(long defIndex);      // 0x005D9A10
CDefBasePointer<CTattooDef> __fastcall GetTattooCardDef(long defIndex);      // 0x005D9AA0
namespace NUISystem {
struct CComponent {
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
    virtual void v080(); virtual void v081(); virtual void v082();
    virtual void SetName(const CCharString& name);                         // slot 83 (+0x14C)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144();
    virtual void SetHeader(const CCountedPointer<CComponent>& header);     // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                 // slot 156 (+0x270)
};
struct CList : CComponent {};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
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
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void push_back(const T& value);                     // 0x005947B6
    T* erase(T* position);                              // 0x00593139
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
struct CComponentMap {                                  // map<long, CComponent*> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    void clear();                                       // 0x00593235
};
struct CTCInventory {
    unsigned char _pad000[0x1fc];
    CComponentMap ItemsMenuEntries;                     // +0x1fc
    CComponentMap AssignedIcons;                        // +0x208
    void GetAllItems(CItemVector& out);                 // 0x005BF482 (CTCInventoryBase)
    void ConstructItemCategoryList(CWideString& title, CList* list, CItemVector& items, CComponent* owner,
                                   const CCharString& templateName, const CCharString& descriptionSymbol, bool checkUsable);   // 0x0058F016
    CComponent* GetItemsListUIComponent(CComponent* owner);
};
CComponent* CTCInventory::GetItemsListUIComponent(CComponent* owner)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    ItemsMenuEntries.clear();
    AssignedIcons.clear();
    CItemVector allItems;
    CItemVector trophies;
    CItemVector potions;
    CItemVector gifts;
    CItemVector produce;
    CItemVector others;
    GetAllItems(allItems);
    for (std::pair<long, long>* it = allItems.begin(); it != allItems.end(); ) {
        if (IsBook(it->first))
            it = allItems.erase(it);
        else
            ++it;
    }
    for (unsigned long i = 0; i < allItems.size(); ++i) {
        if (IsExpression(allItems[i].first))
            continue;
        CDefBasePointer<CHairCardDef> hair = GetHairCardDef(allItems[i].first);
        CDefBasePointer<CTattooDef> tattoo = GetTattooCardDef(allItems[i].first);
        if (!hair.Object && !tattoo.Object) {
            switch (GetUIInventoryCategory(allItems[i].first)) {
            case 0:  trophies.push_back(allItems[i]); break;
            case 1:  potions.push_back(allItems[i]); break;
            case 2:  gifts.push_back(allItems[i]); break;
            case 3:  produce.push_back(allItems[i]); break;
            case 4:  others.push_back(allItems[i]); break;
            default: others.push_back(allItems[i]); break;
            }
        }
    }
    CList* subList = (CList*)fe->CreateComponent("UI_SUB_LIST", false);
    CCountedPointer<CComponent> back(fe->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> select(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
    subList->SetHeader(back);
    subList->SetHeader(select);
    CCountedPointer<CComponent> title(fe->CreateComponent("UI_ITEM_TITLE", false));
    title->SetName(g_ItemsTitleName);
    subList->SetHeader(title);
    subList->SetItemCount(5);
    CDataBank* bank = g_pMainGameComponent->TextBank;
    CWideString caption;
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
    ConstructItemCategoryList(caption, subList, trophies, owner, "UI_RING_PIC_TROPHIES", "TEXT_GUI_MENUGUIDE_ITEMS_TROPHIES", false);
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
    ConstructItemCategoryList(caption, subList, potions, owner, "UI_RING_PIC_POTIONS", "TEXT_GUI_MENUGUIDE_ITEMS_MAGICPOTIONS", false);
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
    ConstructItemCategoryList(caption, subList, gifts, owner, "UI_RING_PIC_GIFTS", "TEXT_GUI_MENUGUIDE_ITEMS_GIFTS", true);
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
    ConstructItemCategoryList(caption, subList, produce, owner, "UI_RING_PIC_PRODUCE", "TEXT_GUI_MENUGUIDE_ITEMS_PRODUCE", false);
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_OTHER");
    ConstructItemCategoryList(caption, subList, others, owner, "UI_RING_PIC_OTHERS", "TEXT_GUI_MENUGUIDE_ITEMS_OTHERS", false);
    return subList;
}
