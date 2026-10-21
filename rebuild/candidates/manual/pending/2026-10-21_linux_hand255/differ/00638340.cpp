// Grade: DIFFER(332v334) -- loops, cell release, active-list find/erase, map delete and the inlined set equal_range erase (upper bound first) match; retail homes find's empty __true_type tag in the LIVE `map` argument slot (lane gap, GOTCHAS 24l/24p: 0x00A76C70 family), materialises the game-map record address (`lea ecx,[eax+ecx*8] ; mov eax,[ecx+4]`, lane folds it) and splits the refcount decrement around the &info temporary store
// CThingMapManager::RemoveGameMap(long map) -- release every 128x128 thing-map cell the game map's area touches; a cell whose reference count drops to zero leaves the active list and its CThingMap is deleted; then the game map leaves the active set
// Leaf: ?RemoveGameMap@CThingMapManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00638340, length 334 bytes, `ret 4`.
//   Name from Ego_r.pdb (`?RemoveGameMap@CThingMapManager@NThingMap@@QAEXJ@Z`).
// Callees: PWorld virtual slot 12 (+0x30, the world map), STLport find(first, last, const T&, tag) @ 0x006387F0
//   (`__fastcall`), memmove @ 0x00BFEAE6, the cell map's virtual deleting destructor (slot 0, `push 1`), the active set's
//   erase(iterator, iterator) @ 0x00638EE0 (iterators by value).
// Layout (retail): as 0x006381A0 -- ThingMaps +0x08, ActiveThingMaps +0x14, ActiveGameMaps +0x20 (STLport set<long>:
//   header node pointer first; node left +8, right +0xc, value +0x10), Dimensions.X +0x30; the world map's game-map
//   records (0x48 bytes, area box first) at +0x20.
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct C2DExtentsI { long X, Y; };
struct CGameMapRecord {
    C2DBoxI       Area;                                 // +0x00
    unsigned char _pad10[0x48 - 0x10];
};
struct CWorldMap {
    unsigned char   _pad00[0x20];
    CGameMapRecord* GameMaps;                           // +0x20
};
struct CWorld {
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
CThingMapInfo** __fastcall find(CThingMapInfo** first, CThingMapInfo** last, CThingMapInfo* const& value, const __true_type&);   // 0x006387F0
struct CThingMapInfoPtrVector {                         // STLport vector<CThingMapInfo*>
    CThingMapInfo** _M_start;
    CThingMapInfo** _M_finish;
    CThingMapInfo** _M_end_of_storage;
    CThingMapInfo** erase(CThingMapInfo** position)
    {
        CThingMapInfo** next = position + 1;
        if (next != _M_finish)
            memmove(position, next, (char*)_M_finish - (char*)next);
        --_M_finish;
        return position;
    }
};
struct CThingMapInfoVector {
    CThingMapInfo* _M_start;
    CThingMapInfo* _M_finish;
    CThingMapInfo* _M_end_of_storage;
    CThingMapInfo& operator[](unsigned int i) { return _M_start[i]; }
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
    const C2DBoxI& area = PWorld->GetWorldMap()->GameMaps[map].Area;
    for (long y = area.TLY / 128; y <= area.BRY / 128; ++y) {
        for (long x = area.TLX / 128; x <= area.BRX / 128; ++x) {
            CThingMapInfo& info = ThingMaps[y * Dimensions.X + x];
            if (--info.RefCount == 0) {
                ActiveThingMaps.erase(find(ActiveThingMaps._M_start, ActiveThingMaps._M_finish, &info, __true_type()));
                delete info.PThingMap;
                info.PThingMap = 0;
            }
        }
    }
    ActiveGameMaps.erase(map);
}
