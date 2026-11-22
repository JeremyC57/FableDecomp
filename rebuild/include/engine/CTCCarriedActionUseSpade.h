#ifndef FABLE_ENGINE_CTCCARRIEDACTIONUSESPADE_H
#define FABLE_ENGINE_CTCCARRIEDACTIONUSESPADE_H
#include "engine/CTCCarriedActionUseFishingRod.h"
#ifdef FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
// Both PDBs and the retail 0x007F0300 entry. The shared carried-action
// base is in CTCCarriedActionUseFishingRod.h.
class CTCCarriedActionUseSpade : public CTCCarriedActionUseBase {
public:
    static bool FABLE_FASTCALL IsThingAbleToDig(const CThingCreatureBase&);
};
#endif
#endif
