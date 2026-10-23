// 00518130 behaviour test: CThingSearchTools::ForEachInRoughArea<CThingPhysical, CThingFilter_None, fn> (match.cpp body, host-modelled)
#include <stdio.h>
#include <string.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C2DBoxI { long X0, Y0, X1, Y1; C2DBoxI() {} C2DBoxI(long a, long b, long c, long d) : X0(a), Y0(b), X1(c), Y1(d) {} };
struct CThing { unsigned long ThingTypeFlags; int Id; };
struct CThingPhysical : CThing {};
struct CThingFilter_None { bool operator()(const CThing&) const { return true; } };
struct CThingMapEntry { CThing* Thing; unsigned long SearchStamp; };
struct CThingMapNode { CThingMapEntry* Entry; CThingMapNode* Next; };
struct CThingMap { long Width; std::vector<CThingMapNode*> Cells; CThingMapNode** GetCell(long x, long y) { return &Cells[y * Width + x]; } };
struct SSearchStamps { unsigned long Searches, Found, Stamp; };
static SSearchStamps g_SearchStamps = { 0, 0, 5 };
static long g_NumMapwhoLevels = 1; static long g_Shift[1] = { 0 };
struct CThingSearchTools {
    std::vector<CThingMap*> All; std::vector<CThingMap*> TempMaps;
    void GetMapsInArea(const C2DBoxI&, std::vector<CThingMap*>* maps) { *maps = All; }
    C2DBoxI GetCellExtents(CThingMap*, const C2DBoxI& a) { return a; }
    template<class T, class F, class G> bool ForEachInRoughArea(const C2DBoxI& area, F filter, const G& func)
    {
        bool result = false;
        GetMapsInArea(area, &TempMaps);
        ++g_SearchStamps.Searches; g_SearchStamps.Found = 0; ++g_SearchStamps.Stamp;
        for (size_t i = 0; i < TempMaps.size(); ++i) {
            CThingMap* map = TempMaps[i];
            C2DBoxI extents = GetCellExtents(map, area);
            for (long level = 0; level < g_NumMapwhoLevels; ++level) {
                long shift = g_Shift[level];
                C2DBoxI box(extents.X0 >> shift, extents.Y0 >> shift, ((extents.X1 - 1) >> shift) + 1, ((extents.Y1 - 1) >> shift) + 1);
                if (box.X0 == box.X1 || box.Y0 == box.Y1) continue;
                for (long y = box.Y0; y < box.Y1; ++y) {
                    CThingMapNode** cell = map->GetCell(box.X0, y);
                    for (long x = box.X0; x < box.X1; ++x, ++cell)
                        for (CThingMapNode* node = *cell; node; node = node->Next) {
                            CThingMapEntry* entry = node->Entry;
                            if (entry->SearchStamp < g_SearchStamps.Stamp) {
                                entry->SearchStamp = g_SearchStamps.Stamp;
                                CThing* thing = entry->Thing;
                                if ((thing->ThingTypeFlags & 3) == 3 && filter(*thing) && func(*(T*)thing)) result = true;
                            }
                        }
                }
            }
        }
        TempMaps.clear();
        return result;
    }
};
static int g_calls[8];
static bool Visit(CThingPhysical& t) { ++g_calls[t.Id]; return t.Id == 2; }
int main()
{
    CThingPhysical t[4]; for (int i = 0; i < 4; ++i) { t[i].Id = i; t[i].ThingTypeFlags = 3; } t[3].ThingTypeFlags = 1;
    CThingMapEntry e[4]; for (int i = 0; i < 4; ++i) { e[i].Thing = &t[i]; e[i].SearchStamp = 0; }
    CThingMapNode n[6] = { { &e[0], 0 }, { &e[1], 0 }, { &e[2], 0 }, { &e[0], 0 }, { &e[3], 0 }, { &e[1], 0 } };
    n[0].Next = &n[1];
    CThingMap m; m.Width = 3; m.Cells.assign(9, (CThingMapNode*)0);
    m.Cells[0] = &n[0]; m.Cells[4] = &n[2]; m.Cells[5] = &n[3]; m.Cells[8] = &n[4]; n[4].Next = &n[5];
    CThingSearchTools s; s.All.push_back(&m);
    bool (*fn)(CThingPhysical&) = Visit;
    CHECK(s.ForEachInRoughArea<CThingPhysical>(C2DBoxI(0, 0, 3, 3), CThingFilter_None(), fn));
    CHECK(g_calls[0] == 1 && g_calls[1] == 1 && g_calls[2] == 1 && g_calls[3] == 0);   // each once, non-physical skipped
    CHECK(s.TempMaps.empty() && g_SearchStamps.Searches == 1);
    memset(g_calls, 0, sizeof g_calls);
    CHECK(!s.ForEachInRoughArea<CThingPhysical>(C2DBoxI(0, 0, 1, 1), CThingFilter_None(), fn));   // only cell 0: things 0,1
    CHECK(g_calls[0] == 1 && g_calls[1] == 1 && g_calls[2] == 0);
    CHECK(!s.ForEachInRoughArea<CThingPhysical>(C2DBoxI(1, 1, 1, 3), CThingFilter_None(), fn));   // empty box
    if (!g_fail) printf("00518130_TEST PASS\n");
    return g_fail;
}