#ifndef FABLE_ENGINE_CISFREEBED_H
#define FABLE_ENGINE_CISFREEBED_H
#ifdef FABLE_RETAIL_FREE_BED_PREDICATE_PASS30
#include "engine/CIntelligentPointer.h"
#include "engine/CThingFirePointFilters.h"
// Ego_d field list 0x60014 records the public unary base and public member.
// Ego_r places the member at zero. FableWin constructs it before std::find_if.
class CIsFreeBed : public std::unary_function<CIntelligentPointer<CThing>,bool> {
public:
    bool operator()(const CIntelligentPointer<CThing>& thing) const;
    CThingFilter_IsOccupiableOccupied IsOccupiableOccupied;
};
#endif
#endif
