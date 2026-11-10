#ifndef FABLE_ENGINE_GFWORLDDISTANCE_H
#define FABLE_ENGINE_GFWORLDDISTANCE_H
#include "engine/C3DVector.h"
inline float FABLE_FASTCALL GFSqr(float n){return n*n;}
// Original debug01f284e0, locals sqr_dist/sqr_comp.
inline bool FABLE_FASTCALL GFWorld2DDistanceSmallerThan(const C3DVector& pos1,const C3DVector& pos2,float compare,bool using_delta_notional_zero) {
 float sqr_dist=GFSqr(pos1.X-pos2.X)+GFSqr(pos1.Y-pos2.Y);
 float sqr_comp=using_delta_notional_zero?compare:GFSqr(compare);
 return sqr_dist<sqr_comp;
}
#endif
