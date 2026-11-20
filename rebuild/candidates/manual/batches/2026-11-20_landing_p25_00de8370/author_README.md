# 00de8370 (p25A, Account B, 2026-11-20): NScript::CQ_RansomVictimScript::CRansommer::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00DE8370 | NScript::CQ_RansomVictimScript::CRansommer::Main() | 4894 | RELOCATION_MATCH (Flags: /d2inlS13 /d2inlT29) | PASS (1,400 targeted + 6,000 random runs) | 44 / 44 killed |

Strict vland: `RELOCATION_MATCH PASS`, no warnings; the payload carries compiler_flags [/d2inlS13, /d2inlT29]. Purity: plain C++ and two per-row
flags. There are no source devices, no asm, no volatile, and no `// Inferred:` lines. No header change (hdr.diff is empty).

## What fixed the drafts' residue
The drafts (hand859 / codex_released) graded DIFFER(4894v4894) with 18 grades on record. Their residue: at the block-1 NOT_ENOUGH_GOLD
wait tests (0x00DE87AD, 0x00DE8A67) retail has eax + `lea ecx,[eax+0ACh]`, the drafts had ecx + `add ecx,0ACh`.

The cause was the first handle release.
- The drafts spelled it `{ CScriptGameResourceObjectScriptedThingBase& handle = ParentClass.seh_Ransommer; ... }`.
- The landed same-TU siblings (CVictim / CVictimsBrother / CVictimsFatherGuard::Main) spell it
  `{ CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Ransommer.IsValid()) parent.seh_Ransommer.Release(); }`.
- Retail keeps &seh_Ransommer in ebx across IsValid and Release (0x00DE83CA).
- The hand859 draft with only that one release respelled is a RELOCATION_MATCH.

## Source shape
Source of truth: the Ego_d.pdb line table (egod_lines/, lines 864..1044) and the Ego_d locals:
- function scope: _Fail_Val;
- each of the two speech scenes: speech_movie and PauseMyEntitiesPlease, with YNQAnswer in each question block;
- hit block: HLStyleConversationIndex.

Each health-gated Speak with its wait is a single Ego_d line (450 / 451 bytes), and each question with its answer wait is one line (396).
Changes against the drafts:
- the answer local is renamed YNQAnswer, and the hit block's conversation local HLStyleConversationIndex (both Ego_d names);
- Game uses the original reference member (CENTITYSCRIPTBASE_ORIGINAL_MEMBERS). With `Game->` the bytes are the same.

## Inline-budget (line 2): flags /d2inlS13 /d2inlT29
This is a pure inline-budget split.
- Retail CALLS the implicit movie ctor COMDAT 0x006E7B60 at both speech scenes (0x00DE84DE, 0x00DE8B19).
- Retail EXPANDS both CWideScreenMagicPauseEntities ctors (PauseAllNonScriptedEntities(true) at 0x00DE851B / 0x00DE8B56).
- With default flags c2 expands the second movie ctor (f=0.2367, score 29) and calls the first pause ctor (f=0.0234, score 118).

Measured flag window: RELOCATION_MATCH for exactly S13..S14 × T22..T29. Outside it:
- S12 + T29, S15 + T29 and S25 + T29 differ;
- S13 + T30 differs; T20 differs.

I took the values nearest the defaults (S13, T29).

Ladder: the drafts' header opt-ins FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR and
FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR also match, with no flags. Following the advisor's ladder rule (flag before
source device), the flags replace them. Each opt-in can be swapped for a single flag:
- without OUTOFLINE_CTOR (WIDESCREEN kept), T22..T29 alone match;
- without the WIDESCREEN opt-in (OUTOFLINE_CTOR kept), S13..S25 alone match.

If you prefer the same-TU convention (landed CVictimsBrother uses the WIDESCREEN opt-in), the opt-in pair is an equivalent RELOCATION_MATCH.

## Device ablation
| Removed | Grade |
|---|---|
| both flags | DIFFER(5054v4894) |
| /d2inlT29 (S13 kept) | DIFFER(4990v4894) |
| /d2inlS13 (T29 kept) | DIFFER(4964v4894) |

These drafts' devices are unneeded with this text, each removed in turn with RELOCATION_MATCH:
- OUTOFLINE_DTOR;
- the family predicate pragma round CIsThingAlive;
- FABLE_RETAIL_CPREDICATEBASE_NOVTABLE.

## Test (test/00de8370.cpp)
The test links the real row object, built with the row flags.
- What runs as compiled: the TU's CIsThingAlive and its vtable, the implicit movie members, the expanded pause ctor, and the virtual
  calls through the parent's seh_Ransommer (slots 12 / 13 / 26) and Me.
- Scripted world, per frame: talk / hit / ability / heal messages and the speech task.
- Scripted per run: termination, StartScriptingEntity, answers (including unanswered frames), the hero's gold (0 / 7499 / 7500 / 9999 /
  10000 / 50000), health, RescuedVictim, RansomHelpersAttacked, and a pre-valid handle.
- Checks: the event order against an independent model, then RescuedVictim, and the balance of things, CBaseObject parts, movies,
  strings and reference counts.
- Coverage: 14 counters, all nonzero.

## Mutants (mutants/)
44 one-decision mutants, 44 killed. They cover the gold thresholds, the payments, the cut-scene and text names, the group selections,
the answer test, the flag stores, the faction / marker / info calls, the wait loops and the releases.
