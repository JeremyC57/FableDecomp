// CThingSearchTools::GetAllThings(const CThingFilter_IsEligibleWitness& filter, list<CThingAICreature*>& out) const -- append every scan-visible CThingAICreature on the class list that passes the filter
// Leaf: ??$GetAllThings@VCThingAICreature@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@VCThingFilter_IsEligibleWitness@@@CThingSearchTools@@QBEXABVCThingFilter_IsEligibleWitness@@AAV?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006c9280, 140 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb ??$GetAllThings@VCThingAICreature@@V?$list@PAVCThingAICreature@@...@std@@VCThingFilter_IsEligibleWitness@@@CThingSearchTools@@QBEX...@Z; the
//   filter callee 0x006C8E90 is CThingFilter_IsEligibleWitness::operator() and the type mask 0x1F is CThingAICreature's; caller CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed 0x006C6040 (twice).
// Callees: CThingSearchTools::PeekClassList() const 0x0049C770, CThingFilter_IsEligibleWitness::operator()(const CThing&) const 0x006C8E90, malloc
//   0x00BFEA0E (the 0x0C-byte list node).
// Shape (family L020): the landed GetPBestThing walk (family K044: IsIncludedInThingScans inline, the type mask through a
//   local) with STLport 4.6 list::push_back = insert(end(), x) (iterator with a user copy constructor; the previous node
//   loaded once -- without it VC7.1 reloads it and sinks the callee-saved pushes below the early exit).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
class CClass { };
class CThing : public CClass {                           // retail layout (as the landed GetPBestThing rows, family K044)
public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                        // +0x6C THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                             // +0x91 Dead 0x01 / IncludeInThingScans 0x04 / Activated 0x08 / InLimbo 0x20
    bool IsIncludedInThingScans() const                  // Ego_r.pdb ?IsIncludedInThingScans@CThing@@QBE_NXZ (inline here)
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* thing)   // the mask goes through a local (VC7.1 folds it late)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { public: enum { ThingTypeMask = 0x7 }; };
class CThingCreatureBase : public CThingGameObject { public: enum { ThingTypeMask = 0xF }; };
class CThingAICreature : public CThingCreatureBase { public: enum { ThingTypeMask = 0x1F }; };
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list, 4 bytes
public:
    struct const_iterator {
        _List_node_base* _M_node;
        const_iterator(_List_node_base* __x) : _M_node(__x) { }
        const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& __y) const { return _M_node != __y._M_node; }
    };
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_node.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        return __tmp;
    }
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* __x) : _M_node(__x) { }
        iterator(const iterator& __x) : _M_node(__x._M_node) { }   // STLport iterators: user copy constructor
    };
    iterator end() { return iterator(_M_node._M_data); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node_base* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}
class CThingSearchTools {                                // Ego_r.pdb (only `this` is passed through here)
public:
    template <class TThing, class TList, class TFilter>
    void GetAllThings(const TFilter& filter, TList& out) const;
protected:
    const std::list<CClass*>& PeekClassList() const;     // 0x0049C770
};
template <class TThing, class TList, class TFilter>
void CThingSearchTools::GetAllThings(const TFilter& filter, TList& out) const
{
    const std::list<CClass*>& things = PeekClassList();
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        if (((CThing*)*it)->IsIncludedInThingScans() && IsThingOfType<TThing>((CThing*)*it)) {
            TThing* thing = (TThing*)*it;
            if (filter(*thing))
                out.push_back(thing);
        }
    }
}
class CThingFilter_IsEligibleWitness {
public:
    bool operator()(const CThing& thing) const;           // 0x006C8E90
};
template void CThingSearchTools::GetAllThings<CThingAICreature, std::list<CThingAICreature*>, CThingFilter_IsEligibleWitness>(const CThingFilter_IsEligibleWitness&, std::list<CThingAICreature*>&) const;