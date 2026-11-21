# Re-port: CThingManager::ActivateThings 0x0051E2F0 and CopyHeroToHero 0x0051F0E0 onto shared headers (Account B, 2026-11-21)
Author p25Q; prerequisite for 0x00521CF0 CThingManager::LoadMainPlayerCreatureState, which matches only with same-TU bodies of both.
The landed local class models become shared-header texts with Ego_d's locals and the PDB virtual names (OnCreate / InitialActivate /
OnInitialActivate, slots 8/9/10); the made-up ShowProgress helper is dropped (Ego_d calls NProgressDisplay::DisplayProgress directly).
Header opt-ins (add-only, hdrcheck2 default=same on all four headers, 0 landed definers of the new macros): CThing.h ENTITYINITLEVEL
(Ego_d enum EEntityInitLevel + private GetEntityInitLevel, ABE in FableWin; `friend class CThingManager`, required by Ego_d's own call
from ActivateThings; Ego_d's TPI carries no LF_FRIENDCLS records at all, so their absence is not evidence), CThingManager.h THING_TYPE_IDS
and LOADMAINPLAYERCREATURESTATE, CTCHeroMorph.h KILLPARTICLES, CTCHeroAttachableAppearanceModifiers.h TCBASE. Strict regrade:
RELOCATION_MATCH PASS for both (regrade.log); new real-object tests (17/17 and 11/11 mutants killed).
