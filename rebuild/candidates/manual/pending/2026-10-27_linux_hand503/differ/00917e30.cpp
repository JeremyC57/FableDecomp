// CAIStateGroup_Clean::GetBench() -- the first free bench among the things owned by the creature's home building (interface 0x21 CTCThingOwner::ObjectList, find_if CIsFreeBench), or null
// Grade: DIFFER(140v134) -- frame: ours reserves 8 bytes (sub esp,8; key/pred/result in the second slot [esp+8]) where retail has one slot (push ecx; [esp+4]); every instruction otherwise identical
// Leaf: ?GetBench@CAIStateGroup_Clean@@AAEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00917E30, 134 bytes, thiscall, plain `ret`, CThing* in eax.
// Identity (proven): Ego_r.pdb ?GetBench@CAIStateGroup_Clean@@AAEPAVCThing@@XZ is the only member that runs
//   find_if<..., CIsFreeBench>, and this is the only caller of the landed find_if 0x00918110 (whose header already says
//   so); its own only caller 0x00917EC0 (the CAIStateGroup_Clean::CleanBench candidate) tests the returned bench.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (home twice, then the found bench), CVectorMap LowerBound
//   @ 0x0040F020 (inlined GetInterface 0x21: word 1 bit 1 of the presence bitset), find_if 0x00918110 (/Gr: hidden
//   result in ecx; first, last and the empty predicate on the stack).
// Layout: CAIStateGroupBase::PBrain +4, CAIBrain::PCreature +0x18, CThingAICreature::PHome +0x168 (landed 0x00917330);
//   CTCThingOwner::ObjectList +0x0C (the 4-byte STLport list header pointer; Ego_r offset, nothing STL before it).
// Shape: `if (creature->GetPHome())` then a second GetPHome() for the interface fetch (retail calls it twice); the
//   list is walked through const iterators (find_if is the _Const_traits instantiation); the empty predicate is copied
//   trivially (retail pushes the dword of its slot, shared with the LowerBound key and the hidden result).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);         // 0x0040F020
};
class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                   // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingBuilding : public CThing { };

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CIsFreeBench {
public:
    CIsFreeBench() { }                                     // user ctor: no zero-stored temporary
    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x00918050
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef bidirectional_iterator_tag iterator_category;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // 4 bytes
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x00918110
}

class CTCThingOwner { public:                        // interface 0x21
    unsigned char _pad00[0xc];
    std::list<CIntelligentPointer<CThing> > ObjectList;   // +0x0C
};

class CThingAICreature { public:
    unsigned char _pad[0x168];
    CIntelligentPointer<CThingBuilding> PHome;      // +0x168
    CThingBuilding* GetPHome() { return PHome.GetPItem(); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                    // +0x18
};
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    bool Terminated;                                // +0x10
    unsigned char Priority;                         // +0x11
    bool Enabled;                                   // +0x12
};
class CAIStateGroup_Clean : public CAIStateGroupBase {
public:
    bool Cleaning;                                  // +0x14
    bool KneelingIntoCleaning;                      // +0x15
    long FrameToClean;                              // +0x18
    long CleanActionID;                             // +0x1c
    float GroundFloorZPos;                          // +0x20
    bool Initialised;                               // +0x24
private:
    CThing* GetBench();
};

CThing* CAIStateGroup_Clean::GetBench()
{
    if (PBrain->PCreature->GetPHome()) {
        CTCThingOwner* owner;
        if (PBrain->PCreature->GetPHome()->GetInterface(0x21, &owner)) {
            const std::list<CIntelligentPointer<CThing> >& objects = owner->ObjectList;
            std::list<CIntelligentPointer<CThing> >::const_iterator it =
                std::find_if(objects.begin(), objects.end(), CIsFreeBench());
            if (it != objects.end())
                return it->GetPItem();
        }
    }
    return 0;
}
