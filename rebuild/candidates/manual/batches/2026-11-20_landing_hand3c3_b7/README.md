# Landing hand3c3 b7: 0x0057C4F1 CTCHeroStats::ApplyBonusItemEffects (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS (40,000 cases), strict vland `--land`. Audited by the rolling auditor ($S/stage/audit_roll).

- **TU mode:** `#pragma optimize("s", on)`, the size-optimised TU of this class, as in 50 landed rows of it.
- **Audit changes** (all byte-neutral):
  - The delivered row-local `GFFloatAbs = (float)fabs(n)` contradicted both debug PDBs, which have GFFloatAbs as the sign-bit mask. With that
    body the row is 1204v1192. It is replaced by <math.h> `fabsf`, labelled `// Inferred:`.
  - The new declarations use Ego_d's parameter names.
  - Two neutral pragmas were removed.
- **Fitted lines:** two `// Inferred:` lines in total: the fabsf spelling, and the product-first fatness sum (retail reads Fatness after the
  GetDefinitionManager call).
- **Headers** (hdrcheck2 default=same; no new header, so no landed row is newly flagged; 127 CTCHeroStats.h includers regrade unchanged):
  - CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_APPLYBONUSITEMEFFECTS (+20);
  - CThingPhysical.h FABLE_RETAIL_CTHINGPHYSICAL_HEALTHMODIFIERS (+6);
  - declarations only.
- **Mutants:** 113 killed + 2 equivalent.
- **Name fix (Account A):** 0x006BB810 is CGameTimeManager::SetTimeOfDay(float) (FableWin 0x02461020 + Ego_d). The landed file calls it
  CThingDrawInformation::SetBarPercentage, a label propagated from a byte-identical body.
