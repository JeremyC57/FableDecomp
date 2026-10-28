// CTCInventoryMap::ConstructPastQuestsList(NUISystem::CComponent* parent) -- quest screen, "past quests" page: copy the quest manager's completed quest cards, collect their things; if there are any, build a UI_SUB_LIST with a UI_TITLE_QUEST_PAST header and let the hero's CTCInventoryQuests fill it (screen type 2); returns the list component, or 0 when no quest is completed
// Grade: DIFFER(649v637) -- only the CDefPointer release: retail decrements in memory (`dec dword [ecx+4] ; jne`), ours loads, decrements and stores (`mov edi,[eax+4] ; add eax,4 ; dec edi ; ... mov [eax],edi`); tried a local-copy destructor, __forceinline, a mutable or int count and the `!--` form. Next try: a same-TU noinline body for GetDef 0x0042B312 (the landed sibling 0x005FC2D0 matched with one)
// Leaf: ?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FCBE0, 637 bytes, thiscall `ret 4`, esp frame (sub esp,2Ch).
// Identity (proven): Ego_r.pdb ?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z (private,
//   CComponent* (CComponent*)); the Ego_r donor order ConstructPastQuestsList -> CloseInventory -> ProcessButtonBReleased ->
//   FindQuestsForActiveRegion maps onto retail 0x005FCBE0 -> 0x005FCE60 -> 0x005FD0B0 (landed) -> 0x005FD0F0 (landed); the
//   string 0x01257298 "UI_TITLE_QUEST_PAST" and CQuestManager::GetCompletedQuestCards (+0x60, next to ActiveQuestCards
//   +0x5C read by the CurrentQuests sibling 0x005FC2D0) agree. The catalog's "ConstructQuestsScreen_PastQuests" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (GetTC<CTCInventoryQuests>, interface 0x19:
//   CTCInventoryQuests::GetInterfaceType 0x00624D60 is `mov eax,19h`), malloc 0x00BFEA0E / free 0x00BFEA14 (list header,
//   vector storage), list<CIntelligentPointer<CThing> >::_M_insert_dispatch<const_iterator> 0x00618100 (the list copy),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, vector<CThing*>::_M_insert_overflow 0x00436AC0,
//   _List_base<CIntelligentPointer<CThing> >::clear 0x004B8050, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (static; IsAllocated 0x0044C6B6 follows it),
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390, CDefinitionManager::GetDef<CUIMiscThingsDef> 0x0042B312,
//   CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D,
//   operator new 0x00BFEA1A / delete 0x00BFE9BC (counted pointer info), CCountedPointer<CComponent>::DeleteData 0x00428B10,
//   CTCInventoryQuests::ConstructQuestList 0x0061B610; virtual CList::AddNonScrollingChild (slot 145, +0x244).
// Layout (local models, retail offsets): CThing UsingComponents +0x20 / Components +0x44 (STLport sizes); CQuestManager
//   CompletedQuestCards +0x60 (STLport lists are 4 bytes; engine/CQuestManager.h carries the Dinkumware Ego_r offsets).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* d, const void* s, unsigned int n);
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* p) { return p; }

#include "engine/CTCBase.h"

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct _List_node_base {
    _List_node_base* _M_next;                                                  // +0x00
    _List_node_base* _M_prev;                                                  // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                                               // +0x08
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef allocator<_List_node<_Tp> > _Node_allocator_type;
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Node_allocator_type> _M_node;   // +0x00
    _List_base(const _Alloc&) : _M_node(_Node_allocator_type(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    __declspec(noinline) void clear()                                          // 0x004B8050 (same-TU body)
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            __tmp->_M_data.~_Tp();
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef unsigned int size_type;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
    list(const list& __x) : _List_base<_Tp, _Alloc>(_Alloc())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = 0;
        for (const_iterator __i = begin(); __i != end(); ++__i)
            ++__result;
        return __result;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = this->_M_node.allocate(1);
        new (&__tmp->_M_data) _Tp(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    template <class _InputIter>
    __declspec(noinline) void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last,
                                                 const __false_type&)
    {                                                                          // 0x00618100 (STLport 4.6 body, same TU)
        for ( ; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                                             // +0x00
    _Tp* _M_finish;                                                            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                    // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            if (this->_M_finish != 0)
                new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                 unsigned int __fill_len, bool __atend = false)
    {                                                                          // 0x00436AC0 (STLport 4.6 body, same TU)
        const size_type __old_size = size();
        const size_type __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
        unsigned int __n = (unsigned int)(__position - this->_M_start);
        memmove(__new_start, this->_M_start, __n * sizeof(_Tp));
        _Tp* __new_finish = __new_start + __n;
        for (unsigned int __i = 0; __i < __fill_len; ++__i)
            *__new_finish++ = __x;
        if (!__atend) {
            unsigned int __m = (unsigned int)(this->_M_finish - __position);
            memmove(__new_finish, __position, __m * sizeof(_Tp));
            __new_finish += __m;
        }
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
        this->_M_start = __new_start;
        this->_M_finish = __new_finish;
        this->_M_end_of_storage._M_data = __new_start + __len;
    }
};

template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    bool test(unsigned int __pos) const { return (this->_M_getword(__pos) & this->_S_maskbit(__pos)) != 0; }
};

}   // namespace std

class CCharStringData;
class CCharString {
public:
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    ~CCharString();                                                            // 0x0099EAE0
    CCharStringData* PStringData;
};

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template <class K, class V, class TCompare> class CVectorMap {                 // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* _M_start;                                                      // +0x00 (CArray / vector)
    value_type* _M_finish;                                                     // +0x04
    value_type* _M_end_of_storage;                                             // +0x08
    TCompare    Compare;                                                       // +0x0C
    bool        Dirty;                                                         // +0x0D
    iterator end() { return _M_finish; }
    iterator LowerBound(const K& key);                                         // 0x0040F020 for the TC map
};

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                                                // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                                             // 0x00A01B50
    CBaseObjectPointer* PData;                                                 // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};

namespace NUISystem {
class CList;
class CComponent;
}
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                        // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        bool present = UsingComponents.test(T::INTERFACE_TYPE);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
        }
        return present;
    }
};

enum QuestScreenType { QUEST_SCREEN_TYPE_FORCE32 = 0x7fffffff };
class CTCInventoryQuests {
public:
    enum { INTERFACE_TYPE = 0x19 };
    void ConstructQuestList(const std::vector<CThing*, std::allocator<CThing*> >& quests, NUISystem::CList* list,
                            QuestScreenType type, NUISystem::CList* parent);   // 0x0061B610
};

class CQuestManager {                                                          // retail layout (STLport lists, 4 bytes)
public:
    unsigned char _pad00[0x60];
    std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > > CompletedQuestCards;   // +0x60
    const std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >& GetCompletedQuestCards() const
    {
        return CompletedQuestCards;
    }
};
extern CQuestManager* g_pQuestManager;                                         // 0x013B89FC (descriptive)

class CDefClassBase {
public:
    virtual ~CDefClassBase();
    virtual void Release();                                                    // slot 1
    int RefCount;                                                              // +0x04
};
template <class T> class CDefPointer {
public:
    T* Data;
    CDefPointer() : Data(0) { }
    __forceinline ~CDefPointer()
    {
        CDefClassBase* data = (CDefClassBase*)Data;
        if (data && !--data->RefCount)
            data->Release();
    }
};
namespace NUISystem {
class CUIMiscThingsDef : public CDefClassBase { };
}
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template <class T> bool GetDef(long globalIndex, CDefPointer<const T>* out) const;     // 0x0042B312 <CUIMiscThingsDef>
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                            // 0x0044C6B0
};

class CCPPointerInfo {
public:
    long RefCount;                                                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);                                 // +0x04
    void* Data;                                                                // +0x08
    CCPPointerInfo(long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                                      // +0x00
    CCPPointerInfo* Info;                                                      // +0x04
    static void __fastcall DeleteData(void* data);                             // 0x00428B10
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
        else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace NUISystem {
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
class CComponent {
public:
    virtual ~CComponent();
    virtual void Slot1(); virtual void Slot2(); virtual void Slot3();
    V4(Slot0_) V4(Slot1_) V4(Slot2_) V4(Slot3_) V4(Slot4_) V4(Slot5_) V4(Slot6_) V4(Slot7_) V4(Slot8_) V4(Slot9_)
    V4(Slot10_) V4(Slot11_) V4(Slot12_) V4(Slot13_) V4(Slot14_) V4(Slot15_) V4(Slot16_) V4(Slot17_) V4(Slot18_)
    V4(Slot19_) V4(Slot20_) V4(Slot21_) V4(Slot22_) V4(Slot23_) V4(Slot24_) V4(Slot25_) V4(Slot26_) V4(Slot27_)
    V4(Slot28_) V4(Slot29_) V4(Slot30_) V4(Slot31_) V4(Slot32_) V4(Slot33_) V4(Slot34_)
    virtual void Slot144();                                                    // slots 0..144 (CList adds 145+)
};
class CList : public CComponent {
public:
    virtual void AddNonScrollingChild(CCountedPointer<CComponent>& child);     // slot 145 (+0x244)
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& name, bool flag);           // 0x0041DB1D
};
class CFrontEndManager : public CManager {
public:
    static CFrontEndManager* __fastcall GetInstance();                        // 0x0041E5F2
};
}

class CTCInventoryMap : public CTCBase {
private:
    NUISystem::CComponent* ConstructPastQuestsList(NUISystem::CComponent* parent);
public:
    NUISystem::CComponent* CallConstructPastQuestsList(NUISystem::CComponent* parent) { return ConstructPastQuestsList(parent); }
};

NUISystem::CComponent* CTCInventoryMap::ConstructPastQuestsList(NUISystem::CComponent* parent)
{
    CTCInventoryQuests* inventoryQuests;
    ParentThing->GetTC(&inventoryQuests);

    std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > > quests =
        g_pQuestManager->GetCompletedQuestCards();
    std::vector<CThing*, std::allocator<CThing*> > questThings;
    for (std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::iterator it = quests.begin();
         it != quests.end(); ++it)
        questThings.push_back((CThing*)it->GetPItem());

    if (quests.size() <= 0)
        return 0;
    {
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef;
    long defIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(defIndex, &miscThingsDef);

    NUISystem::CManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* list = manager->CreateComponent("UI_SUB_LIST", false);
    CCountedPointer<NUISystem::CComponent> title(manager->CreateComponent("UI_TITLE_QUEST_PAST", false));
    static_cast<NUISystem::CList*>(list)->AddNonScrollingChild(title);
    inventoryQuests->ConstructQuestList(questThings, static_cast<NUISystem::CList*>(list), (QuestScreenType)2,
                                        static_cast<NUISystem::CList*>(parent));
    return list;
    }
}
