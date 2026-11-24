// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::AnimUpdate(CAnimateThing&) -- the ice troll's icicle assault does no per-frame
//   animation work of its own: empty.
// Leaf: ?AnimUpdate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXAAVCAnimateThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6850, 3 bytes (`ret 4`); CModeBase slot 2 of the class vtable 0x01268774.
// NF244 (q12681): the self row moves off its stand-in `void AnimUpdate(int)` (?AnimUpdate@CModeIceTrollGroundSmashIcicleAssault@@QAEXH@Z)
//   onto the PDB spelling, Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?AnimUpdate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXAAVCAnimateThing@@@Z,
//   which the landed vtable emitter 0x00743FE0 already binds at this address (NF242 q12677). In-place respell of the row's own class token
//   (namespace and base added, the nf214 in-place rule).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"

class CAnimateThing;
namespace NCreatureMode {
// No shared header models this mode (grep of rebuild/include); the shape of the landed sibling 0x00743FE0's local class (public
// CModeStandBase from engine/CModeStandBase.h), reduced to the two overrides the PDBs give it beyond CModeStandBase.
class CModeIceTrollGroundSmashIcicleAssault : public CModeStandBase {
public:
    virtual void AnimUpdate(CAnimateThing& animateThing);
    virtual bool IsRemoveOnDeactivate() const;
};
}

void NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::AnimUpdate(CAnimateThing&)
{
}
