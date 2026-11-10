#ifndef FABLE_ENGINE_CTHINGFIREPOINTFILTERS_H
#define FABLE_ENGINE_CTHINGFIREPOINTFILTERS_H
#include "stlport/fable_containers.h"
class CThing;
// Original empty PDB functors, actual native operators remain out of line.
class CThingFilter_IsPreferredFirePoint:public std::unary_function<CThing,bool>{public:CThingFilter_IsPreferredFirePoint(){}bool operator()(const CThing&)const;};
class CThingFilter_IsOccupiableOccupied:public std::unary_function<CThing,bool>{public:CThingFilter_IsOccupiableOccupied(){}bool operator()(const CThing&)const;};
namespace std {template<class P> inline unary_negate<P> not1(const P& p){return unary_negate<P>(p);}}
#endif
