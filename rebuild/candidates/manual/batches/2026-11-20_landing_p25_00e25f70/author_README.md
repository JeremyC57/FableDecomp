# 00e25f70 (p25A, Account B, 2026-11-20): NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E25F70 | NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main() | 2049 | RELOCATION_MATCH (no flags) | PASS (480 targeted + 6,000 random runs) | 48 / 48 killed |

Strict vland: `RELOCATION_MATCH PASS`, no warnings. Purity: C++ plus one scoped inline control (below); no asm, no volatile.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 58..139) and locals: ExitDistance, TriggerDistance, seh_me, _Fail_Val, ExitPos;
HLStyleConversationIndex; speech_movie, PauseMyEntitiesPlease; seh_hero, speech_movie, PauseMyEntitiesPlease, mymap.
- The text follows the generated quest-script style of the twin CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Main
  0x00E26E30: TRetVoid returns, and a `for (;!false;Game->NewScriptFrame())` frame loop whose terminate test comes first.
- New against the drafts: ExitDistance (`pGlobalScriptDef->GSI_MazeRegionExitDistance`, Ego_d line 73). It is a dead local, like
  ExitPos (line 66): retail keeps only the GetPos call and never loads +0xC6C.

## Inline-budget (line 2)
Device: a scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor, with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE and
FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY. This is the landed predicate idiom of 0x00E27E90 / 0x00E5B3B0, and the family blocker in
docs/pipeline/INLINE_DEPTH_REVISIT.md section 3.
- What it reproduces: retail expands the predicate ctor but CALLS the nested CScriptThing copy 0x004ABE90 (at 0x00E25F92), while it
  expands the by-value copy in the SetIsPushableByHero argument slot (0x00E260E2..0x00E26104).
- Why no flag works: the c2 trace without the pragma scores the nested site 28 and the by-value site 56 (T=74), and both expand.
  Retail expands the colder site, so no threshold separates them. /d2inlT20..70 give 2028..2070; /d2inlS5..20 give 2070..2073.
- seh_hero's handle ctor needs nothing: in this source c2 already calls it (f=0.0074, score 403), as retail does at 0x007E72A0.

## Device ablation (each removed in turn, the rest kept)
| Device removed | Grade |
|---|---|
| `#pragma inline_depth(0)` region | DIFFER(2070v2049) |
| FABLE_RETAIL_CPREDICATEBASE_NOVTABLE | DIFFER(2066v2049) |
| FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY | DIFFER(2031v2049) |
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | DIFFER(2062v2049) |
| inline_depth(1) instead of (0) | RELOCATION_MATCH (0 kept: the smallest N, and the landed idiom) |

The drafts' four OUTOFLINE opt-ins were unneeded and are dropped: STB DTOR / ASSIGN and MovieBase CTOR / DTOR. RELOCATION_MATCH
without them, one at a time and all four together.

## Header overlay (hdr/, hdr.diff)
NEW engine/CQS_GuardianSisterInfoScript.h. It is modelled on the twin's header engine/CQS_GuardianSisterInfo2_SisterInBanditCampScript.h.
- The level script: retail sizeof 0x4C, GuardianSpokeToHero +0x48 (Ego_r +0x5c on the donor base), vtable 0x012E1DAC with its
  four virtuals.
- The nested CMazeAtTavern entity: 0x28 bytes; ParentClass reference +0x14, PMasterData +0x18, TalkCounter +0x1c, WavedOver +0x20,
  HitCount +0x24; vtable 0x012E1E28 with its five slots.
- The two size asserts are plain FABLE_STATIC_ASSERT (a new file, nothing to pin).

Landed rows flagged for re-port (they model these classes locally): 0x00E259D0, 0x00E25A00, 0x00E25A10, 0x00E25AA0, 0x00E25AB0,
0x00E25C60, 0x00E25C70, 0x00E25D10, 0x00E25D20, 0x00E25D30, 0x00E25D70, 0x00E25D90, 0x00E25EF0, 0x00E25F40, 0x00E26780, 0x00E267C0.

## Test (test/00e25f70.cpp)
The test links the real row object.
- What runs as compiled: the depth-0 predicate ctor with the TU's implicit copy COMDAT, the expanded by-value copy, the implicit
  handle / movie special members, CWideScreenMagicPauseEntities and the TU's own map code.
- Fixtures: Me carries a counted pointer-info record, so the predicate copy and the by-value copy are each checked for their
  increment and release. StartMovieSequence binds the movie to a heap pointer-info record whose delete function marks the movie's end.
- Scripted world: hero distance, hit / any-ability / heal-spell / talk messages, Maze's speech task, health, termination and
  StartScriptingEntity results.
- Checks: the event order against an independent model; WavedOver / HitCount / TalkCounter / GuardianSpokeToHero; the balance of
  strings, things, CBaseObject parts, movies and reference counts.

## Mutants (mutants/)
48 one-decision mutants; 48 killed.
