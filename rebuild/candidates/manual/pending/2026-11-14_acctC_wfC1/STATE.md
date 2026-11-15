# STATE -- 0x00CBFB7D RunCutsceneMacro (wfC1; INTEGRATOR round 2, 2026-11-14 23:30-00:10): **MATCHED**

- `src/00cbfb7d.cpp` (= `differ/00cbfb7d.cpp`, copy tmp/differ_hist/r2_f8_RELOCATION_MATCH.cpp): **RELOCATION_MATCH** under q.py and read-only vland
  (PASS, 0 RELOC_IDENTITY, 25 RELOC_NAME label warnings listed in LABELS.tsv). 74,778 bytes, 20,435 instructions, 4,068 calls, frame 0x18DC.
  No `// Flags:` line, no inline pragma. `test/00cbfb7d.cpp` prints `00cbfb7d_TEST PASS` under vt.py (model test, 11 mutants). addmatch done.
- Integration log (integ/i2/; `ig.sh` = jump-masked instruction grade, `qg.sh` = OFFICIAL q.py grade + byte-level differing instructions):
  | step | change | result |
  |---|---|---|
  | s01 | G6: IsEnd() helper -> named iterator; `$` test `substitutions && ...` | no byte change (17 open) |
  | s02 | G5: default CDefPointer::operator->, 36 dead row asserts, CSPositionY(start), `((duration + random) + time)`, CrowdAnimate order | 2 open (S1) |
  | s03 | S1: `lookAt += C3DVector(offsetX, offsetY, offsetZ)` | 4 open (S1 + S2: four records fewer before `random`) |
  | u1 | 16 `count > 0` crowd asserts -> 16 parsed-parameter asserts before the verb; `long frames = 0;` hoisted; named `offset` | 0 instruction differences, still DIFFER under q.py |
  | u2 | TeleportToHSP: `CUTSCENE_FRAME()` after the teleport (0x00CCA417 `jmp 0x00CCA733`, the one jump-target difference) | RELOCATION_MATCH |
  | f1..f8 | comments, `morality != 0.0f`, stale comment removed, globals respelled (g_ScriptFormattedText, g_pScriptRunner), 44 locals renamed, last parameter `skippable` | RELOCATION_MATCH each |
  Rejected: V060 analyst's two-argument CSDeltaY (G5's CSPositionY covers V060 and keeps two more records); S2-S3 / S1 analysts' one-assert
  CDefPointer overlay (G5's default operator-> text makes the overlay file unnecessary: hdr/engine/CDefPointer.h removed from the overlay).
- IMPORTANT for anyone re-grading: tools/g.py, integ/sm.py and every analyst gr.sh compare instruction TEXT with jump targets masked. Use
  integ/i2/qg.sh (q.py line 1); integ/i2/qd.py prints instructions whose bytes differ outside a zeroed relocation field.
- Devices in the matched source (hand-back questions; each is marked in the source; owner's acceptance test: byte match yes, behaviour
  unchanged yes, portable yes):
  1. 36 dead `FABLE_RETAIL_ASSERT` (class d): 16 on parsed magnitude parameters GiveHeroMorality .. CameraFOVLookBetweenPos, 13 `i < count`
     + 7 time / speed parameters in the Crowd* verbs. Count FITTED to the caller-size budget (34..38), places FITTED to three record-id facts
     (0x00CCB3C6, 0x00CCE8EA, 0x00CCED7D); `// Inferred:` block above the function.
  2. `FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT` (fable_string.h opt-in, one dead arm; FITTED carrier, forceinline EVIDENCED).
  3. FITTED named locals (class a): `float fraction = 0.0f;` (.SlideTeleport / SlideTeleport), `C3DVector offset` (CameraFOVLookBetweenPos),
     hoisted `long frames = 0;` (CrowdAnimate); row-local inline `CSPositionY` (descriptive name; .SlideTeleport only).
  4. Header opt-ins FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE / FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE (G6: image-wide evidence in
     an/G6-devices-and-test/PATCH.md).
  5. Naming: two descriptive globals (g_CutsceneSoundFrame 0x013B83C8, g_CutsceneMusicOverridden 0x0143E8FD). The 38 b<frame offset> bool locals
     and `b_c` / `level` / four `speed` were renamed after the callee parameters of the Anniversary PDB (integ/i2/anniv/params.txt, rn.py; weak third
     source, locals only); `mostAligned` (GetBestThingFromMarkerList's flag) is descriptive.
- Not done (optional follow-ups, none needed for the match): a natural source of the two extra c2 records before CrowdAnimate's `random`
  and of the dead-IL volume (true carrier unknown: probably asserts inside inlined helpers); a behaviour test linked against the row object
  (the lane's tests are models); original names of the parameters (Anniversary: macro_name, entitymap, flagmap, input_args, setupcond, skippable).

---
# STATE -- 0x00CBFB7D RunCutsceneMacro (wfC1; INTEGRATOR round 1, second session 2026-11-14 19:30-21:10)

## Summary of this session (source unchanged: differ/00cbfb7d.cpp still DIFFER(74778v74778), 17 instructions, all x87 order)
- All analyst patches were already integrated by the first integrator session (table below); this session worked the remaining 17 instructions.
- Finding: the four open sites are ONE class -- c2's commutative x87 operand order there is keyed on the IL volume before the site (dead IL counts;
  symbol / named-local rulers do nothing). Measured with DIAGNOSTIC dead-IL rulers (integ/sw2.sh <tag> <source line> <k...>, prints one line per k):
  | ruler position | k (10-tuple dead units) -> S2 0x00CCE8EA / S3 0x00CCED7D / V060 / V061 |
  |---|---|
  | inside V060 (line 1261) | 16: A/o ; 32, 48: B/R (V061 Z sum tmp-first from 48) ; 64: A/R ; 96: A/R + V060 Z sum tmp-first |
  | CameraFOVLookBetweenPos body (line 2089) | 8..24: A/o ; 32..58: B/R ; 59..64, 117..119 (5-tuple unit): A/R |
  | CrowdAnimate body (line 2577) | 36..68: B/R |
  S2 states: A = time,random,duration (ours), B = random,duration,time, retail = duration,random,time (never produced). S3 R = retail. S1 0x00CCB3C6 never moved.
  V060 retail = Y product in memory + Y sum tmp-first; ours only ever flips Z (field creation order X,Y,Z; retail probably X,Z,Y = constructor-built `step`).
- c2 trace of the current source: integ/cp/cur.json (+ integ/pairs.py, cl.py, pk.py); falsified key models are listed in NOTES.md (last 7 lines).
- Hand-back consequence: the G1 dead-assert carriers (fitted volume) also decide these x87 phases, so the remaining residue is a question of WHERE the
  dead IL sits (which helper / which verb carries the asserts), not of statement spellings at the four sites. One inline decision after V060 has a margin
  below 30 tuples (3 dead units inside V060 change the object), so volume must be moved, not added.
- Tools added in integ/: mc.py + ord.py (micro TU: compile + operand-order letters), site.py + cls.py (order at the four sites from a /FAs listing),
  rl.py + sw.sh + sw2.sh (rulers at a source line), cp.sh (c2 trace), pairs.py / cl.py / pk.py (trace readers).

---
# STATE -- 0x00CBFB7D RunCutsceneMacro (wfC1; INTEGRATOR round 1, 2026-11-14)

## Current source and grade (round-1 integration)
- `differ/00cbfb7d.cpp` = baseline + G1 (3 `#define` lines + overlay opt-ins) + G3 `$` change + 8 crowd-analyst changes + 3 G4 changes +
  V060/V061 in-place step (CSSlideTeleportStep device removed). **DIFFER(74778v74778)**: 20435 v 20435 instructions, strict-equal 20418,
  frame-masked equal 20428, 5 hunks (all V060), calls 4068 v 4068 (0 target differences), frame 0x18DC = retail. No flag, no pragma.
- Overlay `hdr/` now holds the G1 analyst's three header changes (fable_assert.h new file, engine/CDefPointer.h new file, fable_string.h
  + one opt-in); previous fable_string.h in tmp/hdr_bak_r1/. Baseline source kept in tmp/differ_hist/r0_baseline.cpp.
- Integration log (one grade per step, tags ig_s01..ig_s14 in GRADES.log; step sources integ/s/sNN.cpp):
  | step | change | bytes | strict | loose | hunks | calls | frame |
  |---|---|---|---|---|---|---|---|
  | s01 | G1: 3 defines + header opt-ins | 74862 | 18495 | 19755 | 628 | 4071 | 0x18DC |
  | s02 | G3: `$` test split (line 401) | 74830 | 18679 | 20081 | 318 | 4069 | 0x18DC |
  | s03 | crowd 1: CrowdAcquire if/else | 74830 | 19135 | 20082 | 316 | 4069 | |
  | s04 | crowd 2: CrowdKill single while | 74762 | 19465 | 20410 | 26 | 4068 | |
  | s05 | crowd 3: Remove withFade first | 74762 | 19466 | 20411 | 24 | 4068 | |
  | s06 | crowd 4: RemoveExtras named iterator | 74766 | 20291 | 20412 | 23 | 4068 | |
  | s07 | crowd 5: CameraFOVLookBetweenPos target first | 74766 | 20292 | 20413 | 21 | | |
  | s08 | crowd 6: CameraFOVLookBetween target first | 74766 | 20293 | 20414 | 19 | | |
  | s09 | crowd 7: .PlayCombatAnim repeat after flags | 74766 | 20294 | 20415 | 17 | | |
  | s10 | crowd 8: .WalkUpToThing moveType = 0 / if | 74778 | 20307 | 20428 | 5 | 4068 | |
  | s11 | G4: CameraLookBetween frame per arm | 74778 | 20417 | 20428 | 5 | | |
  | s12 | G4: CameraFOVLookBetween frame per arm | 74778 | 20417 | 20428 | 5 | | (kept: removes a density tie) |
  | s13 | G4: TintScreenTo default-constructed vectors | 74778 | 20420 | 20428 | 5 | | |
  | s14 | V060/V061 in place, helper removed | 74778 | 20418 | 20428 | 5 | | (+0x00CCED7D operand order) |
  Every step kept: none made the function worse outside its region.

## What is left: 17 instructions, all x87 order (integ/sd.py <tag> lists them)
- V060 .SlideTeleport 0x00CC5962..0x00CC59A2 (11 instructions, 5 hunks): retail keeps the Y product of `step * fraction` in memory
  ([ebp-0x1424]; X and Z on the x87 stack) and adds it as `fld [T] ; fadd [start.Y]`; ours keeps Z in memory as retail's V061 does.
- three commutative `fld A ; fadd B` operand orders (2 instructions each, no length change):
  0x00CCB3C6 CameraFOVLookBetweenPos (retail `fld [ebp+7Ch] ; fadd [ebp-760h]`), 0x00CCE8EA CrowdAnimate (retail `fld [ebp-10h] ;
  fadd [ebp+14h]`), 0x00CCED7D CrowdAnimate (retail `fld [ebp+20h] ; fadd [ebp-10h]`; right before the V060 in-place change).
  These are re-keyed by any text change elsewhere (WF_RULES device class c): settle them LAST, after V060.

## Open devices in the source (hand-back questions)
- G1 dead asserts: `FABLE_RETAIL_CDEFPOINTER_ARROW_ASSERTS` (3 dead arms in operator->; presence EVIDENCED by FableWin /Od size, count
  FITTED) and `FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT` (one dead arm; FITTED carrier, FableWin's debug StartsWith holds no assert;
  new macro FABLE_RETAIL_ASSERT_ARM in fable_assert.h). Allowed class: WF_RULES last-resort (d); the alternative that is certainly
  allowed is `// Flags: /d2inlS8` + the inline_depth regions (tmp/refS8.cpp), which was 3 calls / 6 bytes off.
- row-local `IsEnd()` helper (one use left, 0x00CC61BD GiveHeroExpression); header opt-ins FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE /
  FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE; `float fraction = 0.0f;` (FITTED, V060 / V061); `if (true && ...)` in the `$` test
  (shape EVIDENCED, spelling of the constant operand FITTED); `registered` iterator name (descriptive).

---
# (history) baseline STATE of round 0 follows -- its G1..G4 sections describe the BASELINE source, not the current one
# STATE -- 0x00CBFB7D RunCutsceneMacro (wfC1; written by the BASELINE agent, 2026-11-14)

## Current source and grade
- `differ/00cbfb7d.cpp` (3,174 lines) = hand885 session-9 draft on TODAY's headers + `hdr/` overlay, every device that may not be
  handed back removed. **DIFFER(74782v74778)**: 20480 v 20435 instructions, frame-masked equal 19442, 828 hunks, calls 4036 v 4068
  (41 missing / 9 extra by target), frame 0x18C4 v retail 0x18DC. No `// Flags:` line. Compile ~21 s.
- Analysis-only reference `tmp/refS8.cpp` (= baseline + `/d2inlS8` + the 16 `#pragma inline_depth` lines; NEVER hand back):
  DIFFER(74848v74778), frame-masked equal 19739, 635 hunks, calls 4073 v 4068, frame 0x18D0. It shows the residue that is left once
  the inline decisions are (almost) retail's: use it to tell a local residue from an inline artefact.
- Header overlay: 5 files re-applied by `tools/port_hdr.py` (hdr/README.md, last block). Opt-in set of the source: its `#define` lines.

## What each removed device cost (one grade each, on the ported draft)
| step | bytes | masked-equal | calls (ours v 4068) | frame |
|---|---|---|---|---|
| ported draft, all devices (/d2inlS8, inline_depth x16, twins) | 74846 | 19757 | 4071 (0 missing, 3 extra) | 0x18D0 |
| - /d2inlS8 only | 74887 | 19640 | 4043 (GetLength 23 + vector<CScriptThing>::operator[] 5 missing) | 0x18C4 |
| - inline_depth pragmas only | 74733 | 19574 | 4064 (operator[] 0x0040F110 x7, vector ctors x3, iterator== x1 missing; 7 extra dtor copies) | 0x18D0 |
| - both | 74774 | 19455 | 4036 | 0x18C4 |
| - GetLengthExpanded / StartsWithExpanded forced twins | 74774 | 19455 | same (0 cost: both sites expand naturally without the flag) | |
| - `#pragma auto_inline(off)` around the same-TU helper bodies | 74774 | 19455 | same (0 cost) | |
| - `__declspec(noinline)` on IS_THING_ALIVE; IsEnd FABLE_FORCEINLINE -> inline | 74774 | 19455 | same (0 cost) | |
| - seven unused `bool& xRef = x;` locals (steering) | **74782** | **19442** | same | 0x18C4 |
| NOT applied: CSSlideTeleportStep FABLE_FORCEINLINE -> inline | 74633 | 19358 | 4031 | 0x18AC |
| NOT applied: FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE off | 75326 | 19196 | +34 ctor calls | 0x193C |
| NOT applied: FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE off | 74664 | 19370 | +7 ctor calls | 0x18C8 |
Open devices still in the source (must become natural spellings): FABLE_FORCEINLINE CSSlideTeleportStep (row-local helper),
row-local IsEnd() helper, the two CTOR_FORCEINLINE header opt-ins, CUTSCENE_FRAME() macro (check it is what Lionhead wrote).

## GLOBAL issues (they shift everything; solve before local register work)
**G1 inline budget = ONE fact: retail's caller size is larger than ours, by an amount that GROWS along the function.**
c2 trace of the baseline (`tmp/inl_base.json`, tools/it.py / need.py / dsim.py): every plain-inline site is priced
`cost = sz - savings + pen - 7`, expanded iff cost <= 8, `pen = (sz/2)*(a+g)/9000` (integer; sz > 9), a+g = caller size in tuples:
18348 at the first decision, 26471 at the last. No frequency term matters here. Retail's split gives, at OUR decision index i,
the difference E(i) = retail a+g - ours a+g:
 - E < 931 at i=118 (0x00CC0040 `cue.GetLength()` expanded by retail) and E < 751 at i=172 (0x00CC03EA GetLength inside StartsWith("$"), expanded);
 - E >= 666 at i=193 (0x00CC0510 `.SummonerAttack` params[0].GetLength(): retail CALLS; so do the next 22 sites to 0x00CC1323, ours expands all 23);
 - E < 1815 at i=773; E >= 1058 at i=984 (0x00CC8407 _Rb_tree_base_iterator::operator== 0x0041C820 called, GiveGold);
 - E >= 224 at i=1378..1429 (0x00CCC77C, 0x00CCC7C2, 0x00CCC89F, 0x00CCCA75, 0x00CCCB3B: vector<CScriptThing>::operator[] 0x00CD1FB1 called, CrowdCreateMixed / CrowdCreate);
 - E < 3086 at i=1783; E >= 2404 / 2353 / 2287 at i=1929 / 1941 / 1955 (0x00CD0DA4 vector<CCharString> ctor 0x00414DE0, 0x00CD0E20 vector<CScreenFilterSThingByPass> ctor
   0x00CD2D28, 0x00CD0F3B vector<CScriptThing> ctor 0x008AC5A0: all three flip together at a+g = 28286, TintScreenTo);
 - (E >= 2251..2341 at i=2004..2018 for six const vector<CCharString>::operator[] sites 0x00CD15D5..0x00CD17B0, SetLightScene -- but these and
   0x00CD147E are probably a LOCAL spelling: retail passes `this` from a register / frame slot, i.e. the vectors are reached through a pointer or
   reference local, which lowers the savings and makes the call natural.)
 So: +666..750 tuples by the end of the parser, >= +1058 by verb ~98, >= +2404 by verb 178; and fixing our 28 early flips takes
 another 385 tuples off OUR g. That is ~12-13 tuples per verb on top of ~700 before the first verb: growth that comes from
 EXPANDED inline bodies (helper texts / inline layers / dead IL in accessors), not from the caller's own statements (those are in a
 from the start). `/d2inlS8` and the inline_depth pragmas only imitated this.
**G2 frame 0x18C4 v 0x18DC (24 bytes short).** 3279 instruction pairs differ only in a frame offset. +12 comes back with retail's inline
 decisions (refS8: 0x18D0); the other 12 = one more 12-byte object (vector / CScriptThing-sized temp) that retail does not share.
**G3 callee-saved registers / constants** (hunk kinds: 344 register renames, 184 imm<->reg, 47 const materialisations = 575 of 828 hunks;
 in refS8 369 / 93 / 38 of 635). Three separate facts, first at retail instruction 459:
 - parse (0x00CC023C): retail keeps `dot` in memory and const 0 in edi from the loop head through the parser (`c` of the name loop takes
   over edi); ours gives edi to `dot`, zeroes ebx for `c`, pushes imm 0;
 - V000..V099 (0x00CC04FA..0x00CC8657: two ifs, chain 1, V079, chain 2): retail holds -1 in edi (`or edi,-1` at 0x00CC04F6; the instruction before
   the verb literal push = the `len = -1` of CCharString(const char*, long) is `push edi` at 99 of 100 verbs), ours holds it in esi (`push esi` at 98):
   a pure edi <-> esi swap of two long-lived values, 173 (base) / 242 (refS8) one-instruction rename hunks in chain 1 alone;
 - chain 3 (0x00CC8657..0x00CD17FD): retail pushes imm -1 (80 of 84 verbs; 137 of its 153 `push -1` lie at >= 0x00CC7123) and re-zeroes edi inside the
   verbs (`xor edi,edi ; cmp [eax],edi`, e.g. 0x00CCF555 CrowdKill, 0x00CCE5BD CrowdAnimate). Baseline keeps -1 in edi through chain 3 (`push edi` at all 84 verbs = the 157 C hunks);
   with retail's inline decisions (refS8) the -1 becomes an immediate as in retail (78 of 84), so that part is G1-coupled; what stays is the zero:
   ours compares with imm 0 and keeps something else (often an address, `lea edi,[..]`) where retail holds 0.
 Measurement: KREG_base.txt / KREG_refS8.txt (per 8 regions: push -1 / push 0 / cmp x,0 / edi / esi / ebx uses, retail/ours).
 The priorities are frequency-weighted, and the static frequencies here are extreme (x5 per `for`, kept after the loop: 1012 at the first verb,
 27337 in the epilogue loops; 0.75 per else-if, so verbs past ~V010 weigh nothing): the constants are decided in P1..P3, V000..V010 and the tail.
**G4 cross-jump / destructor-copy placement** (calls extra or missing in place): CUTSCENE_FRAME tail duplicated at 0x00CC44A4 (ours +[vtbl+1Ch] +PlayCutsceneSound);
 CScriptThing dtor copy 0x00CC1B24 (retail) v 0x00CC1C3A (ours), 0x00CC8231 v 0x00CC7838; extra copies at 0x00CCC939 (vector dtor), 0x00CCEC07 / 0x00CCF30C
 (~CScriptGameResourceObjectScriptedThingBase), 0x00CCFA42 (~CScriptThing), 0x00CCFC79 (~CCharString): exit kind / scope of a temp.

## Region map (tools/rm.py; one row per region in REGIONS_base.tsv and REGIONS_refS8.tsv)
189 regions: P0 pre (0x00CBFB7D, line 232), P1 setup (0x00CBFD13, 272), P2 loop head (0x00CC011C, 336), P3 parse (0x00CC0216, 367), V000..V183 one per
`command.StartsWith("<verb>")` test (boundary = the `push <verb literal>` in retail and in ours), T0 tail (0x00CD17FD, loop end + epilogue).
Structure: V000 .SummonerAttack and V001 .DoBossFight are two plain ifs; chain 1 = V002 .WaitTask .. V078 RemoveExtras; V079 DoScriptFrame; chain 2 = V080
ScriptFrame .. V099 GiveGold; chain 3 = V100 WaitBossFight .. V183 SetLightScene.
Columns: nR nO instructions, dB bytes (ours - retail), dm / xo instructions without a frame-masked partner, fr frame-offset-only pairs, hunk kinds
C imm<->reg, N register rename, M const materialisation, S shape, K call set, loc = instructions in S+K hunks, c- / c+ calls missing / extra.
| group | retail | base: dB dm hunks (C N M S K) loc | refS8: dm hunks (C N M S K) loc |
|---|---|---|---|
| P0-P2 pre / setup / loop head | 00CBFB7D-00CC0216 | +4 16 14 (2 11 0 1 0) 3 | 18 13 (2 9 0 1 1) 7 |
| P3 parse | 00CC0216-00CC04FA | -6 40 34 (1 22 0 11 0) 19 | 23 16 (1 5 0 9 1) 19 |
| V000-V001 | 00CC04FA-00CC0783 | +18 22 16 (0 3 1 7 5) 30 | 10 9 (1 0 2 6 0) 8 |
| chain 1 V002-V078 | 00CC0783-00CC7085 | +57 339 297 (20 173 9 72 23) 231 | 305 290 (13 242 4 30 1) 54 |
| V079 | 00CC7085-00CC7123 | 0 7 6 (0 6 0 0 0) 0 | 4 5 (1 0 3 1 0) 1 |
| chain 2 V080-V099 | 00CC7123-00CC8657 | -19 44 39 (4 27 1 4 3) 14 | 42 43 (9 29 1 2 2) 4 |
| chain 3 V100-V183 | 00CC8657-00CD17FD | -49 519 417 (157 98 36 103 23) 277 | 289 255 (66 80 28 80 1) 129 |
| T0 tail | 00CD17FD-00CD1F97 | -1 6 5 (0 4 0 1 0) 1 | 5 4 (0 4 0 0 0) 0 |
| ALL | | +4 993 828 (184 344 47 199 54) 575 | 696 635 (93 369 38 129 6) 222 |
Largest LOCAL residues (loc, refS8 / base): V158 CrowdKill 27/10, V153 CrowdAnimate 25/12, P3 parse 19/19, V060 .SlideTeleport 14/14, V149 CrowdTeleportRipple 10/-,
V025 .WalkUpToThing 9/9, V043 .FollowNavRoute 9/9, P1 setup 7/-; in base only (inline-coupled): V031 .DialogadSpeak 60, V183 SetLightScene 53,
V143 CrowdCreateMixed 43, V178 TintScreenTo 40, V123 UseCamera 25, V144 CrowdCreate 24. 109 regions (base) / 137 (refS8) have no S/K hunk at all.

## Tools (all in tools/; env of BRIEF.md; never print more than ~40 lines)
- `g.py <src> <tag> [--fa] [--flags ..]` one grade line (+ artefacts in tmp/g/<tag>/: m.obj, o.txt, ops.pkl, m.asm with --fa); log GRADES.log.
- `rm.py <tag> [--top N] [--loc] [--all]` region map (needs --fa) -> tmp/regions_<tag>.tsv; `hk.py <tag> <region|all> [--kinds SKCNM] [--n N] [--raw]` hunks of a region
  with retail address, our offset and source line; `kreg.py <tag> [group]` constant / register use per group of regions.
- `cl.py <tag> [--seq N] [--max N]` call-target diff (inline COMDAT symbols bound by alignment vote).
- `inl.sh <src> <tag>` c2 inline trace (~2 min) -> tmp/inl_<tag>.json; `it.py <tag> [callee]` decisions; `need.py <tag> <callee> [--exp]` tuples needed to flip
  each site; `dsim.py <tag> lo hi step` what-if on a constant caller-size offset.
- `rverbs.py` retail string pushes (tmp/R_str.tsv); `port_hdr.py [--force]` rebuilds the 5 overlay headers from today's repo headers.

## COORDINATOR NOTE (2026-11-14, supersedes "never hand back /d2inl* / inline_depth" in your prompt)
The owner's 2026-11-08 decision (EXTRA_RULES.md, "inline-budget residues may use an inline control") ALLOWS `// Flags: /d2inlS<N>`
/ `/d2inlT<N>` and scoped `#pragma inline_depth(N)` ... `#pragma inline_depth()` when the ONLY thing they fix is inline decisions
(expanded v called copies), with a `// Inline-budget:` marker line, the smallest N that works, and plain C++ preferred where it
exists. So `/d2inlS8` and the 16 inline_depth regions of the earlier draft MAY be handed back for this row, provided each is
still needed after the statement-level work and is documented. Prefer the reference build WITH those devices as the working
base if it is the one closer to retail; register / frame residues must still be closed with real source facts. See the
CORRECTION paragraph in wfcommon/WF_RULES.md.
