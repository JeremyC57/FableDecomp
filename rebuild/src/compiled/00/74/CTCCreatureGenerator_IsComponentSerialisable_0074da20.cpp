// CTCCreatureGenerator::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCCreatureGenerator@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074DA20, thiscall, `ret`.
// Identity: slot 23 of CTCCreatureGenerator's vtable 0x01268C24 (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureGenerator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCCreatureGenerator@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCreatureGenerator.h (opt-ins as in the landed row CTCCreatureGenerator_InitialiseCreatureGenerator_0074b770.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CTCShapeManager.h"
#include "engine/CThingMarker.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
bool CTCCreatureGenerator::IsComponentSerialisable() const
{
    return true;
}
