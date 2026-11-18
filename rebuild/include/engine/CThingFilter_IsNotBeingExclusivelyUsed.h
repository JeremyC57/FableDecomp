#ifndef FABLE_ENGINE_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_H
#define FABLE_ENGINE_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_H
#if defined(FABLE_RETAIL_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_PASS24) || defined(FABLE_RETAIL_CTHINGFILTER_EXCLUSIVE_USE_PASS24)
#include "rebuild_abi.h"
class CThing;
// Both original PDBs and the native marker-filter call at 0x0072FE1F.
class CThingFilter_IsNotBeingExclusivelyUsed {
public:
    bool operator()(const CThing& thing) const;              // 0x006A8AF0
};
FABLE_STATIC_ASSERT_AT(11, sizeof(CThingFilter_IsNotBeingExclusivelyUsed) == 1);
#endif
#endif
