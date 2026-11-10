#ifndef FABLE_ENGINE_ECOLLIDETYPE_H
#define FABLE_ENGINE_ECOLLIDETYPE_H
// Complete original four-byte enum from both raw donor TPI records.
// OnCollision and BestCollisionType establish its native uses.
enum ECollideType {
    COLLIDE_TYPE_NONE=0,
    COLLIDE_TYPE_THING_SOLID=1,
    COLLIDE_TYPE_THING_INTANGIBLE=2,
    COLLIDE_TYPE_MAP=3,
    COLLIDE_TYPE_MAP_EDGE=4
};
#endif
