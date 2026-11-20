// Behaviour test for 0x00830130 (links the REAL row object; NF123 q4714): the forwarder passes PBrain's object (the first word of the
// CCountedPointer at +0x1A8) as this to CAIBrain::IsUpdateZoneActiveThisFrame (0x0088B810, defined here as a recorder) and returns its answer.
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGAICREATURE_WIFE_QUERIES
#define FABLE_RETAIL_CAIBRAIN_ISUPDATEZONEACTIVE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include <stdio.h>
static const void* g_brain; static bool g_answer;
bool CAIBrain::IsUpdateZoneActiveThisFrame() const { g_brain = this; return g_answer; }
int main()
{
    unsigned long creature[0x200 / 4];
    unsigned long brain[4];
    for (int i = 0; i < 0x200 / 4; ++i) creature[i] = 0;
    creature[0x1A8 / 4] = (unsigned long)brain;
    const CThingAICreature* c = (const CThingAICreature*)creature;
    g_answer = true;
    bool r1 = c->IsBrainUpdateZoneActiveThisFrame();
    g_answer = false;
    bool r2 = c->IsBrainUpdateZoneActiveThisFrame();
    if (!r1 || r2 || g_brain != brain) { printf("FAIL_00830130\n"); return 1; }
    printf("PASS_00830130\n");
    return 0;
}
