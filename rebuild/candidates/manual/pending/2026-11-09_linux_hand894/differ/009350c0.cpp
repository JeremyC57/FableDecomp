// CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired() -- after the scripted action fires, schedule the next firing: now plus the scratchpad's action frequency (seconds, in frames) scaled by a random factor in [1 - variation, 1 + variation]
// Grade: DIFFER(159v159) -- scheduling only: the 1.0f+variation fstp is issued before the variation int copy, and the variation reload sits after `push ecx` (retail before)
// Chunk 894 (4 grades, probe fi/a-d): reordered scale/variation statements, named and inline forms: inert. Next: hand892 sched/sim.py replay of the x87 DAG.
// Leaf: ?Fired@?$CAIStateGroup_ScriptingBase@VCAIStateGroupBase@@@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009350C0, 159 bytes, __thiscall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCAIScratchpad : public CTCBase {                // Ego_r.pdb (0x84); no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_AI_SCRATCHPAD; }
    unsigned char _pad_0c[0x40 - 0x0c];
    long  ActionFrequency;                               // +0x40
    float ActionFrequencyVariation;                      // +0x44
};

template<class TBase> class CAIStateGroup_ScriptingBase : public TBase {
protected:
    void Fired();
    long NextFrameFireable;                              // +0x14
};

template<> void CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired()
{
    CTCAIScratchpad* scratchpad;
    PBrain->Creature->GetTC(&scratchpad);
    long frames = scratchpad->ActionFrequency * CMainGameComponent::GetConstantFPS();
    float variation = scratchpad->ActionFrequencyVariation;
    float scale = 1.0f + scratchpad->ActionFrequencyVariation;
    long delay = (long)((scale - GFFloatRandom(variation * 2.0f, GetPWorldSeed())) * frames);
    NextFrameFireable = CWorld::GetFrame() + delay;
}
