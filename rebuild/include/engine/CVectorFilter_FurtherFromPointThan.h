#ifndef FABLE_ENGINE_CVECTORFILTER_FURTHERFROMPOINTTHAN_H
#define FABLE_ENGINE_CVECTORFILTER_FURTHERFROMPOINTTHAN_H
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
// Both original PDBs; ctor FableWin02a0fbf0, predicate02a15920.
class CVectorFilter_FurtherFromPointThan:public std::unary_function<C3DVector,bool> {
 float Radius;
 const C3DVector CentrePos;
public:
 CVectorFilter_FurtherFromPointThan(const C3DVector& pos,float radius):Radius(radius),CentrePos(pos){}
 bool operator()(const C3DVector& pos)const{return (pos-CentrePos).GetMagnitude()>Radius;}
};
#endif
