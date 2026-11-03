#ifndef FABLE_ENGINE_CTHINGFILTER_HASTC_H
#define FABLE_ENGINE_CTHINGFILTER_HASTC_H
#include "engine/CThing.h"
// Original TPI23ff9: unary_function<CThing,bool> base, private Type enum.
class CThingFilter_HasTC : public std::unary_function<CThing,bool> {
private: ETCInterfaceType Type;
public:
 CThingFilter_HasTC(ETCInterfaceType type):Type(type){}
 bool operator()(const CThing& thing)const{return thing.HasTC(Type);}
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_HasTC)==4);
#endif
