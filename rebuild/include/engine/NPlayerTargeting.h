#ifndef FABLE_ENGINE_NPLAYERTARGETING_H
#define FABLE_ENGINE_NPLAYERTARGETING_H
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
namespace NPlayerTargeting {
// Original PDB class, size28: intelligent pointer0, index8, vector12, frame24.
class CTargetPoint {
public:
 CTargetPoint();
 bool operator==(const CTargetPoint&);
 CIntelligentPointer<CThing> PThing;
 long Index;
 C3DVector TargetingPosition;
 long FrameRejected;
};
}
#endif
