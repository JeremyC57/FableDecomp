# chunk 4006 -- NOTES (kept current; hand-over in place)

Rows (both CTCInventoryStats, vtable 0x01254ADC):
- 0x005D1F0C slot 89, 17,873 bytes = `?ConstructPersonalityListPC@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z`
- 0x005CE0E6 slot 86, 15,910 bytes = `?ConstructPersonalityList@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z`
Name evidence (two sources each): both spellings are in the Ego_r and FableWin name sets; the virtual order in
engine/CTCInventoryStats.h is ConstructScreen(77, 0x5cd074) / ConstructPersonalityList(86) / ConstructExperienceList
(87, 0x5d6748 landed) / ConstructExperienceListPC(88, 0x5d7081 landed) / ConstructPersonalityListPC(89); the PC row
references the strings PC_STATS_PERSONALITY_LIST / PC_UI_PERSONALITY_FRAME / PC_STATS_PERSONALITY_SCREEN.

## CONTINUATION (second agent) -- FINAL: row 1 RELOCATION_MATCH (src/005d1f0c.cpp + test/005d1f0c.cpp); the steps in order (g07 .. g60)
Best source = probe/base.cpp (generated; `python3 tools/gen.py probe/base.cpp pc`, VIS default = ConvertMap). Measure with
`bash tools/m.sh <abs src> <tag> "<note>"` (size, frame-masked equal, exact equal, hunk addresses; appends to probes.log) and
`python3 tools/fw.py <tag>` (frame-word map retail -> ours).
- Residue 1 (loop entry 0x005D2F77) CLOSED by g07: the visible same-TU body of `CTCInventoryStats::ConvertMap` (0x005CD6B7, landed row,
  written here over the real STLport / CWideString headers, tools/vis/ConvertMap.inc) before the row. CreateTitlePC's body alone
  changes nothing (probe a2). PROVEN in the sense that retail compiles ConvertMap before this function in the same TU.
- Residue 4 (342 frame-word displacements) CLOSED by g08. Frame-packer trace (tools/pk/run.py = agent2042's phook; show.py; wl.py):
  the temporaries of a statement that contains an inline-asm helper (GFFloatToLong*) get +2 weight (two `c` increments from
  c2.dll 0x107E25C9, one per asm instruction, on every escaped temporary of the statement). Retail's FISHING temporaries sit in the
  slots a weight-2 temporary gets (they follow the first GFRoundFloatNear expansion's locals in the packer list: last three in the
  8 / 16 / 12-byte slots, the rest S70..S68), so in retail the helper is NOT inside the FISHING statement:
  `float biggestFish = GFRoundFloatNear(heroStats->GetBiggestFish(), 2);` is its own statement (after `grams`), the value stays on
  the x87 stack into the next statement. CHICKENKICK keeps the helper in the expression (its 14 temporaries are weight 4 in retail too).
  That removed FISHING's three weight-4 nodes from the single slots A / B / C (retail +30 / +10 / +08), whose densities then sort
  below the 8-byte / 12-byte / `it` slots as in retail; the `empty` / `heroTitle` tie came right with it.
- Residues 2 and 3: see "g57 / g58" below.

### g57 / g58: RELOCATION_MATCH (17873 v 17873, 5617 / 5617 instructions) -- how residues 2 and 3 closed
- Residue 3 (0x005D5A40 and every other "bank load late" flake) was the scheduler's 81-tuple region cut: the run from FISHING's loop
  exit 0x005D4B7F to the MONEY title is ONE run of 14 full regions (tools/sched.sh + tools/sched/cuts.py list each cut). Retail itself has
  a cut inside a lookup at 0x005D4EB3 (`push eax ; mov ecx,[bank] ; lea ; push ; call`), which pins the tuple count up to there.
  What moved the later cuts: (a) the three best times are `GFRoundFloatNear(heroStats->GetBestTimeX(), 1)` -- c2 folds the one-trip
  loop and the constant divide (`fmul 10.0f` ... `fmul 0.1f`), the bytes equal the hand-expanded `(float)GFFloatToLongNear(x * 10.0f) *
  0.1f` but the IL has other no-code tuples; (b) the shared GFRoundFloatNear body returns a PARENTHESISED expression,
  `return (GFFloatToLongNear(value * scale) / scale);` (one PAREN tuple after the divide; `(float)(...)` is equivalent). The existing
  shared engine/GFPackFloat.h body without the parentheses gives 5611, the predecessor's own body (named `rounded`) 5614.
  The predecessor's hdr/engine/GFRoundFloatNear.h duplicated the shared GFPackFloat.h helper and is DELETED; the overlay now carries
  hdr/engine/GFPackFloat.h = the repo file with the parenthesised return (regrade of its landed users: see hdr/README.md).
- Residue 2 (0x005D4698 SIB): c2prio with the record id (`sid`, patched into tools/c2p/c2prio.py leaf info) shows the two getter results
  as inline-local records 6082 (houses, eax) and 6086 (shops, ecx); the smaller id becomes the SIB index (c2019), so ours had houses as
  index. Retail (BOTH rows: 0x005D4698 and 0x005D00AB) has shops as index, i.e. the shops record reuses an id freed after the houses
  record was allocated. 25 hand-written forms of the sum in the row (operand order, named locals, casts, parentheses, `+=`) never give
  it; one more inline level on the houses side does: an inline returning `GetNumberOfHousesOwned() + GetNumberOfShopsOwned()` (d3), the
  houses getter through an inner inline (d4 / d8) or with a named local in its body (d10) all give retail's bytes; nesting only the shops
  getter (d5), an identity inline round the sum (d7), an inline Sum(a, b) (d6) do not. Kept: `long GetNumberOfBuildingsOwned() const
  { return GetNumberOfHousesOwned() + GetNumberOfShopsOwned(); }` in the overlay CTCHeroStats.h -- FITTED, DESCRIPTIVE NAME (no PDB has
  it; FableWin's inline COMDAT run after ConstructPersonalityList lists GetNumberOfHousesOwned / GetNumberOfShopsOwned /
  GetNumberOfBuildingsRented as 22-byte plain getters and no sum helper, which points AGAINST a named helper in that later revision).
  Review at landing: the bytes prove an extra inline level on the houses side; which of the three forms the original had is not proven.

## State (first agent; superseded by the CONTINUATION block above)
Row 1: best source = `probe/base.cpp` (generated: `python3 tools/gen.py probe/base.cpp pc`; spec in tools/gen.py,
head in tools/head_pc.inc). See grades.log for the grade history and the section "Residues" below.
Row 2: NOT STARTED (no source). `map/st1.txt` holds its decoded statements; the statistics body is the same as row 1
with CreateTitle (0x005CCF0E) instead of CreateTitlePC and no AddParam action blocks. gen.py has a `console` mode
switch but `tools/head_console.inc` is not written.

## Tools (all in tools/, every Bash call starts with the lane prologue and FABLE_HDR_OVERLAY=$A/hdr)
- `d.sh <from> <to>`: compact retail disassembly window (8-hex addresses), from dis_all.txt (row 1 lines 1..5619).
- `calls.py <0|1>` -> map/calls<row>.txt; `map.py <row>` -> map/ev<row>.txt (one line per call, operands symbolic,
  frame slots Lp<hex>/Lm<hex> = [ebp +/- hex]); `st.py <row>` -> map/st<row>.txt (text quads merged; 1073 / 743 lines).
- `gen.py <out.cpp> [pc|console]`: source from the statement spec; optional tools/patch_pc.txt (`<line>\t<text>`).
- `cc.py <src> <tag>`: compile to tmp/b/<tag>/x.obj (tags starting 'h' = row 2 leaf, else row 1).
- `pd.py <tag> [--frame] [--lo X --hi Y] [--max N] [-n ctx] [--sum]`: relocation-masked instruction diff v retail;
  `--frame` masks `[ebp +/- N]`.
- `v.sh <tag> <lo> <hi> [sed -E ...]`: probe = seds applied to probe/base.cpp, compiled, frame-masked hunks in range,
  one line appended to probes.log.
- `c2p/c2prio.py` (c2 allocator priority tracer; `export C2PRIO_RUNS=$A/tmp/c2runs`; `--func ConstructPersonalityListPC
  --trace --blocks`, about 30 s).
- `rel.py`: copied from agent4003 UNMODIFIED (paths still point there) -- adapt before use.

## Measurements
- TU mode: `#pragma optimize("s", on)` ALONE. The biased frame `push ebp; lea ebp,[esp-74h]; sub esp,0E4h` comes from
  "s" alone; adding `optimize("y", off)` (as the landed twin 0x005D7081 has) gives a plain `mov ebp,esp` frame (g01).
- Frame: saved ebp [ebp+74h], return [ebp+78h], the `parent` parameter home [ebp+7Ch] is reused as the `text`
  CWideString slot. esi = "TEXT_GUI_STATS_LABEL_" literal kept as a constant, ebx = -1, edi = heroStats.
- The unused `CTCHeroExperience* heroExperience; ParentThing->GetTC(TCI_HERO_EXPERIENCE, &heroExperience);` compiles to
  just the bitset test with `mov eax,[esi+4]; lea ecx,[eax+20h]` (retail 0x005D1F66). `HasTC()` gives
  `mov ecx,[esi+4]; add ecx,20h` (wrong).
- Implicit `const char*` -> CCharString temporaries are constructed at statement start; inline helper bodies
  (GFFloatToLongNear/Floor) inside an expression hoist to statement start too -- matched as in-expression calls.
- GFRoundFloatNear as plain `inline` is called out of line in the size TU -> FABLE_FORCEINLINE in
  hdr/engine/GFRoundFloatNear.h (Inline-budget evidence still to be measured with the c2 inline trace).
- EXPERTOTAL: the four-term sum must be a named local (`long totalExperience`) in its own statement (retail keeps it in
  a frame slot; in-expression put it in ebx).
- Renown slider: `float percentage = ...; long renownLevel = ...;` then `(float)renownLevel + percentage` (in-expression
  gave `fiadd`).
- NUMBUILDINGS: Houses + Shops load order matches retail; Shops + Houses does not.
- Float constants: 0x01237F0C 10.0f, 0x0122EE58 0.1f, 0x01254DAC 275.0f, 0x0122DEDC 0.0f, 0x01254DA8 0.8f,
  0x01254D84 285.0f, 0x01254D80 7.0f, 0x01230A08 100.0f. Wide literals 0x01231728 "\n", 0x01236A50 ": ", 0x0122D70C "",
  0x0122D820 " ", 0x0122DE74 "/". 0x013BCA24 = CWideString::NullString.

## (first agent, historical) Residues of the g06 source (differ/ copy deleted after the match) (g06: 17880 v 17873 bytes, frame-masked equal 5610/5617, exact 5270/5617)
Compare with `python3 tools/pd.py g06 --frame --max 60 -n 2` (frame-masked) and `python3 tools/pd.py g06 --max 0` (exact count).

### 1. Creature-list loop entry 0x005D2F77 (+7 bytes; the only size residue)
Retail: `mov ecx,[ebp+50h] ; mov eax,[ecx] ; cmp eax,ecx ; jmp 0x005D2FE6` into the loop bottom's tail
`mov [ebp+0Ch],eax ; lea eax,[ebp+64h] ; push eax ; jne body`. Ours: sentinel in eax, iterator in ecx, so the entry tail is not the
bottom tail (no `lea ; push` hoist, no cross-jump). Row 2 retail 0x005CE981 shows the same register choice in its raw form
(`cmp eax,ecx ; mov [it],eax ; je exit ; jmp body+3`): the iterator's entry piece owns eax in both rows.
MODEL (c2prio + tools/pj.py, measured):
- integer colouring takes esi (label-prefix literal), edi (heroStats), ebx (-1); then ONE split pass over the 36 candidates live across
  calls (`it` #114, `this`, bank, container, const 1 ...). Pieces get priority K1 * cost where K1 counts the candidates still listed in
  the block AFTER the split; the block-local CSE temporary of the sentinel load (`creatures` #74) keeps its pass-0 priority K0 * cost.
- base: the loop init shares its block with the CREATURESKILLED title statement, K0 = 8 (creatures, it, container, prefix, const 1, bank,
  this, -1), K1 = 5 (container, bank, const 1 are single-reference split candidates and vanish): creatures 32 v `it` piece 20 -> the
  sentinel takes eax first.
- with ANY block boundary between the title statement and `it = creatures.begin()` (diagnostics ve / vj1 / vj2: `if (manager == 0)
  return 0;` after ConvertMap, after the title, after the list declaration) K0 = K1, the priorities tie, the larger key wins (`it`:
  its last write is `++it` in a later block) and the WHOLE loop is retail's (entry, jmp, hoisted `lea ; push`, merged tail).
- a memory-only iterator (diagnostic vd: `DiagTouch(&it)`) gives retail's entry too, but an escaped `it` computes `&it->first` before
  the GFIntToWideString call, which retail does not: retail's `it` is not escaped.
So the missing thing is a tuple-block boundary after the title statement that leaves no code, or another way to equal K.
No effect (all compile to base, probes.log): `for` with copy-init / assignment / iterator declared before; `creatures.end() != it`;
`!(it == end())`; `it++`; const_iterator; `while`; `for (;;) { if (it == end) break; ... }`; hand-rotated `if / do-while`; a named
`end` iterator (changes the bottom compare, wrong); `_M_node` compare; inline helper holding the loop (vi1: inlining leaves no
boundary); zero-code constructs after ConvertMap -- empty inline call, `do {} while (0)`, `if (inline true) {}`, `if (x) {}`,
`if (x) EmptyInline();`, zero-trip `for`, `switch (0)`, `goto` + label, `while (false)`, foreach-macro idioms
(`if (bool b = false) {} else for`, `for (bool once ...)`): the front end / optimiser folds them all before allocation.
`__assume(...)` there changes code elsewhere (5606 equal) and is not a fix.

### 2. 0x005D4698 NUMBUILDINGS
Retail `mov ecx,[edi+0E0h] ; mov eax,[edi+0E4h] ; ... ; lea edx,[eax+ecx]` (shops loaded first, houses is the SIB base). Ours
(`GetNumberOfHousesOwned() + GetNumberOfShopsOwned()`) has the same loads and `lea edx,[ecx+eax]`. Shops + Houses swaps the loads
(worse); named locals `houses` / `shops` = same as base (vn1); `buildings = houses; buildings += shops` is worse (vn2, 5606).
The TROPHIES sum `GetNumMountedTrophies() + numTrophies` (0x005D43EE, same shape) matches.

### 3. 0x005D5A40 MONEY title statement
Retail `mov ecx,[ebp+60h] ; push eax ; lea eax,[ebp+1Ch] ; push eax ; call` (the order of all 88 lookups); ours puts the bank load
after the `lea` at this one statement. The flake moves with unrelated edits (g03 had it at 0x005D4EB3 and 0x005D51D6). At this
statement retail's temporaries sit in unusual words ([ebp+38h], [ebp-20h], [ebp+1Ch]), so it is most likely tied to residue 4
(temporary identity), not to the statement's spelling.

### 4. Frame words (exact-only, about 340 instructions)
retail -> ours: +3c->+38 x40 (`this`), +38->+34 x60, +34->+30 x20, +30->+3c x24 (a CWideString temporary, first at 0x005D2D28),
+10->+1c x18 (`heroMorph`, first at 0x005D2884), +14->+10, +18->+14, +1c->+18 x64, +0c->+08 x23 (`it` and the tattoo index),
+08->+0c x16 (a temporary, first at 0x005D426C), -38<->-30 (`empty` v `heroTitle`). I.e. three single words sit on the other side
of their neighbours and one pair is swapped. Not analysed with the packer replay (agent3016 tools/sim2.py + phook trace,
agent3002 LESSONS "c2's final frame packer is reproducible offline"); do that AFTER residue 1, the loop iterator is one of them.

Extra facts for residue 4 (read off retail, not yet used):
- Label lookups `T(name)` keep their three CCharString temporaries in the word set {[ebp+4Ch]..} in most statements, but the statements
  with float work in between (CHICKENKICK 0x005D4A12.., FISHING 0x005D4B6B..) use {[ebp-0Ch], [ebp+4], [ebp+2Ch]} for the label and
  {[ebp+10h], [ebp+8], [ebp-4Ch]} for a second lookup (METRES). Ours gives FISHING's label the second set ([ebp+1Ch] = retail's
  [ebp+10h] word, shared with `heroMorph`), i.e. the node-to-word membership differs per statement, not only the word order.
- `heroMorph`'s word [ebp+10h] is shared in retail with temporaries of BOASTS (0x005D427F / 0x005D4324), CHICKENKICK (0x005D4A9C /
  0x005D4AF5) and the six pub lines (0x005D4D4E .. 0x005D53E1): 18 references; ours has 20 in that word (the two extra are
  FISHING's label CCharString at 0x005D4BBB / 0x005D4C72, which retail keeps at [ebp-0Ch]).
- `empty` / `heroTitle` / `abilityName` all have 5 references: their order is a packer weight tie (order in which each reaches the
  weight during the walk).

## Still to verify
- Every relocation symbol (rel.py not adapted; vland): STLport COMDATs (list ctor 0x005D84B6, list dtor 0x005D864B,
  vector<CWideString> dtor 0x00412130, push_back 0x004128A0, params push_back 0x0042B646), CCountedPointer names,
  `_free` 0x00BFEA14 v operator delete for the vector<pair<long,long>> release.
- Names: 0x0057AA1B GetNumRegionsExplored (landed spelling CTCHeroStats_Retail::GetNumberOfRegionsVisited is in neither
  PDB); 0x0057D23B UpdateAppearanceStats (catalog FrameUpdate_ApplyMorphEffect); landed stand-in class
  `CTCHeroStats_Retail` on 0x0057AA5B / 0x0057AAA8 / 0x0057B3E2 / 0x0057ACC4 / 0x0057AD1D / 0x0057A8AF / 0x0057AAD9.
- Retail-asm sites needing `// Retail-asm:` lines: bare `fistp` at 0x005D4A15, 0x005D4B8C (GFRoundFloatNear x2),
  0x005D4D1A, 0x005D4E4F, 0x005D4F84 (GFFloatToLongFloor x3), 0x005D50B0, 0x005D51D3, 0x005D52F6 (GFFloatToLongNear x3).

## Overlay headers (hdr/engine, opt-in blocks; default text unchanged)
CTCInventoryStats.h (FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY), CTCHeroStats.h (..._CTCHEROSTATS_PERSONALITY),
CTCHero.h (..._CTCHERO_TITLEGUITEXT), CTCInventoryItem.h (..._ISTROPHY), COpinionReactionManager.h (..._HEROSTATS),
NUISystem_CManager.h (..._STATICSLIDERBIDIRECTIONAL), new CTCHeroMorph.h, new GFRoundFloatNear.h.

## Behaviour test (differ/005d1f0c.test.cpp; PASS under g++ and VC7.1)
Generated: `python3 tools/gentest.py <row source> <test.cpp>` = tools/test_head.inc (mock engine) + hero-stats getters generated
from hdr/engine/CTCHeroStats.h (each logs the member offset it reads) + the row's function text unchanged + kRetail + tools/test_main.inc.
- Expectation kRetail comes from retail only: `tools/rtok.py` cuts the retail disassembly into text statements (boundaries: calls to
  CWideString::operator= 0x0099B7D0, operator+= 0x0099B8D0, CreateTitlePC 0x005CCFFC) and lists per statement the string literals
  (S:), semantic direct calls (f<addr>), virtual calls (v<vtable offset>) and `[edi + off]` hero-stats reads (o<off>). 101 statements;
  the arm run 1 does not take (favourite ability == -1, 0x005D34B3..0x005D350E) is skipped.
- Run 1 (everything present, each loop once) must reproduce kRetail statement for statement; runs 2-4 take the other arms and check
  page text, title parts, trophy sum, MaxMoney, sliders, action wiring, no-screen.
- Mutations: `tools/mut.sh differ/005d1f0c.test.cpp` -- 16 single edits of the row statements, 16 fail the test.
- VC7.1: `python3 tools/tvc.py differ/005d1f0c.test.cpp 005d1f0c` (tall.py only looks in test/).
- The mock takes wide literals as `const wchar_t*` (g++ needs it for L"" literals); the row source itself spells `unsigned short`.

## Relocation names (tools/rel.py <tag> --all | --odd; adapted to this chunk)
64 symbols, every one pairs with one retail target. The "several targets" lines rel.py prints for the FISHING statement
(0x005D4BE0..0x005D4C7D) are alignment artefacts of its text matcher: retail uses [ebp-20h] / [ebp-0Ch] there and ours positive
offsets, so the normalised text differs and the matcher slides (the statement's call order was checked by hand against retail).
Spellings that differ from rebuild/oracles/reloc_bindings.tsv (ours are the PDB manglings; see NAME_FIXES.tsv): 0x005CCC91
GetTimeSpentInGame, 0x005D9930 IsTrophy, 0x009C7C20 NGameText::ConstructString. Unbound (unlanded callee or first use): the twelve
CTCHeroStats out-of-line getters, CTCInventoryStats helpers, _List_base ctor / dtor COMDATs 0x005D84B6 / 0x005D864B, 0x007009D0,
0x0071CE80, 0x0042819A (LABELS.tsv).

## Ranked untried ideas (first agent; ALL SUPERSEDED -- ideas 1, 3, 4, 5 were worked and closed the row, idea 6 = row 2 is open)
1. (residues 3, 4, maybe 2) Visible same-TU bodies. The retail TU holds CreateTitle 0x005CCF0E, CreateTitlePC 0x005CCFFC,
   GetFatnessAsString 0x005CC862 (landed), GetHeroSexualityAsString 0x005CCBBD, GetTimeSpentInGame 0x005CCC91, BuildRenownString
   0x005CD433, BuildHeroClass 0x005CD4C4, ConvertMap 0x005CD6B7 (landed, with local stand-in classes) BEFORE this row. Lessons
   0x00B399A0 / 0x00A4D920 / 0x00550DC0: a visible same-TU callee body changes what c2 keeps across the call and how temporaries
   share frame words. None of them is visible in the current source (declarations only).
2. (residue 1) Find what puts a block boundary after the title statement in retail's tuples. Not tried: a real conditional that c2
   only removes after allocation (branch to the next instruction); bodies of same-TU callees (idea 1) making some call "known";
   the row-2 source (console) may be easier to read: its loop keeps the raw `je exit ; jmp body+3` shape (0x005CE981).
3. (residue 4) Replay the frame packer (agent3016 tools/sim2.py; needs the phook pack trace of this function) and name the nodes
   of the displaced words: heroMorph [ebp+10h], the NUMDAYS temporary [ebp+30h], `it` [ebp+0Ch], `empty` [ebp-38h].
4. (residue 2) Operand record order of the houses + shops add: try the sum through a differently typed expression (unsigned,
   a cast), or the two getters called in separate full expressions inside the argument.
5. Inline-budget numbers for GFRoundFloatNear (c2 inline trace, agent4001 / agent2003 inl.sh) -- only the before/after grade is recorded.
6. Row 2 (0x005CE0E6): write tools/head_console.inc (CreateTitle, console component strings from map/st1.txt), `gen.py <out> console`,
   and extend gentest.py's CFG with row 1 (rtok.py already takes the row number).

## FINAL STATE (continuation agent)
- Row 1 0x005D1F0C: RELOCATION_MATCH (q.py with the Leaf; read-only vland RELOCATION_MATCH PASS, 5 RELOC_NAME warnings = catalog labels
  v PDB names already in LABELS / NAME_FIXES; `_List_base` ctor / dtor COMDATs are unlabelled STLport instances). Source = generated
  (`python3 tools/gen.py probe/base.cpp pc`, then copied to src/). Test = `python3 tools/gentest.py src/005d1f0c.cpp test/005d1f0c.cpp`
  (g++ PASS, VC7.1 PASS, `bash tools/mut.sh test/005d1f0c.cpp`: 19 of 19 mutations fail). purity.classify = retail_asm.
- Relocations: `python3 tools/rel.py g60 --odd`: 179 symbols, 179 retail targets, 0 inconsistencies.
- The same-TU ConvertMap copy in the row file: `q.py 005cd6b7 src/005d1f0c.cpp '?ConvertMap@CTCInventoryStats@@'` = RELOCATION_MATCH.
- Wide text in the test mock is `unsigned short`; the two `const wchar_t*` overloads sit under `#ifndef _MSC_VER` for g++ (where the
  row's L"" literals are wchar_t), the convention of the landed test 0x00540980.
- Asm evidence: probe/asm/tbl/ (gen.py, run.py, ctrl.py, mktable.py, table.md): 36 + 15 spellings for the five near sites (two
  fragment shapes: places 2 and places 1), 32 + 14 for the three floor sites, five flag sets, positive control through the helpers.

### FITTED v PROVEN
PROVEN (two sources or retail-only evidence): the row's name and signature; every callee binding (PDB manglings, 179 / 179 relocation
pairing); the same-TU ConvertMap body (its own bytes match 0x005CD6B7, and retail compiles it before the row); GFRoundFloatNear at
the five sites (FableWin ?GFRoundFloatNear@@YIMMJ@Z is the first inline COMDAT after ConstructPersonalityList; the best-time bytes equal
its places-1 expansion; the shared header body is proven out of line by GFPackFloat 0x00AF8D50); the fish weight outside its text
statement (frame-packer weights, measured); statement list and order (behaviour test against the retail statement cut).
FITTED (bytes match, form not provable from the PDBs): `GetNumberOfBuildingsOwned` (name descriptive; one of three forms that give
the bytes); the parenthesised return of the shared GFRoundFloatNear; the local names (biggestFish, totalExperience, percentage,
renownLevel, goodness, ...); the enum EOpinion gap names (first agent); `float biggestFish` being declared after `grams`
(retail order of the two statements is proven by the code order 0x005D4B45 v 0x005D4B6B, the name is not).
Descriptive (made-up) names: CTCHeroStats::GetNumberOfBuildingsOwned; local variable names; OPINION_AGREEABLENESS / OPINION_RENOWN.

### Row 2 hand-over (0x005CE0E6, NOT_ATTEMPTED, untouched)
Everything row 1 needed applies to row 2 (same TU, same statistics body; retail row 2 shows the same loop entry choice 0x005CE981,
the same NUMBUILDINGS `lea edx,[eax+ecx]` 0x005D00AB): the ConvertMap body ahead of the row (ConvertMap precedes row 2 in retail too),
`float biggestFish` as its own statement, GFRoundFloatNear(x, 1) for the three times, GetNumberOfBuildingsOwned, the overlay
GFPackFloat.h. gen.py already has all of it in the shared statement spec; write tools/head_console.inc (CreateTitle 0x005CCF0E,
the console component strings from map/st1.txt, no AddParam action blocks) and extend gentest.py's CFG with row 1 (rtok.py takes the
row number). Tags starting with `h` select row 2 in cc.py / pd.py / m.sh.
