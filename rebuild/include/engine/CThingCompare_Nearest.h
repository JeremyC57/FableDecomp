#ifndef FABLE_ENGINE_CTHINGCOMPARE_NEAREST_H
#define FABLE_ENGINE_CTHINGCOMPARE_NEAREST_H
#include "engine/C3DVector.h"
// Layout: both PDBs, Pos is the complete12-byte object; FableWin018d32d0.
class CThingCompare_Nearest {
 C3DVector Pos;
public:
 CThingCompare_Nearest(const C3DVector& pos):Pos(pos){}
#ifdef FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
 // Per-TU opt-in (wf9, row 0x006F3A30): operator() declared only -- retail's STL TU calls the
 // <const CThingCreatureBase> instance out of line (0x006EF290, from _S_merge 0x006F04C0).
 template<class T> T* operator()(T* first,T* second)const;
#else
 template<class T> T* operator()(T* first,T* second)const{if(!first)return second;if(!second)return first;return (first->GetPos()-Pos).GetSquaredMagnitude()<(second->GetPos()-Pos).GetSquaredMagnitude()?first:second;}
#endif
};
#endif
