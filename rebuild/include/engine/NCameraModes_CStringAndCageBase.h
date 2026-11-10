#ifndef FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGEBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CSTRINGANDCAGEBASE_H
#include "engine/NCameraModes_CBase.h"
// Layout: Ego_r class0xec, base0xc8; native base0xa4 and class0xc8.
// Exact private members and access independently present in both original PDBs.
namespace NCameraModes {
class CStringAndCageBase : public CBase {
private:
    float DesiredHeightOffset,DesiredStringLength,DesiredCageRadius;
    float CurrentHeightOffset,CurrentStringLength,CurrentCageRadius;
    long NextAutoGoBehindWF,TimerForExpansion;
    bool DriftToMaxStringLength;
    void UpdateXYVars();
};
}
#endif
