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
#ifdef FABLE_RETAIL_WHISPER_JUMP_FILTERS_PASS30
 // The retail potion-combat filters compare squared XY distance at 008AABC0.
 bool operator()(const C3DVector& pos) const {
     const C3DVector delta = pos - CentrePos;
     return delta.GetSquaredMagnitude2D() > Radius * Radius;
 }
#else
 bool operator()(const C3DVector& pos)const{return (pos-CentrePos).GetMagnitude()>Radius;}
#endif
};
#endif
