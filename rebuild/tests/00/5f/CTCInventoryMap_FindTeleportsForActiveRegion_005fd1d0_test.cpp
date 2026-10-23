// 005fd1d0 behaviour test: CTCInventoryMap::FindTeleportsForActiveRegion (control flow + data of the final.cpp body)
#include <stdio.h>
#include <stdlib.h>
#include <new>
struct __false_type { __false_type() { } };
struct C2DCoordI {
    long X;
    long Y;
    C2DCoordI() {}
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct CCoordVector {
    C2DCoordI* _M_start;
    C2DCoordI* _M_finish;
    C2DCoordI* _M_end_of_storage;
    C2DCoordI* begin() { return _M_start; }
    C2DCoordI* end() { return _M_finish; }
    static C2DCoordI* copy(C2DCoordI* first, C2DCoordI* last, C2DCoordI* result)
    {
        for (long n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
        return result;
    }
    C2DCoordI* erase(C2DCoordI* first, C2DCoordI* last)
    {
        C2DCoordI* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(C2DCoordI* position, const C2DCoordI& x, const __false_type&, unsigned long n, bool)
    {
        long size = _M_finish - _M_start;
        long len = size + (size > (long)n ? size : (long)n);
        C2DCoordI* p = (C2DCoordI*)malloc(len * sizeof(C2DCoordI));
        C2DCoordI* r = copy(_M_start, position, p);
        for (unsigned long k = 0; k < n; ++k) *r++ = x;
        free(_M_start);
        _M_start = p; _M_finish = r; _M_end_of_storage = p + len;
    }
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
static int g_liveNodes;
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct _STLP_alloc_proxy { _List_node<T>* _M_data; _STLP_alloc_proxy(_List_node<T>* p) : _M_data(p) {} };
template <class T> struct list {
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<T> _M_node;
    list() : _M_node(0) { _Node* node = (_Node*)malloc(sizeof(_Node)); ++g_liveNodes; node->_M_next = node; node->_M_prev = node; _M_node._M_data = node; }
    ~list() { clear(); free(_M_node._M_data); --g_liveNodes; }
    void clear()
    {
        _Node* cur = _M_node._M_data->_M_next;
        while (cur != _M_node._M_data) { _Node* tmp = cur; cur = cur->_M_next; free(tmp); --g_liveNodes; }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    void push_back(const T& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node)); ++g_liveNodes;
        tmp->_M_data = x;
        _Node* n = _M_node._M_data; _Node* p = n->_M_prev;
        tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;
    }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            const SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
};
struct C3DVector { float X, Y, Z; };
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    C3DVector Position;
    long Map;
    const C3DVector& GetPosition() const { return Position; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long kind) const
    {
        long key = kind;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || kind < it->type ? end : it)->component;
    }
    template<class T> T* PeekTC(long kind) const
    {
        T* out = 0;
        if (HasTC(kind))
            out = (T*)GetTC(kind);
        return out;
    }
};
struct CTCTeleporter { bool Enabled; };
struct CRegion {
    long* LinkedMapsStart;
    long* LinkedMapsFinish;
    long* LinkedMapsEnd;
    unsigned long size() const { return LinkedMapsFinish - LinkedMapsStart; }
};
static CRegion g_regions[3];
struct CWorldMap {
    const CRegion& GetRegion(long index) { return g_regions[index]; }
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const { return C2DCoordI(pos.X + map * 100, pos.Y - map); }
};
struct CThingFilter_HasTC { long m_type; CThingFilter_HasTC(long type) : m_type(type) { } };
struct CThingFilter_InMap {
    const CWorldMap* m_worldMap;
    long m_map;
    CThingFilter_InMap(const CWorldMap& worldMap, long map) : m_worldMap(&worldMap), m_map(map) { }
};
struct CFilter_Bind2 {
    const CThingFilter_HasTC* m_first;
    const CThingFilter_InMap* m_second;
    CFilter_Bind2(const CThingFilter_HasTC& first, const CThingFilter_InMap& second) : m_first(&first), m_second(&second) { }
};
inline CFilter_Bind2 operator&&(const CThingFilter_HasTC& a, const CThingFilter_InMap& b) { return CFilter_Bind2(a, b); }
static CThing* g_things[8];
static int g_thingCount;
struct CThingSearchTools {
    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out)
    {
        for (int i = 0; i < g_thingCount; ++i)
            if (g_things[i]->HasTC(filter.m_first->m_type) && g_things[i]->Map == filter.m_second->m_map)
                out.push_back(g_things[i]);
    }
};
struct CWorld {
    CWorldMap* Map;
    CThingSearchTools* PThingSearchTools;
    virtual CWorldMap* PeekWorldMap() { return Map; }
    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out) { PThingSearchTools->GetAllThings(filter, out); }
};
static CWorld* g_world;
struct CTCInventoryMap {
    CCoordVector  ActiveRegionTeleportPositions;
    long          ActiveRegionIndex;
    CWorld* GetWorld() const { return g_world; }
    void FindTeleportsForActiveRegion();
};
void CTCInventoryMap::FindTeleportsForActiveRegion()
{
    ActiveRegionTeleportPositions.clear();
    if (ActiveRegionIndex > 0) {
        CWorldMap* worldMap = GetWorld()->PeekWorldMap();
        const CRegion& region = GetWorld()->PeekWorldMap()->GetRegion(ActiveRegionIndex);
        for (unsigned long i = 0; i < region.size(); ++i) {
            list<CThing*> things;
            GetWorld()->GetAllThings(CThingFilter_HasTC(0xdd) && CThingFilter_InMap(*worldMap, region.LinkedMapsStart[i]), things);
            for (_List_node<CThing*>* node = things._M_node._M_data->_M_next; node != things._M_node._M_data; node = node->_M_next) {
                CThing* thing = node->_M_data;
                CTCTeleporter* teleporter = thing->PeekTC<CTCTeleporter>(0xdd);
                if (teleporter->Enabled) {
                    const C3DVector& pos = thing->GetPosition();
                    C2DCoordI coord((long)pos.X, (long)pos.Y);
                    C2DCoordI local = worldMap->ConvertWorldMapCoordsToLocalCoords(region.LinkedMapsStart[i], coord);
                    ActiveRegionTeleportPositions.push_back(local);
                }
            }
        }
    }
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
struct Teleport {
    CThing thing; CTCTeleporter tc; CInterfaceMap::SEntry entry;
    void Init(long map, float x, float y, bool enabled, bool hasTC)
    {
        for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
        if (hasTC) thing.m_presentBits[0xdd >> 5] |= 1UL << (0xdd & 31);
        entry.type = 0xdd; entry.component = (CTCBase*)&tc;
        thing.m_interfaces.first = &entry; thing.m_interfaces.last = &entry + 1;
        thing.Position.X = x; thing.Position.Y = y; thing.Position.Z = 0; thing.Map = map;
        tc.Enabled = enabled;
    }
};
int main()
{
    CWorldMap worldMap; CThingSearchTools tools;
    CWorld world; world.Map = &worldMap; world.PThingSearchTools = &tools; g_world = &world;
    long maps1[2] = { 4, 7 };
    g_regions[1].LinkedMapsStart = maps1; g_regions[1].LinkedMapsFinish = maps1 + 2; g_regions[1].LinkedMapsEnd = maps1 + 2;
    g_regions[2].LinkedMapsStart = maps1; g_regions[2].LinkedMapsFinish = maps1; g_regions[2].LinkedMapsEnd = maps1;
    Teleport t[5];
    t[0].Init(4, 10.7f, 20.2f, true, true);
    t[1].Init(7, -3.9f, 5.5f, true, true);
    t[2].Init(4, 1, 1, false, true);   // disabled
    t[3].Init(9, 2, 2, true, true);    // other map
    t[4].Init(7, 8, 9, true, true);
    for (int i = 0; i < 5; ++i) g_things[i] = &t[i].thing;
    g_thingCount = 5;
    CTCInventoryMap inv;
    C2DCoordI stale[1] = { C2DCoordI(99, 99) };
    inv.ActiveRegionTeleportPositions._M_start = (C2DCoordI*)malloc(sizeof(C2DCoordI) * 2);
    inv.ActiveRegionTeleportPositions._M_start[0] = stale[0];
    inv.ActiveRegionTeleportPositions._M_finish = inv.ActiveRegionTeleportPositions._M_start + 1;
    inv.ActiveRegionTeleportPositions._M_end_of_storage = inv.ActiveRegionTeleportPositions._M_start + 2;

    inv.ActiveRegionIndex = 1;
    inv.FindTeleportsForActiveRegion();
    CCoordVector& v = inv.ActiveRegionTeleportPositions;
    Check("count", v._M_finish - v._M_start == 3);
    Check("first", v._M_finish - v._M_start == 3 && v._M_start[0].X == 410 && v._M_start[0].Y == 16);
    Check("second (truncates toward zero)", v._M_finish - v._M_start == 3 && v._M_start[1].X == 697 && v._M_start[1].Y == -2);
    Check("third", v._M_finish - v._M_start == 3 && v._M_start[2].X == 708 && v._M_start[2].Y == 2);
    Check("lists freed", g_liveNodes == 0);
    // empty region: cleared, nothing added
    inv.ActiveRegionIndex = 2;
    inv.FindTeleportsForActiveRegion();
    Check("empty region", v._M_finish == v._M_start);
    // inactive index: just cleared
    inv.ActiveRegionIndex = 1; inv.FindTeleportsForActiveRegion();
    inv.ActiveRegionIndex = 0; inv.FindTeleportsForActiveRegion();
    Check("index 0", v._M_finish == v._M_start && g_liveNodes == 0);
    if (g_fail) return 1;
    printf("005fd1d0_TEST PASS\n");
    return 0;
}