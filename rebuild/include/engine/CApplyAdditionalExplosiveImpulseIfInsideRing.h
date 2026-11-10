#ifndef FABLE_CAPPLYADDITIONALEXPLOSIVEIMPULSEIFINSIDERING_H
#define FABLE_CAPPLYADDITIONALEXPLOSIVEIMPULSEIFINSIDERING_H
#include "engine/C3DVector.h"
class CThing;
// Original complete functor: both donor TPI records, size0x14.
class CApplyAdditionalExplosiveImpulseIfInsideRing {
private:
 C3DVector BasePosition;
 float InnerRadius;
 float OuterRadius;
public:
 CApplyAdditionalExplosiveImpulseIfInsideRing(const CApplyAdditionalExplosiveImpulseIfInsideRing&);
 CApplyAdditionalExplosiveImpulseIfInsideRing(const C3DVector&,float,float);
 void operator()(CThing* pnearby_thing) const;
};
#endif
