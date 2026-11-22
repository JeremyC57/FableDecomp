#ifndef FABLE_ENGINE_CBRAINDEF_H
#define FABLE_ENGINE_CBRAINDEF_H
#ifdef FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26
#include "engine/CParentDefClassBase.h"
#include "engine/CBrainBehaviour.h"
#include "stlport/fable_containers.h"
class CBrainUpdateZone;
class CBrainDef : public CParentDefClassBase {
public:
    std::vector<CBrainBehaviour> Behaviours;
    std::vector<CDefString> DisabledBehaviours;
    std::vector<CBrainUpdateZone> UpdateZones;
};
// Native 0088BF30 indexes 12-byte behaviours from +3C and disabled names from +48.
FABLE_STATIC_ASSERT_AT(881,offsetof(CBrainDef,Behaviours)==0x3c);
FABLE_STATIC_ASSERT_AT(882,offsetof(CBrainDef,DisabledBehaviours)==0x48);
#endif
#endif
