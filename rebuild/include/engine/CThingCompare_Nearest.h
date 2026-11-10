#ifndef FABLE_ENGINE_CTHINGCOMPARE_NEAREST_H
#define FABLE_ENGINE_CTHINGCOMPARE_NEAREST_H
#include "engine/C3DVector.h"
// Layout: both PDBs, Pos is the complete12-byte object; FableWin018d32d0.
class CThingCompare_Nearest {
 C3DVector Pos;
public:
 CThingCompare_Nearest(const C3DVector& pos):Pos(pos){}
 template<class T> T* operator()(T* first,T* second)const{if(!first)return second;if(!second)return first;return (first->GetPos()-Pos).GetSquaredMagnitude()<(second->GetPos()-Pos).GetSquaredMagnitude()?first:second;}
};
#endif
