// Behaviour test: the source model over a 4x4 one-level fake mapwho grid (C++03; g++ and VC7.1).
#include <new>
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <string.h>
#include <stdlib.h>
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
// Retail CThing: ThingTypeFlags at +0x6c (THING_TYPE_* inheritance bits); TypeMask selects the searched class.
class CThing { public:
    enum { TypeMask = 0 };
    unsigned char _pad00[0x60];
    class CTCPhysicsBase* PhysicsTC;                      // +0x60
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
struct __true_type { __true_type() {} };
class CThingFilter_IsNot { public:
    bool operator()(const CThing* thing) const;   // 0x006A76A0
};
class CThingFilter_IsNotList { public:
    bool operator()(const CThing* thing) const;   // 0x006A7EB0
};
class CThingFilter_IsWithinArea { public:
    bool operator()(const CThing* thing) const;   // 0x007FD830
};
// CFilter_Bind3: holds references to its sub-filters (+0, +4, ...) and ANDs them in order.
template <class F1, class F2, class F3> class CFilter_Bind3 { public:
    const F1& Filter1;
    const F2& Filter2;
    const F3& Filter3;
    bool operator()(const CThing* thing) const { return Filter1(thing) && Filter2(thing) && Filter3(thing); }
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
// STLport list<CThing*>: push_back is insert(end(), x) over an iterator with a declared copy constructor;
// the node comes from the CRT malloc thunk 0x00BFEA0E.
struct CResultList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CThing* _M_data; };
    struct iterator {
        SNode* _M_node;
        iterator(SNode* node) : _M_node(node) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    SNode* _M_node;
    iterator end() { return iterator(_M_node); }
    SNode* _M_create_node(CThing* const& x)
    {
        SNode* p = (SNode*)malloc(sizeof(SNode));
        new (&p->_M_data) CThing*(x);
        return p;
    }
    iterator insert(iterator position, CThing* const& x)
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
    void push_back(CThing* const& x) { insert(end(), x); }
};
// CThingSearchTools: engine/CThingSearchTools.h names +0x0c "AreaMaps" (vector<NThingMap::CThingMap*>, 16 bytes in
// the PDB); retail's STLport vector is 12 bytes and only the scratch-vector role matters here, so a local model.
class CThingSearchTools { public:
    void*      WorldMap;                                   // +0x00
    void*      PThingMapManager;                           // +0x04
    void*      ThingManager;                               // +0x08
    CMapVector AreaMaps;                                   // +0x0c
    void GetMapsInArea(const C2DBoxI& area, CMapVector* maps);              // 0x0049C6F0
    C2DBoxI GetCellExtents(CThingMap* map, const C2DBoxI& area);            // 0x0049C700
    template<class T, class F> void GetAllThingsInRoughArea(const C2DBoxI& area, const F& filter, CResultList& things);
};
template<class T, class F> void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const F& filter, CResultList& things)
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
template void CThingSearchTools::GetAllThingsInRoughArea<CThing, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_IsWithinArea > >(const C2DBoxI& area, const CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_IsWithinArea >& filter, CResultList& things);
SMapwhoLevel g_MapwhoLevels[2];
long g_NumMapwhoLevels;
SSearchStamps g_SearchStamps;
static CThingMap* g_MapStore[4];
void CThingSearchTools::GetMapsInArea(const C2DBoxI&, CMapVector* maps)
{ maps->_M_finish = maps->_M_start + 1; }
C2DBoxI CThingSearchTools::GetCellExtents(CThingMap*, const C2DBoxI& area) { return area; }
struct TThing : CThing { unsigned char Extra[0x40]; };   // room for the filters' wider views
static TThing g_Things[6];
static bool g_Accept[6];
static int g_Dist[6];
static int ThingIndex(const CThing* thing) { return (int)(static_cast<const TThing*>(thing) - g_Things); }
bool CThingFilter_IsNot::operator()(const CThing* thing) const { return g_Accept[ThingIndex(thing)]; }
bool CThingFilter_IsNotList::operator()(const CThing* thing) const { return g_Accept[ThingIndex(thing)]; }
bool CThingFilter_IsWithinArea::operator()(const CThing* thing) const { return g_Accept[ThingIndex(thing)]; }
static CThingMapEntry g_Entries[6];
static CThingMapNode g_Nodes[8];
static CThingMapNode* g_Cells[16];
static CMapwhoLevelInfo g_Info[1];
static CCellVector g_CellVectors[1];
static int g_NodeCount;
static void Put(int thing, long x, long y)
{
    CThingMapNode* node = &g_Nodes[g_NodeCount++];
    node->Entry = &g_Entries[thing];
    node->Next = g_Cells[y * 4 + x];
    g_Cells[y * 4 + x] = node;
}
static int Fail(int line) { printf("FAIL line %d\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)
static void Setup()
{
    g_NumMapwhoLevels = 1;
    g_MapwhoLevels[0].Shift = 0;
    g_Info[0].Width = 4;
    g_Info[0].Height = 4;
    g_CellVectors[0]._M_start = g_Cells;
    g_CellVectors[0]._M_finish = g_Cells + 16;
    static CThingMap map;
    map.LevelInfo = g_Info;
    map.Cells = g_CellVectors;
    g_MapStore[0] = &map;
    for (int i = 0; i < 6; ++i) {
        g_Things[i].ThingTypeFlags = CThing::TypeMask | 0x100000;
        g_Things[i].PhysicsTC = 0;
        g_Entries[i].Thing = &g_Things[i];
        g_Entries[i].SearchStamp = 0;
        g_Accept[i] = true;
        g_Dist[i] = 10 - i;
    }
    g_Things[5].ThingTypeFlags = 0;   // wrong type unless the mask is 0
    g_Accept[4] = false;              // rejected by the filter
    g_Dist[0] = 5; g_Dist[1] = 1; g_Dist[5] = 0;
    // area cells x 1..2, y 1..2; thing 0 is in two cells (visited once), 2 and 3 are outside the area
    Put(0, 1, 1); Put(0, 2, 1); Put(5, 2, 1); Put(4, 1, 2); Put(1, 2, 2); Put(2, 3, 3); Put(3, 0, 0);
}
int main()
{
    Setup();
    CThingSearchTools tools;
    CThingMap* storage[4];
    tools.AreaMaps._M_start = storage;
    tools.AreaMaps._M_finish = storage;
    tools.AreaMaps._M_end_of_storage = storage + 4;
    storage[0] = g_MapStore[0];
    C2DBoxI area(1, 1, 3, 3);
    CThingFilter_IsNot sub0;
    CThingFilter_IsNotList sub1;
    CThingFilter_IsWithinArea sub2;
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_IsWithinArea > filter = { sub0, sub1, sub2 };
    CResultList::SNode head;
    head._M_next = &head;
    head._M_prev = &head;
    CResultList things;
    things._M_node = &head;
    tools.GetAllThingsInRoughArea<CThing, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_IsWithinArea > >(area, filter, things);
    CThing* expect[4];
    int n = 0;
    expect[n++] = &g_Things[0];
    if (CThing::TypeMask == 0) expect[n++] = &g_Things[5];
    expect[n++] = &g_Things[1];
    int count = 0;
    for (CResultList::SNode* p = head._M_next; p != &head; p = p->_M_next) {
        CHECK(count < n && p->_M_data == expect[count]);
        ++count;
    }
    CHECK(count == n);
    CHECK(head._M_prev->_M_next == &head);
    CHECK(tools.AreaMaps._M_finish == tools.AreaMaps._M_start);   // scratch list cleared
    CHECK(g_SearchStamps.Searches == 1 && g_SearchStamps.Stamp == 1 && g_SearchStamps.Found == 0);
    CHECK(g_Entries[0].SearchStamp == 1 && g_Entries[2].SearchStamp == 0);
    // a second search sees every thing again (new stamp)
    tools.AreaMaps._M_finish = storage;
    tools.GetAllThingsInRoughArea<CThing, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_IsWithinArea > >(area, filter, things);
    count = 0;
    for (CResultList::SNode* q = head._M_next; q != &head; q = q->_M_next) ++count;
    CHECK(count == 2 * n && g_SearchStamps.Stamp == 2);
    printf("0080e8e0_TEST PASS\n");
    return 0;
}