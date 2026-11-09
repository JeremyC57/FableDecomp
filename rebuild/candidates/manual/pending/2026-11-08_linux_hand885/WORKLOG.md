# WORKLOG 0x00CBFB7D (single-row worker)

Row: 0x00CBFB7D, 74,778 bytes, catalog `_global::RunCutsceneMacro`. PDB name `RunCutsceneMacro` (Leaf line in src;
two sources: reloc_bindings `?RunCutsceneMacro@@YIX...` stand-in with CCutsceneCastMap + catalog name; the cutscene
helpers CSFindThing 0xCBF9DE / CSExtractParameterList 0xCBFACA landed alongside with the same PDB naming family).

## Session 6 (2026-11-09, all-means push) -- READ THIS FIRST (src/ = a3; differ/ = src + 2 grade lines)
Leaderboard (strict = gr.sh difflib retail ins matched /20435; m3 = tools/pk/m3x.py exact / regs-masked / regs+frame-masked;
first-reg = tools/pk/first.py --noframe):
| ver | bytes | strict | m3 exact/regs/r+f | first-reg | note |
|---|---|---|---|---|---|
| s5 src | 74882 | 19718 | 18344/18794/20134 | 8 | Session 5 end |
| a1 | 74877 | 19722 | 18348/18796/20136 | 8 | TintScreenOut hoist (0x00CD11E0) |
| a2 | 74877 | 19722 | = a1 | 8 | FABLE_FORCEINLINE respell (rule) |
| **a3** | **74865** | **19733** | 18106/18540/20130 | **459** | 7 flags bound to references: K 19->12, name/-1 = retail |
Measured:
- First wrong decision (instruction 8): retail name ebx / -1 esi. What-if: prio(name) <= ~25-29 reproduces retail (20, 25 do;
  30 does not). Ours 38 = 2*K(entry block), K = 19. The -1 prologue piece (prio 44) is born only after 15 splits of the const -1
  web, after name is coloured (tools/pk/chain.py). Lever: remove candidates from the entry block. The 7 flags never enregistered
  anywhere (restoreFollowers crowdLooking keepActors extrasRemoved relightHandLamp fadeInAtEnd flagWasSet) bound to local
  references -> K 12, prio 24 -> ebx: first register difference 8 -> 459, strict +11, bytes 74877 -> 74865.
  Devices that do NOT remove a candidate: (void)&x, by-ref param of an inlined call, x[1], inline setter (TRACE nc1..nc5).
  The reference block is plain C++ with an evidence comment, but odd: a natural spelling with the same IL (the original probably
  held these flags through references/pointers somewhere) is still wanted.
- Next decision (instruction 459, 0x00CC023C main-loop parse): retail keeps `dot` and `space` in memory and holds the const-0
  piece in edi (`cmp [ebp+0x7c], edi` 0x00CC028D); ours dot (prio 25) -> edi, c -> ebx. What-if dot <= 20 -> space (16) takes edi
  next. Requires: the const-0 region piece coloured before dot and space (split timing again, as for -1). dot/space references
  (nc8) are much worse: they must stay candidates. References for the if-body memory-only locals (skipping, resetThemeAtEnd,
  answered, answer, screenFilter: nc9 first-reg 79; bools only: nc10 252) are worse too: only the 7 top-level flags go.
  c is coalesced with the const-0 piece in retail (edi: `inc edi`, `push edi`); s6/prio_a3.trace is a3's full c2prio trace.
- Frame: packer fully modelled (LESSONS Session 6); a3 frame 0x18D0 v retail 0x18DC follows the spill set -- fix registers first.
- Local residues unchanged: StartsWith("$") 0x00CC0418, SlideTeleport x87 0x00CC5954.., CrowdKill loop 0x00CCF7BE (4 families
  dead), WalkUpToThing moveType 0x00CC2443 (allocation: needs a register holding 0).
Next lever: chain.py on a3's trace for the const-0 piece covering 0x00CC023C (when is it born, what exhausts its parent) and look
for entry/loop-block candidates that never enregister in retail (same K device), then frame weights (slot 533 $T25247 w2).
Resume: `bash gr.sh $PWD/src/00cbfb7d.cpp` (absolute path!) grades; `tools/pk/fit.sh <src> <tag>` and `tools/pk/wi.py` are the
cheap fitnesses (tools/README.md).

## Session 5 (2026-11-08) (resumed from Codex's checkpoint; src/ = differ/ minus 2 grade lines)
State: **DIFFER(74882v74778), frame 0x18DC = retail**, /d2inlS8. src/ = Codex checkpoint (t5/codex_ckpt.cpp, DIFFER(74906v74778))
+ Codex's SlideTeleport alternate (codex/alt_slide_factor_compound, graded here and kept). Metrics (t5/m3.py, code to `ret 10h`):
exact 89.8% / regs-masked 92.0% / regs+frame-masked 98.5% (checkpoint 89.0/91.2/98.5; our v41 49.5/52.4/98.5); gv.sh strict 96.5%,
loose 98.7%, calls 99.2% (4034/4068). t5/fmap.py: 651 of 1051 frame slots at retail's offset (checkpoint 591).
Line 1 restored to the signature comment (Codex's `// 00cbfb7d RunCutsceneMacro` broke finish.py's name parse); the line-3
Inline-budget note now also names Codex's IS_THING_ALIVE device (0x0040DCE0, visible read-only body, noinline under _MSC_VER).
Every step with tool + evidence: TRACE.md. Grades: ATTEMPTS.log `s5` lines.
What was measured and what it pointed at:
- calls.py --counts: every inline site agrees with retail; the call-diff hunks are cross-jump tails (FRAME / ~CScriptThing groups)
  plus 0xCCDAE3 (CrowdTeleportRipple: ours duplicates the `marker.IsAlive()` loop test at the loop entry because the
  count piece #2201 sits in edi and needs a back-edge reload; retail keeps count in memory [ebp+8]) -- register-downstream.
- First difference ins 8: prologue roles (retail name ebx, const -1 esi; ours esi / ebx). c2prio (t5/prio_base.trace; pass
  `--func RunCutsceneMacro`, the default Leaf match finds nothing): `name` #39 prio 38 is chosen with esi = ebx = 0 cost ->
  esi by table order; const -1 is ONE split candidate (prio -2397) whose prologue piece is coloured in the split pass, i.e.
  after name, so nothing can make esi expensive for name in our candidate set. Retail must differ in name's candidate (not in
  the main pass) or in the const -1 piece -- unresolved.
- WalkUpToThing moveType (#888, prio 2, 3 refs) -> split, memory; at FollowNavRoute the same `moveType = 1` gives `inc esi`
  when it has a register, so retail's `xor edi,edi ... inc edi` (0x00CC2443) needs only the register, not a `++` spelling.
- Frame: a contiguous run -0xC18..-0x8A8 is 4 bytes high v retail; at its top boundary 2-ref 4-byte slots (equal density)
  are permuted -- regime-1 density sort with ties scrambled: needs the exact symbol set.
- Dead ends this session (do not repeat): CrowdKill wait loop as `for (;;) { if (!(timer < delay) || IsCutsceneSkipped())
  break; ...}` -- code identical to the `while` (the full function rotates it too); with session-3 minis and Codex crowd_* that
  is three families. Prefix 0x00CC0418: not retried (Codex: 10 spellings).
Left, in order: (1) register roles -- the prologue (name / -1 / def) and the split constant pieces (0, -1, 1, "HERO") drive most
of the 2.2% register hunks and the cross-jump tail choices; only a candidate-set change moves them (c2prio --trace, 5.5 min);
(2) frame-slot tie order (7% of exact share; follows the exact symbol set); (3) SlideTeleport: retail spills the Y product and
multiplies `fld step.c; fmul st(k)` without duplicating fi (alternate still dups fi for Y/Z); (4) prefix 0x00CC0418;
(5) CrowdKill top-tested loop; (6) C++03 behaviour test + finish.py once it matches.

## Session 1 (2026-11-08)
- Decoded the prologue, locals, setup, follower handling, sound parse, main parse loop, cleanup (see src comments).
- src/00cbfb7d.cpp v1 compiles: DIFFER(4374v74778), first diff ins 1 (frame size: ours small `sub esp`, retail
  `mov eax,0x18dc; call __chkstk` biased frame). Needs the full body before frame/inline decisions converge.
- Verbs in src: SummonerAttack, DoBossFight, WaitTask, WalkTo (WalkTo tail bug: must be
  `if (IsTrue(P2) || wait) {wait loop} else {FRAME}`).
- Verbs decoded (not yet in src): RunTo, SneakTo, SetFree, SetAttackable, SetDamageable, SetPushable, SetBound,
  SetScared, SetDrunk, PreloadAnim, PlayAnimation, PlayCombatAnim, PlayLoopingAnim, WaitPlayAnimation, FollowThing,
  StopFollowingThing, Decapitate, Killable, FightWith, FightStop, HoldInHand, ModifyHealth.
- Tools: tools/annot.py -> probe/ann.txt; tools/condense.py <l0> <l1> (ann line range) terse listing; tools/cmp.py;
  gr.sh (grade src, writes probe/g.txt); c.sh (re-cmp). probe/strings.txt = verb order with ann line numbers.
- Overlay hdr/: fable_string.h (+FABLE_RETAIL_CCHARSTRING_APPEND_CHAR operator+=(char) 0x0099F180),
  engine/CSubtitleRenderer.h (+FABLE_RETAIL_CSUBTITLERENDERER_CUTSCENE SetAsInCutscene 0x0049319C,
  SetCutsceneAsSkippable inline).

- Later in session 1: all actor verbs + chain 1 globals (to RemoveExtras), DoScriptFrame, chain 2 (ScriptFrame..GiveGold),
  chain 3 from WaitBossFight to PutUpYourSwords written. Grade DIFFER(~ours 11.5k ins v 20.4k), difflib 36.6%.
  Structure: chain1 = actor verbs + first globals (ends 0xcc7081 `or edi,-1`); `if DoScriptFrame`; chain2 ScriptFrame..GiveGold
  (ends 0xcc8464 `mov esi,'HERO'`); chain3 WaitBossFight.. (ends 0xcd17fd). Next undecoded: CameraRig (ann line 10717).
  tools/cd.sh <l0> <l1> = condensed listing minus noise; start ranges one line before the string line in probe/strings.txt.
  tools/addverbs.py <file> inserts verb code before the anchor (@HERO@ / @FRAME@ macros).

## Session 2 (2026-11-08)
- All verbs written (v15). Then: dropped `optimize("y", off)` (biased frame like retail), +FABLE_RETAIL_STLP_MAP,
  +STLP_VECTOR_ASSIGN, +CCHARSTRING_FORCEINLINE_EQUALSCI, overlay forceinline C3DVector ctors and CRGBColour(r,g,b,a)
  (no out-of-line copy anywhere in retail), `// Flags: /d2inlS8` (S7/S8 reproduce retail's 524 GetLength + 41
  vector<CScriptThing>::operator[] out-of-line calls; S9 default inlines 25/11 extra; /d2inlT5,T40 no effect).
- v20: DIFFER(74553v74778), call sequence 97.5% (tools/calls.py --counts).
- Overlay additions: engine/CGameScriptInterfaceBase.h (GSIBASE_ENDLETTERBOX_PUBLIC: slot 378 called through base ptr at
  0x00CD06CD), engine/CScreenFilterSThingByPass.h (new, Ego_r ctor + layout), fable_string.h APPEND_COUNT
  (?Append@CCharString@@QAEXDK@Z 0x0099F3B0), engine/C3DVector.h, engine/CRGBColour.h.
- Tools: tools/calls.py (call-target sequence diff via reloc_bindings + learned 1:1), cmp.py --big N.
- Known structural residues: FRAME tail-merge groups (retail: WalkTo/RunTo/SneakTo/0xcc1b29 keep SneakTo copy at
  0xcc0e96; AILevel-prev 0xcc44a4 + Decapitate 0xcc5691 form a 2nd group with `cmp [g],edi` (edi=-1)); ours merges all 6
  into the Decapitate copy because edi is not -1 there (register-role swap: retail ebx = GSI ptr, ours edi).

## Session 3 (2026-11-08) -- v21..v34, parked
- Best: v34 = differ/00cbfb7d.cpp, DIFFER(74879v74778), first diff ins 1 (frame `mov eax,0x18E4` v 0x18DC); strict difflib
  95.3%, register/frame-masked ("loose") 97.8%, call-target sequence 99.0% (4026/4068). Grades: ATTEMPTS.log.
- Source shapes that each removed a hunk (retail evidence in ATTEMPTS.log):
  * an argument that contains a call is hoisted into a named local before the call (retail evaluates GFCharStringTo*/
    GetScriptThing before the pDSTGame vtable load; VC preloads the vtable into a callee-saved reg when an argument calls);
  * `x ? a : b` bool/float arguments are written as `if (x) call(.., a) else call(.., b)` (retail hoists the common head,
    branches on `push 1`/`push 0` or two `fld`s, no CSE of -1.0f);
  * block-local iterators declared, then assigned from find, then compared (`it = m.find(k); if (it != m.end())`): retail copies
    the find temp to the local and evaluates find before end(); `!=`/`==` stay out-of-line calls there;
  * `(existing = actors.find(k)) != actors.end()` inside the Create verb's condition;
  * the body after GetDef wrapped in `if (GetDef(name, &def)) {...}` (one shared cleanup, no early return);
  * SetTime: `(duration = GFCharStringToFloat(p2)) > 0.0f` inside the `&&` so a single SetTimeOfDay block remains.
- Inline budget (the blocking residue): VC7.1 caps inlining by the caller's WHOLE IL size, greedy in source order
  (mini t22: S7 26 / S8 146 / S10 386 GetLength sites inlined, +120 per step; mini t25: 600 extra plain calls placed at
  the start OR at the end cut 146 -> 66, so code after a site still counts). Inlines that shrink the IL give headroom
  back, so decisions are not monotone (retail: `find()==end()` 0x00CC61B8/0x00CC6DA9 expanded, 0x00CC6962/0x00CC8407 called).
  /d2inlS8: ours calls GetLength at 0x00CC0040 and 0x00CC03EA where retail expands it (all other 524 sites agree), calls
  `==` at 0x00CC61B8/0x00CC6DA9, expands `==` at 0x00CC8407, the 7 vector<CCharString>::operator[] 0x0040F110 (SetLightScene)
  and 3 vector ctors (0x00414DE0/0x00CD2D28/0x008AC5A0) that retail calls. /d2inlS9 (= default) moves our GetLength cut-off
  from before 0x00CC0040 to past 0x00CC127A (22 more expansions) and leaves the == / operator[] sites unchanged. Retail's IL
  weight sits between S8 and S9 for our headers: needs header IL parity (original STLport 4.6 / CCharString layering), not a
  row source change. /d2inlT5, T40: no effect.
- Downstream of that: register roles (chains 1-3 retail -1 in edi / HERO in esi, ours swapped; chain 4 retail edi=0 with
  `push -1` immediates, ours edi=-1: ~150 one-instruction hunks) and frame-slot order (slots look use-weight sorted near
  the biased ebp; tools/frame.py shows mixed deltas), plus cross-jump survivors (retail keeps the LAST of six
  `call ~CScriptThing ; jmp` tails at 0x00CC8231, ours the first; 0x00CC44A4 FRAME group).
- Local residues not solved: CrowdKill's final wait loop is top-tested in retail (0x00CCF7BE; every while/for(;;)/break
  spelling rotates, `bool more` keeps a flag: mini t24/t26/t27); SlideTeleport loop copies the scale with `fld st(0)`
  (ours) v reload (retail) 0x00CC5947/0x00CC5C02; WalkUpToThing moveType in edi in retail (`xor edi,edi`/`inc edi`) v a
  frame slot in ours (0x00CC2443).
- Tools added: tools/frame.py (ours->retail ebp slot pairing -> probe/frame.txt), tools/mini.py prints relocation names,
  tools/calls.py --counts, cmp.py --loose --bigseq N --from IDX, tools/nc.sh (call diff summary).

## Resume
1. `cp differ/00cbfb7d.cpp src/` (gr.sh / c.sh / calls.py / ours.py read src/00cbfb7d.cpp); drop the differ-only
   `// Grade:` / `// Next:` lines 2-3 or keep them (comments).
2. `bash gr.sh` (46 s), `bash c.sh --loose --hunks N`, `python3 tools/calls.py --counts` (needs `source $S/wine/env.sh`).
3. Retail listing with labels: probe/ann.txt (ann line numbers used everywhere above); ours: probe/ours.txt.

## Session 4 (2026-11-08) -- per-site inline devices, then local fixes (best: v41 = src = differ, DIFFER(74965v74778))
- Base v34 regraded DIFFER(74879v74778). Grades: ATTEMPTS.log (`s4 <label>` lines); variants in probe/s4/ (gen4.py devices on
  probe/s4/base.cpp = v34; t4, v36..v41 full copies).
- Tools: tools/gv.sh <file> <label> (grade + strict/loose/calls + tools/sites.py: one line per non-register call hunk; logs
  ATTEMPTS.log); tools/mcv.sh <file> <label> (side compile into probe/mc/<label> + call-site summary, 20 s, runs beside a grade;
  HV=<dir> stacks an extra header overlay first); tools/mc.py; tools/h1.py <bigseq> (one line per masked hunk); tools/rs.py <va>
  (string at a VA). cmp.py now masks >=0x400000 immediates on both sides (was retail only: 0x3E99999A / 0xFFFFFFFE false hunks).
- Step 1 (per-site devices) DONE -- every inline site agrees with retail (calls.py: only cross-jump tails left):
  * `#pragma inline_depth(0)` ... `()` regions outline what retail calls: SetLightScene `CCharString lightLine(lights[i] /
    lights[index])`, the scene loop (for header + `== ','` condition; the `else if` arm) -> 7x operator[] 0x0040F110;
    GiveGold `if (it == last)` with `last = givenObjects.end()` hoisted (depth 0 would outline end() too) -> == 0x00CC8407;
    the 3 TintScreenTo vectors -> ctors 0x00414DE0 / 0x00CD2D28 / 0x008AC5A0, with a NAMED allocator declared before the region
    (depth 0 also outlines the default argument's `allocator()` ctor: an extra call).
  * forced expansions: a row-local __forceinline wrapper that CALLS the plain inline (`text.GetLength()`, `a == b`) stays capped
    (v35b, no change). The twin must carry the body: hdr opt-in FABLE_RETAIL_CCHARSTRING_GETLENGTH_EXPANDED (GetLengthExpanded /
    StartsWithExpanded) -> 0x00CC0040 / 0x00CC03EA; row-local IsEnd(it, c) = `it._M_node == c.end()._M_node` -> 0x00CC61B8 /
    0x00CC6DA9 (IsEnd(find, c), not SameNode(find, end): the free-function twin evaluates args right to left, so end() was loaded
    before the find call into edi).
  * Decisions are source-order greedy: outlining the late sites does not move earlier ones; forcing the 4 early sites does not
    flip the late ones. Header-weight probes (GetLength as if/return, without the (long) cast, IsEmpty with its own body) changed
    no decision under S8 -> step 2 (header weight) not needed for the inline split.
- Local fixes (plain C++, retail evidence): TeleportFollowers `followers.size() > 0` (jbe 0x00CC6A3C); InteractiveSpeakGroup
  `for (long i = 0; i < lines; ++i)` + `(i + 1) * 10` (test/jle + strength-reduced ebx += 10, 0x00CC2DDC); Sheathe last arm
  `else if (!mode.EqualsCaseInsensitive("none"))` (setne 0x00CC3A34: retail SHEATHES unless the mode is "none" -- a behaviour
  fix); TintScreenTo `unsigned long count` / index (je + pointer/down-counter loop 0x00CD0E4C..0x00CD1119).
- Same-TU callee bodies (retail compiles IsTrue/IsFalse/IsNull 0x00CBEDBA.., IsCutsceneSkipped 0x00CBEB7E, CSFindThing 0x00CBF9DE,
  ReadLightScene 0x00CBF050, CSExtractParameterList 0x00CBFACA in the same file BEFORE RunCutsceneMacro), under
  `#pragma auto_inline(off)`: CSExtract's body made `values` non-escaped -> values._M_start held in esi across the
  GFCharStringToFloat calls (0x00CD0DBC..) and the TintScreenTo loop strength-reduced, as retail (mini probe mini/s4_t1 v s4_t2).
  A PlayCutsceneSound body (v39, decompiled from 0x00CBF7FE: CStringParser over the cue, Play2DSound / PlayMusic / StopMusic)
  and compiling the helpers with optimize("y", off) (v40) changed nothing: dropped.
- v41 (= src = differ): DIFFER(74965v74778) strict 96.4% loose 98.7% calls 99.2%; masked hunks 273 (probe/v38_h1.txt).
- What is left is register allocation, not inlining: the constants 0 / -1 / 1 / "HERO" are split into region pieces (c2prio
  probe/prio_t4.txt: const -1 6 register pieces + 48 immediate, const 0 16 + 104) and retail gives different regions different
  registers (e.g. the main-loop parse: retail `dot` in memory and c = edi from the const-0 copy, ours dot edi / c ebx; chain 2
  `push 1` immediates where ours holds 1 in edi; prologue name/-1/def roles). Frame 0x18E4 v 0x18DC and the cross-jump
  survivors (FRAME tails 0x00CC44A4 / 0x00CC907D / 0x00CCA738, ~CScriptThing tails 0x00CC8231 / 0x00CC864B) follow from it.
- Local residues still open: StartsWith("$") bool 0x00CC0418 (retail threads the GetLength()==0 exit past the temp's flag
  check, so bl comes from neg/sbb/inc; ours keeps a phi: test/jne/mov bl,1/xor bl,bl); SlideTeleport x87 0x00CC5947 / 0x00CC5C02
  (retail keeps (float)i at the bottom and spills step*fi .Y; ours dups fi with fld st(0) and spills .X/.Z; a mini TU does not
  reproduce the big function's choice); CrowdKill loop 0x00CCF7BE; WalkUpToThing moveType 0x00CC2443 (allocation).
- GOTCHA: an overlay header with `#error` makes cl.exe spin under Wine until the timeout.

## Resume (session 5)
1. src/00cbfb7d.cpp = differ/00cbfb7d.cpp = v41. `bash tools/gv.sh src/00cbfb7d.cpp <label>` (45 s) grades and summarises;
   `bash c.sh --bigseq 400 > probe/x.txt; python3 tools/h1.py probe/x.txt` lists the masked hunks.
2. Register roles: c2prio.py --trace takes ~5 min here (see the command in this log's session-4 notes: FABLE_HDR_OVERLAY=hdr,
   --func RunCutsceneMacro); compare one region at a time (prologue first: name/-1/def).
