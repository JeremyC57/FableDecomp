# hand1 lane -- IN PROGRESS (stopped 2026-11-19 at the owner's request)

Owner: "every function it works on to reach a match. No exceptions ... No budget limit" (lane rules: `README.md`, `lane/BRIEF_TEMPLATE.md`).
Owner then: "Save a note about in progress work and stop now ... once done with what you are doing". Nothing below is landed yet;
the 10 rows stay `reserved (hand1, in-house)` in UNMATCHED_ROWS.tsv. Scratch work (may be wiped): `$S/w23/hand1a/`, Fable advisors in
`$S/w23/fable2/` .. `fable8/` (S = this session's scratchpad). The files that matter are copied here: `candidates/`, `advisor_reports/`,
`escalations/`, hand1a's NOTES / STATUS / ATTEMPTS.

## CDefString name swap: DONE 2026-11-19 (`rebuild/candidates/manual/naming/2026-11-19_cdefstring_swap/`)
Executed 2026-11-19: both leaves renamed, bindings swapped, engine/CDefString.h opt-ins re-pointed, 44 caller rows + 19 NScript siblings +
0x006DF860 reworked and rechecked (65/65 RELOCATION_MATCH or MATCH + PASS); 0x0066A9E0 landed (`batches/2026-11-19_hand1_cdsswap/`).
The original plan, kept for the record:

### (was) Owner decision pending execution: CDefString name swap (APPROVED)
Owner chose "Swap and re-land": 0x00415D70 = `?GetString@CDefString@@QBE?AVCCharString@@XZ`, 0x00415DD0 = `??BCDefString@@QBE?AVCCharString@@XZ`
(the reverse of today's binding). Evidence (Fable advisor fable3): 112 / 112 retail small caller->callee pairs put the callee at the lower
address and VC7.1 /O1 /O2 emit callee-first while /Od (FableWin) keeps source order; CDefString's own header compiles GetString, ToWideString,
??B at /O2 (retail 0x00415D70, 0x00415D90, 0x00415DD0); 0x008E7040 rebuilt on the shared header in its natural spelling
(`candidates/008e7040_swap_natural.cpp`: `.GetString().GetLength()` + implicit AnimExists argument) matches only under the swap; 0x008A8C60 /
0x006DEEE0 need no FORCEINLINE_CONVERSION device under it. To do: rebind both addresses in reloc_bindings.tsv; rework and `--recheck-landed`
every landed row that calls them (about 54 rows use CDEFSTRING opt-ins, 42 name the symbols; drop the made-up `N_00415dd0` namespace in
0x008E7040; NScript rows move from OUTOFLINE_CONVERSION to "forwarder inline, GetString declared only"); commit only when all match again.
Then 0x0066A9E0 lands as-is (`candidates/0066a9e0.cpp` + test: strict vland RELOCATION_MATCH PASS apart from that binding conflict; 8/8 mutants).

## Row status (10 rows)
| row | state | next step |
|---|---|---|
| 0x00B0D360 CPSCUpdateNormal::DatabaseSerialise | **MATCHED** (strict vland PASS, test kills 11 mutants) | land `candidates/00b0d360.cpp` + `_test.cpp` (existing opt-ins only; reuses FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE, noted in source) |
| 0x0066A9E0 CTCSoundPlayer::PlayCollisionSound | **LANDED 2026-11-19** (RELOCATION_MATCH + PASS after the swap) | done (`batches/2026-11-19_hand1_cdsswap/`) |
| 0x008EEC70 Mass::Collide | **q.py RELOCATION_MATCH** (fable5) | needs a new shared-header opt-in: grouped `C2DVector::Dot` `return (X*vector.X+Y*vector.Y);` (`candidates/008eec70_C2DVector_dot_group.patch`); then test + strict vland |
| 0x00B6DC40 CEngineWaterRenderer::ProcessSeaLoading | **q.py RELOCATION_MATCH** (fable7 q4, from Ego_d's line table / block extents) | test + strict vland; three callee spellings to check against Ego_d sizes (report) |
| 0x00680050 CThingObjectDef::Validate | **q.py RELOCATION_MATCH** (fable8 c1) | the fix is an include order (CWorld / mesh-bank header before ...): c2 numbers candidate records partly by c1 symbol ordinals (new lever, see report); test + strict vland; decide whether an include-order lever is acceptable |
| 0x00AE3C30 CParticleProgram::ReadFromFile | open (fable7: shape pinned by Ego_d line tables, r4 best) | continue from the report |
| 0x00F3ABF0 CRangeCompressor::CalcCompressionScript | open (fable7 c5 best; `this` prio 58 v 56) | continue; flag two relocation names the report lists |
| 0x005AC10E CTCInventoryTrade::ConstructActualBuyInventoryPC | open (fable8: cause measured -- c2 record numbering by symbol ordinal; no code-neutral source yet) | continue from the report |
| 0x00918A40 CAIStateGroup_GazeAtWork::IsRunnable | open (fable4: best 119v116 with retail's block structure; a removed second predecessor at 0x00918A89) | continue; see landed 0x00929F10's identical residue |
| 0x006E89B0 CCombatWheel::GetAttackerPlacementVector | open (fable6: one u0 no-code node needed at cycle 1-2; draft also has a wrong relocation binding) | continue from the report |

## New levers found (add to GOTCHAS when the rows land)
- Ego_d.pdb also keeps per-line byte tables and `{}` block extents (fable7 tools `egod_lines.py`, `egod_blocks.py`, `egod_fnlines.py`); VC7.1 /Od of a
  candidate reproduces Ego_d's straight-line statement sizes, pinning the source text.
- c2 numbers candidate records from constants freed through a 64-bucket table hashed by value, or by the c1 SYMBOL ORDINAL for addresses: include
  order / declared symbols before a global move register piece order (0x00680050). A fragility for landed rows too.
- A no-code PAREN tuple in a once-expanded inline helper (`C2DVector::Dot`) can be the missing 81-tuple-region node (0x008EEC70).
