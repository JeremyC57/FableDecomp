# 007602f0 (p25Q, Account B, 2026-11-21): 0x007602F0 NShape::IsShapeConvex -- BRACKET (RELOCATION_MATCH, not landable: edge lines contradict Ego_d; see "Advisor 2026-11-21")

- Grade: RELOCATION_MATCH, 0 diff lines. Strict vland (`LANDVERIFY_STRICT_HEADERS=1`, overlay hdr/) gives `RELOCATION_MATCH PASS`
  (vland.log; the only warning is the `__CIacos` import-thunk alias). Leaf ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z,
  1126 bytes, plain C++, `// Flags: /Gr` (the TU's default, as the landed TU-mates 0x00760020 / 0x00760190; the row also matches without it).
- Header change (hdr/engine/GFAngles.h, hdr.diff): one ADD-ONLY opt-in, `FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST`.
  - Body: `return ((float)acos(value)) * 0.15915494f;` in place of `return (float)acos(value) * 0.15915494f;` (default kept in #else).
  - Mechanism (measured with the c2 DAG dump, tools/sched t20 + dagdump, and the IL dump, tools/il2 dump2 + pp2): the parentheses are one
    no-code PAREN tuple (IL 0x269) between the double->float narrowing and the fmul by 1/(2 pi). The plain body's expansion has none.
    Retail's list scheduler needs that node at every GFArcCos site of the row: the loop's fmul then issues after the three last_line copy
    loads (0x007604A1), and two epilogue pops precede `fadd [esp+10h]` (0x00760726). sim.py edge insertions: only a node on the
    call -> fmul path (or later artificial ones) gives retail's epilogue; a copy of the product before the fadd does not.
  - Same family as the landed GFFastOneOverSqrt NEWTON_PAREN / NEWTON_GROUPED / NEWTON_SQUARE_PAREN opt-ins (extra no-code PAREN tuples).
  - Out-of-line copy: our ?GFArcCos@@YIMN@Z COMDAT is SAME as retail 0x005DB180 (18 B) with this body and with the default body.
    /Od sizes are identical, so Ego_d's GFArcCos (line 151, 35 B, param `value` only) cannot tell them apart.
  - One body could serve every known user: landed 0x007C0690 and 0x006AB820 grade RELOCATION_MATCH with the define added too
    (r6/gfu). Not changed here (add-only rule). 0x00A58130 with GFArcCos (paren or plain, interleaved or both-first, with or without its two
    fitted groupings) stays DIFFER(1272v1272), 4 lines: no second retail site there.
  - Declarations only, no layout change: no asserts to pin.
- Robustness: 1..16 literal int dummies at entry and at the tail: all 32 RELOCATION_MATCH (r6/dum).
- TU contract (tc_geometric_shapes.obj): the row compiles with the TU-mates' define set and engine/NShape.h. The landed TU-mates
  0x0075FE30, 0x0075FF00, 0x00760000, 0x00760020, 0x00760190, 0x00760760, 0x00760E70 all still match with this row's four extra defines
  (NORMALISE_SQUARE_HELPER, SQUARE_GROUPED_PASS23, GFANGLES_SIGNED_ROUNDING, ARCCOS_PAREN_CAST) added (r6/tu). None expands the fast Normalise.
- Device ablation (each removed in turn, from the final text; r6/abl):
  - ARCCOS_PAREN_CAST: DIFFER(1126v1126), 8 lines (loop fmul at the call, epilogue fadd after three pops).
  - NORMALISE_SQUARE_HELPER + SQUARE_GROUPED_PASS23 (existing opt-ins): DIFFER(1126v1126), 28 lines; GROUPED alone removed: 12; HELPER alone removed: 28.
  - /Gr: RELOCATION_MATCH. The TU-mates' C2D / CROSSWITHUP defines: RELOCATION_MATCH.
- Newly flagged rows: none. NAME_FIXES.tsv: header only (the catalogue already names the row NShape::IsShapeConvex).

## How Ego_d decided the text (egod_lines/007602f0.txt)
- Locals and scopes are exactly Ego_d's: pts, result, it_finish_him, it_first, last_line, this_line, second, it_second, first, and a
  loop-scope this_line (it_* const_iterator, TPI 0x8B0C).
- Line for line: the body's 26 code lines sit at Ego_d's 304..343 (src line + 245). Changes from the parked 2-line draft, each byte-neutral:
  - 313 (100 B) is the loop test with an end() temporary: `while (it_second != pts.end())`, not a hoisted `it_finish_him = pts.end()`
    (a hoisted end has no Ego_d line between 311 and 313, and the test would be ~23 B).
  - 324 (8 B, after the loop's 322) is `C3DVector this_line;` (a default-ctor call); the draft declared it before the loop.
  - 334 (19 B) is an iterator declaration (RVO from begin()/end(), 15 B, plus a byte EH-state store, 4 B), as GetFirstPointThatBreaksConvexity's
    second iterator declaration (Ego_d 363, 19 B; its first, 362, is 22 B with the dword EH-state store, like this row's 304):
    `std::vector<C3DVector>::const_iterator it_finish_him = pts.begin();`, and closing 2 reads through it.
  - No Normalise lines (310/318/329/339) and no 341: each `.Normalise()` shares its vector's line; the tail is one return statement.
- Inferred lines (labelled in the source):
  1. The three-float ctor for the four edge vectors. Ego_d's 309/317 (24 B) and 328/338 (44 B) decode as `second - first` (VC10 op-, RVO
     16 B; +3 disp32 temp in the assignments) plus one 8-B member call on the same line, i.e. the 2012 text `v = second - first; v.Normalise();`.
     Retail never reads first.Z / second.Z and stores Z = scale * 0.0f with a two-square sum, so its text zeroes Z first. On this text:
     Ego_d's literal spelling DIFFER(1181v1126); op- + `v.Z = 0.0f;` at all four sites DIFFER(1114v1126); first or loop site only 1130v1126.
     Helper search negative: Ego_d's C3DVector field list (TPI 0x245F) has no 2D normalise; no Ego_d procedure matches
     `?..@..@@YI..AAVC3DVector@@@Z` (a fastcall free function taking one C3DVector&); the only no-argument members are Normalise /
     AccurateNormalise / Clear / IsNonZero / the getters. Needs main's ruling (revision difference against retail's own bytes).
  2. Closing 2's named iterator: `second = *(it_second = it_finish_him + 1);`. `*(it_finish_him + 1)` / `it_finish_him[1]` / `pts[1]`:
     DIFFER(1125v1126); `++it_finish_him` forms: 1127v1126. it_finish_him's initialiser and role are fitted to Ego_d 334; the bytes do not
     fix them (an unused `= pts.end()` there, `it_finish_him = pts.begin() + 1` reassigned, or the old hoisted end all match).
  3. GFArcCos at all three sites (Ego_d 319/330/340 = 29 B, the call form; the raw `(float)acos(..) * K` is 36 B at /Od).
  4. The parenthesised GFArcCos body (the header opt-in above): FITTED.

## Test (test/007602f0.cpp): links the REAL row object (alternatename, like the landed 0x00760190 test), prints `007602f0_TEST PASS`
- An independent double-precision model sums the unsigned XY turns at every vertex of the closed outline; each fixture is checked against
  the model and kept at least 0.002 away from the |sum - 1| = 0.05 threshold. GFastOneOverSqrtTable is the start-up table as the landed
  0x007AF0F0 test spells it (retail's 0x013CE558 is .bss).
- Fixtures: regular 3..24-gons both ways round, offset and with varied Z; square, thin rectangle, irregular hexagon; L shape, arrow, star;
  shallow dents at every vertex of an 8-gon (sum 1.040 true, 1.055 and 1.066 false) and a 5-gon (1.023 true, 1.069 false), so each
  closing vertex and the loop vertices are exercised.
- Mutants (mutants/run.py, mutants.tsv, mutants.log): 25 / 27 KILLED; each mutant rebuilds the row object. Survivors are equivalent:
  #26 (`second = *it_second; ++it_second;` = the post-increment) and #27 (`<=` for `<`, differs only at exact equality).

## Files
src/007602f0.cpp, test/007602f0.cpp, hdr/engine/GFAngles.h + hdr.diff, payload.json, oracle.tsv, vland.log, mutants/,
NAME_FIXES.tsv (header only), egod_lines/007602f0.txt. Work: w26/p25Q/r6 (sch/ DAG + IL dumps, v/ body variants, t/ shape variants).

## Advisor 2026-11-21 and the measurements it asked for
- Edge types: Ego_d's first, second, this_line (both), last_line are all TPI 0x2460 C3DVector (a, b in 0x00763060 too). No 2D type.
- Size test of the three-float form: VC7.1 /Od (odl2) ctor declaration 30 B, assignment 47 B; VC10 delta +3 for the 0.0f ctor argument;
  plus the 8-B Normalise call on the same line: 41 / 58 B against Ego_d 24 / 44 B. Can't fit, so Ego_d contradicts it: the row stays
  reserved and this text is a `// Bracket:`. Every Ego_d-fitting edge text fails retail (literal op- 1181v1126; op- + Z=0 1114 / 1130).
- GFArcCos users: the only landed users are 0x007C0690 and 0x006AB820; both RELOCATION_MATCH with the paren define. The out-of-line
  copy at 0x005DB180 compares SAME for both bodies. So the paren text could be the single GFArcCos body. GFArcCos at all three sites
  needs no Inferred label (Ego_d 29-B lines).
