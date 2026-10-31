// CAIStateGroup_MinionWorkInInn::GetFreeBench() -- a random free bench among the objects owned by the creature's workplace (its CTCThingOwner, interface 0x21), or null
// Grade: DIFFER(211v211) -- one register: the owner component is loaded into edx (`mov edx,[eax+4] ; mov eax,[edx+0Ch]`) where retail reuses eax; this needs the end() iterator in a local before the vector and begin() taken from its node (the natural for_each(ObjectList.begin(), ObjectList.end(), ...) form schedules the three vector zero stores before the ObjectList node load instead)
// Leaf: ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@AAEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090BC60, 211 bytes (224 with padding), __thiscall, returns CThing*.
// Identity: Ego_r.pdb ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@AAEPAVCThing@@XZ (private; donor 0x00930EC0, just before
//   CleanBench 0x00930FE0 = retail 0x0090BD40, its only caller, whose landed source already binds this spelling). The
//   catalog label "CleanBench_PickRandomMarker" is descriptive.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..>
//   >::LowerBound(const ETCInterfaceType&) const 0x004365B0, std::for_each<list<CIntelligentPointer<CThing> >::const_iterator,
//   CAddFreeBenchToList> 0x0090C100 (landed; link-time __fastcall, the functor returned through the hidden pointer in ecx),
//   CAIStateGroupBase::GetPWorldSeed() const 0x008FCFD0 (Ego_r.pdb, protected), GFRandom(unsigned long, unsigned long*)
//   0x004CDAE0 (__fastcall), free 0x00BFEA14 (the vector's block). FableWin.pdb: CAddFreeBenchToList(vector<CThing*>&).
// Layout: CAIStateGroupBase::PBrain +4, CAIBrain::Creature +0x18, the creature's workplace intelligent pointer +0x170 (as the
//   landed CAIStateGroup_MinionWorkInInn::Update 0x0090BED0); CTCThingOwner::ObjectList (STLport list, one node pointer) +0x0C.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                 // Ego_r: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;              // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCBase;
enum ETCInterfaceType { };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    bool empty() const { return this->_M_start == this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, _Alloc> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
};
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);    // 0x0090C100 (link-time __fastcall)
}
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > {
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* first;
    value_type* last;
    const value_type* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
};

class CAddFreeBenchToList {
public:
    std::vector<CThing*>* BenchList;            // +0x00
    CAddFreeBenchToList(std::vector<CThing*>& list) : BenchList(&list) { }
    void operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x0090C060
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
};
class CTCThingOwner : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x21 };
    std::list<CIntelligentPointer<CThing> > ObjectList;   // +0x0C
};

class CThingAICreature : public CThing {
public:
    unsigned char _pad4c[0x170 - 0x4c];
    CIntelligentPointer<CThing> PWorkplace;     // +0x170 (descriptive, as the landed Update 0x0090BED0)
};
class CAIBrain {
public:
    unsigned char _pad00[0x18];
    CThingAICreature* Creature;                 // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
protected:
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
};
class CAIStateGroup_MinionWorkInInn : public CAIStateGroupBase {
private:
    CThing* GetFreeBench();
};

CThing* CAIStateGroup_MinionWorkInInn::GetFreeBench()
{
    CThing* workplace = PBrain->GetCreature().PWorkplace.GetPItem();
    if (workplace && workplace->HasTC(CTCThingOwner::INTERFACE_TYPE)) {
        const CTCThingOwner* owner = workplace->GetTC<CTCThingOwner>(CTCThingOwner::INTERFACE_TYPE);
        std::list<CIntelligentPointer<CThing> >::const_iterator last = owner->ObjectList.end();
        std::vector<CThing*> benches;
        std::for_each(std::list<CIntelligentPointer<CThing> >::const_iterator(last._M_node->_M_next), last, CAddFreeBenchToList(benches));
        if (!benches.empty())
            return benches[GFRandom(benches.size(), GetPWorldSeed())];
    }
    return 0;
}
