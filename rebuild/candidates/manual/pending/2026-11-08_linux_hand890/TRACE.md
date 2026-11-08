# TRACE -- chunk 890 (agent890): every experiment, hypothesis -> probe -> evidence -> conclusion -> change

Paths: `$A` = `$S/w22/agent890`. Grades: `$A/work/grades/<stem>.txt`, objects `$A/work/objs/<stem>.obj`, diffs `work/d.sh <addr> <stem>`
(work/dd.py: strict = registers + frame offsets kept, loose = both masked), call sequences `work/calls.py`, frame symbols
`work/fas.py` (/FAs listing: `_name$ = -off` per frame symbol), allocator `c2prio.py --trace` (outputs `work/c2/*.trace`),
inline decisions `work/inl/run.sh` + `work/inl/sites.py` (outputs `work/inl/*.json`; per site: r = inlined, f = c2's static
frequency of the call site, sc = score, accepted iff sc < T).

## Extents (verified with $S/w17/xdis.py)
- 0x00551EA0: `ret` at 0x00552DA8, int3 0x00552DA9..AF, next function 0x00552DB0 (`mov eax,ecx; mov [eax],0; ret`) -> 3,849 bytes. The
  catalog gap 4,512 holds the small functions from 0x00552DB0 on (not this row).
- 0x0056AD40: early `ret` at 0x0056AE99 (inside the body, no padding after), final `ret` 0x0056BDFB, int3 0x0056BDFC..FF, next 0x0056BE00
  -> 4,284 bytes.
- 0x00498490: `ret 0Ch` at 0x0049987A, int3 0x0049987D..7F, next 0x00499880 -> 5,101 bytes.

## Row 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw

### E1 (v1) first draft
- Hypothesis: the body is 4 corner blocks + 4 edge lines (CTable virtuals 143/144 = ConstructHorizontalLine / ConstructVerticalLine,
  rtti-vtables 0x0124A224) + 2 separation loops; the line calls take `sep != end ? sep->second : CCountedPointer<CComponent>()`.
- Probe: grade + dd.py + fas.py. Evidence: work/grades/r1_v1.txt, work/fas/r1_v1.asm.
- Result: DIFFER(4025v3849), loose 80.5%; frame 0x68 v 0x58. /FAs shows `$T9886` (4 bytes, -96) = a conditional-temporary flag
  (`or $T9886,2` / `test` / dtor) for the `?:` null arm, plus a second 8-byte slot (-24) for the first loop's position.
- Conclusion: retail has no flag and constructs both arms' argument in the call's own slot, with two call sites (cross-jumped
  when their tails are identical, separate when the registers differ: 0x5526ED/0x55272B v the merged 0x5528A3) -> if/else calls.
- Change: v2 = `if (sep != end) F(it->second, sep->second, ...); else F(it->second, CCountedPointer<CComponent>(), ...);`

### E2 (v2) if/else line calls
- Result: DIFFER(3820v3849), strict 82.2% / loose 91.0%; frame 0x58 = retail. calls.py: retail calls the CCountedPointer(T*) ctor
  COMDAT 0x00429C15 at corner sites 2-4 (inline at site 1), ours expands all four; retail calls tree::find 0x00553520 three times
  (sections 1, 2 and the SEPARATION_TOP lookup), ours twice; the last ConstructVerticalLine pair is cross-jumped in ours.
- Probe: inline trace (work/inl/r1_v2.json, sites.py). VC7.1 regime-0 decision (c2 0x1078D1AA): cost X (c1, +27 for tree::find),
  score = (X/f + 2X)/3 (k = 3), accepted iff score < T; f = c2's static frequency of the call site: function entry 1.0, an `if`
  halves it, a loop body x5, and after a `for` loop the current frequency stays at the loop body's (joins add the not-taken
  share). Ours: sections 1/2 each double f (1 -> 2 -> 4), so the corners run at f 4: CCP sites 2-4 at f 1 (sc 70, inlined) and
  the SEPARATION_TOP find at f 2 (sc 70, inlined). Retail's decisions need the corner code at f 2 (CCP 2-4 at 0.5 -> sc 80
  rejected; SEPARATION_TOP find at 1 -> sc 85 rejected; push_back at 0.5 -> sc 64 still accepted).
- Micro probe (work/micro/f1.cpp, work/inl/m1.json): `for (i = 0; i < n; ++i)` inflates the following code (fA: 1 -> 2 -> 4),
  `unsigned long i = 0; while (i < n) { ...; ++i; }` does not (fB: f back to 1.0 after the loop).
- Change: v3a = section 1's loop as the while form (v3b = section 2's instead: same decisions, same bytes).

### E3 (v3a / v3b) while loop in one section
- Inline trace work/inl/r1_v3a.json: every decision now equals retail (tree::find rejected at sections 1, 2, SEPARATION_TOP; CCP
  inlined at site 1 only; push_back inlined everywhere).
- Result: DIFFER(3817v3849), strict 95.8% / loose 97.5% (v3a and v3b identical).

### E4 (v4) positions declared first; edge totals written inline
- Evidence from v3a (work/d.sh 00551ea0 r1_v3a --hunks; allocator trace work/c2/r1_v2.trace): ours `xor ebp,ebp` at +0xb and stores
  the C2DVector zeroes through ebp (one zero register shared by the integer and float zero stores), retail stores the floats as
  immediates (`mov [esp+..],0`) and keeps `mov ebx,ecx` first. c2 assigns the shared zero to a register only when the integer
  locals (declared first) start the zero run; with the four vectors first, their stores are emitted as immediates. Separately, the
  named `float totalWidth/totalHeight` locals were stored (`fstp`) and reloaded (`fld`) where retail keeps the converted product on
  the x87 stack (`fst` + `fadd`) and reuses it as a CSE for the later `-=`.
- Change: vectors before integers; `topRightPosition.X += horizontalSize * nbHorizontal` (and the `-=`) written inline.
- Result: DIFFER(3849v3849) -- size equal; strict 96.8% / loose 98.4%. First diff: zero-store order (`[esp+0x24],edi` v
  `[esp+0x20],ebp`) and the imul destination at 0x00552084.

### E5 (v5) integer declaration order
- Observation (c2 trace work/c2/r1_v4.trace, d.sh): the order of the integer zero stores follows declaration order, and in
  `horizontalSize * nbHorizontal` / `verticalSize * nbVertical` the later-declared symbol becomes the imul destination. Retail:
  `imul ebp,[nbVertical]`, stores nbHorizontal, nbVertical, horizontalSize, verticalSize in that order.
- Change: declare nbHorizontal, nbVertical, horizontalSize, verticalSize.
- Result: DIFFER(3849v3849), strict 99.2% / loose 99.4%. Remaining: 0x00552162 -- ours `lea ecx,[esp+0x34]` + key store before the
  push for `GetStates()[0]`, retail pushes the key's address after storing it later (scheduling around the map operator[] call).

### E6 (v6) FABLE_RETAIL_STLP_MAP
- Hypothesis: with the map<unsigned long,CUIState>::operator[] body invisible (only a declaration), c1 materialises the key
  temporary before the argument setup; with the body visible (still rejected for inlining, called out of line 0x0042D5B1) the
  temporary's store is scheduled after `mov edx,[esi]`, as retail.
- Change: `#define FABLE_RETAIL_STLP_MAP`. Result: DIFFER(3849v3849), strict 99.8% / loose 100.0%: the only residue is two frame
  offsets at 0x005527A8 (`fld [esp+0x28]` = bottomLeftPosition.X in retail, ours loads the copied position.X `[esp+0x40]` first).

### E7 (v7a/b/c) the separation line's X
- Hypothesis: retail computes `position.X` from bottomLeftPosition.X (the copy source) plus the converted width, not from the
  copy itself. Three spellings, one grade each: v7a `position.X = bottomLeftPosition.X + horizontalSize;` -> RELOCATION_MATCH;
  v7b `position.X = horizontalSize + position.X;` and v7c `position.X += (float)horizontalSize;` -> the E6 residue unchanged.
- Conclusion: the load source is the only decision; v7a keeps the vertical loop's `position.Y += verticalSize` form (that one
  matches as written).

### E8 (v8) owner decision D1 -- the tree find as a member template
- vland reloc identity: retail calls 0x00553520 at three sites; landed under a placeholder spelling (`?find@?$_Rb_tree@UCUnprovenKey_00553520@@...`).
  Owner D1: STLport's `_Rb_tree::find` is a member template (`??$find@W4ETableSprites@NUISystem@@@?$_Rb_tree@...`).
- Change: hdr/stlport/fable_map.h opt-in FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (`template<class KT> iterator find(const KT&)`).
- Result: RELOCATION_MATCH (bytes unchanged; objdump -r shows the `??$find@W4ETableSprites...` symbol at the three call sites).
  Final text src/00551ea0.cpp (adds the header comment and a `SpriteIterator` typedef) re-graded RELOCATION_MATCH (work/grades/r1_final.txt);
  test/00551ea0.cpp PASS under g++ (tall.sh) and VC7.1 (tall.py).
- NAME_FIX: 0x00553520 -> the member-template spelling (NAME_FIXES.tsv).

## Row 0x0056AD40 CTCBoastUI::Activate (catalog: CQuestManager::UpdateUI_QuestMenu)
### A1 identification (no grade)
- Hypothesis: the catalog label is a forge-autoname guess; `this` is not CQuestManager (the quest manager is read from
  [0x013B89FC] separately, and `this` has a map at +0x0C cleared inline, a bool at +0x18 that ends the call, a byte +0x2A cleared
  when nothing is offered).
- Probe: reloc_bindings.tsv (landed caller 0x0056BE00 binds 0x0056AD40), fablewin/egor_pdb_names.tsv, struct_layouts_egor.tsv.
- Evidence: 0x0056BE00 calls it as ?Activate@CTCBoastUI@@QAEXXZ; FableWin.pdb 0x0239BB40 and Ego_r.pdb 0x005E17E0 have that
  symbol (thiscall, no arguments = plain `ret`); Ego_r CTCBoastUI: BoastGroups (map<CMenuEntry*,CBoast>) +0x0C, Active +0x18,
  Components +0x1C, ActivationRequested +0x2A -- every offset the body touches.
- Conclusion: the row is CTCBoastUI::Activate (LABELS.tsv; NOTES label correction).
### A2 decode (no grade)
- work/r2.ann (annotated listing, work/ann.py) and work/r2.cc (destructor runs collapsed): full decode in differ/0056ad40.cpp.
- Not drafted against headers: the UI list classes behind PC_BOAST_LIST / PC_BOAST_SUB_LIST (virtual slots 144, 151, 152, 156) are in
  no NUISystem header, CTCBoastUI has no retail branch, CQuestManager lacks ActiveQuests / GetBoastsForScript /
  GetActiveQuestCardFromScriptName, and three local tree instantiations are out of line (0x0056D930 / 0x0056D9F0 / 0x0056E020 inserts,
  0x0056D130 find, make_pair 0x0056CB40, pair converters 0x0056CB60 / 0x0056DAB0). Session budget ended here: 0 grades.

## Row 0x00498490 CProgressDisplay::DrawRetailDisplay
### B1 identification and layout (no grade)
- Evidence: FableWin.pdb 0x018AEEB0 / Ego_r.pdb 0x004A2EC0 ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z (12
  argument bytes = `ret 0Ch`); the landed caller 0x00499880 binds a non-PDB spelling ?DrawProgress@... (NAME_FIXES.tsv).
- Layout probe: Ego_r member order with STLport vectors (0xC) and 8-aligned doubles gives sizeof 0x88 and puts every offset the row
  touches on a member (+0x04 / +0x10 / +0x1C vectors, +0x28 LoadingScreenIndex, +0x4C TextTag, +0x50 LastTextDisplayed, +0x54
  GuildSealFrameIndex, +0x58 StageCount, +0x70 LastProgressUpdate, +0x7A..+0x81 bools); no header exists (engine/ has only CAProgressDisplay).
### B2 partial decode (no grade)
- 0x00498490..0x00498972 decoded (differ/00498490.cpp: stage total, clamp by reference, installer progress, loading-picture
  advance, the auto-save frame / background split); the rest summarised from work/r3.ann. 0 grades.
