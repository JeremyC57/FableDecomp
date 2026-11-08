// CTCInventoryMap::GetRegionsToDisplay(std::set<long>& regions) -- collect the regions the map screen shows: the region of the hero's current map, every visited region, and the region named by each active quest card that names one
// Leaf: ?GetRegionsToDisplay@CTCInventoryMap@@QAEXAAV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FD410, 451 bytes, thiscall, `ret 4`. Speed TU (no pragma).
// Identity (proven): Ego_r.pdb ?GetRegionsToDisplay@CTCInventoryMap@@QAEXAAV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z
//   (00651990) and FableWin.pdb (0209C2B0); its six callers 0x0060C9CA .. 0x0062140F are CTCInventoryMap map-screen code, and
//   the body reads VisitedRegions (Ego_r CTCInventoryMap member) and the quest manager's ActiveQuestCards (+0x5C,
//   engine/CQuestManager.h). The catalog label is right.
// Callees: CTCBase::GetWorldMap 0x00686D30 (Ego_r ?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ), CWorldMap::GetRegionNumberMapIsIn
//   0x004FC190, _Rb_tree<long,..>::insert_unique 0x00512DA0, malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list node allocator),
//   list<CIntelligentPointer<CThing> >::_M_insert_dispatch 0x00618100 (the list copy constructor's range insert),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCQuestCard::GetRegionName 0x007025A0, CCharString::operator!= 0x0099E960,
//   CCharString::~CCharString 0x0099EAE0, CTCBase::PeekWorld 0x00686A80, CWorld slot 13 (+0x34) PeekWorldMap,
//   CWorldMap::GetRegionNumberFromName 0x004FC210.
// Shape: the quest cards are iterated over a by-value copy of CQuestManager::ActiveQuestCards (list copy ctor: header node,
//   then _M_insert_dispatch; ~list's clear() with the virtual element destructor `push 0 ; call [vt]` inline); the TC lookup
//   is the Lionhead PeekTC (bitset probe, inline CVectorMap::LowerBound with the signed `/ 2`, end() fallback) into an
//   uninitialised card pointer; the CCharString temporaries are destroyed at the end of each full expression.
// Layout: local view of CTCInventoryMap (no engine header; Ego_r VisitedRegions sits at retail +0x134) over a polymorphic
//   CTCBase (engine/CTCBase.h is the flattened Ego_r layout: ParentThing +0x04); CThing members from Ego_r with retail
//   offsets (UsingComponents bitset +0x20, Components CVectorMap +0x44, CurrentMapNumber +0x8E); the quest-manager singleton
//   is CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (Ego_r dynamic initializer) at 0x013B89FC with ActiveQuestCards +0x5C
//   (engine/CQuestManager.h, flattened there as bytes).
#include "fable_compiler.h"
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960 (Ego_r ??9CCharString@@QBE_NPBD@Z)
private:
    void* PStringData;
};

namespace std {
struct __false_type { };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };

// ---- set<long> (STLport 4.6 _tree.h / _set.h; only what this body uses) ----
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    pair<iterator, bool> insert_unique(const _Value& __x);   // 0x00512DA0
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node_base, allocator<_Rb_tree_node_base> > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    pair<iterator, bool> insert(const _Key& __x)
    {
        pair<typename _Rep_type::iterator, bool> __p = _M_t.insert_unique(__x);
        return pair<iterator, bool>(__p.first, __p.second);
    }
    _Rep_type _M_t;
};

// ---- vector<long> ----
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

// ---- list<T> (STLport 4.6 _list.h / _list.c) ----
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _List_base(const allocator_type& __a) : _M_node(__a, (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    allocator_type get_allocator() const { return allocator_type(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    template <class _T1, class _T2> static void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)   // 0x00618100
    {
        for ( ; __first != __last; ++__first)
            insert(__pos, *__first);
    }
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
};
}   // namespace std

// ---- engine classes (local views; offsets are retail's) ----
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50 (Ego_r ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    void* PData;
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    T* operator->() const { return (T*)GetPItem(); }
};

class CTCBase;
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                       // retail 0x10: STLport vector<pair<K, V> >, Compare +0x0C, Dirty +0x0D
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    C Compare;
    bool Dirty;
    const value_type* end() const { return _M_finish; }
    const value_type* LowerBound(const K& key) const
    {
        long count = this->_M_finish - this->_M_start;
        const value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};
enum ETCInterfaceType { TC_INTERFACE_QUEST_CARD = 0x6c };   // descriptive enumerator name for 0x6C (the CTCQuestCard interface)

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components;   // +0x44
    unsigned char _pad54[0x8e - 0x54];
    short CurrentMapNumber;                              // +0x8E
    FABLE_FORCEINLINE unsigned long HasTC(ETCInterfaceType type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(ETCInterfaceType type) const
    {
        const std::pair<ETCInterfaceType, CTCBase*>* end = Components.end();
        const std::pair<ETCInterfaceType, CTCBase*>* it = Components.LowerBound(type);
        return (it == end || type < it->first ? end : it)->second;
    }
    template <class T> FABLE_FORCEINLINE T* PeekTC(ETCInterfaceType type) const
    {
        T* out;
        if (HasTC(type))
            out = (T*)GetTC(type);
        return out;
    }
};

class CTCQuestCard {
public:
    CCharString GetRegionName() const;                   // 0x007025A0 (Ego_r ?GetRegionName@CTCQuestCard@@QBE?AVCCharString@@XZ)
};

class CWorldMap {
public:
    long GetRegionNumberMapIsIn(long map) const;         // 0x004FC190
    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                    // slot 12
    virtual const CWorldMap* PeekWorldMap() const;       // slot 13 (+0x34)
};

class CQuestManager {
public:
    unsigned char _pad00[0x5c];
    std::list<CIntelligentPointer<CThing> > ActiveQuestCards;   // +0x5C
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {
public:
    static CCountedPointer<T> PClass;                    // 0x013B89FC for T = CQuestManager
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
protected:
    CWorldMap& GetWorldMap();                            // 0x00686D30
    const CWorld& PeekWorld() const;                     // 0x00686A80
};

class CTCInventoryMap : public CTCBase {
public:
    unsigned char _pad0c[0x134 - 0x0c];
    std::vector<long> VisitedRegions;                    // +0x134
    void GetRegionsToDisplay(std::set<long>& regions);
};

void CTCInventoryMap::GetRegionsToDisplay(std::set<long>& regions)
{
    CThing* thing = ParentThing;
    regions.insert(GetWorldMap().GetRegionNumberMapIsIn(thing->CurrentMapNumber));
    for (std::vector<long>::iterator it = VisitedRegions.begin(); it != VisitedRegions.end(); ++it)
        regions.insert(*it);

    std::list<CIntelligentPointer<CThing> > cards(CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass->ActiveQuestCards);
    for (std::list<CIntelligentPointer<CThing> >::iterator card = cards.begin(); card != cards.end(); ++card) {
        CTCQuestCard* questCard = (*card)->PeekTC<CTCQuestCard>(TC_INTERFACE_QUEST_CARD);
        if (questCard->GetRegionName() != "")
            regions.insert(PeekWorld().PeekWorldMap()->GetRegionNumberFromName(questCard->GetRegionName()));
    }
}