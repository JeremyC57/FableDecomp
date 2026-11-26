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
#ifdef FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO_INITIALISE
    // NF476 add-only nested opt-in (row 0x00902045, T3 nf374): the slot-1 override, Ego_r.pdb / FableWin.pdb
    // ?Initialise@CAIStateGroup_MinionFaceHero@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z (public virtual;
    // retail vftable 0x0128B3D4 slot 1 = 0x00902045). Declaration only; it changes the emitted ??_7 slot 1 of an includer that emits the
    // table, so it stays off the default view. Needs FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS (the base virtual and EScriptAIPriority).
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority,
                            const CCharString& name);                    // slot 1 0x00902045
#endif
private:
    CIntelligentPointer<const CThingPlayerCreature> Hero;
    unsigned long FrameLastLookedAtHero;
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CAIStateGroup_MinionFaceHero) == 0x20);
#elif defined(FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO_INITIALISE)
#error FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO_INITIALISE needs FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO (the retail view)
#endif

#endif
