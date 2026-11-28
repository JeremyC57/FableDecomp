#ifndef FABLE_ENGINE_CTHINGBODYREORIENTER_H
#define FABLE_ENGINE_CTHINGBODYREORIENTER_H
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingBodyReorienter {
public:
    void ReorientBodyYZForSlopes();
#ifdef FABLE_RETAIL_CHECK_BODY_REORIENTATION_PASS30
    void UpdateBodyReorienation();                                      // 0x00863E10
#endif
private:
#ifdef FABLE_RETAIL_CHECK_BODY_REORIENTATION_PASS30
    // Ego_r 008325F0 and FableWin 02834530 identify this private method.
    void CheckForNewBodyReorientationAngle();                           // 0x008637D0
#endif
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
