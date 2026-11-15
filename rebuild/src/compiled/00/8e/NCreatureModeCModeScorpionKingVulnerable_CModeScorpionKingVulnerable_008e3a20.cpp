// NCreatureMode::CModeScorpionKingVulnerable::~CModeScorpionKingVulnerable()
// Leaf: ??1CModeScorpionKingVulnerable@NCreatureMode@@UAE@XZ
// Retail: 0x008E3A20, 115 bytes, thiscall.
// Flags: none
// Evidence: FableWin.pdb 0284D450; native RTTI slot 0 at 0128582C calls this body.
#define FABLE_RETAIL_CMODESCORPIONKINGVULNERABLE_PASS22_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"

// The original constructor materialises the implicit destructor COMDAT.
// Carried constructor: 0x008D8120; audited separately.
NCreatureMode::CModeScorpionKingVulnerable::CModeScorpionKingVulnerable(CThing& thing)
    : CModeStandBase(thing), Filter("VULNERABLE")
{
    PCreature = (CThingCreatureBase*)&thing;
}