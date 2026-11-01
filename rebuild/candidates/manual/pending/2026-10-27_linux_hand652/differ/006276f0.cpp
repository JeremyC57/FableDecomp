// CTCQuickAccessMenu::GetItemsListUI(long& numItems, NUISystem::CComponent* parent) -- sorts the hero's inventory items (augmentations excepted) into trophy / potion / gift / produce / other lists, counts them, and builds the quick-access item list with one category per kind (null when every kind is empty)
// Grade: DIFFER(1244v1260) -- register allocation only (464 masked lines, all from one choice): retail keeps the others-vector finish in edi across the item loop (`xor edi,edi` init, `cmp edi,[esp+38h]` in the inline push_back) and saves ebx late (after the inventory lookup), keeping `this` in a frame word; here VC7.1 must treat the six category vectors as escaping, because ConstructItemCategoryList 0x00626E40 -- compiled earlier in the same retail TU (it ends at 0x006273FF, ~1.5 KB) -- is only declared, so every vector word lives in memory and ebx takes `this`. Diagnostic: this exact file plus a same-TU body for ConstructItemCategoryList that reads its inputs and keeps nothing is RELOCATION_MATCH (probe/006276f0_p05.cpp; the body is invented, so it is not landed). 12 probes (vector order, const& items, numItems through a local, _M_insert_overflow body present / absent, text-bank accessor). Next: reconstruct CTCQuickAccessMenu::ConstructItemCategoryList 0x00626E40 and compile it ahead of this function in the same source (the prior's other residue, the text-bank load order, is gone with the accessor).
// Leaf: ?GetItemsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006276F0, 1238 bytes + a 5-entry jump table (0x00627BC8) inside the row, `ret 8`.
//   FableWin.pdb ?GetItemsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, NUISystem::CFrontEndManager::GetInstance 0x0041E5F2,
//   NUISystem::CManager::CreateComponent 0x0041DB1D, operator new 0x00BFEA1A / operator delete 0x00BFE9BC (the counted-pointer
//   info block), CCountedPointer<NUISystem::CComponent>::DeleteData 0x00428B10, CVectorMap LowerBound 0x0040F020 (inlined GetTC,
//   type 0x11), CTCInventoryBase::GetAllItems 0x005BF482, IsAugmentation 0x005D9830 / GetInventoryCategory 0x005D96F0 (both
//   `__fastcall` on the def index), vector<pair<long,long> >::push_back 0x005947B6 / _M_insert_overflow 0x0059400C,
//   CWideString() 0x0099AED0 / operator= 0x0099B7D0 / ~ 0x0099B510, CDataBank::GetTextBySymbol 0x009C95E0,
//   CTCQuickAccessMenu::ConstructItemCategoryList 0x00626E40, free 0x00BFEA14; the list's virtual slot 156 (+0x270).
// Shape: real STLport 4.6 vector<pair<long,long> > (push_back inline, _M_insert_overflow with its real body from the landed
//   0x0059400C text): VC7.1 on its own expands push_back at the default case and calls the COMDAT (0x005947B6, ICF-folded) at
//   the other five, as retail; the counted pointer's ctor and release inline; the text bank through the inline
//   GetGameTextBank() accessor (landed 0x005FB8F0 form); the CTCInventory lookup is the inline PeekTC returning an unset local.
// Layout: CTCBase ParentThing +0x04; CThing interface bitset +0x20 (type 0x11 = word 0 bit 17), interface map +0x44.
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public: unsigned char _pad[0x14]; CDataBank* GameTextBank; };   // +0x14
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
inline CDataBank* GetGameTextBank() { return g_pMainGameComponent->GameTextBank; }

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void __cdecl operator delete(void* block);              // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference operator[](size_type __n) { return *(begin() + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);      // <pair<long,long> > 0x0059400C
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
}   // namespace std
typedef std::vector<std::pair<long, long> > CItemVector;

class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer { public:
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
class CComponent { public: virtual void v00(); };
class CList : public CComponent { public:
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
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}

class CInterfaceMap { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
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

class CTCInventory { public:                                   // interface 0x11
    void GetAllItems(CItemVector& items) const;         // 0x005BF482 (CTCInventoryBase)
};
bool __fastcall IsAugmentation(long defIndex);          // 0x005D9830
long __fastcall GetInventoryCategory(long defIndex);    // 0x005D96F0

class CTCQuickAccessMenu {
public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04 (CTCBase)
private:
    void ConstructItemCategoryList(CWideString& name, NUISystem::CList* list, CItemVector& items,
                                   NUISystem::CComponent* parent, bool flag);   // 0x00626E40
    NUISystem::CComponent* GetItemsListUI(long& numItems, NUISystem::CComponent* parent);
};

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
        default: others.push_back(items[i]); break;
        }
        numItems += items[i].second;
    }
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CList* list = 0;
    if (!trophies.empty() || !potions.empty() || !gifts.empty() || !produce.empty() || !others.empty()) {
        list = static_cast<NUISystem::CList*>(manager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false));
        list->SetNumberOfVisibleEntries(5);
        CWideString name;
        name = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
        ConstructItemCategoryList(name, list, trophies, parent, false);
        name = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
        ConstructItemCategoryList(name, list, potions, parent, false);
        name = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
        ConstructItemCategoryList(name, list, gifts, parent, true);
        name = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
        ConstructItemCategoryList(name, list, produce, parent, false);
        name = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_OTHERS");
        ConstructItemCategoryList(name, list, others, parent, false);
    }
    return list;
}
