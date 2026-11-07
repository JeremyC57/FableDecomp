# chunk 824 (retry of six parked rows) -- notes

All six names were already proven (Ego_r.pdb / FableWin.pdb, vtable slots) by their first chunks; none re-derived, none descriptive.
Every attempt (tag, grade, masked diff count, what VC7.1 emitted) is in ATTEMPTS.log and in the STATUS.tsv notes column.
md = relocation-masked diff lines (bin/md.py), md3 = the same with [esp+N] offsets masked (frame-shifted rows).

## 0x00EDD170 NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main -- RELOCATION_MATCH (src/00edd170.cpp)
- Start: hand811 parked DIFFER(6597v6594), 50 masked lines: the shared [~altar, ~seh_me, ret] return tail sat after the FIRST movie
  pad instead of after the last (vector) pad.
- Diagnostics (tolA/tolC): VC7.1 keeps the tail on the first pad whatever the exit_point section's frequency.
- Fix (tolD): the chunk 813 loop form. `for (; !IsActiveThreadTerminating(); Game->NewScriptFrame())` becomes
  `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); ... }`. The loop-top return shares
  the expanded ~altar / ~seh_me exit, and VC7.1 then places the shared tail after the last pad as retail does.
- Bindings: only SYM_ELSEWHERE 0x0143E8E4 / 0x0143E8E8 for NullCoord (two field offsets of the one symbol, not a conflict).
- Test: test/00edd170.cpp PASS under g++ and VC7.1 (tall.sh / tall.py). Needs the chunk 811 opt-in header (hdr/README.md).

## 0x00684610 CTCCreatureOpinionOfHero::OnSerialise -- parked DIFFER(2823v2823), 4 lines (was 122)
- The lever (po17): same-TU bodies of the two loop callees, Transfer<long> 0x00410BE0 and Transfer<COpinionTransientOffset> 0x00685F80,
  written above OnSerialise under `#pragma auto_inline(off)` as explicit specializations of the landed rows' switch bodies.
  With the bodies visible VC7.1 knows the `const T&` default is only read. It then hoists the GetDefaultVal temporaries' stores out of
  the LastFrameAppliedDeedEffect and TransientOffsets loops into the preheaders (retail 0x006847EF, 0x00684A46, 0x00684F37), reloads
  the context from its argument slot, and homes the strings in the dead argument slots +0x80 / +0x84, all as retail does.
- Left: the hoisted COpinionTransientOffset default is zeroed in the ctor's body order (+0, +4, +8, +0x10, +0x14, +0xc), where retail
  uses memory order. The load loop's `offset` local needs the body-order ctor (retail 0x00684F5E..0x00684F7A). A memory-order
  init-list ctor fixes the default but breaks the local (po20, 2 lines, probe header not shipped). GetDefaultVal as a named local,
  a CPersistTraits copy or a memset all lose the hoist (125-186 lines).
- Also tried: explicit 3-arg defaults (po8 90, po11 89), scopes for each save / load section (po2-po7: 156-288), a const named default
  (po12 90, po14 83 but 2810 bytes).
- Header: new opt-in FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY (COpinionTransientOffset::TransferBinaryIn, the PDB spelling).
  The row also turns on FABLE_RETAIL_CDATAOUTPUTSTREAM (virtual Write for the binary-save arm) and includes engine/CPersistTraits.h /
  engine/CPersistContext_TransferableComponent.h.
- Spellings / NAME_FIXES:
  - 0x004793F8: the landed 0x00685F80 row binds `...QAEXPAVCMemoryDataInputStream@@@Z` (pointer). The PDB has only the reference
    spelling.
  - 0x00685F80: the landed caller 0x006861C0 binds the non-template `?Transfer@CPersistContext@@QAEXPBDAAVCOpinionTransientOffset@@ABV2@@Z`.
    The PDB spelling is the member template `??$Transfer@VCOpinionTransientOffset@@@...ABV1@@Z`.
  - The graded function itself binds no new conflicting symbol (bindchk: only "new" rows 0x00681BA0 / 0x00685CC0 / 0x00685F80).
- Label: already correct in ghidra_out (landing 2026-11-06c). q.py / the chunk oracle still print the propagated catalog name
  COpinionReactionManager::TransferOpinionData, a stale catalog entry only, so there is no LABELS.tsv row.

## 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main -- parked DIFFER(7692v7655), 36 masked lines (13 attempts)
- Three residues, all unchanged by any attempt:
  - The multiple-follower teleport loop register pair: retail has i = ebx and begin = ebp; ours are swapped, plus eax/edx swaps in the
    two size() computations.
  - The natural-exit ~seh_me stores: VC7.1 emits Data / Info / vptr in REVERSE program order in this TU only. The TOL twin and retail
    keep program order. It is not caused by the number of locals: coeD1 removes talk_point and the order stays.
  - Pad A (the thread check before the loop) uses the [esp+10h] PauseAll variant and does not cross-jump with the args pad.
- The TOL lever (for(;;NSF){if (T) return}) gives identical bytes here (coe6).
- Also tried: index types and loop forms, a named reference, a named rand index, a separate i declaration, the continue form.

## 0x007A7D20 CTCBattleCharge::CreateHitEffect -- parked DIFFER(2504v2520), md3 56 (11 attempts this chunk + 10 before)
- Residues:
  - The frame is 0x140 v 0x148. The first box sits at +0x90 in retail and +0x74 here.
  - Both C2DBoxI boxes are stored as computed. Retail computes the four corners in registers first, with x+r / y+r in place.
  - The sound-player lookup registers differ.
- Tried, none better:
  - Block forms (bc1-bc5).
  - The C2DCoordI as a temporary (identical).
  - The 4-arg C2DBoxI ctor (FABLE_RETAIL_C2DBOXI_CTORS) and Initialise from named x/y/r (md3 58).
  - A named filter (2480 bytes).
  - range declared first (md3 60).
  - region as a temporary (2488 bytes).

## 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate -- parked DIFFER(1699v1705), 61 lines (11 attempts this chunk)
- Residue: a consistent register rotation, plus PullBackAmount reloaded where retail spills it with `fst [esp+58h]`. Rotation sites:
  - Retail loads AnimIndex into edx before `push 5Ah`.
  - Retail keeps the creature in eax for the carrying test (`test cl,40h`; ours `test al,40h`, so our build is 6 bytes shorter).
  - The ParentThing push uses eax in retail, edx / ecx here.
- Tried:
  - Named amount forms, which flip esi/edi (263).
  - A reference creature, declaration orders and the template PeekTC (all identical).
  - GetTimeToBlendIn(*creature / strafer), which is worse.
  - A same-TU stand-in of GetAnimDistanceAndIndexFromAnimName (identical).
- Untried and plausible (retail compiled these earlier in the TU): bodies of GetStrafingAnimationName 0x008E8FF0 (not landed) and the
  landed GetBestFrameInAimingAnim 0x008E9460.

## 0x00D01630 NScript::CQ_BanditCampScript::CGate1GuardOuter::Main -- parked DIFFER(5134v5178), 78 masked lines (documented blocker)
- The MovieBase caller-size / inline-budget blocker: retail expands all 4 implicit ~CScriptGameResourceObjectMovieBase sites. Two are
  natural scope ends (0x00D025A8 / 0x00D0273A) and two are shared pads. Ours calls the COMDAT 0x006E7B80 at all 4.
- Tried here and not in GOTCHAS:
  - `__forceinline` CIsThingAlive and the CTimer ctor, to free budget: inert.
  - The diagnostic meters. /d2inlS15 fixes the frame slots but overshoots (it also expands ~CTimer and swaps ebp/edi), so the budget
    is about one unit short. T72 gives identical bytes. These are diagnostics only; no /d2 flag is kept in the source.
- The whole-block / SAY-level do-while(0) expands 1-3 sites (g5: 5177v5178 but half-expanded pads, 87 lines). The source is left at r0.

## Header debt
None beyond the opt-ins in hdr/ (see hdr/README.md).
