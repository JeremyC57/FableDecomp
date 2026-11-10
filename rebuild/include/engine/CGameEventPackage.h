#ifndef FABLE_ENGINE_CGAMEEVENTPACKAGE_H
#define FABLE_ENGINE_CGAMEEVENTPACKAGE_H
#include "engine/CGameEvent.h"
// Retail opt-in (Codex pass 17): both PDBs 648h class with private stamp/count
// and forty events; native timestamp reads +0, donor accessor 018A8340.
#ifdef FABLE_RETAIL_CGAMEEVENTPACKAGE
class CGameEventPackage {
public:
    long GetTimeStamp() const { return TimeStamp; }
private:
    long TimeStamp;
    unsigned long NoEvents;
    CGameEvent Events[40];
};
FABLE_STATIC_ASSERT(sizeof(CGameEventPackage)==0x648);
#endif
#endif
