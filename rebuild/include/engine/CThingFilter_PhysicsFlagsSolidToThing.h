#ifndef FABLE_ENGINE_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_H
#define FABLE_ENGINE_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_H
// New header (hand4c 2026-11-19, row 0x0066A1E0): CThingFilter_PhysicsFlagsSolidToThing, Ego_d.pdb class record (0x3925A / field list
// 0x39259: base std::unary_function<CThing,bool>; constructor (const CThing&) and operator()(const CThing&) const; private const CThing&
// SourceThing +0, const CTCPhysicsBase* SourceThingPhysicsTC +4; sizeof 8; Ego_r layout the same, struct_layouts_egor.tsv).
// The constructor body is not recovered (declaration only). Opt-in FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_BODY: the operator()
// body (FableWin.pdb ??RCThingFilter_PhysicsFlagsSolidToThing@@QBE_NABVCThing@@@Z, debug 0x01DAEB10, 119 bytes, Ego_d local
// test_thing_physics_tc: both components present, the source collides with things or one way, then the tested component's answer), which
// retail expands in 0x0066A1E0 (0x0066A354..0x0066A397) and in the out-of-line Bind3 copy 0x00669040; needs CTCPhysicsBase complete and
// CThing::GetPhysicsTC.
#include "stlport/fable_containers.h"
class CThing;
class CTCPhysicsBase;
class CThingFilter_PhysicsFlagsSolidToThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_PhysicsFlagsSolidToThing(const CThing& source_thing);
    bool operator()(const CThing& test_thing) const;
private:
    const CThing& SourceThing;                                         // +0x00
    const CTCPhysicsBase* SourceThingPhysicsTC;                        // +0x04
};
#ifdef FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_BODY
inline bool CThingFilter_PhysicsFlagsSolidToThing::operator()(const CThing& test_thing) const
{
    const CTCPhysicsBase* test_thing_physics_tc = test_thing.GetPhysicsTC();
    if (SourceThingPhysicsTC && test_thing_physics_tc &&
        (SourceThingPhysicsTC->CanCollideWithThings() || SourceThingPhysicsTC->CanCollideWithThingsOneWay()))
        return test_thing_physics_tc->CanCollideWithThings();
    return false;
}
#endif
#endif
