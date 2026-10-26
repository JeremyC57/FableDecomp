// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) -- append every thing of type TThing in the mapwho cells under the area that passes the filter to the result list, each thing visited once per search stamp; instance <CThing, std::list<CThing*>, CThingFilter_None>
// Leaf: GetAllThingsInRoughArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079BF30, length 556 bytes (`ret 0xc`).
// Identity (sweep3): Ego_r/FableWin name this instance ??$GetAllThingsInRoughArea@VCThing@@V?$list@PAVCThing@@...VCThingFilter_None@@@CThingSearchTools@@QBEX....
//   The landed name FindThingsInRoughArea has no PDB match: CThingSearchTools' rough-area collector of things is
//   GetAllThingsInRoughArea (ForEachInRoughArea applies a functor, GetAllTCsInRoughArea / GetAllTCDsInRoughArea collect components).
//   The catalog label "GetPBestThingInRoughArea<CThingFilter_HasDefName,CThingCompare_Nearest,CThing>" was propagated across byte-identical instances.
// Callees: GetMapsInArea 0x0049C6F0 (forwarder), GetCellExtents 0x0049C700 (C2DBoxI by value), memmove 0x00BFEAE6 (scratch-list clear), malloc 0x00BFEA0E (list node).
// Globals: search stamps 0x013B89A8 (+0 searches, +4 found, +8 stamp), mapwho level count 0x013BA848, level shift table 0x0137DADC (16-byte stride).
// Layout: engine/CThingSearchTools.h gives AreaMaps (+0x0c) the PDB's 16-byte vector; the retail STLport vector is 12 bytes, so the
//   search tools, the thing map (+0x04 level infos, +0x14 cell vectors) and the cell entries are modelled locally.
// Shape: the type test copies the mask into a local (`(flags & mask) == mask`); inline filters are written out in the TU.

#include <string.h>
#include <stdlib.h>
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
// Retail CThing: ThingTypeFlags at +0x6c (THING_TYPE_* inheritance bits); TypeMask selects the searched class.
class CThing {                                        // PDB def type: class
public:
    enum { TypeMask = 0 };
    unsigned char _pad00[0x60];
    struct CTCPhysicsBase* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                          // +0x6c
};
// The type test: the mask is copied into a local, so the CThing (mask 0) test folds late and keeps retail's
// `xor dl,dl ; jne`, and a byte-sized mask compares through the low byte (`and edx,0Fh ; cmp dl,0Fh`).
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::TypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
inline void* operator new(unsigned int, void* where) { return where; }
namespace std { struct __true_type { __true_type() {} }; }
using std::__true_type;
class CThingFilter_None {
public:
    bool operator()(const CThing&) const { return true; }
};
// One mapwho cell list node: {entry, next}; the entry holds the thing (+4) and its search stamp (+0x4c).
struct CThingMapEntry {
    unsigned char _pad00[4];
    CThing*       Thing;                                   // +0x04
    unsigned char _pad08[0x4c - 0x08];
    unsigned long SearchStamp;                             // +0x4c
};
struct CThingMapNode {
    CThingMapEntry* Entry;
    CThingMapNode*  Next;
};
// A thing map: per-level grid infos (20 bytes, width at +4) at +0x04, per-level cell vectors at +0x14.
struct CMapwhoLevelInfo { long SingleCell; long Width; long Height; long _pad[2]; };
struct CCellVector { CThingMapNode** _M_start; CThingMapNode** _M_finish; CThingMapNode** _M_end_of_storage; };
struct CThingMap {
    void*             _vfptr;
    CMapwhoLevelInfo* LevelInfo;                           // +0x04
    CMapwhoLevelInfo* LevelInfoEnd;
    CMapwhoLevelInfo* LevelInfoCap;
    void*             _pad10;
    CCellVector*      Cells;                               // +0x14
    CThingMapNode** GetCellPtr(unsigned char level, long x, long y) const
    {
        return &Cells[level]._M_start[y * LevelInfo[level].Width + x];
    }
};
struct SMapwhoLevel { long Shift; long _pad[3]; };
extern SMapwhoLevel g_MapwhoLevels[];                      // 0x0137DADC
extern long g_NumMapwhoLevels;                             // 0x013BA848
struct SSearchStamps { unsigned long Searches; unsigned long Found; unsigned long Stamp; };
extern SSearchStamps g_SearchStamps;                       // 0x013B89A8
// STLport vector<CThingMap*> (the search tools' scratch map list); clear() is erase(begin, end) over memmove.
struct CMapVector {
    CThingMap** _M_start;
    CThingMap** _M_finish;
    CThingMap** _M_end_of_storage;
    static CThingMap** CopyDown(CThingMap** src, CThingMap** srcEnd, CThingMap** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long count = (const char*)srcEnd - (const char*)src;
        return (CThingMap**)((char*)memmove(dest, src, count) + count);
    }
    void erase(CThingMap** first, CThingMap** last) { _M_finish = CopyDown(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
};
// STLport 4.6 std::list<_Tp>: push_back is insert(end(), x) over an iterator with a declared copy constructor; the node
// (_M_next, _M_prev, _M_data) comes from the allocator, i.e. the CRT malloc thunk 0x00BFEA0E.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    struct _Node { _Node* _M_next; _Node* _M_prev; _Tp _M_data; };
    struct iterator {
        _Node* _M_node;
        iterator(_Node* __x) : _M_node(__x) {}
        iterator(const iterator& __x) : _M_node(__x._M_node) {}
    };
    _Node* _M_node;
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _Node* __n = __position._M_node;
        _Node* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}
// CThingSearchTools: engine/CThingSearchTools.h names +0x0c "AreaMaps" (vector<NThingMap::CThingMap*>, 16 bytes in
// the PDB); retail's STLport vector is 12 bytes and only the scratch-vector role matters here, so a local model.
struct CThingSearchTools {
    void*      WorldMap;                                   // +0x00
    void*      PThingMapManager;                           // +0x04
    void*      ThingManager;                               // +0x08
    CMapVector AreaMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template <class TThing, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things);
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things)
{
    CMapVector* maps = &AreaMaps;
    GetMapsInArea(area, maps);
    ++g_SearchStamps.Searches;
    g_SearchStamps.Found = 0;
    ++g_SearchStamps.Stamp;
    for (CThingMap** it = maps->_M_start; it != AreaMaps._M_finish; ++it) {
        CThingMap* map = *it;
        C2DBoxI extents = GetCellExtents(map, area);
        for (long level = 0; level < g_NumMapwhoLevels; ++level) {
            long shift = g_MapwhoLevels[level].Shift;
            C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1,
                        ((extents.Y1 - 1) >> shift) + 1);
            long width = box.X1 - box.X0;
            if (width == 0 || box.Y1 - box.Y0 == 0)
                continue;
            for (long y = box.Y0; y < box.Y1; ++y) {
                CThingMapNode** cell = map->GetCellPtr((unsigned char)level, box.X0, y);
                for (long x = box.X0; x < box.X1; ++x, ++cell) {
                    for (CThingMapNode* node = *cell; node; node = node->Next) {
                        CThingMapEntry* entry = node->Entry;
                        if (entry->SearchStamp < g_SearchStamps.Stamp) {
                            entry->SearchStamp = g_SearchStamps.Stamp;
                            CThing* thing = entry->Thing;
                            if (IsThingOfType<TThing>(thing) && filter(*static_cast<TThing*>(thing))) {
                                TThing* found = static_cast<TThing*>(thing);
                                things.push_back(found);
                            }
                        }
                    }
                }
            }
        }
    }
    maps->clear();
}
template void CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<CThing*>, CThingFilter_None >(const C2DBoxI& area, const CThingFilter_None& filter, std::list<CThing*>& things);
