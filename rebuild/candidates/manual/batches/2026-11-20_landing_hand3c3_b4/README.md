# Landing hand3c3 b4: 0x0078A690 CTCHeroRangedAttacks::FrameUpdate + re-port of 0x00789A60 GetShotBuildupTime (Account B, 2026-11-20)

Both rows are RELOCATION_MATCH with their tests passing under strict vland `--land`. Audited by the rolling auditor ($S/stage/audit_roll).

- **Retail TU-mates in the row**, each under the exact Fidelity line:
  - GetRootsNumerically 0x0078B7C0;
  - GetDamageMultiplierFromThingAtBuildUpTime 0x00789CE0 (anonymous namespace, so it has to be in the TU);
  - GetShotBuildupTime 0x00789A60, out of line after the row, as Ego_d lines 751..753 have it.
- **0x00789A60 re-port:** the owner row moves onto the shared engine/CTCHeroRangedAttacks.h, using the copy's text, so the copy is verbatim.
- **Fitted item:** one `// Bracket:` on `((float)AimingTimer) / GetConstantFPS()` in GetShotBuildupTime. The author measured it in c2's list
  scheduler: the bracketed tuple frees `fidiv` one cycle after `fild`, which is retail's order.
- **Headers** (hdrcheck2 default=same; the only DIFFs are the new opt-ins, which no landed row defines; no new header, so no landed row is
  newly flagged):
  - CTCWeapon.h: FABLE_RETAIL_CTCWEAPON_AUTOLOADING, declaration only, Account A's request. It sits beside A's ISPROJECTILEWEAPONLOADED,
    which is kept;
  - CTCSoundPlayer.h: PLAYATTACHEDSOUNDEVENT;
  - CTCHeroRangedAttacks.h: GETSHOTBUILDUPTIME_OUT_OF_LINE, with 25 asserts pinned.
- **Mutants:** 148 killed + 1 equivalent operand swap for the row, 10/10 for the re-port. The row's test runs 34,793 cases.
- **NAME_FIXES.tsv** (for Account A):
  - 0x005DA950 is IsProjectileWeaponAutoLoading; three landed local models call it IsAnalogueLoading;
  - IsProjectileWeapon (landed binder spells it IsMissileWeapon);
  - private IsAbleToUseRangedWeapon and FireRangedWeapon;
  - CMainGameComponent::ConstantFPS (labelled g_framesPerSecond);
  - and others.
