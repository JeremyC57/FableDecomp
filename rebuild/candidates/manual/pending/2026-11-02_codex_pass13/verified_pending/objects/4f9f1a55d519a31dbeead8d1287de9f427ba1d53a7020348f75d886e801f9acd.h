#ifndef FABLE_ENGINE_EMOVEMENTBANDTYPE_H
#define FABLE_ENGINE_EMOVEMENTBANDTYPE_H
#include "rebuild_abi.h"

// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
enum EMovementBandType {
    MOVEMENT_BAND_STATIONARY=0,
    MOVEMENT_BAND_SLOW_WALKING=1,
    MOVEMENT_BAND_WALKING=2,
    MOVEMENT_BAND_RUNNING=3,
    MOVEMENT_BAND_JOGGING=4,
    MOVEMENT_BAND_SNEAKING=5
};
#endif
