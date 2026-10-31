// CTCHeroCentre::OnInitialActivate() -- collect the quest-card position markers: every live thing owned by the hero centre whose script name starts with "HeroCentreQuestCardPos" is recorded with its position, the records are sorted by script name and their positions appended to QuestCardPositions
// Grade: DIFFER(550v626) 182 diff lines (parked 591/224): same-TU noinline _M_insert_dispatch over a stand-in that never touches the list object (the LTCG label is mod-ref knowledge). Not landable as is: port the real 0x00618100 body (insert loop) in place of InsertNodesStandIn. Left: frame 0x28 v 0x2c, ebx/ebp/edi pushes after the TC lookup, by-value iterator push shape (retry wave 2 sec2c)
// Was: DIFFER(591v626) -- whole-program (LTCG) knowledge: retail keeps the copied list's header node in esi although &list goes to the out-of-line _M_insert_dispatch (the callee is known not to read it), homes the list object on the spilled-this word and the comparator on the found flag's word, and saves no callee-saved register late; ours stores the header, re-reads it, shrink-wraps ebx/ebp and differs in the inline list/vector teardown. Next try: same-TU auto_inline(off) bodies for _M_insert_dispatch and the push_back overflow helpers (GOTCHAS chunk 205-206), probably with ltcg_grade.py
// Leaf: ?OnInitialActivate@CTCHeroCentre@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077DC40, length 626 bytes, thiscall, plain `ret`.
// Name: CTCHeroCentre vtable 0x0126AF94 slot 6 (the CTCBase OnInitialActivate slot: landed CTCTurncoat 0x007C7840 and
//   CTCHeroCentreDoorMarker 0x0077F750) and Ego_r.pdb donor order GetQuestCards / OnInitialActivate = retail 0x0077DBC0
//   (landed) / 0x0077DC40. Ego_r.pdb `?OnInitialActivate@CTCHeroCentre@@UAEXXZ`.
// Callees: the const CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0 (inline GetTC on interface 0x21 =
//   CTCThingOwner, ObjectList at +0x0c), list<CIntelligentPointer<CThing> >::_M_insert_dispatch 0x00618100 (the list copy),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, strncmp 0x00BFEAF8 (prefix test), CThing::GetPos 0x004C73D0,
//   CCharString copy 0x0099EC30 / ~CCharString 0x0099EAE0, vector<pair<CCharString,C3DVector> >::_M_insert_overflow
//   0x0077EF30, std::sort<pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector> > 0x0077F440 (Ego_r
//   names the comparator in the sort helpers' spellings), vector<C3DVector>::_M_insert_overflow 0x0066E370, malloc/free.
// Globals: the CCharString at 0x013BADAC, built from "HeroCentreQuestCardPos" (0x0126B004) by its dynamic initializer
//   0x0121EF10; no PDB names data, so the landed spelling g_CCharString_013badac is kept (descriptive).
// Layout: retail CTCHeroCentre QuestCardPositions +0x24 (engine/CTCHeroCentre.h is a flat pad class, so modelled
//   locally as the landed GetQuestCards 0x0077DBC0); CThing ScriptName +0x74, Dead = bit 0 of +0x91.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8
inline void* __cdecl operator new(unsigned int, void* p) { return p; }

class CCharStringData { public:
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
};
extern CCharString g_CCharString_013badac;              // 0x013BADAC ("HeroCentreQuestCardPos")

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
inline void _Destroy(C3DVector*, C3DVector*) { }

template <class _Tp> class allocator { public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc&, _Value __p) : _M_data(__p) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(_Alloc(), 0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x0077EF30 / 0x0066E370
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    typename _Traits::reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list { public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _STLP_alloc_proxy<_List_node_base*, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    list(const list<_Tp, _Alloc>& __x) : _M_node(allocator<_List_node<_Tp> >(), 0)
    {
        _M_node._M_data = (_List_node_base*)_M_node.allocate(1);
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
        _M_insert_dispatch(begin(), __x.begin(), __x.end(), __false_type());
    }
    ~list()
    {
        clear();
        _M_node.deallocate((_List_node<_Tp>*)_M_node._M_data, 1);
    }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    template <class _InputIterator>
    void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, const __false_type&);   // 0x00618100
};

// same-TU stand-in for the out-of-line copy loop 0x00618100: it links new nodes before pos and never touches the list object
void __fastcall InsertNodesStandIn(_List_node_base* pos, _List_node_base* first, _List_node_base* last);
template <class _Tp, class _Alloc> template <class _InputIterator>
__declspec(noinline) void list<_Tp, _Alloc>::_M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, const __false_type&)
{
    InsertNodesStandIn(__pos._M_node, __first._M_node, __last._M_node);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0077F440 (this instance)

} // namespace std

template <class K, class V> class CKeyPairCompareLess { public:
    CKeyPairCompareLess() { }
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
template <class K, class V, class TCompare> class CVectorMap { public:   // retail 0x10 bytes
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* _M_start;                               // +0x00
    value_type* _M_finish;                              // +0x04
    value_type* _M_end_of_storage;                      // +0x08
    TCompare    Compare;                                // +0x0c
    bool        Dirty;                                  // +0x0d
    const_iterator end() const { return _M_finish; }
    const_iterator LowerBound(const K& key) const;      // 0x004365B0 for the TC map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TComponentMap;

class CThing;
class CTCThingOwner { public:                           // interface 0x21
    unsigned char _pad00[0x0c];
    std::list<CIntelligentPointer<CThing> > ObjectList; // +0x0c
};

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];                  // +0x20 bitset<274>
    TComponentMap  Components;                          // +0x44
    unsigned char  _pad54[0x74 - 0x54];
    CCharString    ScriptName;                          // +0x74
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    template <class T> __forceinline bool GetTC(T** out, long interfaceType) const
    {
        bool present = (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            const TComponentMap* map = &Components;
            TComponentMap::const_iterator it = map->LowerBound(type);
            *out = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
        }
        return present;
    }
    const C3DVector& GetPos() const;                    // 0x004C73D0
};

class CTCHeroCentre { public:
    virtual void v00();
    CThing*                ParentThing;                 // +0x04
    unsigned char          _pad08[0x24 - 0x08];
    std::vector<C3DVector> QuestCardPositions;          // +0x24
    virtual void OnInitialActivate();
};

void CTCHeroCentre::OnInitialActivate()
{
    CTCThingOwner* owner;
    if (ParentThing->GetTC(&owner, 0x21)) {
        std::list<CIntelligentPointer<CThing> > objects(owner->ObjectList);
        std::vector<std::pair<CCharString, C3DVector> > markers;
        bool found = false;
        for (std::list<CIntelligentPointer<CThing> >::iterator it = objects.begin(); it != objects.end(); ++it) {
            CThing* thing = (*it).GetPItem();
            if (thing != 0 && !thing->Dead &&
                strncmp(thing->ScriptName.GetPString(), g_CCharString_013badac.GetPString(), g_CCharString_013badac.Length()) == 0) {
                markers.push_back(std::pair<CCharString, C3DVector>(thing->ScriptName, thing->GetPos()));
                found = true;
            }
        }
        if (found)
            std::sort(markers.begin(), markers.end(), CKeyPairCompareLess<CCharString, C3DVector>());
        for (unsigned int i = 0; i < markers.size(); ++i)
            QuestCardPositions.push_back(markers[i].second);
    }
}
