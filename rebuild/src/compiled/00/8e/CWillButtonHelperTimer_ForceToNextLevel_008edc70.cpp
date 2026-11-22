// CWillButtonHelperTimer::ForceToNextLevel() advances the charged level and restores its held-button time.
// Leaf: ?ForceToNextLevel@CWillButtonHelperTimer@@QAEXXZ
// Flags: (none)
// Both PDBs identify the member. The checked caller and original method use the same
// timer layout, accessor, AI cast limit, and level-indexed timing table as the retail copy.
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_GET_ACCUMULATED_LEVEL_PASS26
#include "engine/CWillButtonHelperTimer.h"

void CWillButtonHelperTimer::ForceToNextLevel() {
    long cur_level = GetWillAccumulatedLevel();
    if (IsAICasting) {
        if (cur_level < LevelToCast) {
            ++WillAccumulatedLevel;
            ButtonHeldDownTimer = (long)WillPowerUpAccumulatedTimings[WillAccumulatedLevel - 1];
        }
    } else if (cur_level < CurMaxLevels) {
        ++WillAccumulatedLevel;
        ButtonHeldDownTimer = (long)WillPowerUpAccumulatedTimings[WillAccumulatedLevel - 1];
    }
}