# Landing hand3c2 b17: 0x00A4D770 C3DAnimation2::CalcLocalTransformationsRelative + five helper-owner re-ports (Account B, 2026-11-20)

All six rows are RELOCATION_MATCH with their tests passing under strict vland `--land` (`--recheck-landed` before landing). Audited by the
rolling auditor ($S/stage/audit_roll).

| address | row | kind |
|---|---|---|
| 0x00A4D770 | C3DAnimation2::CalcLocalTransformationsRelative | new |
| 0x00A4D920 | C3DAnimation2::CalcLocalBoneTransformationRelative | re-port: the family's Ego_d-shaped helper block; tables renamed to GFastArcCosTable / GFastCosTable |
| 0x00A52760 | C3DAnimationSequenceData::InlineGetKeyframeInterpolate | re-port: the two fitted lines |
| 0x00A53860 | C3DAnimationSequence::InlineGetKeyframeInterpolate | re-port: the two fitted lines |
| 0x00A4C1F0 | C3DAnimationSequenceData::GetKeyframeInterpolate | re-port: the two fitted lines |
| 0x00A22530 | GFFastArcCos | re-port: Ego_d statements (lines 234..243) on GFastArcCosTable; a new real-object test (52,316 cases, bit-exact against a model of retail's x87 sequence) replaces the landed test, which pasted the body and spelled the fistp with `__asm` |

## Why the re-ports
The row needs two fitted helper lines, each marked `// Inferred:` with the measurements in probes.txt:
- SetSlerp's weight is `w2`, not `fraction`;
- the float cast in GFFastArcCos's `pos`. Ego_d line 235 is 24 bytes.

The retail owners of those helpers were landed with the other spelling. Under one text per function (GOTCHAS 455), the owners are re-ported
to the same text. Each re-port is byte-neutral for its own row. The audit also carried over the landed `goto rotation_done;` label.

## Headers
- engine/C3DAnimation2.h (+10) and engine/C3DMesh2.h (+5): add-only; GETBONEKEYFRAME is new (no landed definer).
- hdrcheck2 default=same.
- 58 includers preprocess raw-identical. 11 of them show HEADER_POLICY under strict, the same with the overlay off, so those are
  pre-existing.
- No new header, so no landed row is newly flagged.
- The row's own ConvertTimeToFrames definition is replaced by the existing C3DAnimation2.h body (FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY).
  Ego_d has it in a header.
- Purity retail_asm through the shared GFFloatToLongNear.h / GFFloatToLongFloor.h only, with the `// Retail-asm:` line.

## Mutants
- 0x00A4D770: 101 killed. 20 survived: 17 hit text that is dead in the compiled row, and 3 are equivalent (analysis in the audit report).
- Re-ports: 12/12, 8/8, 8/8, 8/8.
- **Follow-up:** 0x00A4D920's landed test is a host model that does not link the row object, so it cannot kill row mutants. This predates
  this set and is queued.

## Name fixes
NAME_FIXES.tsv, for Account A: the tables are still spelled g_ArcCosTable / g_ACosTable / g_CosTable in 13 landed rows (the PDB has
GFastArcCosTable / GFastCosTable), plus the helper-text variants.
