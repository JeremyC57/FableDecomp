// Grade: DIFFER(1113v1067) -- retail homes this in [ebp-8] at entry and pushes ebx/esi/edi only after the sound block; the special-shop loops' lea ecx,[ebp-4] hoisted to the latch; third special-shop tail not cross-jumped; first normal loop entry tested instead of jmp to the condition; end block reloads this into ebx
// CTCInventoryTrade::OpenInventory() -- build the trade screen: refresh the sound categories, reset the trade lists, cache the shopping-valid flag, collect the owner shop's item lists and build the top-level list (the barber / tattoo / special shops get the barber list of their own cards, a normal shop the buy and sell lists with the card items removed), then reset it and register it as a component
// Leaf: OpenInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ACEDA, length 1067 bytes, plain `ret`. Frame-pointer size TU:
//   `#pragma optimize("y", off)` + `("s", on)`. Only caller: CTCInventoryTrade::OpenInventoryPC 0x005AE86B.
// Callees: the sound system (0x013B8394) slot 46 (+0xB8, RefreshCategory(long, float, float), 500.0f @ 0x01237584),
//   the trade-list reset @ 0x0059D9AD, this's slots 48 (+0xC0), 75 (+0x12C, the trade-allowed query) and 32 (+0x80),
//   the owner getter @ 0x005BC4C4, std::bitset<>::test @ 0x00410DE0 + LowerBound @ 0x0040F020 (inline GetInterface 0x53),
//   CTCShop's three shop-kind queries @ 0x00772490 / 0x00772510 / 0x00772590, InitialiseSellableItems @ 0x0059D820,
//   InitialiseBuyableItems @ 0x005A30A1, the item-list copy @ 0x005BF482, GetHighlightedItem @ 0x005BCAE3,
//   SetNumberOfSelectedItemOwned @ 0x005BE284, CFrontEndManager::GetInstance @ 0x0041E5F2 (slots 29 +0x74, 23 +0x5C),
//   GetHairCardDef @ 0x005D9A10 / GetTattooCardDef @ 0x005D9AA0 (__fastcall, by value), CTattooCardDef's
//   IsInvalidCustomTattoo @ 0x00581765, vector<pair<long, long> >::erase @ 0x00593139, CCharString @ 0x0099EBF0 / ~ @
//   0x0099EAE0 ("PC_TOP_LEVEL_BARBER_LIST" @ 0x0125385C), ConstructSpecialShopListPC @ 0x005A8A5D,
//   ConstructNormalShopListPC @ 0x005A6769, the component's slots 43 (+0xAC) and 48 (+0xC0), Components.push_back
//   @ 0x00486024, the CRT free thunk @ 0x00BFEA14.
// Layout (retail; the PDB's 16-byte containers are 12 here): IsShoppingValid +0x15C directly before CategoryCount +0x160
//   (as the PDB +0x188 / +0x18C pair), PTop_Level_List +0x220 (landed OpenInventoryPC), Components +0x0C.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CTCBase;
struct CPresenceBits { bool test(long kind) const; };      // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    bool HasTC(long interfaceType) const { return present.test(interfaceType); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CRefCounted {                                        // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {                     // a counted def reference returned by value
    T* m_p;
    __forceinline ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
struct CTattooCardDef : CDefBase {
    bool IsInvalidCustomTattoo() const;                     // 0x00581765
};
struct CHairCardDef : CDefBase { };
CDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType);   // 0x005D9AA0
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType);       // 0x005D9A10

struct CItem {                                              // pair<long, long>
    long DefIndex;
    long Count;
};
struct CPairVector {                                        // STLport vector<pair<long, long> > (12 bytes)
    CItem* _M_start;
    CItem* _M_finish;
    CItem* _M_end_of_storage;
    CPairVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPairVector()
    {
        if (_M_start)
            free(_M_start);
    }
    CItem* begin() { return _M_start; }
    CItem* end() { return _M_finish; }
    CItem* erase(CItem* position);                          // 0x00593139
};
struct CTCShop {
    bool IsBarberShop() const;                              // 0x00772490
    bool IsTattooParlour() const;                           // 0x00772510 (name descriptive)
    bool IsSpecialShop() const;                             // 0x00772590 (name descriptive)
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void Reset();                                   // slot 43 (+0xAC) (name descriptive)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetState(long state);                      // slot 48 (+0xC0) (name descriptive)
};
struct CFrontEndManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void OnInventoryScreenOpened(void* screen);     // slot 23 (+0x5C) (name descriptive)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void OnTradeScreenOpened();                     // slot 29 (+0x74) (name descriptive)
    static CFrontEndManager* __fastcall GetInstance();      // 0x0041E5F2
};
}
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void RefreshCategory(long category, float a, float b);   // slot 46 (+0xB8)
};
extern CSoundSystem* g_pSoundSystem;                        // 0x013B8394
struct CComponentVector {                                   // vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    void push_back(NUISystem::CComponent* const& value);    // 0x00486024
};

struct CTCInventoryTrade {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void CloseInventory();                          // slot 32 (+0x80) (name descriptive)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47();
    virtual bool GetShoppingValid();                        // slot 48 (+0xC0) (name descriptive)
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74();
    virtual bool IsTradeActionAllowed();                    // slot 75 (+0x12C)
    CThing*          ParentThing;                           // +0x04
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                            // +0x0c (CTCInventoryBase)
    unsigned char    _pad18[0x15c - 0x18];
    bool             IsShoppingValid;                       // +0x15c
    unsigned char    _pad15d[0x220 - 0x15d];
    NUISystem::CComponent* PTop_Level_List;                 // +0x220
    CThing* GetOwner() const;                               // 0x005BC4C4 (CTCInventoryBase)
    long GetHighlightedItem() const;                        // 0x005BCAE3 (CTCInventoryBase)
    void SetNumberOfSelectedItemOwned(long item);           // 0x005BE284 (CTCInventoryBase)
    void GetAllItems(CPairVector& items);                   // 0x005BF482 (CTCInventoryBase)
    void ResetTradeLists();                                 // 0x0059D9AD
    void InitialiseSellableItems();                         // 0x0059D820
    void InitialiseBuyableItems(CPairVector& items);        // 0x005A30A1
    NUISystem::CComponent* ConstructSpecialShopListPC(CPairVector& items, const CCharString& name);   // 0x005A8A5D
    NUISystem::CComponent* ConstructNormalShopListPC(CPairVector& buyable, CPairVector& sellable);    // 0x005A6769
    void OpenInventory();
};

void CTCInventoryTrade::OpenInventory()
{
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x40, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
    }
    ResetTradeLists();
    IsShoppingValid = GetShoppingValid();
    CPairVector sellable;
    CPairVector buyable;
    CPairVector items;
    CTCShop* shop = 0;
    CThing* owner = GetOwner();
    if (owner->HasTC(0x53))
        shop = owner->GetTC<CTCShop>(0x53);
    if (IsTradeActionAllowed()) {
        if (!shop->IsBarberShop() && !shop->IsTattooParlour() && !shop->IsSpecialShop()) {
            InitialiseSellableItems();
            GetAllItems(sellable);
        }
        InitialiseBuyableItems(buyable);
        GetAllItems(items);
    }
    long highlighted = GetHighlightedItem();
    if (highlighted > 0)
        SetNumberOfSelectedItemOwned(highlighted);
    if (IsTradeActionAllowed()) {
        NUISystem::CFrontEndManager::GetInstance()->OnTradeScreenOpened();
        NUISystem::CFrontEndManager::GetInstance()->OnInventoryScreenOpened(this);
        if (shop->IsBarberShop()) {
            for (CItem* it = items.begin(); it != items.end(); ) {
                CDefPointer<CHairCardDef> hair = GetHairCardDef(it->DefIndex);
                if (hair.m_p == 0)
                    it = items.erase(it);
                else
                    ++it;
            }
            CCharString listName("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(items, listName);
        } else if (shop->IsTattooParlour()) {
            for (CItem* it = items.begin(); it != items.end(); ) {
                CDefPointer<CTattooCardDef> tattoo = GetTattooCardDef(it->DefIndex);
                if (tattoo.m_p == 0 || tattoo.m_p->IsInvalidCustomTattoo())
                    it = items.erase(it);
                else
                    ++it;
            }
            CCharString listName("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(items, listName);
        } else if (shop->IsSpecialShop()) {
            CCharString listName("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(items, listName);
        } else {
            for (CItem* it = buyable.begin(); it != buyable.end(); ) {
                if (GetTattooCardDef(it->DefIndex).m_p != 0 || GetHairCardDef(it->DefIndex).m_p != 0)
                    it = buyable.erase(it);
                else
                    ++it;
            }
            for (CItem* it2 = sellable.begin(); it2 != sellable.end(); ) {
                if (GetTattooCardDef(it2->DefIndex).m_p != 0 || GetHairCardDef(it2->DefIndex).m_p != 0)
                    it2 = sellable.erase(it2);
                else
                    ++it2;
            }
            PTop_Level_List = ConstructNormalShopListPC(buyable, sellable);
        }
    } else {
        CloseInventory();
    }
    if (PTop_Level_List) {
        PTop_Level_List->Reset();
        PTop_Level_List->SetState(5);
        Components.push_back(PTop_Level_List);
    }
}
