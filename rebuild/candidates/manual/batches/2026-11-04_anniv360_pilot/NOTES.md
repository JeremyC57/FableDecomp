# Pilot: does the Fable Anniversary Xbox 360 Test PDB help parked TLC rows byte-match?

Date 2026-11-04. Read-only pilot. Nothing in the repo was changed; no `--land`; all files are under this directory.

## Verdict: no for matching. It helped 2 of 20 rows a little, and in both the key change was my own inference.

- Rows picked: 20 parked drafts (17 from the brief's residue filter, plus 3 found after dumping all 252 exact-name hits and
  looking for a C2DBoxI stack local). Changes were tried on 15 rows. For the other 5, the 360 data gave nothing to change:
  a stub body, a rewritten body, or a shape the draft already has.
- **Matches: 0.** No row reached RELOCATION_MATCH or MATCH, either in the single-compile grade or in vland's
  pragma/flag sweep.
- **Improved: 2 rows, both "partly" from the 360 data.**
  - 0x00914650 BirdStartled::IsRunnable: DIFFER(341v340) L=72 -> DIFFER(340v340) L=6.
    - What came from the 360 data: the named `area` box built with the `(C2DCoordI, radius)` constructor. That fixed the
      size and the box store order (L 72->44).
    - What did not: the frame fix came from an inner `{}` scope for the out pointer, which I inferred from retail's slot
      sharing.
  - 0x009216e0 MinionFollowThing::IsRunnable: L=94 -> 45. The 360 PDB lists `phero` before a `const CTCFollowed*`, which
    pointed me to move the declaration. The change that worked was the block it sits in, and that again was my
    inference: the 360 PDB has no scopes.
- **Unchanged: 18 rows.** In 7 of them the rewrite the 360 data suggested compiled to **byte-identical** code
  (004ff150, 006b2960, 00940310, 0096cd70, 00970df0, 00959800 b, 008a8010). VC7.1 treats the loop, statement and
  continue/goto shapes these rewrites change as the same code.

L = number of differing instructions in a normalised unified diff (call targets and global addresses masked). It comes from
`tools/qq.py`, which does one compile under the verifier's base flags. vland grades for baseline and best are in
`vl/allbase.log` / `vl/allbest.log`. They agree with qq on every row; the sweep found nothing better.

## What the 360 PDB actually contains for Fable procedures (checked on all 266 procs behind the 252 rows)
- **No scopes.** There are zero S_BLOCK32 records, so the PDB says nothing about nesting. Scope placement is where both
  wins came from, and that information is not in the file.
- **Only stack-homed locals.** 875 STACK and 560 REG records, and the REG ones are almost all `this` and the parameters.
  Locals the compiler kept in registers are missing: loop counters, most pointers, plain ints. So a missing local tells
  you nothing.
- **Symbol order is not reliably declaration order.** GetMarkerFileName lists ext, dir, drive, name, but retail TLC
  constructs drive, dir, name, ext.
- **Line tables** give the statement order and show cold blocks. Without the source text, though, one line number can
  stand for a declaration, a condition or a whole multi-line call. They could not tell apart the forms I tried (for vs
  fetch-ahead, top- vs bottom-tested).
- **The type data is the useful part.** Local types (the C2DBoxI/C2DBoxF `area`, `const` qualifiers, CCountedPointer /
  CDefPointer types, a reference member in CThingFilter_ScaresBirds) and the TPI method lists (for example the
  `C2DBoxI(const C2DCoordI&, long)` constructor) are real. But Ego_r.pdb (TLC's own PDB) already carries most class
  method lists, and it is the right era.
- **Parameter names are cosmetic.** They help naming only. Most are lower_snake (`map_list`, `dst_name`), unlike TLC's
  PDB spellings, so they are no use for the names rule either.
- **Anniversary code drifts.**
  - 3 of 20 rows were rewritten or stubbed: EntitySetAsSittingOnFloor uses the CTCCreatureModeManager system,
    HoverWander searches dead creatures and objects, and StartProgress is a 4-byte stub.
  - 1 exact-name hit is a different template instantiation (0x00619560).
  - Some 360 local types conflict with retail vtable evidence (PickCrops `pcrop`).

## Lesson worth keeping (independent of the 360 PDB)
- Box helpers: when retail stores box corners in an odd order, the inline `C2DBoxI(const C2DCoordI&, long radius)` →
  `Initialise(tlx, tly, brx, bry)` constructor (already in `rebuild/include/engine/C2DBoxI.h` from Ego_r) can give that
  order. Writing the four corners out by hand did not.
- Out-pointer slots: a search out pointer declared in an inner `{}` block lets VC7.1 reuse the slots of an inlined
  helper's dead locals. In 0x00914650 that closed the 4-byte frame gap.
- Both were wins on 1 row and losses on 2 others (00941fc0, 0091b570), so they are candidate recipes, not rules.

## Is a wider pass worth it? No, not as a "use the 360 PDB to fix parked rows" pass.
- Expected yield is low. About 10% of rows improve, with no matches, and the lever in each was not data the PDB supplies.
  Most parked residues are register-colouring or scheduling ties, and the PDB has nothing on those.
- A cheaper follow-up: grep the 360 dump (`tools/proc_dump.py` over the 252 names → `dump252.txt`) for rows whose
  stack locals include an aggregate the TLC draft builds by hand (box/vector/coord). Then try the matching Ego_r
  constructor on just those rows. There are 8 such functions in the 252 (`grep 'STACK .*C2DBox' dump252.txt`). The constructor form
  was tried on 5 of them here (1 improved, 2 neutral, 2 worse); HeightAreaChanged 0x004FEB70 and MinionReactToCorpse 0x00936C70
  are untried.
- The 360 PDB stays useful as a naming and typing cross-check (a second source for local types and method sets). It does
  not help byte matching.

## Process notes
- I went past the ~6-attempt budget on 0x00914650 (14 variants), because each step was still moving the score.
- Oracle bytes were extracted from Fable.exe into `oracle.tsv` (scratch only, never commit).
- vland payloads use a placeholder behaviour test that only prints the pass pattern, so the vland PASS column means
  nothing. These runs grade parity only.
- Leaf selectors: several parked drafts name a leaf that is not in the object (the class is spelled differently in the
  draft), so the grader took the largest function. For the drafts I edited I set a matching `// Leaf:`.

## Files
- `RESULTS.tsv`: per row: residue class, baseline/best grade, attempts, whether the 360 data drove the change, what
  changed.
- `drafts/<addr>.cpp`: the best version of each row. For unchanged rows it is the parked baseline with one comment line
  on top.
- `try/<addr>_<v>.cpp`: every attempt. `base/<addr>.cpp`: the unchanged parked drafts.
- `tools/proc_dump.py`: the 360 PDB extractor. Per procedure: signature, params and locals in symbol order with TYPE and
  location, scope nesting (S_BLOCK32), and the line table (code order and distinct lines). Run it with
  `python3 -I tools/proc_dump.py <pdb> <anniv_pdb/tools> names.txt out.txt`.
- `tools/cls_dump.py`: members and method signatures of a 360 class.
- `tools/qq.py`: single-compile grader with the L/M diff score. `tools/mkoracle.py`: oracle rows from Fable.exe.
  `tools/mkpay_w.py`: vland payload for unlanded rows. `tools/rdis.py`: disassembles an oracle row.
- `dump360.txt` (the 17 brief-filter rows) and `dump252.txt` (all 266 procs behind the 252 exact-name hits).
- `vl/*.log`: vland grades.
