// Flags: 
// 006bc4a0 CRegion::AddMap
// Leaf: ?AddMap@CRegion@@QAEXJ@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_ADDMAP_PASS30
#define FABLE_RETAIL_STLP_LONG_OVERFLOW_PASS30
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#include "fable_assert.h"
#include "engine/CRegion.h"

void CRegion::AddMap(long map_index)
{
    FABLE_RETAIL_ASSERT(std::find(Maps.begin(), Maps.end(), map_index) == Maps.end());
    Maps.push_back(map_index);
}
