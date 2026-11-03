#ifndef FABLE_ENGINE_CTHINGFILTER_NONE_H
#define FABLE_ENGINE_CTHINGFILTER_NONE_H
#include "stlport/fable_containers.h"
class CThing;
// Ego_r TPI0x23f3e, original unary_function<CThing,bool> base, size1.
class CThingFilter_None : public std::unary_function<CThing,bool> {
public: bool operator()(const CThing&) const { return true; }
};
#endif
