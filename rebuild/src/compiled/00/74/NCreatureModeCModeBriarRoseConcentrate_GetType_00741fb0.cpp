// NCreatureMode::CModeBriarRoseConcentrate::GetType() const -- returns 0x3f
// Leaf: ?GetType@CModeBriarRoseConcentrate@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741FB0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBriarRoseConcentrate's vtable 0x01268548 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBriarRoseConcentrate or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBriarRoseConcentrate@NCreatureMode@@UBEKXZ; `ret` matches the signature.
// Header: engine/NCreatureMode_CModeBriarRoseConcentrate.h (opt-ins as in the landed row NCreatureModeCModeBriarRoseConcentrate_GetRandomImposter_008e7f80.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "stlport/fable_count_if.h"
unsigned long NCreatureMode::CModeBriarRoseConcentrate::GetType() const
{
    return 0x3f;
}
