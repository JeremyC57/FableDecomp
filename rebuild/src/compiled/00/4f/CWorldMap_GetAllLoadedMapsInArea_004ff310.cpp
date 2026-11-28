// 004ff310 CWorldMap::GetAllLoadedMapsInArea
// Retail extent 123 bytes through the ret 8 at 004FF388.
// Inline-budget: keep the shared STLport overflow in its exact size-built COMDAT.
// Leaf: ?GetAllLoadedMapsInArea@CWorldMap@@UBEXABVC2DBoxI@@AAV?$vector@KV?$allocator@K@std@@@std@@@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_OVERLAPPING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHREADEDFILE_ULONG_OVERFLOW_PASS30
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#ifdef _DEBUG
#include <cassert>
#endif

void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI& box, std::vector<unsigned long>& map_list) const
{
    for (CArray<long>::const_iterator it = LoadedMaps.begin(); it != LoadedMaps.end(); ++it) {
        long index = *it;
        if (Maps[index].Area.Overlapping(box)) {
#ifdef _DEBUG
            assert(Maps[index].Used);
            assert(!Maps[index].PMap.IsNull());
#endif
            map_list.push_back(index);
        }
    }
}
