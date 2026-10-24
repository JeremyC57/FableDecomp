// CAIStateGroup_MinionWorkInInn::GetFreeBench() -- a random free bench among the objects owned by the creature's workplace (CTCThingOwner, interface 0x21), or null
// Leaf: ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@
// Grade: DIFFER(211v211) -- scheduling only: ours stores the bench vector's three zero words before the
//   ObjectList end() load and the functor push; retail loads end() (`mov eax,[eax+0Ch]`), pushes &benches, then zero-stores.
//   Tried: plain vector ctor, iterator without copy ctor (fixed the by-value push), STLport _Vector_base/_STLP_alloc_proxy
//   model, end() hoisted into a named local (reloads the list, 214 bytes).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090BC60, 211 bytes (224 with padding), __thiscall, returns CThing*.
// Identity: Ego_r.pdb ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@AAEPAVCThing@@XZ, between IsRunnable (0x0090BBF0) and
//   CleanBench (0x0090BD40, its only caller) as in the PDB. The catalog label "CleanBench_PickRandomMarker" is an autoname.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound const 0x004365B0,
//   for_each<list<CIntelligentPointer<CThing> >::const_iterator, CAddFreeBenchToList> 0x0090C100 (out of line, /Gr: the
//   functor result through the hidden pointer in ecx, both iterators and the functor on the stack; its catalog label
//   "_Find_if<..CMiniMapDisplay..>" is propagated), CAIStateGroupBase::GetPWorldSeed 0x008FCFD0,
//   GFRandom(range, seed) 0x004CDAE0 (__fastcall), free 0x00BFEA14 (the vector's block).
// Layout: CAIStateGroupBase::PBrain +4, CAIBrain::PCreature +0x18, the creature's workplace pointer +0x170 (as the landed
//   CAIStateGroup_MinionWorkInInn::Update 0x0090BED0); interface 0x21 = CTCThingOwner, ObjectList (the 4-byte STLport list
//   sentinel pointer) at +0x0C (landed CTCTavern::GetFreeSeat 0x007BABE0); CAddFreeBenchToList holds the vector pointer
//   (landed operator() 0x0090C060).
// Shape: HasTC on the constant type tests the byte (`test byte ptr [eax+24h],2`); the one-ternary const lookup; the
//   bench vector is block-scoped, so its destructor (free of _M_start) is duplicated into both returns of the block and
//   the for_each result temporary shares the lookup key's frame word; the functor needs a user ctor for the /Gr hidden
//   return in ecx.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

class CThing;
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing {
public:
    void* __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
};

namespace _STL {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp> class list {               // STLport list: one sentinel pointer in retail
public:
    typedef _List_iterator<_Tp> const_iterator;
    _List_node_base* _M_node;
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};

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
class vector : protected _Vector_base<_Tp, _Alloc> {   // STLport vector<T*>, 12 bytes
public:
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }                               // pointers: _Destroy is trivial
    bool empty() const { return this->_M_start == this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
};

template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);    // 0x0090C100

}

class CAddFreeBenchToList {
public:
    _STL::vector<CThing*>* BenchList;           // +0x00
    CAddFreeBenchToList(_STL::vector<CThing*>* benches) : BenchList(benches) { }
};

struct CTCThingOwner {
    void* __vftable;
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned char _pad0a[2];
    _STL::list<CIntelligentPointer<CThing> > ObjectList;   // +0x0C
};

struct CThingAICreature {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PWorkplace;         // +0x170 (descriptive)
    CThing* GetPWorkplace() { return PWorkplace.GetPItem(); }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};

struct CAIStateGroup_MinionWorkInInn : CAIStateGroupBase {
    CThing* GetFreeBench();
};

CThing* CAIStateGroup_MinionWorkInInn::GetFreeBench()
{
    CThing* workplace = GetCreature()->GetPWorkplace();
    if (workplace && workplace->HasTC(0x21)) {
        CTCThingOwner* owner = (CTCThingOwner*)workplace->GetTC(0x21);
        _STL::vector<CThing*> benches;
        _STL::for_each(owner->ObjectList.begin(), owner->ObjectList.end(), CAddFreeBenchToList(&benches));
        if (!benches.empty())
            return benches[GFRandom(benches.size(), GetPWorldSeed())];
    }
    return 0;
}
