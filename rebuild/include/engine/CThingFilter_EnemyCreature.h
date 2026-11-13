#ifndef FABLE_ENGINE_CTHINGFILTER_ENEMYCREATURE_H
#define FABLE_ENGINE_CTHINGFILTER_ENEMYCREATURE_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThing;class CThingCreatureBase;class CThingAICreature;
class CThingFilter_EnemyCreature : public std::unary_function<CThing,bool> {
public:
#ifdef FABLE_RETAIL_CTHINGFILTER_ENEMYCREATURE_CTOR
 // Retail opt-in (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): Ego_r.pdb / FableWin.pdb
 // ??0CThingFilter_EnemyCreature@@QAE@ABVCThing@@@Z with its body (no out-of-line copy in retail: FrameUpdate stores the source
 // pointer straight into the filter, `mov [esp+1Ch],eax` 0x007F2EDB). Same layout.
 CThingFilter_EnemyCreature(CThing const & p0) : SourceThing(p0) { }
#else
 CThingFilter_EnemyCreature(CThing const & p0);
#endif
 bool operator()(CThing const * p0) const;
 bool operator()(CThing const & p0) const;
private:
 CThing const & SourceThing;
};
#endif
