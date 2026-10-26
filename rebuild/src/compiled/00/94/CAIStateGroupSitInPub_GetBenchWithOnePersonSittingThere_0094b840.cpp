// CAIStateGroup_SitInPub::GetBenchWithOnePersonSittingThere(const CThingBuilding& building) const -- the first bench in the pub's owned-object list that has exactly one person sitting on it (find_if with CIsBenchWithOnePerson), or 0 when the building owns nothing (no thing-owner component) or no bench qualifies
// Leaf: ?GetBenchWithOnePersonSittingThere@CAIStateGroup_SitInPub
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094B840, 100 bytes (+ int3 padding to 112), `ret 4`.
// Callees: CVectorMap LowerBound const @ 0x004365B0 (the inlined const GetInterface of type 0x21 = CTCThingOwner:
//   bitset word +0x24 bit 1), std::find_if<list const iterator, CIsBenchWithOnePerson> @ 0x0094BA90 (/Gr __fastcall:
//   the iterator result by hidden pointer in ecx, first / last / the empty predicate on the stack),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50. The predicate's operator() is 0x0094B9B0 (called inside find_if).
// Shape: `if (GetInterface(&owner, 0x21)) { it = find_if(begin, end, CIsBenchWithOnePerson()); if (it != end) return
//   (CThingObject*)it->GetPItem(); } return 0;`. The empty predicate (user ctor, no members) is pushed from the dead key
//   word (`mov edx,[esp+8] ; push edx`); LowerBound is a same-TU stand-in (auto_inline(off)) so the key's word is shared
//   with find_if's result, as in retail; find_if itself stays declaration-only here.
// Layout: Ego_r.pdb CTCThingOwner::ObjectList at +0x0c (STLport list header node pointer; the node's
//   CIntelligentPointer<CThing> at +8); CThing presence bitset +0x20, interface map +0x44. Local model (the engine headers
//   flatten these classes).
// Label: catalog "CAIStateGroup_SitInPub_FindAndValidateTask" -> Ego_r.pdb
//   ?GetBenchWithOnePersonSittingThere@CAIStateGroup_SitInPub@@ABEPAVCThingObject@@ABVCThingBuilding@@@Z.
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

// Same-TU stand-in for the const CVectorMap::LowerBound (its retail body is 0x004365B0, outside these bytes).
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 presence bitset of the interface types
    CInterfaceMap m_interfaces;                             // +0x44 sorted {type, component} vector map
    template <class T> __forceinline bool GetInterface(T** out, long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingBuilding : public CThing { };
class CThingObject : public CThing { };

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: 
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
inline bool operator==(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> class list {
public:
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _List_node_base* _M_node;                               // the header node
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x0094BA90
}

class CTCThingOwner { public:                                      // interface 0x21
    virtual void v00();
    unsigned char _pad04[0x8];
    std::list<CIntelligentPointer<CThing> > ObjectList;    // +0x0c
};

class CIsBenchWithOnePerson { public:                              // empty predicate
    CIsBenchWithOnePerson() { }
    bool operator()(const CIntelligentPointer<CThing>& bench) const;   // 0x0094B9B0
};

class CAIStateGroup_SitInPub {
public:
    virtual void v00();
private:
    CThingObject* GetBenchWithOnePersonSittingThere(const CThingBuilding& building) const;
};

CThingObject* CAIStateGroup_SitInPub::GetBenchWithOnePersonSittingThere(const CThingBuilding& building) const
{
    const CTCThingOwner* owner;
    if (building.GetInterface(&owner, 0x21)) {
        std::list<CIntelligentPointer<CThing> >::const_iterator it =
            std::find_if(owner->ObjectList.begin(), owner->ObjectList.end(), CIsBenchWithOnePerson());
        if (it != owner->ObjectList.end())
            return static_cast<CThingObject*>(it->GetPItem());
    }
    return 0;
}