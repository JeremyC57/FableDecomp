#ifndef FABLE_ENGINE_CFRAMERATESMOOTHER_H
#define FABLE_ENGINE_CFRAMERATESMOOTHER_H
#include "rebuild_abi.h"
#include "engine/CInitBaseClass.h"
#include "engine/CArray.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CFrameRateSmoother : public CInitBaseClass {
private: CArray<float> Times;
private: long MaxNoTimes;
private: long NoTimes;
private: long NoFramesToChangeFrameRateOver;
private: long FirstTime;
private: long LastTime;
private: double SmoothedTime;
public: virtual ~CFrameRateSmoother();
};
#endif
