// CThingSearchTools::GetAllLeafThings(const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>& filter, std::list<const CThingMarker*>& things) const -- (<CThingMarker>) every scan-visible CThingMarker passing both filters, appended to the list
// Leaf: ??$GetAllLeafThings@VCThingMarker@@V?$list@PBVCThingMarker@@V?$allocator@PBVCThingMarker@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_InMap@@@@@CThingSearchTools@@QBEXABV?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_InMap@@@@AAV?$list@PBVCThingMarker@@V?$allocator@PBVCThingMarker@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00617780, 144 bytes, thiscall, `ret 8`.
// Identity (proven): PeekTypeList(11) is CThingMarker's type list (ThingListType 11, as the landed CThingSearchTools rows) and
//   the first filter called is CThingFilter_HasTC::operator() 0x006A80A0; Ego_r.pdb/FableWin.pdb ??$GetAllLeafThings@VCThingMarker@@V?$list@...
//   V?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_InMap@@@@@CThingSearchTools@@QBEX...@Z is the only list + Bind2<CThingFilter_HasTC, InMap>
//   instantiation for CThingMarker; its callers are CTCInventoryMap::FindTeleports 0x005FC620, FindTeleportsForActiveRegion 0x005FD1D0 and CreateMiniMapMarkers 0x00603000. The manifest's "GetAllLeafThings<CThingAICreature, vector<...>>" is propagated.
// Callees: CThingSearchTools::PeekTypeList(long) 0x0049C780, CThingFilter_HasTC::operator()(const CThing&) const 0x006A80A0,
//   CThingFilter_InMap::operator()(const CThing&) const 0x006A7930, malloc 0x00BFEA0E (the list node).
// Shape (family M167, representative 0x00617780): the M111 loop (end() re-read every pass, the inline IsIncludedInThingScans
//   test, push_back as insert(end(), x) with placement new's null test on the node's value) with both filters out of line.
// Header debt: engine/CThingSearchTools.h declares no member functions, so the class is modelled locally with its Ego_r.pdb
//   members to carry the member template's real name.
#include <stdlib.h>
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
// ---- CThing (retail layout): the state bit byte +0x91 = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char StateBits;                              // +0x91
    bool IsIncludedInThingScans() const                  // out of line at 0x0048BC40, inlined here
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
class CThingPhysical : public CThing { };
class CThingCreatureBase : public CThingPhysical { };
class CThingAICreature : public CThingCreatureBase { public: enum { ThingListType = 1 }; };
class CThingBuilding : public CThingPhysical { public: enum { ThingListType = 3 }; };
class CThingMarker : public CThing { public: enum { ThingListType = 0xb }; };
class CThingFilter_HasTC {
public:
    bool operator()(const CThing& thing) const;           // 0x006A80A0
    long Type;                                            // ETCInterfaceType
};
class CThingFilter_HasInformation {
public:
    bool operator()(const CThing& thing) const;           // 0x006A7140
};
class CThingFilter_InMap {
public:
    bool operator()(const CThing& thing) const;           // 0x006A7930
};
template <class TF1, class TF2> class CFilter_Bind2 {     // Ego_r.pdb: F1, F2 (pointers)
public:
    template <class T> __forceinline bool operator()(const T& thing) const { return (*F1)(thing) && (*F2)(thing); }
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
};
// The thing manager's per-type lists: STLport list<CThing*> (node = next, prev, value).
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* node) : _M_node(node) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    iterator end() { return iterator(_M_node); }
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __p = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    _List_node_base* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
class CWorldMap;
class CThingManager;
namespace NThingMap { class CThingMap; }
struct CThingMapManager;
class CThingSearchTools {                                 // Ego_r.pdb members (retail 0x18 bytes)
public:
    CThingList* PeekTypeList(long type) const;            // 0x0049C780
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& things) const;
    CWorldMap* WorldMap;                                  // +0x00
    CThingMapManager* PThingMapManager;                   // +0x04
    CThingManager* ThingManager;                          // +0x08
    std::vector<NThingMap::CThingMap*> AreaMaps;          // +0x0c
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) const
{
    CThingList* list = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = list->begin(); it != list->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            things.push_back((TThing*)thing);
    }
}
template void CThingSearchTools::GetAllLeafThings<CThingMarker, std::list<const CThingMarker*>, CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> >(
    const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>&, std::list<const CThingMarker*>&) const;