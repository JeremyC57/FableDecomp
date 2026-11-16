// Behaviour test for 0x00959380 CThingSearchTools::GetAllThingsInRoughArea<const CThingObject, std::vector<const CThingObject*>, CThingFilter_None>: the row's
// defines, includes and explicit instantiation verbatim over the shared headers, run on zeroed object images (no engine
// constructor runs): a 2x2 one-layer mapwho with things of type bits 0x807 / 0x47 / 0x1f / 0x847 (the first listed in two
// cells); only the CThingObject type matches are appended, each once per search, over two searches. Doubles:
// GetAllThingMapsInArea (hands back the fixture map), ClipAndConvertWorldMapAreaToLocalArea (returns the area) and the
// search / layer statics.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
template void CThingSearchTools::GetAllThingsInRoughArea<const CThingObject,std::vector<const CThingObject*>,CThingFilter_None>(const C2DBoxI&,const CThingFilter_None&,std::vector<const CThingObject*>&) const;


// ---- doubles ----
unsigned long CThingSearchTools::NoLookups = 0, CThingSearchTools::NoThingsScanned = 5, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 1; }
namespace NThingMap { extern const CLayerInfo LayerInfo[2] = { { 0 }, { 0 } }; }   // ScaleShift 0
static unsigned char g_mapImage[0x40];
static int g_bad = 0, g_mapCalls = 0, g_clipCalls = 0;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    ++g_mapCalls;
    void*** v = (void***)&maps;                             // STLport vector: start, finish, end of storage
    if (area.TLX != 0 || area.BRX != 2 || v[0] != v[1]) g_bad = 1;
    v[0][0] = g_mapImage; v[1] = v[0] + 1;
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    ++g_clipCalls;
    if ((const void*)&map != (const void*)g_mapImage) g_bad = 1;
    return area;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };       // NThingMap::CMapwhoNode
int main()
{
    static unsigned char things[4][0x100], mapwhos[4][0x60], toolsImage[0x18];
    static long levelInfo[5];                               // CMapwhoLevelInfo: Size, Extents.X, Extents.Y, ScaleShift, NoThings
    static SNode nodes[5]; static SNode* cells[4]; static void* cellVector[3]; static void* scratch[2];
    static const unsigned long flags[4] = { 0x807, 0x47, 0x1f, 0x847 };
    levelInfo[1] = 2; levelInfo[2] = 2;
    cellVector[0] = cells; cellVector[1] = cells + 4; cellVector[2] = cells + 4;
    *(void**)(g_mapImage + 0x04) = levelInfo; *(void**)(g_mapImage + 0x08) = levelInfo + 5; *(void**)(g_mapImage + 0x0c) = levelInfo + 5;
    *(void**)(g_mapImage + 0x14) = cellVector; *(void**)(g_mapImage + 0x18) = cellVector + 3; *(void**)(g_mapImage + 0x1c) = cellVector + 3;
    for (int i = 0; i < 4; ++i) {
        *(unsigned long*)(things[i] + 0x6c) = flags[i];     // CThing::ThingTypeFlags
        *(void**)(mapwhos[i] + 0x04) = things[i];           // CTCBase::ParentThing
        nodes[i].mapwho = mapwhos[i];
    }
    nodes[4].mapwho = mapwhos[0];                           // thing 0 again, in another cell
    nodes[0].next = &nodes[1]; nodes[2].next = &nodes[3];
    cells[0] = &nodes[0]; cells[1] = &nodes[4]; cells[2] = &nodes[2]; cells[3] = 0;
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 2;
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    static void* outImage[3];
    std::vector<const CThingObject*>& out = *(std::vector<const CThingObject*>*)outImage;
    CThingFilter_None filter;
    C2DBoxI area, empty;
    area.TLX = 0; area.TLY = 0; area.BRX = 2; area.BRY = 2;
    empty.TLX = 0; empty.TLY = 0; empty.BRX = 0; empty.BRY = 2;
    check(sizeof(std::vector<const CThingObject*>) == 12 && sizeof(CThingSearchTools) == 0x18, "layouts");
    for (int run = 0; run < 2; ++run) {
        tools.GetAllThingsInRoughArea<const CThingObject,std::vector<const CThingObject*>,CThingFilter_None>(area, filter, out);
        void** start = (void**)outImage[0]; void** finish = (void**)outImage[1];
        check(!g_bad && g_mapCalls == run + 1 && g_clipCalls == run + 1, "map list and clip called once per search");
        check(finish - start == 2 * (run + 1), "two matches appended per search");
        check(finish - start == 2 * (run + 1) && start[2 * run] == things[0] && start[2 * run + 1] == things[3], "the 0x807 and 0x847 things, in cell order, each once");
        check(*(void**)(toolsImage + 0x10) == scratch, "scratch map list cleared");
        check(CThingSearchTools::NoLookups == (unsigned long)(run + 1) && CThingSearchTools::SearchID == (unsigned long)(run + 1) && CThingSearchTools::NoThingsScanned == 0, "search statics");
        for (int i = 0; i < 4; ++i)
            check(*(unsigned long*)(mapwhos[i] + 0x4c) == (unsigned long)(run + 1), "every scanned component stamped");
    }
    // an area with no cells is skipped without touching the level tables
    tools.GetAllThingsInRoughArea<const CThingObject,std::vector<const CThingObject*>,CThingFilter_None>(empty, filter, out);
    check(g_bad == 1 && ((void**)outImage[1] - (void**)outImage[0]) == 4, "empty area appends nothing (the map double flags the different area)");
    free(outImage[0]);
    if (!g_fail) printf("00959380_TEST PASS\n");
    return g_fail;
}