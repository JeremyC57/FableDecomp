#ifndef FABLE_ENGINE_CTHINGFILTER_ISNOTLIST_H
#define FABLE_ENGINE_CTHINGFILTER_ISNOTLIST_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThing;class CThingCreatureBase;class CThingAICreature;
class CThingFilter_IsNotList : public std::unary_function<CThing,bool> {
public:
 CThingFilter_IsNotList(std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > const & p0);
 CThingFilter_IsNotList(std::vector<CThing const *,std::allocator<CThing const *> > const & p0);
 bool operator()(CThing const & p0) const;
private:
 std::vector<CThing const *,std::allocator<CThing const *> > const * PNotList;
 std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > const * PNotList2;
};
#endif
