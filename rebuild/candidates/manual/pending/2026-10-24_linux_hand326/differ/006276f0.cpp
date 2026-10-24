// CTCQuickAccessMenu::GetItemsListUI(long& numItems, NUISystem::CComponent* parent) -- sorts the hero's inventory items (augmentations excepted) into trophy / potion / gift / produce / other lists, counts them, and builds the quick-access item list with one category per kind (null when every kind is empty)
// Grade: DIFFER(1264v1260) -- prologue, item loop, inline push_back and the five-vector test now match (same-TU stand-ins for push_back 0x005947B6, _M_insert_overflow 0x0059400C and ConstructItemCategoryList 0x00626E40 let VC7.1 keep the others-vector finish in edi as retail); left: in the five category blocks retail loads g_pMainGameComponent->TextBank into ecx before pushing the GetTextBySymbol arguments, ours pushes first in some blocks (4 bytes). Next: fetch the bank through an inline accessor or a named CDataBank* local per block.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006276F0, 1238 bytes + a 5-entry jump table (0x00627BC8) inside the row, `ret 8`.
//   FableWin.pdb ?GetItemsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, NUISystem::CFrontEndManager::GetInstance 0x0041E5F2,
//   NUISystem::CManager::CreateComponent 0x0041DB1D, operator new 0x00BFEA1A / operator delete 0x00BFE9BC (the counted-pointer
//   info block), CCountedPointer<NUISystem::CComponent>::DeleteData 0x00428B10, CVectorMap LowerBound 0x0040F020 (inlined GetTC,
//   type 0x11), CTCInventoryBase::GetAllItems 0x005BF482, IsAugmentation 0x005D9830 / GetInventoryCategory 0x005D96F0 (both
//   `__fastcall` on the def index), vector<pair<long,long> >::push_back 0x005947B6 / _M_insert_overflow 0x0059400C,
//   CWideString() 0x0099AED0 / operator= 0x0099B7D0 / ~ 0x0099B510, CDataBank::GetTextBySymbol 0x009C95E0,
//   CTCQuickAccessMenu::ConstructItemCategoryList 0x00626E40, free 0x00BFEA14; the list's virtual slot 156 (+0x270).
// Shape: the counted pointer's ctor and release are inline (its Data word is never read, so VC7.1 drops its store); the
//   inventory component comes from an inline accessor returning an unset local on the absent path (homed in the dead
//   numItems slot); retail expands push_back once (the default case) and calls the out-of-line copy at the other five
//   sites: the out-of-line push_back is defined above under auto_inline(off) and the default case uses a __forceinline
//   copy; same-TU stand-ins for _M_insert_overflow and ConstructItemCategoryList (read their inputs, write only the
//   vector's own words) tell VC7.1 the category vectors never escape, so the others-vector finish stays in edi; the
//   category names go through one CWideString homed in the dead numItems slot.
// Layout: CTCBase ParentThing +0x04; CThing interface bitset +0x20 (type 0x11 = word 0 bit 17), interface map +0x44.
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};
struct CDataBank { CWideString GetTextBySymbol(const CCharString& symbol); };   // 0x009C95E0
struct CMainGameComponent { unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void __cdecl operator delete(void* block);              // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() {} };
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
typedef std::pair<long, long> CItemPair;
struct CItemVector {                                    // STLport vector<pair<long,long> > (12 bytes)
    CItemPair* _M_start;
    CItemPair* _M_finish;
    CItemPair* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    CItemPair* begin() { return _M_start; }
    CItemPair& operator[](unsigned long n) { return *(begin() + n); }
    void _M_insert_overflow(CItemPair* position, const CItemPair& x, const __true_type&, unsigned long fillLength,
                            bool atEnd);       // 0x0059400C
    void push_back(const CItemPair& x);         // 0x005947B6
    __forceinline void push_back_inline(const CItemPair& x)   // the one expanded site
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CItemPair(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
// Same-TU stand-in for the grow path 0x0059400C (the real body reallocates): reads the value, writes only this vector.
void CItemVector::_M_insert_overflow(CItemPair* position, const CItemPair& x, const __true_type&, unsigned long fillLength,
                                     bool atEnd)
{
    if (x.first == 0)
        return;
    _M_finish = position + fillLength;
    if (atEnd)
        _M_end_of_storage = _M_finish;
}
void CItemVector::push_back(const CItemPair& x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) CItemPair(x);
        ++_M_finish;
    } else
        _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
}
#pragma auto_inline(on)

struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    static void __fastcall DeleteData(void* data);      // 0x00428B10
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
        else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};

namespace NUISystem {
struct CComponent { virtual void v00(); };
struct CList : CComponent {
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85();
    virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90();
    virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145();
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetNumberOfVisibleEntries(long count);  // slot 156 (+0x270), name descriptive
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type, T*)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* PeekTC(long type, T*)
    {
        T* tc;
        if (HasTC(type))
            tc = GetTC(type, (T*)0);
        return tc;
    }
};

struct CTCInventory {                                   // interface 0x11
    void GetAllItems(CItemVector& items) const;         // 0x005BF482 (CTCInventoryBase)
};
bool __fastcall IsAugmentation(long defIndex);          // 0x005D9830
long __fastcall GetInventoryCategory(long defIndex);    // 0x005D96F0

struct CTCQuickAccessMenu {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void ConstructItemCategoryList(CWideString& name, NUISystem::CList* list, CItemVector& items,
                                   NUISystem::CComponent* parent, bool flag);   // 0x00626E40
    NUISystem::CComponent* GetItemsListUI(long& numItems, NUISystem::CComponent* parent);
};

// Same-TU stand-in for ConstructItemCategoryList 0x00626E40 (compiled earlier in the retail TU): it reads its inputs and
// keeps none of them.
long ReadCategoryInputs(const void* name, const void* list, const void* first, const void* last, const void* parent, bool flag);
#pragma auto_inline(off)
void CTCQuickAccessMenu::ConstructItemCategoryList(CWideString& name, NUISystem::CList* list, CItemVector& items,
                                                   NUISystem::CComponent* parent, bool flag)
{
    ReadCategoryInputs(name.PStringData, list, items._M_start, items._M_finish, parent, flag);
}
#pragma auto_inline(on)

NUISystem::CComponent* CTCQuickAccessMenu::GetItemsListUI(long& numItems, NUISystem::CComponent* parent)
{
    CCountedPointer<NUISystem::CComponent> actions(
        NUISystem::CFrontEndManager::GetInstance()->CreateComponent("UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS", false));
    CTCInventory* inventory = ParentThing->PeekTC(0x11, (CTCInventory*)0);
    CItemVector items;
    CItemVector trophies;
    CItemVector potions;
    CItemVector gifts;
    CItemVector produce;
    CItemVector others;
    inventory->GetAllItems(items);
    numItems = 0;
    for (unsigned long i = 0; i < items.size(); ++i) {
        if (IsAugmentation(items[i].first))
            continue;
        switch (GetInventoryCategory(items[i].first)) {
        case 0: trophies.push_back(items[i]); break;
        case 1: potions.push_back(items[i]); break;
        case 2: gifts.push_back(items[i]); break;
        case 3: produce.push_back(items[i]); break;
        case 4: others.push_back(items[i]); break;
        default: others.push_back_inline(items[i]); break;
        }
        numItems += items[i].second;
    }
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CList* list = 0;
    if (!trophies.empty() || !potions.empty() || !gifts.empty() || !produce.empty() || !others.empty()) {
        list = static_cast<NUISystem::CList*>(manager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false));
        list->SetNumberOfVisibleEntries(5);
        CWideString name;
        name = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
        ConstructItemCategoryList(name, list, trophies, parent, false);
        name = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
        ConstructItemCategoryList(name, list, potions, parent, false);
        name = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
        ConstructItemCategoryList(name, list, gifts, parent, true);
        name = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
        ConstructItemCategoryList(name, list, produce, parent, false);
        name = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_OTHERS");
        ConstructItemCategoryList(name, list, others, parent, false);
    }
    return list;
}
