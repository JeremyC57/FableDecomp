#ifndef FABLE_ENGINE_CROUGHFRAMECOUNTER_H
#define FABLE_ENGINE_CROUGHFRAMECOUNTER_H
#include "rebuild_abi.h"

// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CRoughFrameCounter {
private: unsigned long FrameStart;
private: float FPS;
};
FABLE_STATIC_ASSERT(sizeof(CRoughFrameCounter)==8);
#endif
