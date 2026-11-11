// NCreatureMode::CModeThunderLightningStorm::GetType() const -- returns 0x36
// Leaf: ?GetType@CModeThunderLightningStorm@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740730, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeThunderLightningStorm's vtable 0x01267DF4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeThunderLightningStorm or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeThunderLightningStorm@NCreatureMode@@UBEKXZ; `ret` matches the signature.
// Header: engine/NCreatureMode_CModeThunderLightningStorm.h (opt-ins as in the landed row NCreatureModeCModeThunderLightningStorm_CreateSkyBeam_008daa80.cpp).
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"
#include "engine/ThingAlive.h"
unsigned long NCreatureMode::CModeThunderLightningStorm::GetType() const
{
    return 0x36;
}
