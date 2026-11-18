#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONFACEHERO_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONFACEHERO_H

#ifdef FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO
// Retail opt-in (Codex pass 24): both PDBs name the IsRunnable override. Ego_r gives the
// Hero pointer at +0x14 and FrameLastLookedAtHero at +0x1c, corroborated by 0x00935580.
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"

class CThingPlayerCreature;

class CAIStateGroup_MinionFaceHero : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
private:
    CIntelligentPointer<const CThingPlayerCreature> Hero;
    unsigned long FrameLastLookedAtHero;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionFaceHero) == 0x20);
#endif

#endif
