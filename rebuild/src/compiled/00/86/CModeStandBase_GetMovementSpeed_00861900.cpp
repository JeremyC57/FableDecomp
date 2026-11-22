// CModeStandBase::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) const -- the mode's walk (MaxWalkSpeed +0x30) or
//   run (MaxRunSpeed +0x28) ceiling for AI_MOVEMENT_WALKING / AI_MOVEMENT_RUNNING, 0.0 for any other type or when the parent thing is the
//   player creature (CClass::Type +0x10 == THING_TYPE_PLAYER_CREATURE).
// Leaf: ?GetMovementSpeed@CModeStandBase@@UBEMABVC3DVector@@0W4EMovementType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861900, 45 bytes, `ret 0Ch`; CModeBase slot 7 of the CModeStandBase-derived vtables 0x012857A8 /
//   0x01285860 / 0x012681EC / 0x01267D10 / 0x01278338 / 0x01285900 / 0x01268774 (classes that are not CModeCarryingStandBase).
// Shape: 0.0f (0x0122DEDC) loaded first and dropped (`fstp st(0)`) on the two member paths, the subtract chain of the type compare.
// NF244 (q12681): the self row moves off its local NCreatureMode::CModeCarryingStandBase model (the catalog name was propagated) onto
//   engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE): Ego_r.pdb / FableWin.pdb / Ego_d.pdb
//   ?GetMovementSpeed@CModeStandBase@@UBEMABVC3DVector@@0W4EMovementType@@@Z (FableWin 0x0283DC30 / Ego_r 0x00816080), the spelling every
//   landed vtable emitter already binds at this address.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_THING_TYPE_IDS
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/EMovementType.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"

float CModeStandBase::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType type) const
{
    float speed = 0.0f;
    if (ParentThing.GetType() != THING_TYPE_PLAYER_CREATURE) {
        switch (type) {
        case AI_MOVEMENT_WALKING:
            speed = MaxWalkSpeed;
            break;
        case AI_MOVEMENT_RUNNING:
            speed = MaxRunSpeed;
            break;
        }
    }
    return speed;
}
