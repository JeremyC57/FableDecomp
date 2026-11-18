#ifndef FABLE_ENGINE_COPINIONDEEDFILTERS_TRESPASSING_H
#define FABLE_ENGINE_COPINIONDEEDFILTERS_TRESPASSING_H
#ifdef FABLE_RETAIL_MINION_TRESPASS_UPDATE_PASS24
#include "engine/CThing.h"
// Original PDB layout: empty predicate and one pointer at offset zero.
// FableWin names the TargetIs constructor taking const CThing&.
class COpinionDeedFilter_IsStillHappening {
public:
    COpinionDeedFilter_IsStillHappening() {}
};
class COpinionDeedFilter_TargetIs {
public:
    explicit COpinionDeedFilter_TargetIs(const CThing& creature) : Creature(&creature) {}
private:
    const CThing* Creature;
};
#endif
#endif
