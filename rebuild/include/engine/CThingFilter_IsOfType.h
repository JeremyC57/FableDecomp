#ifndef FABLE_CTHINGFILTER_ISOFTYPE_H
#define FABLE_CTHINGFILTER_ISOFTYPE_H
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_IsOfType:public std::unary_function<CThing,bool> {
public:CThingFilter_IsOfType(unsigned long type):Type(type){}bool operator()(const CThing&)const;
private:unsigned long Type;
};
#endif
