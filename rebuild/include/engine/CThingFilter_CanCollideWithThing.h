#ifdef FABLE_RETAIL_RIGIDBODY_CHECK_COLLISIONS_PASS29
#ifndef FABLE_ENGINE_CTHINGFILTER_CANCOLLIDEWITHTHING_H
#define FABLE_ENGINE_CTHINGFILTER_CANCOLLIDEWITHTHING_H
#include "stlport/fable_containers.h"
class CTCPhysicsBase;
class CThing;
// Layout: original TCPhysics at +0, both PDBs and FableWin constructor 025C1C90.
class CThingFilter_CanCollideWithThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CanCollideWithThing(const CTCPhysicsBase& tc_physics):TCPhysics(tc_physics) { }
    bool operator()(const CThing&) const;
private:
    const CTCPhysicsBase& TCPhysics;
};
#endif
#endif
