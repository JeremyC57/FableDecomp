#ifndef FABLE_ENGINE_CTHINGFILTER_IN2DRANGE_H
#define FABLE_ENGINE_CTHINGFILTER_IN2DRANGE_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThing;class CThingCreatureBase;class CThingAICreature;
class CThingFilter_In2DRange : public std::unary_function<CThing,bool> {
public:
 CThingFilter_In2DRange(CThingFilter_In2DRange const & p0):Pos(p0.Pos),Range(p0.Range){}
 CThingFilter_In2DRange(C3DVector const & p0, float p1):Pos(p0),Range(p1){}
 bool operator()(CThing const & p0) const;
private:
 C3DVector Pos;
 float Range;
};
#endif
