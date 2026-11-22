// 0x00743CB0 test (NF244, q12681): links the row's NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::IsRemoveOnDeactivate (the row's
// class declaration on engine/CModeStandBase.h; no shared header models the class) and calls it non-virtually: true.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#include <stdio.h>
#include <string.h>
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
static unsigned long storage[0x40];
int main()
{
    memset(storage, 0, sizeof(storage));
    const NCreatureMode::CModeIceTrollGroundSmashIcicleAssault* mode = (const NCreatureMode::CModeIceTrollGroundSmashIcicleAssault*)storage;
    if (!mode->NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::IsRemoveOnDeactivate()) { printf("FAIL_00743cb0\n"); return 1; }
    printf("PASS_00743cb0\n");
    return 0;
}
