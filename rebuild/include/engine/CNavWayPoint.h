#ifndef FABLE_ENGINE_CNAVWAYPOINT_H
#define FABLE_ENGINE_CNAVWAYPOINT_H
#include "engine/C2DVector.h"
// Layout: both original PDBs Position0,NavLayer8,Removable9,size10; native list allocation18.
// Existing C2DVector alignment1 supplies the genuine layout without any new packing scope.
class CNavWayPoint {
public:
 C2DVector Position; unsigned char NavLayer; bool Removable;
 CNavWayPoint(const C2DVector& position,unsigned char navLayer,bool removable):Position(position),NavLayer(navLayer),Removable(removable){}
};
FABLE_STATIC_ASSERT(sizeof(CNavWayPoint)==10);
#endif
