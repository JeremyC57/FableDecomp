# 2026-11-10 landing ae -- chunk 1010, CGameScriptInterface script-API rows (5 rows)

Source: `pending/2026-11-10_linux_hand1010/`. vland --land: 5 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x008930F0 GetThingWithScriptName (shared headers + the real same-TU noinline CThingSearchTools::GetPFirstThing template body, landed
  0x008ABDF0 shape; two separate null-return ifs), 0x00895190 GiveThingItemInSlot and 0x00897FA0 MakeHeroCarryItemInHand (local-model
  priors rebased on shared headers -- first grade), 0x0089DA70 EntitySetAlpha (PDB template IS_THING_ALIVE(carried[i]), named byte alpha),
  0x0089FEB0 AddCrimeCommitted (STLport list::size distance form, cached end, `if (criminal)` not a ternary). Manifest names were mangled
  or doubled-scope; set to the plain method names (the CGameScriptInterface convention).
- Headers (opt-in only; the agent's CRGBFloatColour forward declaration in CEnvironmentSpellEffectInterpolator.h moved under its opt-in at
  landing): new CThingFilter_HasScriptName.h, CScreenFilterSThingByPass.h, CTCChest.h; 13 new opt-ins (hdr/README.md).

Parked (differ/): OpenChest DIFFER(711v711) (block order), ScreenFilterFadeTo DIFFER(347v363), KickOffQuestStartScreen DIFFER(437v445)
(needs CTCQuestCompletionUI / CScriptRunner headers), EntitySetAsSittingOnFloor DIFFER(472v472), EntityWillTeleportToArea DIFFER(618v620),
StartScriptingEntity (CScriptGameResourceObjectScriptedThing.h is a flat generated layout). Unworked: 0x0089C910, 0x0089CE70, 0x008A7A70,
0x008A8260, 0x0088EDC0. Queued `land1011ae`: 14 name fixes.
