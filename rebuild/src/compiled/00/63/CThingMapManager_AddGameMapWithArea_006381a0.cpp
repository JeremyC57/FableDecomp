// CThingMapManager::AddGameMapWithArea(long map, const C2DBoxI& area) -- reference every 128x128 thing-map cell the area touches, creating (and activating) the cells not yet in use, then record the game map as active
// Leaf: ?AddGameMapWithArea@CThingMapManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006381A0, length 348 bytes, `ret 8`.
//   Name and arguments from the catalog / engine_api (`CThingMapManager::AddGameMapWithArea(long, C2DBoxI*)`).
// Callees: operator new @ 0x00BFEA1A, CThingMap::CThingMap(CThingMapManager*, const C2DBoxI&) @ 0x0085D370, the active
//   list's STLport _M_insert_overflow(pos, x, const __true_type&, size_type, bool) @ 0x00639090, the active game-map
//   set's insert_unique(const long&) @ 0x00512DA0 (returns pair<iterator, bool> by hidden pointer).
// Layout (retail; rebuild/include/engine/CThingMapManager.h carries the Ego_r.pdb offsets, whose STL containers have a
//   trailing allocator, so it does not fit): vptr +0, PWorld +4, ThingMaps +0x08 (STLport vector, 12 bytes; 8-byte
//   CThingMapInfo {PThingMap, RefCount}), ActiveThingMaps +0x14, ActiveGameMaps +0x20 (set<long>, 12 bytes), NoThings
//   +0x2c, Dimensions {X +0x30, Y +0x34} -- retail indexes the cells as y * Dimensions.X + x.
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
struct C2DExtentsI { long X, Y; };

struct CThingMapManager;
struct CThingMap {
    unsigned char _body[0x34];
    CThingMap(CThingMapManager* manager, const C2DBoxI& area);    // 0x0085D370
};
struct CThingMapInfo {
    CThingMap* PThingMap;
    long       RefCount;
};

struct __true_type { __true_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
void* operator new(unsigned int size);

struct CThingMapInfoPtrVector {                         // STLport vector<CThingMapInfo*>
    CThingMapInfo** _M_start;
    CThingMapInfo** _M_finish;
    CThingMapInfo** _M_end_of_storage;
    template <class Tag> void _M_insert_overflow(CThingMapInfo** position, CThingMapInfo* const& x, const Tag&, unsigned long fill_len, bool atend);   // 0x00639090
    void push_back(CThingMapInfo* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CThingMapInfo*(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
// Same-TU body of the STLport CThingMapInfoPtrVector::_M_insert_overflow instantiation (tag unused).
template <class Tag> __declspec(noinline) void CThingMapInfoPtrVector::_M_insert_overflow(CThingMapInfo** pos, CThingMapInfo* const& x, const Tag&, unsigned long n, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    CThingMapInfo** ns = len ? (CThingMapInfo**)malloc(len * sizeof(CThingMapInfo*)) : 0;
    CThingMapInfo** nf = ns;
    CThingMapInfo** p;
    for (p = _M_start; p != pos; ++p) { *nf++ = *p; }
    for (; n != 0; --n) { *nf++ = x; }
    if (!atend)
        for (p = pos; p != _M_finish; ++p) { *nf++ = *p; }
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
struct CThingMapInfoVector {                            // STLport vector<CThingMapInfo>
    CThingMapInfo* _M_start;
    CThingMapInfo* _M_finish;
    CThingMapInfo* _M_end_of_storage;
    CThingMapInfo& operator[](unsigned int i) { return _M_start[i]; }
};
struct CLongSet {                                       // STLport set<long>
    struct iterator { void* _M_node; };
    struct pair_iterator_bool { iterator first; bool second; };
    void* _M_header;
    void* _M_node_count;
    void* _M_key_compare;
    pair_iterator_bool insert_unique(const long& value);   // 0x00512DA0 (_Rb_tree::insert_unique)
    pair_iterator_bool insert(long value) { return insert_unique(value); }
};

struct CWorld;
struct CThingMapManager {
    void*                   __vftable;                  // +0x00
    CWorld*                 PWorld;                     // +0x04
    CThingMapInfoVector     ThingMaps;                  // +0x08
    CThingMapInfoPtrVector  ActiveThingMaps;            // +0x14
    CLongSet                ActiveGameMaps;             // +0x20
    long                    NoThings;                   // +0x2c
    C2DExtentsI             Dimensions;                 // +0x30
    void AddGameMapWithArea(long map, const C2DBoxI& area);
};

void CThingMapManager::AddGameMapWithArea(long map, const C2DBoxI& area)
{
    for (long y = area.TLY / 128; y <= area.BRY / 128; ++y) {
        for (long x = area.TLX / 128; x <= area.BRX / 128; ++x) {
            CThingMapInfo* info = &ThingMaps[y * Dimensions.X + x];
            if (info->RefCount == 0) {
                info->PThingMap = new CThingMap(this, C2DBoxI(x * 128, y * 128, x * 128 + 128, y * 128 + 128));
                ActiveThingMaps.push_back(info);
            }
            ++info->RefCount;
        }
    }
    ActiveGameMaps.insert(map);
}