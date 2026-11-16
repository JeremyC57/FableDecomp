#ifndef FABLE_ENGINE_CTHINGFILTER_HASDEFNAME_H
#define FABLE_ENGINE_CTHINGFILTER_HASDEFNAME_H
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;
// Both original PDBs: unary_function base, private CCharString at0; native size4.
class CThingFilter_HasDefName:public std::unary_function<CThing,bool>{
public:
 CThingFilter_HasDefName(const CCharString& name):Name(name){}
 bool operator()(const CThing&)const;
private:
 CCharString Name;
};
#endif
