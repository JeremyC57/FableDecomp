#ifndef FABLE_ENGINE_CBRAINBEHAVIOUR_H
#define FABLE_ENGINE_CBRAINBEHAVIOUR_H
#ifdef FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26
#include "engine/CDefString.h"
class CBrainBehaviour {
public:
    CDefString Name;
    long Priority;
    long ScriptAIPriority;
};
FABLE_STATIC_ASSERT_AT(880, sizeof(CBrainBehaviour)==12);
#endif
#endif
