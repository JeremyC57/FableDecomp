# 00ec4a00 (p25B, Account B, 2026-11-20): NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00EC4A00 | CRTFE_RockTroll::Main() | 1540 | RELOCATION_MATCH (no flags) | PASS (56 targeted + 20,000 random runs) | 35 / 38 killed (3 same code) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings. Purity: plain C++, no asm, no
flag. **Inline control (line 2 `// Inline-budget:`): a scoped `#pragma inline_depth(0)` ... `#pragma inline_depth()` inside `#ifdef _MSC_VER` round
an explicit plain-`inline` CSpawnedFunc1<CV_RockTrollFirstEncounterScript,CScriptThing> ctor specialization**. The coordinator ruled it
admissible on 2026-11-20 under the owner decision of 2026-11-08 (EXTRA_RULES, "inline-budget residues may use an inline control"). The idiom is the
one landed in 0x00ED5F50 / 0x00E33EA0 / 0x00E2FF00, with the specialization's init list and body token-identical to engine/CSpawnedFunc1.h.

## The residue and why no flag closes it
Retail expands both spawn ctors (forceinline in the header). Inside them it CALLS the nested `Par1(par1)` CScriptThing copy 0x004ABE90 at
0x00EC4A97 and 0x00EC4E58. The WatchForRockTrollKilled by-value Me argument copy expands (0x00EC4A25..0x00EC4A44), and the WatchForRockTrollHit
argument copy is called (0x00EC4DFA). On the plain header everything else matches. The nested copy expands at both sites (+18 bytes each), and the
rest of the full diff follows from that: the loop-alignment pad moves from the seh_Hero loop to the seh_Troll loop, ebp holds &ParentClass
instead of PauseMyEntitiesPlease.Game, and eax/edx swap. Grade DIFFER(1588v1540).

c2 inline trace (tools/trace.sh, tools/trrow.py; real c2.dll decisions at 0x1078D1AA):

| site | f | sz | savings | c | score | c2 |
|---|---|---|---|---|---|---|
| nested Par1 copy (ctor's own pipe, then again in Main) | 1.0 | 33 | 8 | 25 | 25 | EXPAND |
| WatchForRockTrollKilled argument copy | 0.5 | 33 | 5 | 28 | 32 | EXPAND |
| WatchForRockTrollHit argument copy | 0.0059 | 33 | 5 | 28 | 706 | call |
| CSpawnedFunc1 ctor, site 2 (plain inline) | 0.0059 | 82 | 19 | 63 | 1589 | call (so forceinline is right) |

The caller growth counter reaches only about 560..730, far below the 9000 caller-size hard reject. The nested copy scores below the argument
copy, so no threshold can call the first and expand the second.

## Ladder and per-device ablation (advisor decisions 2026-11-20, docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md)
Flag attempts, on the plain-header port (ablation/grades.txt):
- /d2inlT50 / 55 / 60 / 65 / 70: DIFFER 1540 / 1516 / 1516 / 1516 / 1588 v1540.
- /d2inlS15 / 30: DIFFER 1588 / 1620.

Each device removed in turn from the final source (ablation/*.cpp):

| device removed | grade without it |
|---|---|
| the inline_depth(0) specialization (the header's forceinline ctor is used) | DIFFER(1588v1540) |
| `inline_depth(0)` -> `inline_depth(1)` | RELOCATION_MATCH. 0 is the smallest N and the family's value. |
| FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (every copy is then called) | DIFFER(1524v1540) |
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (plain inline calls the pause ctor at f 0.0117) | DIFFER(1532v1540) |
| FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING (the plain header expands the table call) | DIFFER(1556v1540) |

Dropped as unnecessary: the specialization's FABLE_FORCEINLINE, FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN, and
FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR / _DTOR. Each one grades RELOCATION_MATCH when removed. Without them the TU emits its own
COMDATs for ScriptedThingBase::operator=, the movie ctor and the movie dtor, and each is byte-equal to retail 0x008ABD10 / 0x006E7B60 /
0x006E7B80 apart from relocations (tools/cmpcomdat.py).
- CDefString: the 2026-11-19 binding swap. The draft's FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION gave vland RELOC_CONFLICT / RELOC_NAME
  warnings (??B bound to 0x00415D70). The NScript-TU opt-in FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING calls GetString 0x00415D70 behind the
  expanded conversion, and the warnings are gone.

## Family search (coordinator question 1): no pragma-free precedent
I searched for a landed row of this family in which the nested Par1 copy is CALLED without a pragma or flag. There is none.
- Every landed row that constructs a CSpawnedFunc1<..., CScriptThing> without an inline control uses the out-of-line copy for every copy (0x00E07640,
  0x00DA7830, 0x00DA0EF0, ...), so it has no split.
- The pragma-free rows with an inline copy are SuspendableProcess bodies, which have no ctor.

**Entry for docs/journal/FINDINGS_LOG.md:** 2026-11-20 (p25B, 0x00EC4A00) CSpawnedFunc1 nested-copy split measured with the c2 inline trace.
- c2 compiles the forceinline ctor in its own pipe before Main (bottom-up) and decides the nested Par1 CScriptThing copy there: f 1.0, c 25, score 25,
  EXPAND. It decides it again identically inside Main.
- The by-value argument copy at the first site scores 32 (f 0.5, c 28), and caller growth is only about 560..730 (hard reject at 9000).
- So no /d2inlT or /d2inlS value can produce retail's split (nested called, argument expanded). /d2inlT50..70 and /d2inlS15/30 were tried.
- Defining the copy ctor inline before or after the caller changes nothing (c2 orders functions bottom-up).
- Retail's SuspendableProcess 0x00EC52F0 of the same instantiation expands the copy, so the copy is an inline candidate in that TU.
- Mechanism still unknown. The landed idiom (inline_depth(0) specialization) closes it because Main inherits the ctor's own-pipe decision.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 186..232) and locals:
- function scope: `_Fail_Val`, `seh_Troll`;
- one block: {csEntMap, seh_Hero, speech_movie, PauseMyEntitiesPlease}.

Generated quest-script text as the landed Mains:
- the spawn statement on one line (Ego_d 189, 510 bytes);
- the seh macro on one line (192);
- RunCutsceneMacro and the second spawn on one line (215, 543 bytes);
- each terminate check on its statement's line.

No `// Inferred:` lines.

## Headers (hdr/, hdr.diff: one NEW file)
- NEW engine/CV_RockTrollFirstEncounterScript.h. Carried from chunk 842/856's pending overlay (rebuild/candidates/manual/pending/
  2026-11-08_linux_hand856/hdr/engine/), unchanged. I rechecked it against two sources:
  - Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv, CV_RockTrollFirstEncounterScript 108 bytes): MissionSucceeded 92 .. AddedItemsToRockTroll 105,
    minus 0x14 gives 0x48..0x55;
  - the retail bytes of this row: the +0x55 test 0x00EC4B84, the +0x55 store, the +0x54 test and the +0x54 store 0x00EC4EC0; ParentClass +0x14.
  - sizeof 0x58, as the landed Alloc 0x00EC5360. Nested CRTFE_RockTroll: ParentClass +0x14, PMasterData +0x18, sizeof 0x1C.
- NEWLY FLAGGED (owner policy: listed, not a blocker). These landed rows define CV_RockTrollFirstEncounterScript / CRTFE_RockTroll locally:
  0x00EC3C20 0x00EC3C50 0x00EC3CA0 0x00EC3D30 0x00EC3D40 0x00EC41E0 0x00EC41F0 0x00EC4290 0x00EC42A0 0x00EC42B0 0x00EC42C0 0x00EC42D0
  0x00EC42F0 0x00EC4490 0x00EC4530 0x00EC4540 0x00EC4590 0x00EC45A0 0x00EC45C0 0x00EC4660 0x00EC4670 0x00EC4680 0x00EC4690 0x00EC46B0
  0x00EC47C0 0x00EC4920 0x00EC4970 0x00EC5010 0x00EC5140 0x00EC5330 0x00EC5360 0x00EC53A0.

## Test (test/00ec4a00.cpp)
Links the real row object. These run as compiled:
- the expanded ctors;
- the implicit CScriptThing copy COMDAT;
- the STLport map operator[] / insert / ~map COMDATs;
- the ScriptedThingBase / movie ctor, dtor and operator= COMDATs;
- the pause expansion.

Every external is bound by /alternatename to a __fastcall stub, and every vtable slot or unreached COMDAT reference to a trap. Real
STLport nodes are linked by the insert_unique stub. Counted-pointer info blocks are real; the row frees them itself.

What the inputs script: terminate positions, StartScriptingEntity retries for troll and hero, IsPerformingScriptTask sequences, allocation
failure of either spawn, the AddedItems / PlayedExhume flags and the physics flag.

What the test checks:
- the event order against an independent model;
- the spawned objects' vtable, Func (WatchForRockTrollKilled / Hit), PClass, Par1 = Me (one reference each), base name "ParentClass.WatchFor..."
  and id 0;
- reward items 1 and 2 on Me;
- the cutscene map holding HERO -> seh_Hero and TROLL -> seh_Troll under the paused entities;
- the animation names and flags;
- PlayedExhumeCutScene stored after the second spawn;
- the release of every string and handle, and Me's reference count.

## Mutants (mutants/)
38 one-decision mutants, 35 killed. The 3 SAME_CODE are literal swaps that change only a relocation target, which mut2 does not see (it compares
bytes). The test's string checks catch them: the concat order and the group name "" are checked in the stubs, and the map keys are checked in
RunCutscene.
