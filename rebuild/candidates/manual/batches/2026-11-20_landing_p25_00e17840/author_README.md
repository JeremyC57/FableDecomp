# 00e17840 (p25A, Account B, 2026-11-20): NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E17840 | NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main() | 2515 | RELOCATION_MATCH (Flags: /d2inlS13) | PASS (720 targeted + 6,000 random runs) | 28 / 28 killed |

Strict vland: `RELOCATION_MATCH PASS`, no warnings; the payload carries compiler_flags [/d2inlS13]. Purity: C++ plus one scoped
inline control and one per-row flag (below). No asm, no volatile, no `// Inferred:` lines.

## What fixed the drafts' residues
The drafts (hand833 / codex_released) graded DIFFER(2501v2515) and listed three residues.
1. The predicate copy split. Retail calls 0x004ABE90 for the CIsThingAlive copy but expands the by-value SetIsPushableByHero copy. The
   family idiom of 0x00E25F70 fixes it: inline_depth(0) round the ctor, NOVTABLE, IMPLICIT_COPY.
2. The mixed movie ctor and (3) the per-return pause pads and the seh_me dtor that is expanded only at the "natural" exit. Both come
   from the loop form.
   - The drafts' `while (!IsActiveThreadTerminating())` made the final exit the loop's cold natural exit (c2 f=0.028): the dtor was
     called there and expanded at the talk wait-loop exit instead (f=1.23).
   - The family's generated loop `for (;!false;Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return ...; }` makes it an
     in-loop return. Retail's pre-loop test 0x00E17921 and bottom test 0x00E18146 share the expanded exit 0x00E1814E, which is exactly
     this form's rotated layout.
   - With this form the drafts' movie devices (OUTOFLINE_CTOR / OUTOFLINE_DTOR, the counted-pointer declaration) are unneeded, and
     so is a pragma round the hit speech_movie.

## Source shape
Source of truth: the Ego_d.pdb line table (egod_lines/, lines 681..742) and the Ego_d locals:
- function scope: seh_me, _Fail_Val;
- each of the two speech scenes: speech_movie and PauseMyEntitiesPlease.

The text is the family's generated style: TRetVoid returns and the frame loop above, with the trailing unreachable terminate test.
Without the trailing test the bytes are the same; it is kept as the family's generated text (0x00E25F70).

## Inline-budget (line 2)
(1) Predicate: a scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor, with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE,
FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY and a row-local inline ~CIsThingAlive.
- Retail expands the ctor but CALLS its nested copy 0x004ABE90 (0x00E17863), while it expands the by-value SetIsPushableByHero copy
  (before the slot-844 call 0x00E178F4).
- Without the pragma, c2 scores the nested copy 28 (f=1) and the by-value copy 32 (f=0.5). Retail calls the hotter site and expands
  the colder one, so no threshold works: S13 with T30 / T50 / T65 gives 2362 / 2485 / 2531.

(2) Flag /d2inlS13.
- Retail expands both CWideScreenMagicPauseEntities ctors: PauseAllNonScriptedEntities(true) at 0x00E17A2C and 0x00E1803B.
- With the default cap, c2 expands the talk ctor (score 45) and calls the hit ctor (f=0.0203, score 133).
- /d2inlS<N> makes callees of at most about 2N IL units always inline, and the pause ctor is 25 units. S13..S15 match; S12 differs.
- The flag replaces the drafts' FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR opt-in. The opt-in also matches, but the
  ladder puts the flag first.

## Device ablation (each removed in turn, the rest kept)
| Device removed | Grade |
|---|---|
| predicate `#pragma inline_depth(0)` | DIFFER(2531v2515) |
| ctor and dtor both in the pragma / inline_depth(1) | RELOCATION_MATCH (ctor-only depth 0 kept: the 0x00E25F70 idiom) |
| FABLE_RETAIL_CPREDICATEBASE_NOVTABLE | DIFFER(2515v2515) |
| FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY | DIFFER(2483v2515) |
| the row-local inline ~CIsThingAlive | DIFFER(2499v2515) |
| /d2inlS13 | DIFFER(2525v2515) |

## Header overlay (hdr/, hdr.diff)
engine/CQ_WhiteBalverineKnotholeGladeScript.h gets the chunk-833 opt-in FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_CKG_CHIEF,
carried over unchanged from rebuild/candidates/manual/pending/2026-11-07_linux_hand833/hdr/. Only its comment line gains this row
address.
- Layout: CKG_Chief, Ego_r 28 bytes; ParentClass& +0x14, PMasterData +0x18.
- Vtable 0x012E0B98, re-read from retail .rdata: sdd 0x00E17820, Main 0x00E17840, Init 0x00E17800, GetParentScript 0x00E177F0, base
  OnPersist 0x00CDEBC0, OnPredicateFail 0x00E17810, base OnInterrupted 0x00CDEBE0.
- The block is ADD-ONLY with its own size assert. No existing assert changed.

Landed rows flagged for re-port (each models CKG_Chief locally): 0x00E17750 (Alloc), 0x00E177F0 (GetParentScript), 0x00E17800 (Init),
0x00E17810 (OnPredicateFail), 0x00E17820 (sdd).

## Test (test/00e17840.cpp)
The test links the real row object, built with the row flag.
- What runs as compiled: the depth-0 predicate ctor with the TU's implicit copy COMDAT, the inline ~CIsThingAlive, the expanded
  by-value copy (counted against Me's record), the implicit handle and movie members, and the expanded pause ctor.
- Scripted world, per frame: BalverineState, talk / hit / ability / heal messages and the speech task.
- Scripted results: termination, StartScriptingEntity, IsQuestActive and health.
- Checks: the event order against an independent model, then the balance of things, CBaseObject parts, movies, strings and
  reference counts.
- Coverage: 13 counters, all nonzero.

## Mutants (mutants/)
28 one-decision mutants, 28 killed. They cover the state constants, the quest test, the hit chain and the state-7 guard, the setup
calls, the group selection, the health gates, the wait loops, the friends flag, the extra frame and the release.
