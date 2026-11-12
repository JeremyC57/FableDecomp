// CTCFollowed::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFollowed@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D58E0, thiscall, `ret`.
// Identity: slot 8 of CTCFollowed's vtable 0x0125F644 (defined here: the slot holds this body in 1 vtable(s), all of CTCFollowed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFollowed@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCFollowed.h (opt-ins as in the landed row CTCFollowed_IsCreatureValidFollower_006d4e60.cpp).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_IS_ALLOWED_TO_FOLLOW
#define FABLE_RETAIL_CTHING_SCRIPTSUPERVISION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_FOLLOWER_LIMIT
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#include "engine/CThingAICreature.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCCreatureOpinionOfHero.h"
bool CTCFollowed::HasFrameUpdate() const
{
    return true;
}
