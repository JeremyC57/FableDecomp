#ifndef FABLE_ENGINE_COPINIONDEED_H
#define FABLE_ENGINE_COPINIONDEED_H
#include "engine/EOpinionDeedType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
// Original Ego_r TPI19b94/234c1; native00683b00 reads+0/+4; native006c5d8d destroys PTarget+18.
class COpinionDeed {
public:
 EOpinionDeedType DeedType;
 long DeedHandle;
 C3DVector Position;
 long FrameHappened;
 CIntelligentPointer<const CThing> PTarget;
 bool WasSearchingForWitnesses;
 bool IsStillHappening;
 bool IsCrimeReportedToVillage;
 bool IsCrime;
 long ExtraData;
};
FABLE_STATIC_ASSERT(sizeof(COpinionDeed)==0x28);
#endif
