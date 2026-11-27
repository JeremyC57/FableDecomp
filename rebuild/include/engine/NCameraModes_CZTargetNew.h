#ifndef FABLE_ENGINE_NCAMERAMODES_CZTARGETNEW_H
#define FABLE_ENGINE_NCAMERAMODES_CZTARGETNEW_H
#include "engine/NCameraModes_CBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NCameraModes {
class CZTargetNew: public CBase {
private:
    CIntelligentPointer<const CThing> PTargetedThing;
    float BaseOffsetXY, BaseHeightOffset, Theta;
    float DeltaBaseOffsetXY, DeltaBaseHeightOffset, DeltaTheta;
    float TargetBaseOffsetXY, TargetBaseHeightOffset, StringLengthXY, CageRadius;
    C3DVector XAxis, YAxis;
    long FramesForTransition;
    C3DVector GetCameraPosition();
};
}
#endif
