#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONREACTTOCORPSE_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONREACTTOCORPSE_H

#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"

class CThingDeadCreature;

// Original Ego_r/FableWin names and field lists. Native vtable 0x0128B488.
class CAIStateGroup_MinionReactToCorpse : public CAIStateGroupBase {
public:
    virtual void OnActivate();
    virtual void Cleanup();
    virtual bool IsRunnable();
    virtual void Update();
private:
    CIntelligentPointer<CThingDeadCreature> PDeadThing;
};

FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionReactToCorpse) == 0x1C);
#endif
