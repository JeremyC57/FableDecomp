#ifndef FABLE_ENGINE_CLIPSYNCDATABANKENTRY_H
#define FABLE_ENGINE_CLIPSYNCDATABANKENTRY_H
#ifdef FABLE_RETAIL_DISPLAYNEXTMESSAGE_PASS30
#include "engine/CIVCountedPointer.h"
// Ego_r TPI: Data is offset zero, Duration offset four. Native reads the float at +4.
class CLipSyncDataBankEntry {
public:
    float GetDuration() const { return Duration; }
private:
    class CData;
    CIVCountedPointer<CData> Data;
    float Duration;
};
#endif
#endif
