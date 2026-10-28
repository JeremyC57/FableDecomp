// CTCInventoryTrade::ConstructSpecialShopListPC(const std::vector<std::pair<long,long> >& items, CCharString& name) -- creates the special-shop list, totals the price of the special items the hero can still buy and writes it into the buy entry's text
// Leaf: ConstructSpecialShopListPC
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A8A5D, 521 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CGameDefinitionManager::Get 0x0044C6B0 /
//   GetDefGlobalIndexFromName 0x009AD390 / GetDef 0x0042B312 (same-TU noinline stand-in), GetMainPlayer 0x00449960,
//   PeekPControlledCreature 0x00487DD0, the item interface-type lookup 0x005D99D0 (`__fastcall`, manifest "GetInventoryCategory"),
//   bitset::test 0x00410DE0 + LowerBound 0x004365B0 (inline GetInterface), IsItemAffordable 0x0059DA10, CanAddItem 0x005BF2ED,
//   IsSpecialCase 0x0059C983, AlreadyHaveSpecial 0x0059FC17, component slots 3 (+0x0C) / 142 (+0x238) / 144 (+0x240),
//   CWideString copy 0x0099B720 / operator= 0x0099B7D0 / += 0x0099B8D0 / ~ 0x0099B510, GetXString 0x005CC772,
//   IntToWideString 0x00405F00.
// Shape: the front-end manager goes through a named local so GetInstance is called before CreateComponent's pushes.
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
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
};
struct CMiscThingsDef : CDefBase { };
void* FindDef_c();
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name, bool b);   // 0x009AD390
    bool GetDef(long index, CDefPointer<CMiscThingsDef>& out);         // 0x0042B312
};
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer<CMiscThingsDef>& out)
{
    void* def = FindDef_c();
    if (!def)
        return false;
    out.Object = (CMiscThingsDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CPresenceBits { bool test(long kind) const; };      // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                   // 0x004365B0
};
struct CTCInventoryBase { bool CanAddItem(long defIndex) const; };   // 0x005BF2ED
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };    // 0x00487DD0
class CPlayerManager { public: CPlayer* GetMainPlayer() const; };   // 0x00449960
class CMainGameComponent { public: unsigned char _pad[0x1c]; CPlayerManager* PlayerManager; };
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
long __fastcall GetInventoryInterfaceType(long defIndex);   // 0x005D99D0
namespace NUISystem {
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81();
    virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93();
    virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual const CWideString& GetText();               // slot 142 (+0x238)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
class CFrontEndManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CFrontEndManager* __fastcall GetInstance();    // 0x0041E5F2
};
}
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
}
struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
struct CLongList {
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
// Layout: retail CTCInventoryTrade (landed ctor 0x0059EC48): NotInStock +0x140, TotSpecialItemsToBuy +0x1F8, SpecialItemString +0x1FC.
class CTCInventoryTrade { public:
    unsigned char _pad00[0x140];
    CLongList     NotInStock;                           // +0x140
    unsigned char _pad144[0x1f8 - 0x144];
    long          TotSpecialItemsToBuy;                 // +0x1f8
    CWideString   SpecialItemString;                    // +0x1fc
    bool IsItemAffordable(long defIndex, bool b);       // 0x0059DA10
    bool IsSpecialCase(long defIndex);                  // 0x0059C983
    bool AlreadyHaveSpecial(long defIndex);             // 0x0059FC17
    NUISystem::CComponent* ConstructSpecialShopListPC(const std::vector<std::pair<long, long> >& items, CCharString& name);
};
NUISystem::CComponent* CTCInventoryTrade::ConstructSpecialShopListPC(const std::vector<std::pair<long, long> >& items, CCharString& name)
{
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* list = frontEnd->CreateComponent(name, false);
    CDefPointer<CMiscThingsDef> misc;
    long miscIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_MISC_THINGS_DEF", true);
    CGameDefinitionManager::Get().GetDef(miscIndex, misc);
    NUISystem::CComponent* entry = list->FindChild("PC_SPECIALSHOP_MENU_BUY_ENTRY");
    TotSpecialItemsToBuy = 0;
    if (!items.empty() || !NotInStock.empty()) {
        for (const std::pair<long, long>* it = items.begin(); it != items.end(); ++it) {
            long defIndex = it->first;
            long count = it->second;
            const CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->PeekPControlledCreature();
            CTCInventoryBase* inventory = 0;
            hero->GetInterface(&inventory, GetInventoryInterfaceType(defIndex));
            if (IsItemAffordable(defIndex, true) && (inventory->CanAddItem(defIndex) || IsSpecialCase(defIndex)) && count > 0
                && !AlreadyHaveSpecial(defIndex))
                TotSpecialItemsToBuy += it->second;
        }
        NUISystem::CComponent* text = entry->FindChild("PC_TRADING_BUY_TEXT");
        SpecialItemString = text->GetText();
        CWideString price(SpecialItemString);
        price += GetXString();
        price += IntToWideString(TotSpecialItemsToBuy);
        text->SetText(price, true, true);
    }
    return list;
}