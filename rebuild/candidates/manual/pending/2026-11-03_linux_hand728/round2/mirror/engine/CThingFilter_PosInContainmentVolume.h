#ifndef FABLE_ENGINE_CTHINGFILTER_POSINCONTAINMENTVOLUME_H
#define FABLE_ENGINE_CTHINGFILTER_POSINCONTAINMENTVOLUME_H
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;
// Ego_r TPI Pos +0, VolumeName+12, original value-owning functor.
class CThingFilter_PosInContainmentVolume : public std::unary_function<CThing,bool> {
private:
    C3DVector Pos;
    CCharString VolumeName;
public:
    CThingFilter_PosInContainmentVolume(const C3DVector& p,const CCharString& n):Pos(p),VolumeName(n){}
    bool operator()(const CThing&) const;
};
#endif
