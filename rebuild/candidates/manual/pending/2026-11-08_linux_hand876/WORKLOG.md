# WORKLOG 0x00CBFB7D (single-row worker)

Row: 0x00CBFB7D, 74,778 bytes, catalog `_global::RunCutsceneMacro`. PDB name `RunCutsceneMacro` (Leaf line in src;
two sources: reloc_bindings `?RunCutsceneMacro@@YIX...` stand-in with CCutsceneCastMap + catalog name; the cutscene
helpers CSFindThing 0xCBF9DE / CSExtractParameterList 0xCBFACA landed alongside with the same PDB naming family).

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
