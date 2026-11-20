# Landing hand3c3 b6: 0x0079D360 CTCBulletTime::CTCBulletTime(CThing&) (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS (20,000 cases, the whole construction order logged), strict vland `--land`. Audited by the rolling auditor
($S/stage/audit_roll).

- **Source:** a shared-header port. The define set and the second half follow the landed sibling UnPause 0x0079D7E0.
- **Audit changes** (all byte-neutral):
  - The CTCBulletTime.h overlay is rewritten to add lines only (+24 -0). The delivered one widened an existing `#ifdef` line.
  - The row-local GetSubDef template is replaced by the shared engine/CDefClassBase_GetSubDef.h.
  - `pcreature` is spelled with GFDebugPDynamicCast, as in Ego_d's 23-byte line and the landed 0x0079C880.
  - An `// Inferred:` label is added on the tick-count statement.
- **Header:** engine/CTCBulletTime.h FABLE_RETAIL_CTCBULLETTIME_CTOR declares the constructor and types DisplacementEffectPrimitive /
  PItemRemoved / HitCreatureBlur as Ego_r has them.
  - hdrcheck2 default=same.
  - 20 includers regrade unchanged.
  - No new header, so no landed row is newly flagged.
- **Mutants:** 82 killed + 2 equivalent.
