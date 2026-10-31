// Grade: DIFFER(1239v1248) 103 of 398 lines (was 196): GetInterface passes the address of its (forceinline) wanted parameter to LowerBound, no separate key local -- frame now 0xAC as retail and the key shares a slot; residue: retail keeps the item offset in edi and the inventory in ebx (ours swaps), and the key compare reads the slot (cmp ecx,[eax]) where retail compares the constant 0x11; a block-scoped key local keeps the constant compare (115 lines); if-assign select inert
// CTCInventoryAbilities::ContructItemLists(const std::vector<std::pair<long,long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner, const CCountedPointer<NUISystem::CComponent>& actionsList) -- console item rows: for every item build a weapon-menu branch entry captioned with the item name, remember it in the inventory's ItemsMenuEntries, add the assigned left / right / up / down quick-slot icon when the item sits in that quick slot (recording it in AssignedIcons), add the description text (shown in the owner's scrolling viewport), the name text and the item graphic icon, store the def index as the entry's user value, append the entry to the list and give it the actions list
// Leaf: ContructItemLists@CTCInventoryAbilities
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F077F, 1248 bytes, `ret 10h`. Biased frame (lea ebp,[esp-0x68]): optimize("s",on).
//   FableWin.pdb ContructItemLists (sic) on CTCInventoryAbilities.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / dtor 0x004291DE, CCountedPointer<CText> ShareData 0x00429460 / dtor
//   0x0042925C, CCountedPointer<CMenuEntry> ShareData 0x0048524C / dtor 0x004850F9, std::bitset::test 0x00410DE0 +
//   CVectorMap::LowerBound 0x0040F020 (inline GetInterface, type 0x11 = CTCInventory), map<long,CComponent*>::operator[]
//   0x005949C3, CTCInventoryItem::GetDescription 0x005D95C0 (the item name) / GetItemDefDescription 0x005D9630 /
//   GetEngineGraphic 0x005D96A0 (all `__fastcall`, result by hidden pointer), CWideString ~ 0x0099B510, CCharString
//   0x0099EBF0 / ~ 0x0099EAE0; component slots 3 FindChild, 59 AddChildAtEnd, 65/145 (icon graphic bank get/set),
//   142 SetContent, 144 SetText, 150 SetUserValue, 152 AddEntry.
// Shape: the quick-slot icons are an else-if chain whose four icon pointers share one destructor call; the two CText
//   pointers and the CMenuEntry pointer are default-constructed and assigned from the component pointers (ShareData).
// Layout: retail CTCInventoryAbilities ParentThing +0x04; retail CTCInventory QuickAccessItems (vector<long>, start pointer)
//   +0x148, ItemsMenuEntries / AssignedIcons +0x1FC / +0x208; CEngineGraphic is the Ego_r.pdb 14-byte struct.
//   engine/CTCInventoryAbilities.h is the Ego_r.pdb layout, so the classes are modelled locally.
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
#pragma pack(push, 2)
struct CEngineGraphic {
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0C
    unsigned char Type;                                 // +0x0D
    CEngineGraphic();                                   // user constructor: the struct is returned through ecx under __fastcall
};
#pragma pack(pop)
CWideString __fastcall GetDescription(long defIndex);          // 0x005D95C0 (CTCInventoryItem, the item name)
CWideString __fastcall GetItemDefDescription(long defIndex);   // 0x005D9630
CEngineGraphic __fastcall GetEngineGraphic(long defIndex);     // 0x005D96A0
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              m_pObject;
    CCPPointerInfo* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);
    ~CCountedPointer();
    void ShareData(T* object, CCPPointerInfo* count);
    template<class U> __forceinline CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData((T*)other.m_pObject, other.m_pCount);
        return *this;
    }
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CActionParam {
    virtual void v00();
    long Type;                                          // +0x04
};
struct CComponent;
struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                       // slot 3 (+0x0C)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChildAtEnd(const CCountedPointer<CComponent>& child);    // slot 59 (+0xEC)
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
    virtual void v139(); virtual void v140(); virtual void v141();
    virtual void SetContent(CComponent* content);                          // slot 142 (+0x238, viewport)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);         // slot 144 (+0x240, CText)
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void SetUserValue(long value);                                 // slot 150 (+0x258, CMenuEntry)
    virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);       // slot 152 (+0x260, CList)
    unsigned char _pad04[0x18 - 0x04];
};
struct CComponent : CComponentBase, CActionParam {};
struct CText : CComponent {};
struct CMenuEntry : CComponent {};
struct CList : CComponent {};
struct CIcon {                                          // the item graphic component (its own view of the vtable)
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
    virtual long GetGraphicBank();                                         // slot 65 (+0x104)
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetGraphicBank(unsigned long bank);                       // slot 145 (+0x244)
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
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long n) const { return _M_start[n]; }
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
struct CComponentMap {                                  // map<long, CComponent*> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    CComponent*& operator[](const long& key);          // 0x005949C3
    __forceinline void Set(const long& key, CComponent* value) { (*this)[key] = value; }
};
struct CPresenceBits { bool test(long kind); };        // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&wanted);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CTCInventory {                                   // interface type 0x11
    unsigned char _pad000[0x148];
    long*         QuickAccessItems;                     // +0x148 (vector<long> start: up, down, left, right)
    unsigned char _pad14c[0x1fc - 0x14c];
    CComponentMap ItemsMenuEntries;                     // +0x1fc
    CComponentMap AssignedIcons;                        // +0x208
};
struct CTCInventoryAbilities {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    void ContructItemLists(const CItemVector& items, CList* list, CComponent* owner, const CCountedPointer<CComponent>& actionsList);
};
void CTCInventoryAbilities::ContructItemLists(const CItemVector& items, CList* list, CComponent* owner,
                                              const CCountedPointer<CComponent>& actionsList)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    for (unsigned long i = 0; i < items.size(); ++i) {
        CWideString name = GetDescription(items[i].first);
        CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(name, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                       0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CTCInventory* inventory = 0;
        ParentThing->GetInterface(&inventory, 0x11);
        inventory->ItemsMenuEntries.Set(items[i].first, entry.m_pObject);
        if (inventory->QuickAccessItems[2] == items[i].first) {
            CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
            entry->AddChildAtEnd(icon);
            inventory->AssignedIcons.Set(2, icon.m_pObject);
        } else if (inventory->QuickAccessItems[3] == items[i].first) {
            CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
            entry->AddChildAtEnd(icon);
            inventory->AssignedIcons.Set(3, icon.m_pObject);
        } else if (inventory->QuickAccessItems[0] == items[i].first) {
            CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
            entry->AddChildAtEnd(icon);
            inventory->AssignedIcons.Set(0, icon.m_pObject);
        } else if (inventory->QuickAccessItems[1] == items[i].first) {
            CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
            entry->AddChildAtEnd(icon);
            inventory->AssignedIcons.Set(1, icon.m_pObject);
        }
        CCountedPointer<CComponent> description(fe->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
        CCountedPointer<CText> descriptionText;
        CWideString details = GetItemDefDescription(items[i].first);
        descriptionText = description;
        descriptionText->SetText(details, true, true);
        entry->AddChildAtEnd(description);
        CComponent* viewport = owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
        if (viewport)
            viewport->SetContent(description.m_pObject);
        CCountedPointer<CComponent> nameComponent(fe->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
        CCountedPointer<CText> nameText;
        nameText = nameComponent;
        nameText->SetText(name, true, true);
        entry->AddChildAtEnd(nameComponent);
        if (GetEngineGraphic(items[i].first).BankIndex) {
            CCountedPointer<CComponent> icon(fe->CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
            if (!((CIcon*)icon.m_pObject)->GetGraphicBank())
                ((CIcon*)icon.m_pObject)->SetGraphicBank(GetEngineGraphic(items[i].first).BankIndex);
            entry->AddChildAtEnd(icon);
        }
        CCountedPointer<CMenuEntry> menuEntry;
        menuEntry = entry;
        menuEntry->SetUserValue(items[i].first);
        list->AddEntry(entry);
        entry->AddChildAtEnd(actionsList);
    }
}
