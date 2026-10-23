// CThingSearchTools::FindThingsInRoughArea(const C2DBoxI& area, const F& filter, list<T*>& things) -- append every thing of type T in the mapwho cells under the area that passes the filter to the result list, each thing visited once per search stamp; instance <CThingMask47, CThingFilter_OverlappingSphere>
// Leaf: FindThingsInRoughArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785DD0, length 581 bytes (`ret 0xc`).
// Identity: the catalog label "GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer,CThingCompare_Nearest,CThingCreatureBase>" is propagated across byte-identical instances; this body is
//   a list collector (FindThingsInRoughArea into an STLport list<T*>) -- the instance arguments above are read from the body's own type mask and callees.
// Callees: GetMapsInArea 0x0049C6F0 (forwarder), GetCellExtents 0x0049C700 (C2DBoxI by value), memmove 0x00BFEAE6 (scratch-list clear), malloc 0x00BFEA0E (list node); filter/compare: CThingFilter_OverlappingSphere 0x006689E0.
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
struct CThing {
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
struct __true_type { __true_type() {} };
struct CThingMask47 : CThing { enum { TypeMask = 0x47 }; };
struct CThingFilter_OverlappingSphere {
    bool operator()(const CThing* thing) const;   // 0x006689E0
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
// STLport list<CThingMask47*>: push_back is insert(end(), x) over an iterator with a declared copy constructor;
// the node comes from the CRT malloc thunk 0x00BFEA0E.
struct CResultList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CThingMask47* _M_data; };
    struct iterator {
        SNode* _M_node;
        iterator(SNode* node) : _M_node(node) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    SNode* _M_node;
    iterator end() { return iterator(_M_node); }
    SNode* _M_create_node(CThingMask47* const& x)
    {
        SNode* p = (SNode*)malloc(sizeof(SNode));
        new (&p->_M_data) CThingMask47*(x);
        return p;
    }
    iterator insert(iterator position, CThingMask47* const& x)
    {
        SNode* tmp = _M_create_node(x);
        SNode* n = position._M_node;
        SNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(CThingMask47* const& x) { insert(end(), x); }
};
// CThingSearchTools: engine/CThingSearchTools.h names +0x0c "AreaMaps" (vector<NThingMap::CThingMap*>, 16 bytes in
// the PDB); retail's STLport vector is 12 bytes and only the scratch-vector role matters here, so a local model.
struct CThingSearchTools {
    void*      WorldMap;                                   // +0x00
    void*      PThingMapManager;                           // +0x04
    void*      ThingManager;                               // +0x08
    CMapVector AreaMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template<class T, class F> void FindThingsInRoughArea(const C2DBoxI& area, const F& filter, CResultList& things);
};
template<class T, class F> void CThingSearchTools::FindThingsInRoughArea(const C2DBoxI& area, const F& filter, CResultList& things)
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
                            if (IsThingOfType<T>(thing) && filter(thing)) {
                                T* found = static_cast<T*>(thing);
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
template void CThingSearchTools::FindThingsInRoughArea<CThingMask47, CThingFilter_OverlappingSphere >(const C2DBoxI& area, const CThingFilter_OverlappingSphere& filter, CResultList& things);
