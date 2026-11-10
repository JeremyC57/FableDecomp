#ifndef FABLE_ENGINE_CTHINGCOMPARE_BESTFORTARGETING2_H
#define FABLE_ENGINE_CTHINGCOMPARE_BESTFORTARGETING2_H
// Retail opt-in (Codex pass 17): original class, both PDB signatures and raw type records.
#include "engine/NPlayerTargeting.h"
#include "engine/CVectorCompare_BestForTargeting.h"
class CThingCompare_BestForTargeting2 {
public:
 bool operator()(const NPlayerTargeting::CTargetPoint&,const NPlayerTargeting::CTargetPoint&) const;
private:
 float GetThingCompareMultiplier(const CThing*) const;
 CThing* Hero;
 CVectorCompare_BestForTargeting VectorCompare;
 float EnemyPreferenceFactor;
};
#endif
