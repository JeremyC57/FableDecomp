# NOTES -- chunk 835 (expert retry of ten parks; Linux Wine lane, VC7.1 RTM 13.10.3077, BASE_FLAGS /O2 /Oy /W3)

Result: 1 matched (0x00EA5230, RELOCATION_MATCH), 9 parked. The CIsThingAlive/CIsThingConscious nested-copy blocker (five rows) is
now characterised exactly (below); no plain-C++ spelling reproduces it, and the one construct that does is forbidden -- owner question.

## The nested-copy blocker (0x00DE2610, 0x00E27E90, 0x00D82640, 0x00DC5F80, 0x00DAF080)
Retail shape at every `SetPredicate(CIsThingAlive(x))` / `CIsThingConscious(x)` site: the predicate ctor is expanded (vtable store
0x012C32F0 / 0x012C2FE8, CPredicateBase's own vtable store dead), but the CScriptThing copy inside it is a CALL to 0x004ABE90, and the
predicate dtor's nested ~CScriptThing is a CALL to 0x004AA840, while by-value CScriptThing copies in the same function expand inline
(double vtable store 0x01231710 then 0x01238C8C).
- Retail evidence (bin/vtscan.py over every absolute reference to the two vtables in Fable.exe, tmp outputs now deleted; counts in
  LESSONS.md): 413 CIsThingAlive + ~90 CIsThingConscious construction sites, nested copy CALLED at 100% (373 + 78 expanded ctors), nested
  dtor CALLED where expanded (32 + 7); including the 20-byte CPredicate_And ctor 0x00E24760, Clone 0x00CDF130 and the scalar deleting
  dtor 0x00CDF160. Only the standalone ~CIsThingAlive 0x00E241B0 / ~CIsThingConscious 0x00EF7AB0 expand ~CScriptThing (depth 1).
  The retail 0x004ABE90 has one vtable store (out-of-line COMDAT); the CCountedPointer<CScriptThing> copy COMDAT 0x00704430 has one
  caller (0x00CF699C). Ego_r.pdb: one copy ctor only (??0CScriptThing@@QAE@ABV0@@Z, out of line at 0x00429C30).
- Lane behaviour (harness probe/x_*.cpp on a 4-statement Main, ~45 builds, bin/cc.py + bin/rep.sh + bin/meter.py): the inliner consumes a
  per-caller budget in source order, depth first; an outer inline tree is all-or-nothing (when outer + nested do not fit, the outer is
  called whole). /d2inlT acts budget-like (T30 nothing, T40 nested + 1 by-value, T50 + 2, T60 + 3, T100 + 4; default in the T60-T80
  band); the copy ctor costs ~29 units at either depth; no depth-dependent threshold. __forceinline on the predicate ctor, definition
  position, inline helpers (depth 3/4), inline_recursion, /Ob1 /Os /Ot /Oa /Gy /GX /EHsc /Zi, pragma regions auto_inline(off),
  optimize("",off), optimize("g",off) (call the outer whole), optimize s/t/y/a/p/w, check_stack, runtime_checks (inert): none split the
  tree. A global inline_depth(1) calls the CPredicateBase / CBaseClass / CCountedPointer copies too (not retail).
- Not LTCG: bin/ltcg.py builds two TUs (/GL) and links /LTCG (tmp/ltcg, deleted): cross-module inlining expands the nested copy as well,
  gives the copy ctor a custom convention (this in eax, other in ecx) and Main `this` in esi. Retail's NScript Mains are plain thiscall.
- THE MODEL (diagnostic only, probes <addr>_r0.cpp): a region `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the
  row-local `inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing&)` / `~CIsThingAlive()` definitions attaches to the inlinee
  and stops expansion of the calls inside it when it is expanded into Main: ctor expanded, nested copy called, nested dtor called, by-value
  copies expanded, retail's exact order. Residues: 0x00DE2610 35 -> 9, 0x00D82640 38 -> 14, 0x00DC5F80 36 -> 19, 0x00DAF080 30 -> 20,
  0x00E27E90 31 -> 22. What is left is one thing: the nested `CPredicateBase()` ctor becomes a CALL under the region (retail has no such
  call -- its vtable store is dead-stored; 0x00DE2610's whole residue is that call + two alignment nops). An implicit CPredicateBase
  ctor (diagnostic overlay, r2/r3) is still emitted and called. The nested ~CPredicateBase does NOT become a call. GOTCHAS already records
  the pragma at 0x00D08B40 (chunk 799) and at the 13 rows of chunk 282 (landed in October); the brief forbids it in new rows.
- Owner question: retail behaves as if the predicate special members were compiled under inline_depth(0)/(1) at 100% of ~500 sites
  (a header-level property of CIsThingAlive.h / CIsThingConscious.h in Lionhead's tree, e.g. a pragma round those classes, or a
  compiler setting the lane does not have). Options: (a) allow a SHARED-HEADER `#pragma inline_depth(0)` region round the predicate
  ctor/dtor definitions in engine/CIsThingAlive.h / CIsThingConscious.h (one place, documented, not row-local) and find the spelling that
  also keeps CPredicateBase() expanded (e.g. no user-declared CPredicateBase ctor plus a __forceinline on it -- untested); or (b) keep the
  five rows parked as a lane gap. The parked sources are the best plain-C++ texts (hand799/798/789/790 unchanged; 0x00DAF080 with the
  row-local __forceinline ~CIsThingConscious dropped to plain inline as instructed: 5571/30 -> 5520/147, see STATUS.tsv).

## 0x00EA5230 NScript::CV_LostTraderScript::CLostTraderBanditSpawn::Main -- RELOCATION_MATCH (src/00ea5230.cpp)
- Rewritten from the hand648 local model (VPAD vtables) onto the shared headers: engine/CV_LostTraderScript.h (+ opt-in
  FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BANDITNAME, hdr/), CEntityScriptBase.h, CGameScriptInterfaceBase.h, CScriptThing.h (NSCRIPT),
  fable_script_distance.h. Identity: slot 1 of the CLostTraderBanditSpawn entity vtable; Ego_r.pdb ?Main@CLostTraderBanditSpawn@...
- Shape: first bandit wait `while (bandit.IsAlive())` devirtualises the row-local `inline bool CScriptThing::IsAlive() const { return
  PImp.Data && PImp.Data->IsAlive(); }` (retail tests the impl: `mov ecx,[esp+24h]; test; je; call [edx+12Ch]`); the two attacker waits
  use `reinterpret_cast<const CScriptThing&>(attacker).IsAlive()` on the VALUE local (same-type cast, owner ruling 2026-11-07, flagged
  on line 2) so VC7.1 keeps the virtual call and hoists `lea ecx,[esp+14h]` above the terminate jne; a row-local
  `inline CScriptThing::~CScriptThing() { }` gives the inline-once dtor at the first terminate exit (every later exit calls 0x004AA840).
- Names: ParentClass (reference, +0x14), ReachedTrader (+0x4c, Ego_r.pdb member; the park's descriptive `SpawnNextBandit` is gone),
  GetRandomBanditName 0x00EA5690 (PDB spelling, unbound elsewhere in reloc_bindings.tsv), IsDistanceBetweenThingsUnder 0x00CBE2FF
  (fable_script_distance.h). Slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature, 108 RemoveThing, 421 IsCameraPosOnScreen,
  451 GiveThingBestEnemyTarget; CScriptThing slots 6 GetPos, 75 IsAlive; operator= 0x008AB980; "" 0x0122D70E.
- Test: test/00ea5230.cpp (state model: four spawns in order, off-camera / distance waits, ReachedTrader, early termination).

## 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput -- parked DIFFER(2369v2339), 27 (unchanged best)
- Residue: (a) five InitialiseFromAngles expansions: ours copies the CSE temp cosYZ*length into the Y multiply then pops it
  (`fld st(1); fmulp st(1); fxch st(1); fstp st(0)`), retail consumes it (`fmulp st(1)`), 15 lines; (b) the cross product
  `right = forward ^ up`: the copy of temp.X into right.X is scheduled before `fld` (ours) vs between `fld` and `fmul` (retail), 8 lines;
  (c) the 0x12 arm's y lives at [esp+78h] (ours) vs [esp+7Ch] (retail), 4 lines.
- 13 grades this chunk (STATUS.tsv): x/y declaration order, named C2DVector, copy-init of right, named `horizontal` (changes the body:
  2286 bytes), Y operand order, chained temporaries, /G5 /G6 /G7 -- none moved a line. The named-cosYZ body is retail's (its spill to
  [esp+7Ch] and the `fld st(1); fmulp` X multiply match); the Y consume is an x87 kill-placement difference.
- Header debt (for the shared-header rewrite the row still needs before landing): engine/CCamera.h lacks PeekUpDirection, Move,
  MoveTo, PointAt 0x00A0C2B0, MoveForward 0x00A0C250, MoveSideways 0x00A0C1B0 (opt-in FABLE_RETAIL_CCAMERA_ANGLES has GetAngleXY/YZ);
  engine/CInputEvent.h lacks GetMouseMovement 0x00A043D0 (C3DVector by hidden pointer); engine/CProcessedInput.h lacks
  SetAsPerformedEvent 0x00A0D390; engine/CInputProcessControlFreeCamera.h is a generated flat layout without the virtual ProcessInput.
  The parked text keeps hand829's local models for these four classes.
- Inherited row-local `__forceinline float __fastcall GET_ROUNDED_ANGLE` is the landed 0x00A14260 WrapAngle model (an inline helper
  with retail evidence, not a residue helper).

## 0x00D3D150 NScript::CQ_GuildTrainingScript::CAppleGirl::Main -- parked DIFFER(4252v4252), 28 (unchanged best)
- Residue: register colouring in the MANY_MORE_APPLES / NO_MORE_APPLES arms only (vtable temp eax vs edx, string address ecx vs eax).
  Retail's eight AddNewConversation sites: edx at 0x00D3D3B8, 0x00D3DA9B (the ANY_APPLES else arm), 0x00D3DC39 (ONE arm); eax at the five
  others. Ours differs at 0x00D3DCA9 / 0x00D3DD12 only.
- c12 switch (ONE / default MANY / case 0 NO): 4256 bytes, 38. c13 (reference alias) and c14 (one-member struct) are byte-identical to the
  plain local: the colouring is not about ApplesGiven being a memory variable (it is one: [esp+34h] is shared with an earlier
  address-taken local). Lane gap: 26 spellings over three chunks.

## 0x00E50910 NScript::CV_BeardyBaldyScript::WatchForQuestCardConditions -- parked DIFFER(1392v1328), 57 (unchanged best)
- Residue: the ebx zero register is kept live into the MsgIsTalkedToBy wait (`cmp ecx,ebx` at the loop head, `xor ebx,ebx; jmp` back-edge)
  where retail uses `test ecx,ecx` and a `je` back-edge falling into the shared epilogue placed right after that loop (0x00E50CF5);
  ours puts the epilogue and both inline-dtor tails at the end. e1 (`&&` body) 1395/89, e2 (talked branch inside a for(;;)) 57, e3
  (do-while + post-loop check) 1375/71. Control flow per the FSE Lua port (refs/script_recovery/.../BeardyBaldy.lua).
- The parked text's `// Flags: none` line was dropped (gall.sh would pass `none` as a flag); one `// Leaf:` line.

## 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main -- parked DIFFER(7692v7655), 36 (unchanged best)
- Residue: teleport-loop register pair (i = ebx / begin = ebp in retail, swapped in ours, plus the eax/edx pair in the two size()
  computations before it), the natural-exit ~seh_me store order (ours vptr, Data, Info; retail Data, Info, vptr -- a scheduler choice:
  the TOL twin 0x00EDD170 keeps program order with the same headers), and the pad-A thread-check exit copy. L1/L1b/L1c (where the loop
  index is declared) unchanged. Uses hand824's CV_ChapelOfEvilScript.h opt-in (hdr/).

## Names / labels / header debt
- No wrong landed spelling found (NAME_FIXES.tsv has the header only). The park's `SpawnNextBandit` descriptive name is replaced by the
  PDB member ReachedTrader in src/00ea5230.cpp. LABELS.tsv lists the matched row's (already correct) label.
- Header debt: see 0x0068CB90 above; engine/CV_LostTraderScript.h could carry GetRandomBanditName on the default path (PDB member of the
  class) -- kept opt-in here per the brief.
- Attempt counts: the five blocker rows share the ~45-build harness (listed under 0x00D82640 in STATUS.tsv) plus the r0/r1/r2/r3
  diagnostics; their own plain-C++ texts were not re-spelled because the residue is the structural blocker above.
