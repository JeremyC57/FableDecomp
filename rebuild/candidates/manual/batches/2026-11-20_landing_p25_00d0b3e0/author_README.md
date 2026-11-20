# 00d0b3e0 (p25A, Account B, 2026-11-20): NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00D0B3E0 | NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main() | 3802 | RELOCATION_MATCH (no flags) | PASS (660 targeted + 6,000 random runs) | 51 / 51 killed |

Strict vland: `RELOCATION_MATCH PASS`. Its one warning is RELOC_ALIAS `_rand -> 00bfeb16`, the MSVCR71 import thunk, and it is expected.
Purity: C++ plus scoped inline controls (below); no asm, no volatile.

## Source shape
Source of truth: the Ego_d.pdb line table (egod_lines/, lines 2035..2372; lines 2294..2356 hold no code) and the Ego_d locals:
King, village, KingHealthLast (int), seh_me, ActionTimer, _Fail_Val, SetPausedForCutscene, and Conversation in the taunt block.

The text follows the generated style of the TU's landed CBanditKing::Main 0x00D0A830:
- TRetVoid returns.
- Arena loop: `for (;!false;Game->NewScriptFrame())` with the terminate test first, so its bottom exit is the pre-loop exit edge
  (0x00D0B60E / 0x00D0BD8B).
- Second loop: keeps the pre-loop test and a bottom `frame; test` (0x00D0BE26 / 0x00D0C200).
- Last loop: `while (true) { frame; test }` (0x00D0C278, no leading test).

Semantic fix against the drafts: the arena loop's `if (ParentClass.BanditKingFightEnded) break;` sits at the end of the
"distance over 6.5 && ActionTimer <= 0" block.
- Retail skips it on the attack path and when that test fails: 0x00D0B786 / 0x00D0B7A6 / 0x00D0B7BD jump past it to 0x00D0BD7A.
- The drafts tested it on every non-attack frame. The test checks this, and so does mutant 38.

## Inline-budget (line 2)
(1) A scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor and dtor, with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
and FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY. This is the family predicate idiom of 0x00E27E90 / 0x00E25F70 (INLINE_DEPTH_REVISIT.md
section 3).
- Retail expands the predicate ctor and dtor but CALLS their nested CScriptThing copy 0x004ABE90 (0x00D0B409) and ~CScriptThing
  0x004AA840 (0x00D0B42B).
- It expands the by-value SetIsPushableByHero copy (0x00D0B461..0x00D0B483).

(2) One in-body `#pragma inline_depth(0)` region, from the arena loop's post-loop terminate test to the end of the second loop's
first cutscene-wait block.
- Retail CALLS ~CTimer 0x00CD4470 at those exits, which jump to the shared tail 0x00D0C28B. It expands ~CTimer only at the arena
  loop's own exits (0x00D0BD91).
- Our c2 trace scores the post-loop exit f=0.072 (score 39) and the cutscene-wait retry exit f=0.035 (score 69), both under T=74,
  so it expands both.
- The arena exits that retail expands score 37, so no threshold splits them. Without this pragma, /d2inlT30..60 give 3770..3818,
  /d2inlS5 gives 3826 and /d2inlS15 gives 3879.

(3) FABLE_FORCEINLINE on the row-local CTimer::operator=(long) and CTimer::GetTime().
- Retail expands every use. Plain inline calls one GetTime (f=0.0088, score 238) and some operator= sites.
- The TU's landed 0x00D0A830 text already forces operator=.

Ladder: flags were tried before the source devices (the T and S sweeps above, run with devices (1) and (3) in place); none matches.

## Device ablation (each removed in turn, the rest kept)
| Device removed | Grade |
|---|---|
| predicate `#pragma inline_depth(0)` (1) | DIFFER(3834v3802) |
| FABLE_RETAIL_CPREDICATEBASE_NOVTABLE | DIFFER(3818v3802) |
| FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY | DIFFER(3786v3802) |
| tail `#pragma inline_depth(0)` region (2) | DIFFER(3826v3802) |
| FABLE_FORCEINLINE on operator= (3) | DIFFER(3796v3802) |
| FABLE_FORCEINLINE on GetTime (3) | DIFFER(3802v3802) (same length, different bytes) |

The draft's OUTOFLINE_DTOR opt-in was unneeded and is dropped.

## Header overlay (hdr/, hdr.diff)
engine/CQ_BanditCampBossBattleScript.h gets a new ADD-ONLY opt-in, FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS: class
CCROWDBANDITS.
- Layout: Ego_r 28 bytes; CEntityScriptBase, ParentClass& +0x14, PMasterData +0x18.
- Declarations: Alloc 0x00D0B2D0, GetParentScript 0x00D0B370 (slot 3), and the private Main 0x00D0B3E0 (slot 1) and OnPredicateFail
  0x00D0B380 (slot 5). The vtable 0x012C95F0 was read from retail .rdata.
- The new size assert sits after the FORGER assert. pin_asserts.py pinned the 7 existing asserts to their line numbers
  (FABLE_STATIC_ASSERT_AT).
- Existing includers do not define the macro.

## NAME_FIXES.tsv
0x00D0B3C0 is CCROWDBANDITS::`scalar deleting destructor'.
- Evidence: its only retail reference is slot 0 of vtable 0x012C95F0, and labels_anniversary_2026-11-04_high.tsv names it the same.
- The landed file rebuild/src/compiled/00/d0/CLandscapeBackgroundPatch_vectordeletingdestructor_00d0b3c0.cpp carries the old wrong
  name in its filename. Its body is the generic struct T VecDel template, so only the filename and catalog name need the fix.

Landed rows that model this class locally: none. NScriptCQBanditCampBossBattleScript_Main_00d038e0.cpp binds CCROWDBANDITS via its
FABLE_BANDITCAMP_ENTITY macro; it can switch to the opt-in after landing, but that is not required.

## Test (test/00d0b3e0.cpp)
The test links the real row object.
- What runs as compiled: the depth-0 predicate ctor and dtor with the TU's implicit copy COMDAT, the expanded by-value copy, the
  forced CTimer members, the called and expanded ~CTimer, and the handle special members.
- Timers: pDSTGame timer slots 87..90 are a ticking timer model.
- Game: a fake vtable covers the slots the row uses (7, 8, 70, 72, 81, 108, 364..366, 451, 458, 460, 469, 475, 492, 512, 516, 529,
  690, 844). Unused virtuals trap.
- rand: defined by the test, so the jeer, boo and wave branches can be chosen.
- Scripted parent state: the start, ended and all-over flags, BanditsNeededForCutscene, KingHealth, TwinBladeKilled and
  AngryBanditNeeded. The model absorbs the row's AngryBanditNeeded decrement.
- Checks: the event order against an independent model, plus the balance of strings, things, CBaseObject parts and pointer-info
  reference counts.
- Coverage counters for all 15 branch groups are nonzero.

## Mutants (mutants/)
51 one-decision mutants, 51 killed. They cover the thresholds 3.5 / 6.5 / 8.0, the rand moduli, the timer values, the break
placement, the flags, the animation, sound and text names, the AngryBanditNeeded handling and the RemoveThing arguments.
