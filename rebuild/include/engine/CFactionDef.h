#ifndef FABLE_ENGINE_FACTION_DEFINITION_PASS30_H
#define FABLE_ENGINE_FACTION_DEFINITION_PASS30_H
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
// Retail RTTI at 01233B14 derives from CParentDefClassBase. Both original PDBs identify the two vectors.
class CFactionDef : public CParentDefClassBase {
public:
    std::vector<CCharString> EnemyFactions;
    std::vector<CCharString> AlliedFactions;
};
#endif
