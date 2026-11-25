# NF421 hand-back (2026-11-25): the /Os GetClassTypeName rows and the CTC header debt

48 rows landed as `2026-11-24_landing_nf421_t1` / `_p1` (RELOCATION_MATCH PASS).
- t1 (13): the 23-byte /Os slot-22 rows respelt from `?GetTypeAsString@<C>_Methods@@QAE...` to `?GetClassTypeName@<C>@@UBE?AVCCharString@@XZ`;
  they spell `return CCharString("<C>", -1);` (the only VC7.1 form that gives retail's 23 bytes; NOTES). The coordinator applied
  RENAMES.tsv: catalog names (functions.tsv, auto-re-candidates.tsv), build_candidates.ps1 Module / Source / TestSource, and git mv of
  the 13 sources and tests; a recheck after the rename grades all 13 RELOCATION_MATCH PASS.
- p1 (35): NF417's local-class rows moved onto their own headers with a prelude of existing opt-ins; no header edits; all sections EQ.
48 self lines applied.
