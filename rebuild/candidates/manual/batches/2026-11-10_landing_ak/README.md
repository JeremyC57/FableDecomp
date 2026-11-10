# 2026-11-10 landing ak -- Codex pass 17 return (38 of 40 rows)

Source: `pending/2026-11-10_codex_pass17/return_2026-11-10/` (START_HERE_CLAUDE.md, STATUS / LABELS / NAME_FIXES / LESSONS).
vland --land: 2 MATCH (0x007767A0 CMatrix3x4::Set, 0x00C0A790 LHA::CEventPlayOptions ctor) + 36 RELOCATION_MATCH, tests PASS.

- Headers: Codex's 121-file overlay merged 3-way onto HEAD (base = 3668a4f19, the package's include/): 50 new, 55 applied cleanly,
  12 auto-merged, 2 adjacent-insertion conflicts resolved by keeping both opt-in blocks (NUISystem_CManager.h MENUCOUNT + CLEAR,
  CTCHeroStats.h MORALITYMAX + STAMINA / ADDEXPERIENCE), and 2 headers both sides created today merged by hand:
  CTCSpecialAbilities.h (Codex's 0x28-byte layout + CreateParticleEffect under new FABLE_RETAIL_CTCSPECIALABILITIES_FULL, which row
  0x0078C150 now defines) and CTCAnimationComplex.h (Codex's full class on CTCAnimationBase under new
  FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL, defined by the GetHelperPoint helper header used by 0x004C1060). hdrcheck2: default=same for
  all 71 changed files (logs/header_merge.log).
- Source normalisation: `// Flags: none` / empty Flags lines dropped; 0x00809030 is (anonymous namespace)::CThingFilter_IsInTargetingRangeAndFOV
  with leaf prefix `??0CThingFilter_IsInTargetingRangeAndFOV@?A0x` (the TU hash is path-dependent); 0x006A7FB0 / 0x008098B0 are operator().
  12 manifest names respelled to the proven names (mangled / placeholder / wrong-scope entries, incl. 0x00C0A790 catalogued as
  CEngineWaterBackgroundSubPatch and 0x00632830 as CPersistTraits TransferBinaryIn).
- 0x0057B26F ForceStaminaTo calls the shared GFFloatToLongNear helper (`// Retail-asm: 0x0057B29C` line).

Not landed:
- 0x00871FD0 NCameraModes::CStringAndCageBase::UpdateXYVars: parked 124/124, one swapped fld/mov pair (differ/ + PROBES).
- 0x009D5D30 CBankFileAsyncData ctor: RELOCATION_MATCH per Codex, but its source and the new engine/CBankFileAsyncData.h declare three
  `volatile bool` members. CLAUDE.md / ASM_RULES forbid `volatile`; the package cites an owner exception given in the Codex chat, which
  this lane cannot verify. Held (held/, header in held/hdr/) until the owner confirms here.
Queued `land1011aj`: 45 Codex name fixes.
