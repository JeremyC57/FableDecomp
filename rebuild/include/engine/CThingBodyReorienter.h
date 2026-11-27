#ifndef FABLE_ENGINE_CTHINGBODYREORIENTER_H
#define FABLE_ENGINE_CTHINGBODYREORIENTER_H
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingBodyReorienter {
public:
    void ReorientBodyYZForSlopes();
private:
    enum ERotationDirection { RD_NULL, RD_CLOCKWISE, RD_ANTI_CLOCKWISE };
    bool BodyReorientationDone, ReorientingBody;
    long ReorientationCount;
    bool StillCheckingForNewBodyReorientationAngle, HaveFinalReorientationAngle;
    float FinalReorientationAngleXY;
    ERotationDirection RotationDirection;
    bool HasPossibleIntersectingThings;
    std::vector<CIntelligentPointer<CThing> > PossibleIntersectingThings;
    CThing* Body;
};
#endif
