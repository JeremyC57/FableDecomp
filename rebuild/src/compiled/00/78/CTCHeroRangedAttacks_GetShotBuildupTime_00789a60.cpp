// CTCHeroRangedAttacks::GetShotBuildupTime() const -- seconds the hero has spent aiming the current shot (aiming timer frames / frames per second)
// Leaf: ?GetShotBuildupTime@CTCHeroRangedAttacks@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789A60, 10 bytes: `fild dword [ecx+18h] ; fidiv dword [CMainGameComponent::ConstantFPS] ; ret`.
// Identity (re-port onto the shared header, audit 2026-11-20; landed before on a row-local class model): Ego_r.pdb / FableWin.pdb
//   ?GetShotBuildupTime@CTCHeroRangedAttacks@@QBEMXZ; Ego_d.pdb procedure in tc_hero_ranged_attacks (file id 2496, the .cpp of
//   FrameUpdate; lines 751..753, debug length 42, no local). An ordinary member of that .cpp, expanded by FrameUpdate 0x0078A690
//   (0x0078A861 `fild` / 0x0078A868 `fidiv`), whose row carries this text as a same-TU copy.
// Bracket: `((float)AimingTimer)` is byte-required in the FrameUpdate expansion (0x0078A868: without it the fidiv is scheduled after
//   `test eax,eax`, DIFFER 1097v1097) and byte-neutral here (the same 10 bytes with or without it); it is written here so that
//   the address has one text.
// Globals: 0x01375550 CMainGameComponent::ConstantFPS (protected static long) through the inline static GetConstantFPS().
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_GETSHOTBUILDUPTIME_OUT_OF_LINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_CTCWEAPON_AUTOLOADING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSOUNDEVENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCHeroRangedAttacks.h"

float CTCHeroRangedAttacks::GetShotBuildupTime() const
{
    return ((float)AimingTimer) / CMainGameComponent::GetConstantFPS();
}