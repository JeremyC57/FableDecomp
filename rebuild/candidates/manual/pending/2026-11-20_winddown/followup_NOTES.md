# followup author (Account B) -- resume notes, 2026-11-20 (wind-down)

Env: `export S=/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad; source $S/wine/env.sh; W=$S/w25/followup`.
Tools in $W/tools (copied from hand3c2 + hand3c3 c2/): pp.py (probe grade; oracle from $W/oracle_override.tsv), g.py (diff),
var.py (text replacement variants), isite.sh (c2 inline-decision trace of one callee in a function: `FN=<func substring>
tools/isite.sh <src> [callee]`, uses tools/c2/inl_host.py + INLPY=inl.py; inl2.py adds loop-site / callee-flag hooks),
egod_locals_all.py / carve.sh (Ego_d locals + line table), rfind.py (byte search in retail Fable.exe).
FableWin debug bodies (same revision as Ego_d) for many nav / UI functions: ghidra_out/decomp_navmesh2.c and other ghidra_out/decomp_*.c.

## Done
- batches/navedge: 9 rows, READY (0x00A78800 no pragma; new uncatalogued row 0x00A75B60 ForceRegionTo needs a manifest row).
- batches/reg589: 0x00589A30 re-authored in Ego_d text, RELOCATION_MATCH PASS; test is still a host model (externals listed in README).

## c2 inline heuristic facts (measured here, useful elsewhere)
- 0x1078D1AA: cost = IL size - savings + (callee-info byte +0x4E) * size / 254; score = (cost/f + 6 cost)/7 for f <= 1.0, (cost/f + 2 cost)/3
  for f > 1.0 (0x10749C7C); accept if score < 74. Then 0x1078F15A: in-loop sites (site flag 4) of a callee with info flag +0x50 bit 4 get an
  extra check that accepts unless callee sym +0xC3 bit 2 (seems set on functions that do not read memory).
- Static frequency: a `for` with a step expression leaves the code after the loop at the loop body's frequency (x5); a body-stepped for /
  while does not; a body-stepped loop makes its own body hotter.

## Queued, not started
- 0x007E2A60 CTCFineDialogue::GlobalUpdate: DIFFER(707v707) since a36a181a1 shifted CBankFile.h's 37 static-assert lines (pins now 246..,
  were 215..). Overlay with the old pins that matches: $S/bis/pinfix_frozen. Expected lever: c2 symbol numbering (typedef names
  fable_static_assert_<line> are symbols); see the c2 symbol-order kit (rebuild/candidates/manual/pending/2026-11-20_c2_symbol_order) --
  re-author the row in its Ego_d / FableWin text first (that closed 0x00589A30 at once), then count pool symbols if needed.
- Item 3, 0x00A4D920 C3DAnimation2::CalcLocalBoneTransformationRelative: write a real-object test (model: $S/w25/hand3c2/batches/b17,
  0x00A4D770: _alloca pre-fill, call, compare with a double model under _PC_53 / _PC_24; tools/mut2.py with L<line> occurrences).
- Item 6, 0x0040F0E0 CUIScreenUnitConverter::ResetDimensions (Account A's offer): check it is not landed first.
- Pitfall: mut2.py replaces the FIRST occurrence in the file, comments included: check the occurrence lands in code (see navedge README).
