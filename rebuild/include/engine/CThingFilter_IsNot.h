#ifndef FABLE_ENGINE_CTHINGFILTER_ISNOT_H
#define FABLE_ENGINE_CTHINGFILTER_ISNOT_H
#include "stlport/fable_containers.h"
// Layout: original PDB types and FableWin01ddbff0 store its reference.
class CThingFilter_IsNot:public std::unary_function<CThing,bool>{
 const CThing& Thing;
public:
 CThingFilter_IsNot(const CThing& thing):Thing(thing){}
 bool operator()(const CThing&)const;
};
#endif
