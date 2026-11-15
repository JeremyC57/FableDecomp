// NCreatureMode::CModeGuildmasterPowerUpShield::~CModeGuildmasterPowerUpShield() -- release guards, beam references and filter.
// Leaf: ??1CModeGuildmasterPowerUpShield@NCreatureMode@@UAE@XZ
// Retail: 0x00743600,121 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CMODEGUILDMASTERPOWERUPSHIELD_PASS22_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode_CModeGuildmasterPowerUpShield.h"
// The original constructor materialises the compiler-generated destructor.
// Carried constructor00742DA0 is graded separately against its own151-byte extent.
NCreatureMode::CModeGuildmasterPowerUpShield::CModeGuildmasterPowerUpShield(CThing& thing)
    : CModeStandBase(thing), Filter("POWER_UP"), Timer(0)
{
}