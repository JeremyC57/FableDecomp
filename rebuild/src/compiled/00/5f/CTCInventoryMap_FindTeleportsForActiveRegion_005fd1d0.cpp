// CTCInventoryMap::FindTeleportsForActiveRegion() -- rebuilds the teleport marker list for the active region: for every map linked to the region, every enabled teleporter (TC 0xDD) in that map contributes its position converted to the map's local coordinates
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FD1D0, 564 bytes, plain `ret`. Speed TU (no pragmas; RTM `and esp,-8` frame for the
//   8-byte filter/coordinate structs whose addresses are taken in the loop).
// Callees: CTCBase::PeekWorld() const 0x00686A80, CWorld slot 13 (+0x34) PeekWorldMap, CWorldMap::GetRegion 0x004FC180,
//   malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list), CThingSearchTools::GetAllThings<CThing, list,
//   CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>> 0x00617780 (same-TU stand-in), LowerBound const 0x004365B0,
//   CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, CWorldMap::ConvertWorldMapCoordsToLocalCoords 0x004FC3C0,
//   vector<C2DCoordI>::_M_insert_overflow 0x00618550.
// Layout: local view -- CTCInventoryMap teleport positions vector<C2DCoordI> +0x1E8 (12 bytes), ActiveRegionIndex +0x238
//   (landed sibling FindQuestsForActiveRegion 0x005FD0F0); CWorld +0x20 thing search tools (engine/CWorld.h); CRegion's
//   linked-map vector<long> +0x00; teleporter component (0xDD) enabled flag +0x10.
#include <new>
struct __false_type { __false_type() { } };
class C2DCoordI { public:
    long X;
    long Y;
    C2DCoordI() {}
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct CCoordVector {                              // std::vector<C2DCoordI> (12 bytes)
    C2DCoordI* _M_start;
    C2DCoordI* _M_finish;
    C2DCoordI* _M_end_of_storage;
    C2DCoordI* begin() { return _M_start; }
    C2DCoordI* end() { return _M_finish; }
    static C2DCoordI* copy(C2DCoordI* first, C2DCoordI* last, C2DCoordI* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    C2DCoordI* erase(C2DCoordI* first, C2DCoordI* last)
    {
        C2DCoordI* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(C2DCoordI* position, const C2DCoordI& x, const __false_type& tag, unsigned long n, bool atEnd);   // 0x00618550
    void push_back(const C2DCoordI& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C2DCoordI(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
template <class T>
struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T>
struct _STLP_alloc_proxy {
    _List_node<T>* _M_data;
    _STLP_alloc_proxy(_List_node<T>* p) : _M_data(p) {}
};
template <class T>
struct list {
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<T> _M_node;
    list() : _M_node(0)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node._M_data = node;
    }
    ~list()
    {
        clear();
        free(_M_node._M_data);
    }
    void clear()
    {
        _Node* cur = _M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    void push_back(const T& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        tmp->_M_data = x;
        _Node* n = _M_node._M_data;
        _Node* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
};

class CTCBase;
struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const; // 0x004365B0
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        const SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
class C3DVector { public: float X, Y, Z; };
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20 bitset of interface types
    CInterfaceMap m_interfaces;                       // +0x44
    const C3DVector& GetPos() const;             // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long kind) const
    {
        long key = kind;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || kind < it->type ? end : it)->component;
    }
    template<class T> __forceinline T* PeekTC(long kind) const
    {
        T* out;
        if (HasTC(kind))
            out = (T*)GetTC(kind);
        return out;
    }
    template<class T> __forceinline bool GetInterface(T** out, long kind) const
    {
        if (HasTC(kind)) {
            long key = kind;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCTeleporter { public:
    unsigned char _pad00[0x10];
    bool Enabled;                                     // +0x10
};
class CRegion { public:
    long* LinkedMapsStart;                            // +0x00 vector<long>
    long* LinkedMapsFinish;
    long* LinkedMapsEnd;
    unsigned long size() const { return LinkedMapsFinish - LinkedMapsStart; }
};
class CWorldMap { public:
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const;   // 0x004FC3C0
public:
    const CRegion& GetRegion(long) const;   // 0x004FC180
};
class CThingFilter_HasTC { public:
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};
class CThingFilter_InMap { public:
    const CWorldMap* m_worldMap;
    long m_map;
    CThingFilter_InMap(const CWorldMap& worldMap, long map) : m_worldMap(&worldMap), m_map(map) { }
};
struct CFilter_Bind2 {                                // CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>
    const CThingFilter_HasTC* m_first;
    const CThingFilter_InMap* m_second;
    CFilter_Bind2(const CThingFilter_HasTC& first, const CThingFilter_InMap& second) : m_first(&first), m_second(&second) { }
};
inline CFilter_Bind2 operator&&(const CThingFilter_HasTC& a, const CThingFilter_InMap& b) { return CFilter_Bind2(a, b); }
CThing* PeekSearchCandidate();
class CThingSearchTools { public:
    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out);   // 0x00617780
};
#pragma auto_inline(off)
// Same-TU stand-in for GetAllThings<CThing, list, CFilter_Bind2<HasTC, InMap>> 0x00617780: it appends through the out
// list without writing or keeping its node word (retail keeps the list's node in esi and reuses the list's slot).
void CThingSearchTools::GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out)
{
    CThing* thing = PeekSearchCandidate();
    if (thing && filter.m_first->m_type)
        out.push_back(thing);
}
#pragma auto_inline(on)
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                 // slot 12
    virtual const CWorldMap* PeekWorldMap() const;    // slot 13 (+0x34) (FableWin.pdb: const)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;             // +0x20
    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out) const { PThingSearchTools->GetAllThings(filter, out); }
};
class CTCBase { protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCInventoryMap : public CTCBase { public:
    unsigned char _pad00[0x1e8];
    CCoordVector  ActiveRegionTeleportPositions;      // +0x1e8
    unsigned char _pad1f4[0x238 - 0x1f4];
    long          ActiveRegionIndex;                  // +0x238
    void FindTeleportsForActiveRegion();
};
void CTCInventoryMap::FindTeleportsForActiveRegion()
{
    ActiveRegionTeleportPositions.clear();
    if (ActiveRegionIndex > 0) {
        const CWorldMap* worldMap = PeekWorld().PeekWorldMap();
        const CRegion& region = PeekWorld().PeekWorldMap()->GetRegion(ActiveRegionIndex);
        for (unsigned long i = 0; i < region.size(); ++i) {
            list<CThing*> things;
            PeekWorld().GetAllThings(CThingFilter_HasTC(0xdd) && CThingFilter_InMap(*worldMap, region.LinkedMapsStart[i]), things);
            for (_List_node<CThing*>* node = things._M_node._M_data->_M_next; node != things._M_node._M_data; node = node->_M_next) {
                CThing* thing = node->_M_data;
                CTCTeleporter* teleporter = thing->PeekTC<CTCTeleporter>(0xdd);
                if (teleporter->Enabled) {
                    const C3DVector& pos = thing->GetPos();
                    C2DCoordI coord((long)pos.X, (long)pos.Y);
                    C2DCoordI local = worldMap->ConvertWorldMapCoordsToLocalCoords(region.LinkedMapsStart[i], coord);
                    ActiveRegionTeleportPositions.push_back(local);
                }
            }
        }
    }
}