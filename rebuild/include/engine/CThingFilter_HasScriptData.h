#ifndef FABLE_ENGINE_CTHINGFILTER_HASSCRIPTDATA_H
#define FABLE_ENGINE_CTHINGFILTER_HASSCRIPTDATA_H
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;
// Both original PDBs: unary_function base, private CCharString at0; native size4.
class CThingFilter_HasScriptData:public std::unary_function<CThing,bool>{
public:
 CThingFilter_HasScriptData(const CCharString& data):Data(data){}
 bool operator()(const CThing&)const;
private:
 CCharString Data;
};
#endif
