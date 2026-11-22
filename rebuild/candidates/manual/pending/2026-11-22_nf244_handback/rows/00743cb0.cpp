// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::IsRemoveOnDeactivate() const -- the icicle assault mode is removed when it is
//   deactivated: true.
// Leaf: ?IsRemoveOnDeactivate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743CB0, 3 bytes (`mov al,1 ; ret`); CModeBase slot 11 of the class vtable 0x01268774 (catalog vfunc_11).
// NF244 (q12681): the self row moves off its gapscan stand-in `struct T { bool Get(); }` (?Get@T@@QAE_NXZ) onto the PDB spelling, Ego_r.pdb /
//   FableWin.pdb / Ego_d.pdb ?IsRemoveOnDeactivate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBE_NXZ, which the landed vtable
//   emitter 0x00743FE0 already binds at this address (NF242 q12677). The class has no shared header; its local declaration has the shape of
//   the landed siblings 0x00743FE0 / 0x008D6850, on the shared CModeStandBase base.
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

bool NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::IsRemoveOnDeactivate() const
{
    return true;
}
