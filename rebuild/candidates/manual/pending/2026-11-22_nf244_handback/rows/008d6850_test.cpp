// 0x008D6850 test (NF244, q12681): links the row's NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::AnimUpdate (the row's class
// declaration on engine/CModeStandBase.h; no shared header models the class) and calls it non-virtually: an empty body that leaves the
// mode and the animation storage untouched.
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
static unsigned char mode_storage[0x100];
static unsigned char anim_storage[0x40];
int main()
{
    unsigned char mode_before[sizeof(mode_storage)], anim_before[sizeof(anim_storage)];
    memset(mode_storage, 0x5a, sizeof(mode_storage)); memset(anim_storage, 0xa5, sizeof(anim_storage));
    memcpy(mode_before, mode_storage, sizeof(mode_storage)); memcpy(anim_before, anim_storage, sizeof(anim_storage));
    NCreatureMode::CModeIceTrollGroundSmashIcicleAssault* mode = (NCreatureMode::CModeIceTrollGroundSmashIcicleAssault*)mode_storage;
    mode->NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::AnimUpdate(*(CAnimateThing*)anim_storage);
    if (memcmp(mode_before, mode_storage, sizeof(mode_storage)) || memcmp(anim_before, anim_storage, sizeof(anim_storage))) { printf("FAIL_008d6850\n"); return 1; }
    printf("PASS_008d6850\n");
    return 0;
}
