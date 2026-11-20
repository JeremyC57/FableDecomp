# 00dd4ad0 (p25A, Account B, 2026-11-20): NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00DD4AD0 | NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main() | 3467 | RELOCATION_MATCH (no flags) | PASS (770 targeted + 6,000 random runs) | 46 / 48 killed, 2 equivalent |

Strict vland: `RELOCATION_MATCH PASS`, no warnings. Purity: C++ plus inline controls on row-local helpers (below); no asm, no volatile.
There are no `// Inferred:` lines.

## Source shape
Source of truth: the Ego_d.pdb line table (egod_lines/, lines 760..880) and the Ego_d locals:
- function scope: CellDoor, seh_me, TalkTimer, _Fail_Val;
- talk block: speech_movie, PauseMyEntitiesPlease;
- door block and both hit blocks: HLStyleConversationIndex.

The text follows two landed rows:
- The TU's twin CPrisoner::Main 0x00DD5A60: TRetVoid returns, Game as the original reference member (CENTITYSCRIPTBASE_ORIGINAL_MEMBERS),
  `TalkTimer == 0`, SetEntityAsRegionFollowing(hero, Me, ...), and the final `for (;; Game.NewScriptFrame())`. The courtyard and guards
  tail is the twin's text.
- The speech scene of 0x00E25F70: speech_movie, `CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game)`, and the health-gated
  Speak with its wait.

Against the drafts (hand848 / codex_released, DIFFER 3467v3467):
- The drafts' `game` local and pointer ParentClass are replaced by the Ego_d locals and the PDB reference member.
- The drafts' SetEntityAsRegionFollowing(Me, hero) argument order is fixed to retail's (hero, Me): 0x00DD4C63 pushes 1, Me, then
  GetHero's result.
- The class lives in the shared header opt-in instead of a row-local definition.

## Inline-budget (line 2)
(1) Predicate: a scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor and dtor, with
FABLE_RETAIL_CPREDICATEBASE_NOVTABLE. This is the family idiom (0x00E25F70, 0x00D0B3E0).
- Retail expands the predicate ctor and dtor (vptr 0x012C32F0 in ebx) but CALLS the nested copy 0x004ABE90 (0x00DD4AF3) and
  ~CScriptThing 0x004AA840 (0x00DD4B0C).
- Retail expands ~CScriptThing at the cutscene-wait exit 0x00DD5385.
- Without the pragma, c2 scores the nested ~CScriptThing 63 and that exit 65 (T=74), so a single threshold cannot call one and expand
  the other. Without the pragma, T30..60 give 3317..3365 and S5 / S15 give 3531 / 3620.

(2) Timer operators: FABLE_FORCEINLINE on the row-local CTimer::operator=(long) and operator==(long).
- Retail expands SetTimer / GetTimer at all four sites (0x00DD4D08, 0x00DD54A2, 0x00DD565F, 0x00DD56D5).
- Plain inline scores them 169 / 89 and calls them.
- With plain operators, T80..T170 give 3419..3546 and S5..S15 give 3447..3562.

(3) Movie release: the out-of-line declaration of CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer (0x006E7AB0).
This is a landed idiom (139 rows).
- Retail expands the implicit movie dtor at the talk block's end and CALLS this release from it: 0x00DD527F, then the GROBase vptr store
  and ~CBaseObject.
- At every exit retail calls the movie dtor COMDAT 0x006E7B80.
- With the release inline, the end site costs 60 and scores 199 (f=0.058).
- Removing every terminate test only raises f to 0.08, so no source shape gets the site under T.
- With the release out of line, the end site expands and the exits stay calls.

## Device ablation (each removed in turn, the rest kept)
| Device removed | Grade |
|---|---|
| predicate `#pragma inline_depth(0)` (1) | DIFFER(3531v3467) |
| pragma round the ctor only (dtor outside) | DIFFER(3531v3467) |
| inline_depth(1) instead of (0) | RELOCATION_MATCH (0 kept: the landed idiom) |
| FABLE_RETAIL_CPREDICATEBASE_NOVTABLE | DIFFER(3483v3467) |
| FABLE_FORCEINLINE on operator= (2) | DIFFER(3455v3467) |
| FABLE_FORCEINLINE on operator== (2) | DIFFER(3473v3467) |
| the movie-release declaration (3) | DIFFER(3450v3467) |
| the row-local `inline CScriptThing::~CScriptThing() { }` (the twin's text) | DIFFER(3414v3467) |

FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY and FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR were unneeded: RELOCATION_MATCH
without each of them, and without both together. Both are dropped.

## Header overlay (hdr/, hdr.diff)
engine/CQ_PrisonEscapeRescueMotherScript.h gets an ADD-ONLY opt-in, FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_PRISONERACCOMPLICE:
class CPrisonerAccomplice.
- Layout: Ego_r 32 bytes; ParentClass& +0x14, PMasterData +0x18, ReleasedAndFollowing +0x1c. Retail's new size is 0x20 (Alloc
  0x00DD49A0).
- Vtable 0x012DBEF0, read from retail .rdata: sdd 0x00DD4AB0, Main 0x00DD4AD0, Init 0x00DD4A50, GetParentScript 0x00DD4A40, OnPersist
  0x00DD9490; OnPredicateFail and OnInterrupted are the base members.
- The member set and access copy the landed CPrisoner opt-in in the same header.
- New size assert inside the block. Nothing existing changed, so nothing to pin.

Landed rows that model this class locally: none. The landed Alloc / GetParentScript / Init / sdd of CPrisonerAccomplice
(0x00DD49A0 / 0x00DD4A40 / 0x00DD4A50 / 0x00DD4AB0) can switch to the opt-in after landing.

## Test (test/00dd4ad0.cpp)
The test links the real row object.
- What runs as compiled: the depth-0 predicate pair, the TU's inline ~CScriptThing (its counted release and the ~CBaseClass call),
  the forced timer operators, the expanded and called ~CTimer, the implicit handle and movie members, the inline MsgIsUsedBy through
  the door implementation (slot 20), and CWideScreenMagicPauseEntities.
- The stubbed movie release (0x006E7AB0) does retail's release.
- Scripted world, per frame: the cutscene flag, talk / door-use / hit / ability / heal messages, region load and the speech task.
- Scripted results: termination, StartScriptingEntity, MakePrisonerComment, health, the master data (key, race won, race number), a
  null door, and a pre-set ReleasedAndFollowing.
- Checks: the event order against an independent model, then ReleasedAndFollowing, timer deregistration, and the balance of things,
  CBaseObject parts, movies, strings and reference counts.
- Coverage: 14 branch counters, all nonzero.

## Mutants (mutants/)
48 one-decision mutants: 46 killed, 2 equivalent survivors.
- Both survivors remove an `if (seh_me.IsValid()) seh_me.Release();`: the one before the talk block's StartScriptingEntity loop, and
  the one after the release loop.
- Neither guard can fire. seh_me starts empty, and each talk block releases it at its normal end; every other path out of the talk
  block leaves the function.
- Retail keeps both guards: 0x00DD4DE4 and 0x00DD4C21.
