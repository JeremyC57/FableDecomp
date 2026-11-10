#ifndef FABLE_ENGINE_CTCCONTAINERREWARDHERO_H
#define FABLE_ENGINE_CTCCONTAINERREWARDHERO_H
#include "engine/CTCContainer.h"
#include "engine/C3DVector.h"
// Layout: native RTTI confirms the base at0; donor Enabled+28 maps to native+24
// after the vector shrinks by4. Adjacent OnSerialise reads Enabled at+24.
class CTCContainerRewardHero : public CTCContainer {
protected:
    void AdjustTrajectoryVector(long row,C3DVector& direction) const;
private:
    C3DVector GeneratePopoutVectorFromVars(long index,long count,long row,float spread) const;
    bool Enabled;
    bool CreateObjectsThatTimeOut;
};
#endif
