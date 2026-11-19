// Behaviour test for 0x009871C0 CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind4<CThingFilter_IsNot, CThingFilter_IsCreatureType,
// CThingFilter_In2DRange, CThingFilter_In2DRange>, const CThingAICreature>: the row's defines, includes and body verbatim over the shared
// headers, run on zeroed object images. Map B (empty local area, NULL tables) then map A (layers 0 and 1; layer 1 at shift 1). Node
// chains before the hit test each rejection: the excluded thing, the AI-creature type mask (flags 0x0F fails, 0x1F / 0x3F pass), the
// creature type, the first and the second 2D range, the search-id stamp (equal / greater skipped). The hit returns true through the
// out pointer and leaves the scratch list uncleared; a later thing in the same chain is not stamped. A second search with nothing
// left returns false, keeps *out and clears the scratch list.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ---- row body ----
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsCreatureType.h"
#include "engine/CThingFilter_In2DRange.h"
template<class Filter,class T>
bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area,const Filter& filter_func,T** thing) const
{
    unsigned long flags=T::StatGetTypeFlags();
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        NThingMap::CThingMap& thing_map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
            if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            if((loop_thing->GetTypeFlags()&flags)==flags) {
                                T* tthing=static_cast<T*>(loop_thing);
                                if(filter_func(*tthing)) {
                                    *thing=tthing;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    return false;
}
template bool CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsCreatureType,CThingFilter_In2DRange,CThingFilter_In2DRange>,const CThingAICreature>(const C2DBoxI&,const CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsCreatureType,CThingFilter_In2DRange,CThingFilter_In2DRange>&,const CThingAICreature**) const;

// ---- doubles ----
unsigned long CThingSearchTools::NoLookups = 4, CThingSearchTools::NoThingsScanned = 7, CThingSearchTools::SearchID = 0;
namespace NThingMap { long MaxLayers = 2; }
namespace NThingMap { extern const CLayerInfo LayerInfo[3] = { { 0, 0 }, { 0, 1 }, { 0, 0 } }; }   // Size, ScaleShift
static unsigned char g_mapA[0x40], g_mapB[0x40];
static int g_bad = 0, g_mapCalls = 0, g_clipCalls = 0;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    ++g_mapCalls;
    void*** v = (void***)&maps;
    if (area.TLX != 0 || area.TLY != 0 || area.BRX != 3 || area.BRY != 3 || v[0] != v[1]) g_bad = 1;
    v[0][0] = g_mapB; v[0][1] = g_mapA; v[1] = v[0] + 2;
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    ++g_clipCalls;
    C2DBoxI local;
    if ((const void*)&map == (const void*)g_mapA) { local.TLX = area.TLX + 1; local.TLY = area.TLY; local.BRX = area.BRX + 1; local.BRY = area.BRY; }
    else { local.TLX = 4; local.TLY = 0; local.BRX = 4; local.BRY = 2; }
    return local;
}
static const CThing* g_excluded = 0;
bool CThingFilter_IsNot::operator()(const CThing& thing) const { if (&Thing != g_excluded) g_bad = 1; return &thing != &Thing; }
ECreatureType CThingCreatureBase::GetCreatureType() const { return (ECreatureType)*(const long*)((const char*)this + 0x204); }
bool CThingFilter_In2DRange::operator()(const CThing& thing) const
{
    // range 10 reads the value at +0x208, range 20 the value at +0x20C: each must be below its range
    const float v = Range < 15.0f ? *(const float*)((const char*)&thing + 0x208) : *(const float*)((const char*)&thing + 0x20C);
    return v < Range;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SNode { void* mapwho; SNode* next; void* previous; void* cell; };
enum { NTHINGS = 14 };
static unsigned char things[NTHINGS][0x400], mapwhos[NTHINGS][0x60];
static void SetThing(int i, unsigned long flags, long type, float r1, float r2)
{
    *(void**)(mapwhos[i] + 0x04) = things[i]; *(unsigned long*)(things[i] + 0x6c) = flags;
    *(long*)(things[i] + 0x204) = type; *(float*)(things[i] + 0x208) = r1; *(float*)(things[i] + 0x20C) = r2;
}
int main()
{
    static long levelInfo[3][5];
    static SNode* cells0[16]; static SNode* cells1[4];
    static void* cellVectors[3][3]; static void* scratch[4]; static unsigned char toolsImage[0x18];
    static SNode nodes[NTHINGS];
    levelInfo[0][1] = 4; levelInfo[1][1] = 2;
    cellVectors[0][0] = cells0; cellVectors[1][0] = cells1;
    *(void**)(g_mapA + 0x04) = levelInfo; *(void**)(g_mapA + 0x08) = levelInfo + 3; *(void**)(g_mapA + 0x0c) = levelInfo + 3;
    *(void**)(g_mapA + 0x14) = cellVectors; *(void**)(g_mapA + 0x18) = cellVectors + 3; *(void**)(g_mapA + 0x1c) = cellVectors + 3;
    const long TYPE = 7;
    SetThing(0, 0x1f, TYPE, 1, 1);       // cell (0,0): left of the area (never visited)
    SetThing(1, 0x1f, TYPE, 1, 1);       // cell (1,0): the excluded thing
    SetThing(2, 0x0f, TYPE, 1, 1);       // cell (1,0): a creature but not an AI creature
    SetThing(3, 0x1f, TYPE + 1, 1, 1);   // cell (1,0): wrong creature type
    SetThing(4, 0x1f, TYPE, 10, 1);      // cell (1,0): first range fails (10 is not < 10)
    SetThing(5, 0x1f, TYPE, 1, 20);      // cell (1,0): second range fails (20 is not < 20)
    SetThing(6, 0x1f, TYPE, 1, 1);       // cell (2,0): stamp equal to the search id: skipped
    SetThing(7, 0x1f, TYPE, 1, 1);       // cell (2,0): stamp greater: skipped
    SetThing(8, 0x3f, TYPE, 9.5f, 19.5f);// cell (2,1): the hit
    SetThing(9, 0x1f, TYPE, 1, 1);       // cell (2,1) after the hit: not reached
    SetThing(10, 0x1f, TYPE, 1, 1);      // layer 1 cell (1,1): not reached in search 1
    for (int i = 0; i < NTHINGS; ++i) nodes[i].mapwho = mapwhos[i];
    *(unsigned long*)(mapwhos[6] + 0x4c) = 1; *(unsigned long*)(mapwhos[7] + 0x4c) = 3;
    cells0[0 * 4 + 0] = &nodes[0];
    cells0[0 * 4 + 1] = &nodes[1]; nodes[1].next = &nodes[2]; nodes[2].next = &nodes[3]; nodes[3].next = &nodes[4]; nodes[4].next = &nodes[5];
    cells0[0 * 4 + 2] = &nodes[6]; nodes[6].next = &nodes[7];
    cells0[1 * 4 + 2] = &nodes[8]; nodes[8].next = &nodes[9];
    cells1[1 * 2 + 1] = &nodes[10];
    *(void**)(toolsImage + 0x0c) = scratch; *(void**)(toolsImage + 0x10) = scratch; *(void**)(toolsImage + 0x14) = scratch + 4;
    const CThingSearchTools& tools = *(const CThingSearchTools*)toolsImage;
    g_excluded = (const CThing*)things[1];
    C3DVector pos; pos.X = 0.0f; pos.Y = 0.0f; pos.Z = 0.0f;
    CThingFilter_IsNot is_not(*g_excluded);
    CThingFilter_IsCreatureType is_type((ECreatureType)TYPE);
    CThingFilter_In2DRange range1(pos, 10.0f), range2(pos, 20.0f);
    CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsCreatureType,CThingFilter_In2DRange,CThingFilter_In2DRange> filter(is_not, is_type, range1, range2);
    C2DBoxI area; area.TLX = 0; area.TLY = 0; area.BRX = 3; area.BRY = 3;
    check(sizeof(std::vector<CThing*>) == 12 && sizeof(CThingSearchTools) == 0x18, "layouts");
    const CThingAICreature* found = 0;
    bool r1 = tools.GetPFirstThingInRoughArea(area, filter, &found);
    check(r1 && found == (const CThingAICreature*)things[8], "first matching AI creature returned");
    check(!g_bad && g_mapCalls == 1 && g_clipCalls == 2, "maps fetched once, both clipped");
    check(*(void**)(toolsImage + 0x10) == (void*)(scratch + 2), "scratch map list left as is on a hit");
    check(CThingSearchTools::NoLookups == 5 && CThingSearchTools::SearchID == 1 && CThingSearchTools::NoThingsScanned == 0, "search statics");
    for (int i = 1; i <= 5; ++i) check(*(unsigned long*)(mapwhos[i] + 0x4c) == 1, "rejected things stamped");
    check(*(unsigned long*)(mapwhos[8] + 0x4c) == 1, "hit stamped");
    check(*(unsigned long*)(mapwhos[9] + 0x4c) == 0 && *(unsigned long*)(mapwhos[10] + 0x4c) == 0 && *(unsigned long*)(mapwhos[0] + 0x4c) == 0, "search stops at the hit");
    check(*(unsigned long*)(mapwhos[6] + 0x4c) == 1 && *(unsigned long*)(mapwhos[7] + 0x4c) == 3, "equal / later stamps left alone");

    // search 2: things 8, 9 and 10 now fail the second range; thing 7's stamp (3) is still above the search id (2)
    *(float*)(things[8] + 0x20C) = 25.0f; *(float*)(things[9] + 0x20C) = 25.0f; *(float*)(things[10] + 0x20C) = 25.0f;
    *(unsigned long*)(mapwhos[6] + 0x4c) = 9;            // thing 6 now above every later search id
    *(void**)(toolsImage + 0x10) = scratch;
    g_mapCalls = 0; g_clipCalls = 0;
    bool r2 = tools.GetPFirstThingInRoughArea(area, filter, &found);
    check(!r2 && found == (const CThingAICreature*)things[8], "no hit: false, out pointer untouched");
    check(*(void**)(toolsImage + 0x10) == scratch && g_clipCalls == 2, "no hit: scratch map list cleared after every map");
    check(*(unsigned long*)(mapwhos[9] + 0x4c) == 2 && *(unsigned long*)(mapwhos[10] + 0x4c) == 2 && *(unsigned long*)(mapwhos[7] + 0x4c) == 3, "second search stamps");
    // search 3: thing 10 on layer 1 now passes: found after layer 0 is exhausted
    *(float*)(things[10] + 0x20C) = 1.0f;
    bool r3 = tools.GetPFirstThingInRoughArea(area, filter, &found);
    check(r3 && found == (const CThingAICreature*)things[10], "hit on layer 1");
    if (!g_fail) printf("009871c0_TEST PASS\n");
    return g_fail;
}