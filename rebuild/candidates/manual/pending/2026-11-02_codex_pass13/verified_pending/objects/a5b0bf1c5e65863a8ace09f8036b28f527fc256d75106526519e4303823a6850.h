#ifndef FABLE_ENGINE_CGAMEEVENTPACKAGE_H
#define FABLE_ENGINE_CGAMEEVENTPACKAGE_H
#include "rebuild_abi.h"
#include "engine/CGameEvent.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CGameEventPackage {
private: long TimeStamp;
private: unsigned long NoEvents;
private: CGameEvent Events[40];
};
FABLE_STATIC_ASSERT(sizeof(CGameEventPackage)==1608);
#endif
