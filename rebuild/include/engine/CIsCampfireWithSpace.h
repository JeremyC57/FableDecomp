#ifndef FABLE_ENGINE_CISCAMPFIREWITHSPACE_H
#define FABLE_ENGINE_CISCAMPFIREWITHSPACE_H
#include "stlport/fable_containers.h"
class CThing;class CThingObject;
// Ego_d60b62, both original method/public names; native0093b5e0.
class CIsCampfireWithSpace:public std::unary_function<CThing,bool>{
public:CIsCampfireWithSpace(){} bool operator()(const CThingObject&)const;
};
#endif
