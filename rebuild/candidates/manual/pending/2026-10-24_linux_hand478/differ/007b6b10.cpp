// CTCCrateStack::FillStacks(const CTCVillage& village, long type) -- collect the village markers whose crate stack has the given type, then fill each stack with one or two crates at the marker's position, using its physics up vector
// Grade: DIFFER(343v333) -- register allocation: retail keeps the physics component pointer in ebx (seeded from the stale frame word that also homes `stack`, with esi shared by the seed pointer and the random bit); VC7.1 here spills physics to its own frame word (an extra frame slot, a reload on the not-found path) and splits the seed pointer / random bit over edi / esi
// Leaf: ?FillStacks@CTCCrateStack@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6B10, length 336 bytes (static __fastcall: village in ecx, type in
//   edx; plain `ret`; 0x14D bytes + int3 pad).
// Identity: Ego_r.pdb ?FillStacks@CTCCrateStack@@SIXABVCTCVillage@@J@Z (static, /Gr __fastcall); the catalog label
//   "FillStacks" is right. Caller 0x005065B1 (`FillStacks(village, 2)`).
// Callees: copy_if<list<CIntelligentPointer<CThingMarker> >::const_iterator, back_insert_iterator<vector<...> >,
//   IsCrateStackOfType> 0x007B7140 (Lionhead's copy_if, inserter returned through the hidden pointer),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times per marker), CInterfaceMap::LowerBound 0x0040F020 (inline
//   GetInterface: 0xB5 = CTCCrateStack, bitset word +0x34 bit 21; 2 = CTCPhysicsBase, word +0x20 bit 2),
//   CTCPhysicsBase::GetRHSet() const (virtual slot 72, +0x120), CWorld::GetPRandomSeed 0x0049D810 (on
//   g_pMainGameComponent 0x013B86A0 ->World +0x24), GFROR13 0x00497890, CThing::GetPosition 0x004C73D0,
//   CTCCrateStack::FillStack(long, const C3DVector&, const C3DVector&) 0x007B6810, the marker pointers' virtual
//   destructors (slot 0, flag 0) and free 0x00BFEA14 (the vector teardown).
// Layout: retail CTCVillage VillageMarkers +0x70 (STLport list, 4 bytes; Ego_r.pdb +0xEC with 12-byte lists, so the
//   generated engine/CTCVillage.h offsets do not apply); CRightHandedSet Up +0x0C.
// Shape: `vector markers; copy_if(village.VillageMarkers.begin(), ..end(), back_inserter(markers),
//   IsCrateStackOfType(type)); CTCCrateStack* stack; CTCPhysicsBase* physics; for (it = markers.begin(); it !=
//   markers.end(); ++it) { (*it)->GetInterface(&stack, 0xB5); (*it)->GetInterface(&physics, 2); const CRightHandedSet&
//   rhset = physics->GetRHSet(); long extra = GFRandom(seed) & 1; stack->FillStack(extra + 1, (*it)->GetPosition(),
//   rhset.Up); }` -- both component pointers are declared before the loop and never initialised (retail loads the
//   physics register from its stale frame word); GFRandom is the Lionhead LCG (`seed = seed * 0x24A1 + 0x24DF;
//   seed = GFROR13(seed)`).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

class CTCBase;

class C3DVector {
public:
    float X, Y, Z;
};

class CRightHandedSet {                                     // Ego_r.pdb, 24 bytes
public:
    C3DVector Forward;                                      // +0x00
    C3DVector Up;                                           // +0x0C
};

class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

class CThingMarker : public CThing {
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* PItem;                                          // +0x04
    CThing* GetPItem() const;                               // 0x00A01B50
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0125A860 for CThingMarker
public:
    T* operator->() const { return (T*)GetPItem(); }
};

#define VF(n) virtual void v##n();
class CTCPhysicsBase {                                      // interface 2
public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31) VF(32)
    VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48)
    VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63) VF(64)
    VF(65) VF(66) VF(67) VF(68) VF(69) VF(70) VF(71)
    virtual const CRightHandedSet& GetRHSet() const;        // slot 72 (+0x120)
};
#undef VF

class CWorld {
public:
    unsigned long* GetPRandomSeed();                        // 0x0049D810
};

class CMainGameComponent {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                                    // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}

class IsCrateStackOfType {
public:
    long Type;                                              // +0x00
    explicit IsCrateStackOfType(long type) : Type(type) { }
    bool operator()(CIntelligentPointer<CThingMarker> marker) const;
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};

template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // 4 bytes
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)(_M_node._M_data->_M_next)); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
};

template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}

}

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred);   // 0x007B7140

class CTCVillage {
public:
    unsigned char _pad00[0x70];
    std::list<CIntelligentPointer<CThingMarker> > VillageMarkers;   // +0x70
};

class CTCCrateStack {
public:
    void FillStack(long numCrates, const C3DVector& position, const C3DVector& up);   // 0x007B6810
    static void __fastcall FillStacks(const CTCVillage& village, long type);
};

void __fastcall CTCCrateStack::FillStacks(const CTCVillage& village, long type)
{
    std::vector<CIntelligentPointer<CThingMarker> > markers;
    copy_if(village.VillageMarkers.begin(), village.VillageMarkers.end(), std::back_inserter(markers),
            IsCrateStackOfType(type));
    CTCCrateStack* stack;
    CTCPhysicsBase* physics;
    for (std::vector<CIntelligentPointer<CThingMarker> >::iterator it = markers.begin(); it != markers.end(); ++it) {
        (*it)->GetInterface(&stack, 0xb5);
        (*it)->GetInterface(&physics, 2);
        const CRightHandedSet& rhset = physics->GetRHSet();
        long extra = GFRandom(g_pMainGameComponent->World->GetPRandomSeed()) & 1;
        stack->FillStack(extra + 1, (*it)->GetPosition(), rhset.Up);
    }
}
