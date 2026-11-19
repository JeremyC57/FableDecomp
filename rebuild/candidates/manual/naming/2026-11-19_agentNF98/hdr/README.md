# NF98 overlay headers (round 8)

Each file under hdr/engine/ is the current rebuild/include/engine copy (base copy in ../orig/engine/, still identical to the repo
header) with `#ifdef` opt-ins only. Default path: for every header, `cpp -P -D_MSC_VER=1310` of `#include "engine/<H>.h"` gives the
same token stream with the overlay as with the base copy (checked 2026-11-19, work/tok). pin_asserts.py was run after every edit;
"pinned" is the number of FABLE_STATIC_ASSERT lines it respelled FABLE_STATIC_ASSERT_AT(<old line>, ...). No assert was moved.

| header | opt-in (NF98) | what it adds | pinned | landed includers | rows using it |
|---|---|---|---|---|---|
| CCreatureActionBase.h | FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED_BODY | inline body of SetPItemRemoved, the statement 0x0072D360 defined (`#else` keeps the declaration); retail expands it in 0x0072C790 | 0 | 225 | 0x0072C790, 0x0072D360 (pick-any owner) |
| CCreatureAction_CastStrengthBerserk.h | FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHBERSERK_CLASS | retail class on CCombatActionBase, ctor (CThingCreatureBase&) 0x008E6880; generated body under `#else` | 2 | 0 | 0x0072CBA0 |
| CCreatureAction_CastWillEnflameSpell.h | FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_TARGETCTOR | 4-arg ctor (CThingCreatureBase&, CThing*, bool, long) 0x008BB500, inside the existing _CLASS block | 0 | 1 | 0x0072C790 |
| CCreatureAction_SheatheItemToInventory_ForEnflame.h | FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_FORENFLAME_CLASS | class on CCreatureAction_SheatheItemToInventory, 4-arg ctor 0x0069F060, declared virtual dtor 0x0072D390 | 2 | 1 | 0x0072C790 |
| CCreatureAction_UndeadReturnToEarth.h | FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_CLASS | class on CCombatActionBase, ctor (CThingCreatureBase&) 0x008EC6A0 | 2 | 0 | 0x0074ACE0 |
| CCreatureAction_UndeadRiseUp.h | FABLE_RETAIL_CCREATUREACTION_UNDEADRISEUP_CLASS | class on CCreatureAction_RiseUp, FableWin-only inline ctor (body from the bytes 0x0074ACE0), member StillUnderground | 3 | 1 | 0x0074ACE0 |
| CDisplayManager.h | FABLE_RETAIL_CDISPLAYMANAGER_BLITRENDERTARGETSURFACETOTEXTURE | declaration BlitRenderTargetSurfaceToTexture(CTexture&, const CSurface&) 0x009BF350 | 0 | 95 | 0x009BF350, 0x009BF5E0 |
| CPixelFormat.h | FABLE_RETAIL_CPIXELFORMAT_OPERATOREQUAL (needs _ISEQUIVALENT; `#if defined(..) && defined(..)`) | FableWin inline ??8CPixelFormat@@QBE_NABV0@@Z over IsEquivalent | 0 | 81 | 0x009BF350 |
| CTCCreatureGenerator.h | FABLE_RETAIL_CTCCREATUREGENERATOR_TRIGGERERS | typed CreatureTriggerers vector<CIntelligentPointer<CThingCreatureBase>> + protected UpdateUndead decl; `#else` keeps the opaque bytes | 0 | 21 | 0x0074ACE0 |
| CTCInventoryWeapons.h | FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVERANGED | FableWin inline GetActiveRangedWeapon() const (PActiveRangedWeapon.Get()) | 0 | 48 | 0x0072CBA0 |
| CRandomAppearanceMorph.h | FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMTEXTUREMORPHS_BODY | the one definition of GetRandomTextureMorphs (0x0086B810), `inline __declspec(noinline)`, moved unchanged from 0x0086BE40's same-TU body; includes engine/GFRandom.h inside the opt-in | 0 | 7 | 0x0086BE40, 0x0086B810 (pick-any owner) |
| CTCKraken.h | FABLE_RETAIL_CTCKRAKEN_SETHEADTRACKING_INLINE | FableWin-only inline SetHeadTracking body (`#else` the declaration) | 1 | 4 | 0x00924930 |

Sibling overlays read before adding members (NF96: CCategory, CGamePlayerInterface, CTCBase, CTCTavernGame*; NF97:
CGameEventDataPackage, CIsHornTaskWithThisHorn): no header in common, no member declared twice.
