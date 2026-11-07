# Chunk 832 -- expert retry of the ten closest parked rows (agent832)

Lane: VC7.1 RTM under Wine, `$S/tools/q.py` parity grades (BASE_FLAGS `/O2 /Oy /W3`), no header overlay (hdr/ is empty), no
git, nothing written under the repo. Grades are ~1-2 s per TU on this lane, so every row got a regrade first and then batches of
distinct spellings (`probe/<addr>_<tag>.cpp`, outputs `tmp/g/` deleted at hand-over; STATUS.tsv lists every attempt and grade).

## Matched (2): the IsAlive lea hoist, same lever for both

- **0x00D6A560 `NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main`** -- RELOCATION_MATCH (`src/00d6a560.cpp`).
  Shape: chunk 781's parked body unchanged except that the assault guard is a VALUE local (`CScriptThing guard = Game->
  GetNearestWithScriptName(Me, "HTE_AssaultGuard");`) and its three `IsAlive()` tests go through a same-type
  `reinterpret_cast<const CScriptThing&>(guard).IsAlive()` (the owner-accepted CMapKeeper 0x00DA91A0 form, 2026-11-07). The
  cast keeps the virtual call virtual and makes VC7.1 compute `this` first, so the shared `lea ecx,[esp+2Ch]` is hoisted above
  the wait loop's terminate `jne` into the exit pad (0x00D6AC5A -> 0x00D6AD54), which was the only residue. Names: all from the
  chunk 781 park (PDB spellings, two sources cited there); no callee or binding changed. Test: chunk 781's, PASS (g++ and VC7.1).
- **0x00EA1720 `NScript::CV_KnotholeGladeGatesScript::DoInitialBalverineAttack`** -- RELOCATION_MATCH (`src/00ea1720.cpp`,
  `// Flags: /Gr` as parked). Same lever at both sites: the two murderer balverines as VALUE locals, the wait-loop `IsAlive()`
  through the same-type reinterpret_cast; both leas hoist as retail (0x00EA1CB2 and 0x00EA1E41 -> 0x00EA1E60). Names as parked
  by chunk 782. Test PASS (g++ and VC7.1).
- Read-only vland on both: RELOCATION_MATCH PASS, WINS 2 (`tmp/vland_v1.log` at the time; lv/ deleted).
- Label note: `$S/batch/chunk832_oracle.tsv` / `$S/w22/chunk832.tsv` name 0x00D6A560 `NScript::CHTE_FreedomFighter::Main`
  (no `CQ_HangingTreeEvilScript::` scope); `ghidra_out/labels_*.tsv` already carry the full PDB scope, and the source line 1 /
  payload use the full name, so no LABELS.tsv row (the short name is a chunk-list spelling, not a Ghidra label).

## Parked (8) -- best source in differ/<addr>.cpp (+ differ/<addr>.test.cpp), line 2 residue, line 3 next idea

- **0x00B5AF80 RenderColorFilteredQuad** DIFFER(2863v2865) 17 lines (8 relocation-equivalent table+8 operands + the x87 shape
  of `tint * scaledContrast * 0.5f`). The brief's unjustified `scaledContrast * 1.0f` is removed. The CRenderStateManager
  bookmark `test/or` flip is FIXED by the clamp mix "contrast clamp written out (if / else if), GFLimit (PDB const-ref
  `??$GFLimit@MMM@@YIXAAMABM1@Z`) for the level and alpha clamps". The flip position is a whole-function count effect: every
  knob moves it by whole setter sites (see STATUS.tsv: all-GFLimit = 2 sites late, by-value GFLimit = exact, all written out =
  1 early, contrast written out + two GFLimit = exact; a const-local 0.5f multiplier or an operator- inverse = 1 later). The
  product residue: retail keeps the X product on the x87 stack and spills Y/Z/W; ours spills X and keeps W. Only the rvalue
  multiplier (`scaledContrast * 1.0f`, probe x6 on the same base) reproduces it -- x6 grades RELOCATION_MATCH but is not
  handed back as a match because the fold has no source justification (brief). GFLimitReturn<float> (PDB-named, returns a
  const ref) gives the address-of-temporaries shape, not retail's.
- **0x00B5A3F0 ApplyColourFilter** DIFFER(2923v2916) 15 lines: the same clamp mix fixes the flip; the remaining residue is the
  offset-site `or [eax+3D8h],ecx` RMW (ours load/or/store with inverse.X hoisted) + the alignment nop + 8 reloc operands. The
  RMW moves with the same kind of count knob but at a different threshold (grey built from a C3DVector gives the RMW and
  moves the flip); no byte-preserving spelling hit both.
- **0x005E78F0 CTCQuestCompletionUI::GlobalUpdate** DIFFER(2340v2340) 4 lines: two x87/int interleave swaps (spill store vs
  `fmul`, load vs `fdivp`). Not the /G pairing model (/G5 /G6 /GB identical, /G7 worse -- diagnostics only); 10 spellings
  identical or worse. Reads like a memory dependence retail's scheduler honoured: next try a divisor read through an
  address-taken local / `const float&` accessor, and a `const CUIMiscThingsDef*` local (my s28 misspelled the type).
- **0x00CFE030 COracleTablet::Main** DIFFER(3575v3575) 8 lines ([IV+begin] vs [begin+IV] element addresses). 31 grades: no
  element-access spelling moves it; removing the phase-1 DiggingPoints erase loop (diagnostic) flips both arms to retail's
  order, but every byte-preserving respelling of that loop and of the final loop does not. Micro-TUs show the operand order is
  set by the IR temp numbering of the code before the arms (and ebp as IV forces index).
- **0x00DE8370 CRansommer::Main** DIFFER(4894v4894) 12 lines: the two block-1 not-enough-gold latch copies colour the parent
  temp ecx (`add`) where retail has eax (`lea`); an explicit source rotation gives the identical bytes (no colour change), the
  other 7 spellings are identical or worse. Chunk 821's 10 attempts are not repeated.
- **0x00DD5A60 CPrisoner::Main** DIFFER(2246v2250) 21 lines, the documented per-exit ~CTimer split. New finding: the GUARDS
  comment-arm and hit-arm exits sit in ONE inliner bucket here (/d2inlT73 outlines both, T74..T90 neither; diagnostics only),
  so retail's source scores the hit-arm exit lower than the comment-arm exit. A nested inline `CTimer::operator==` (PDB
  member) at 1-4 sites does not move the bucket; plain / `while (!T)` loop tops cost 58 lines.
- **0x00D18DF0 CBH_GangDeputy2::Main** DIFFER(3337v3321) 2 lines (two MovieBase vptr stores of the FORCEINLINE user dtor) --
  the documented caller-size gap (implicit dtor expands only under /d2inlS15). Untried ideas tried: explicit CCharString("")
  movie name (frame moves), dtor spec after the header (C2908). Parked early per the brief.
- **0x00D0A830 CBanditKing::Main** DIFFER(2700v2720) 20 lines (one called MovieBase dtor at the merged terminate exit; rest
  jump-table bytes) -- the documented heat gap. Untried ideas tried: post-wait T alone in do{}while(0) (expands the in-loop
  exit only, 24), CCharString("") (448), spec after header (C2908). Parked early per the brief.

## Names, bindings, headers
- Every name is the parked (proven) spelling; no new callee, global or symbol was bound, so no reloc_bindings one-symbol-
  one-address check was needed. NAME_FIXES.tsv and LABELS.tsv are header-only.
- hdr/ is empty: none of the ten rows needs a header overlay (the two colour-filter rows use the repo CRenderStateManager /
  CShaderRenderManager / C4DVector headers as they stand).
- Two descriptive spellings remain in the parked colour-filter rows as parked: `g_pDisplayManager_01436e1c` (extern name for
  0x01436E1C) and the C4DVector/GFLimit template in-row definition (PDB signature). Nothing new.
