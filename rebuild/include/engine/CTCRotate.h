#ifndef FABLE_ENGINE_CTCROTATE_H
#define FABLE_ENGINE_CTCROTATE_H
// Layout and names: both original PDBs; donor026fd4f0 and native007c1000.
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
class CTCRotate:public CTCBase {public:
 void SetRotationInfo(const C3DVector&,float,float);
 void SetRotating(bool rotating){Rotating=rotating;}
private:C3DVector RotationAxis;float RotationSpeed,NumRotations;long Timer,TimerLimit;bool Rotating;CCharString ObjectToRotateAround;long Index;bool DummyRatherThanHelper;
};
FABLE_STATIC_ASSERT(sizeof(CTCRotate)==56);
#endif
