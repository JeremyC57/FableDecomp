//   (1) STLport's out-of-line `__find(first, last, val, __true_type)` gets its real four-way-unrolled body in the TU under
//       auto_inline(off) + noinline. VC7.1 then homes the empty tag in the live `map` slot (GOTCHAS 24l recipe).
//   (2) A named `CThingMapInfo* pinfo = &info;` declared before `--info.RefCount` splits the decrement (load / dec / &info
//       store / store) as retail does.
//   (3) ThingMaps operator[] is STLport's `*(begin() + i)`: one more inline level loads Dimensions.X before _M_start.
//   (4) `__find(ActiveThingMaps.begin(), ActiveThingMaps.end(), ...)` through inline begin()/end() loads _M_start into ecx
//       before the pushes.
//   (5) An inline `CWorldMap::GetGameMaps()` accessor materialises the record address (`lea ecx,[eax+ecx*8]`) before
//       the TLY load.
//   The vector erase tests `_M_finish != next`.
// CThingMapManager::RemoveGameMap(long map) -- release every 128x128 thing-map cell the game map's area touches; a cell whose reference count drops to zero leaves the active list and its CThingMap is deleted; then the game map leaves the active set
// Leaf: ?RemoveGameMap@CThingMapManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00638340, length 334 bytes, `ret 4`.
//   Name from Ego_r.pdb (`?RemoveGameMap@CThingMapManager@NThingMap@@QAEXJ@Z`).
// Callees: PWorld virtual slot 12 (+0x30, the world map), STLport __find(first, last, const T&, tag) @ 0x006387F0 (same-TU body)
//   (`__fastcall`), memmove @ 0x00BFEAE6, the cell map's virtual deleting destructor (slot 0, `push 1`), the active set's
//   erase(iterator, iterator) @ 0x00638EE0 (iterators by value).
// Layout (retail): as 0x006381A0 -- ThingMaps +0x08, ActiveThingMaps +0x14, ActiveGameMaps +0x20 (STLport set<long>:
//   header node pointer first; node left +8, right +0xc, value +0x10), Dimensions.X +0x30; the world map's game-map
//   records (0x48 bytes, area box first) at +0x20.
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#define __forceinline inline
#include <stdint.h>
#endif
#include <stdio.h>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DExtentsI { public: long X, Y; };
struct CGameMapRecord {
    C2DBoxI       Area;                                 // +0x00
    unsigned char _pad10[0x48 - 0x10];
};
class CWorldMap { public:
    unsigned char   _pad00[0x20];
    CGameMapRecord* GameMaps;                           // +0x20
    CGameMapRecord* GetGameMaps() const { return GameMaps; }
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                   // slot 12 (+0x30)
};

struct CThingMap {
    virtual ~CThingMap();
};
struct CThingMapInfo {
    CThingMap* PThingMap;
    long       RefCount;
};

#include <string.h>
struct __true_type { __true_type() {} };
#pragma auto_inline(off)
__declspec(noinline) CThingMapInfo** __fastcall __find(CThingMapInfo** first, CThingMapInfo** last, CThingMapInfo* const& value, const __true_type&)   // 0x006387F0
{
    int trip_count = (last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
struct CThingMapInfoPtrVector {                         // STLport vector<CThingMapInfo*>
    CThingMapInfo** _M_start;
    CThingMapInfo** _M_finish;
    CThingMapInfo** _M_end_of_storage;
    CThingMapInfo** begin() { return _M_start; }
    CThingMapInfo** end() { return _M_finish; }
    CThingMapInfo** erase(CThingMapInfo** position)
    {
        CThingMapInfo** next = position + 1;
        if (_M_finish != next)
            memmove(position, next, (char*)_M_finish - (char*)next);
        --_M_finish;
        return position;
    }
};
struct CThingMapInfoVector {
    CThingMapInfo* _M_start;
    CThingMapInfo* _M_finish;
    CThingMapInfo* _M_end_of_storage;
    CThingMapInfo* begin() { return _M_start; }
    CThingMapInfo& operator[](unsigned int i) { return *(begin() + i); }
};
struct CLongSet {                                       // STLport set<long>
    struct _Node { long _M_color; _Node* _M_parent; _Node* _M_left; _Node* _M_right; long _M_value; };
    struct iterator {
        _Node* _M_node;
        iterator(_Node* node) : _M_node(node) {}
        iterator(const iterator& other) : _M_node(other._M_node) {}
    };
    _Node* _M_header;
    unsigned int _M_node_count;
    unsigned int _M_key_compare;
    void erase(iterator first, iterator last);          // 0x00638EE0
    struct iterator_pair {
        iterator first, second;
        iterator_pair(const iterator& a, const iterator& b) : first(a), second(b) {}
    };
    __forceinline iterator lower_bound(const long& key)
    {
        _Node* y = _M_header;
        _Node* x = _M_header->_M_parent;
        while (x)
            if (!(x->_M_value < key)) { y = x; x = x->_M_left; } else x = x->_M_right;
        return iterator(y);
    }
    __forceinline iterator upper_bound(const long& key)
    {
        _Node* y = _M_header;
        _Node* x = _M_header->_M_parent;
        while (x)
            if (key < x->_M_value) { y = x; x = x->_M_left; } else x = x->_M_right;
        return iterator(y);
    }
    __forceinline iterator_pair equal_range(const long& key)
    {
        return iterator_pair(lower_bound(key), upper_bound(key));
    }
    __forceinline void erase(const long& key)
    {
        iterator_pair range = equal_range(key);
        erase(range.first, range.second);
    }
};

struct CThingMapManager {
    void*                   __vftable;                  // +0x00
    CWorld*                 PWorld;                     // +0x04
    CThingMapInfoVector     ThingMaps;                  // +0x08
    CThingMapInfoPtrVector  ActiveThingMaps;            // +0x14
    CLongSet                ActiveGameMaps;             // +0x20
    long                    NoThings;                   // +0x2c
    C2DExtentsI             Dimensions;                 // +0x30
    void RemoveGameMap(long map);
};

void CThingMapManager::RemoveGameMap(long map)
{
    const C2DBoxI& area = PWorld->GetWorldMap()->GetGameMaps()[map].Area;
    for (long y = area.TLY / 128; y <= area.BRY / 128; ++y) {
        for (long x = area.TLX / 128; x <= area.BRX / 128; ++x) {
            CThingMapInfo& info = ThingMaps[y * Dimensions.X + x];
            CThingMapInfo* pinfo = &info;
            if (--info.RefCount == 0) {
                ActiveThingMaps.erase(__find(ActiveThingMaps.begin(), ActiveThingMaps.end(), pinfo, __true_type()));
                delete info.PThingMap;
                info.PThingMap = 0;
            }
        }
    }
    ActiveGameMaps.erase(map);
}

// ---- test stubs ----
static int g_deleted;
static CLongSet::_Node* g_eraseFirst; static CLongSet::_Node* g_eraseLast; static int g_erases;
void CWorld::v00() { } void CWorld::v01() { } void CWorld::v02() { } void CWorld::v03() { } void CWorld::v04() { }
void CWorld::v05() { } void CWorld::v06() { } void CWorld::v07() { } void CWorld::v08() { } void CWorld::v09() { }
void CWorld::v10() { } void CWorld::v11() { }
static CWorldMap g_worldMap;
CWorldMap* CWorld::GetWorldMap() { return &g_worldMap; }
CThingMap::~CThingMap() { ++g_deleted; }
void CLongSet::erase(iterator first, iterator last) { ++g_erases; g_eraseFirst = first._M_node; g_eraseLast = last._M_node; }
int main()
{
    int ok = 1;
    static CGameMapRecord records[3];
    records[2].Area.TLX = 0; records[2].Area.TLY = 0; records[2].Area.BRX = 200; records[2].Area.BRY = 130;
    g_worldMap.GameMaps = records;
    static CThingMapInfo infos[9];
    for (int k = 0; k < 9; ++k) { infos[k].PThingMap = new CThingMap; infos[k].RefCount = 2; }
    infos[0].RefCount = 1; infos[3].RefCount = 1; infos[4].RefCount = 1;   // cell 1 keeps one reference
    static CThingMapInfo* active[8];
    active[0] = &infos[4]; active[1] = &infos[7]; active[2] = &infos[0]; active[3] = &infos[3];
    static CWorld world;
    static CLongSet::_Node header, node;
    node._M_value = 2; node._M_left = 0; node._M_right = 0;
    header._M_parent = &node;
    static CThingMapManager mgr;
    mgr.PWorld = &world;
    mgr.ThingMaps._M_start = infos; mgr.ThingMaps._M_finish = infos + 9;
    mgr.ActiveThingMaps._M_start = active; mgr.ActiveThingMaps._M_finish = active + 4; mgr.ActiveThingMaps._M_end_of_storage = active + 8;
    mgr.ActiveGameMaps._M_header = &header;
    mgr.Dimensions.X = 3; mgr.Dimensions.Y = 3;
    mgr.RemoveGameMap(2);
    if (g_deleted != 3 || infos[0].PThingMap || infos[3].PThingMap || infos[4].PThingMap || !infos[1].PThingMap
        || infos[1].RefCount != 1 || infos[0].RefCount != 0 || infos[2].RefCount != 2 || infos[6].RefCount != 2
        || mgr.ActiveThingMaps._M_finish != active + 1 || active[0] != &infos[7]
        || g_erases != 1 || g_eraseFirst != &node || g_eraseLast != &header) { ok = 0; printf("s1 %d %d\n", g_deleted, (int)(mgr.ActiveThingMaps._M_finish - active)); }
    if (ok) printf("00638340_TEST PASS\n");
    return ok ? 0 : 1;
}