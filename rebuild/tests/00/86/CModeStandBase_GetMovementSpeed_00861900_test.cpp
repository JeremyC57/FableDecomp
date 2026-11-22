// 0x00861900 test (NF244, q12681): links the row's CModeStandBase::GetMovementSpeed (engine/CModeStandBase.h) and calls it non-virtually
// on storage laid out as the header's asserts give it (ParentThing reference +0x04, MaxRunSpeed +0x28, MaxJogSpeed +0x2C, MaxWalkSpeed
// +0x30) with a thing whose CClass::Type (+0x10) is set per case. One case per path: walking / running for a non-player thing, the
// other movement types, and both speed types for the player creature.
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
#include <stdio.h>
#include <string.h>
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/EMovementType.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
static unsigned char mode_storage[0x100];
static unsigned char thing_storage[0x200];
static void set_float(unsigned char* base, int offset, float value) { memcpy(base + offset, &value, sizeof(value)); }
int main()
{
    memset(mode_storage, 0, sizeof(mode_storage)); memset(thing_storage, 0, sizeof(thing_storage));
    void* thing = thing_storage; memcpy(mode_storage + 4, &thing, sizeof(thing));
    set_float(mode_storage, 0x28, 7.0f); set_float(mode_storage, 0x2c, 5.0f); set_float(mode_storage, 0x30, 2.0f);
    const CModeStandBase* mode = (const CModeStandBase*)mode_storage;
    C3DVector direction(0.0f, 0.0f, 0.0f), facing(0.0f, 0.0f, 0.0f);
    int bad = 0;
    thing_storage[0x10] = THING_TYPE_AI_CREATURE;
    if (mode->CModeStandBase::GetMovementSpeed(direction, facing, AI_MOVEMENT_WALKING) != 2.0f) bad |= 1;
    if (mode->CModeStandBase::GetMovementSpeed(direction, facing, AI_MOVEMENT_RUNNING) != 7.0f) bad |= 2;
    if (mode->CModeStandBase::GetMovementSpeed(direction, facing, AI_MOVEMENT_JOGGING) != 0.0f) bad |= 4;
    if (mode->CModeStandBase::GetMovementSpeed(direction, facing, AI_MOVEMENT_NULL) != 0.0f) bad |= 8;
    thing_storage[0x10] = THING_TYPE_PLAYER_CREATURE;
    if (mode->CModeStandBase::GetMovementSpeed(direction, facing, AI_MOVEMENT_WALKING) != 0.0f) bad |= 16;
    if (mode->CModeStandBase::GetMovementSpeed(direction, facing, AI_MOVEMENT_RUNNING) != 0.0f) bad |= 32;
    if (bad) { printf("00861900_TEST FAIL %d\n", bad); return 1; }
    printf("00861900_TEST PASS\n");
    return 0;
}
