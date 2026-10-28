# Chunk 525 (agent525, 2026-10-28): 19 of 20 re-queued single functions landed (161-357 bytes)

Re-queue chunk of 20 rows of 161-357 bytes (0x00847470..0x008BF740): creature-action PerformAction / Initialise /
FinishAction bodies and constructors, a camera-mode base constructor and an STLport `__find`. One Opus 5.5 agent at high
effort.

- **Landed:** 19 rows with `vland.py --land` (1 MATCH 0x008AB5D0 `std::__find<CScriptThing*,CScriptThing>`, 18
  RELOCATION_MATCH), every test PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Parked (1, differ/):** 0x0084E420 NCameraModes::CBase::CBase (357v357; the last 8 instructions: retail reuses `bl`
  for the two CullPhysics stores after `pop edi`).
- **Identities:** all 20 row names proven (RTTI vtable / vtable store plus Ego_r / FableWin). New callee proofs:
  0x00692910 PeekDefinitionManager, creature slot 77 IsPlayerCharacter, physics slots 69/70 GetFacingVector overloads,
  CJoystick::AddRumble slot 17, CScriptThing::IsEqualTo slot 78.

Name fixes (NAME_FIXES.tsv, 26 rows, deferred to the naming pass): 0x00692910 spelled GetPDefinitionManager /
GetDefinitionManager in 11 landed callers, 0x006925C0 GetFramesRunning landed as DrawGetEnvironment, 0x008AB5D0 declared
in the global namespace by 0x008A80F0, 0x00692C10 GetWorldMap returns a reference, pointer-vs-reference forms of
ApplyBonusItemEffects, UnsheatheItemFromInventory and CCombatActionBase ctors. Label correction: 0x00849E80 is
PutItemIntoPocket (byte twin of 0x00849D90).
