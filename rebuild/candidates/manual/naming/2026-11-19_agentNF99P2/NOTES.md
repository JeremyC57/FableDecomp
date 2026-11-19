# NF99P2 notes (2026-11-19, round 9): GameCODA part 2

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 30 entries: 17 done, 0 done-already, 0 rejected, 13 blocked, 0 deferred,
0 open. 16 rows changed, all ported (16 RELOCATION_MATCH, every test PASS in recheck_final.log): 15 rows.txt rows + 0x00C54190
(nflock NF99P2; a landed binder of 0x00C6AEF0 that callers.py / reloc_bindings.tsv did not list, found by an exe call-site scan).
0 not-a-binder among changed rows; 2 rows.txt rows listed not-a-binder (B's 0x00C77800 / 0x00C88E60, read-only, already SDK names);
12 rows.txt rows listed blocked (10 family rows, 0x00C74420 / 0x00C74450 for q11517). EH funclet 0x01216140 (includes 0x00C54510 by
path) regraded against the ported parent: RELOCATION_MATCH PASS (work/fun1.log); no self rename moves a funclet parent.
Grades: 2 per row (one q.py + the batch vland) except 0x00C783F0 (3); no failed form anywhere. selfcheck.log: 0 problems.

## What closed: the Translate / Commit family (split-free)
Every self rename below takes all its landed binders along (exe call-site scan, not only reloc_bindings):
- 0x00C74BB0 Set_HighPass, 0x00C74AB0 Set_LpButterworth2 (CFloatFilterParams.h): only binder 0x00C783F0.
- 0x00C783F0 Translate (NF99's held draft): binders 0x00C6AEF0 (this hand-back) and 0x00C6B050 (already right).
- 0x00C6AEF0 CFloatReverbEngine::Commit on engine/CFloatReverbEngine.h: no cast (the parameter block is the header's m_RvbParams; the
  body is the landed Create 0x00C6B050's Translate / slot-1 Commit / decay tail). Binders 0x00C54510 and 0x00C54190.
- The translator's private callees: 0x00C78150 UpdateSize, 0x00C77FC0 UpdateEchoTime, 0x00C78040 UpdateModulationDepth, 0x00C77AE0
  UpdateReflectionsOutputGain, 0x00C77C00 UpdateReverbOutputGain, 0x00C783B0 UpdateReverbPan, the private static 0x00C77D90 PanToGains
  and the free 0x00C77700 mBToLinear: each self row ported, and its only binders (0x00C783F0; 0x00C783B0 for PanToGains) follow.
  These heal the existing splits (Stage_xxx@CEffectPart260 vs CReverbProcessor::UpdateX; the YAXPBMPAMKK / YAXPAM0JJ pair at 0x00C77D90).
- 0x00C780C0 UpdateDiffusion, 0x00C77C80 UpdateReverbDelay, 0x00C77B30 UpdateReflectionsDelay are not landed: 0x00C783F0 binds the SDK
  spelling, the catalog / label fix is in NAME_FIXES.
- Data: 0x012BF138 (only binder 0x00C78150) -> `_sc_fDefaultLongReverbInputDelay`; 0x012BF088 (binders 0x00C75CE0, 0x00C75E90) ->
  `_sc_fDefaultShortReverbCrossfeedGainMatrix`, both declared `extern "C" const float [..][4]` (the SDK symbols are undecorated).
- 0x012B89C4: 0x00C54510's format local is now the header CReverbConstants, so it binds ??_7CReverbConstants@@6B@. That address was
  already split three ways (0x00C53AE0 CVtable_012b89c4, 0x00C6A780 CPlain28_00c6a780, 0x00C6AE90 CTimerSlot_00c6ae90); this moves one
  binder to the SDK name and creates no new split. 0x00C6B050 (Create) was split (self Create@CFloatReverbEngine vs 0x00C54510's
  Initialise@CVoiceEffect) and heals.

## Byte identity (SDK 2.10 CODAApi.lib release member vs retail oracle, relocations masked; work/tools/sdkcmp.py, sdkfind.py)
Identical: 0x00C74BB0 (293), 0x00C74AB0 (256), 0x00C783F0 (405), 0x00C6AEF0 (92), 0x00C77D90 (557), 0x00C783B0 (57), 0x00C77700 (109),
0x00C77C00 (125), 0x00C77FC0 (72), 0x00C78040 (116), 0x00C78150 (561); and the blocked family's 0x00C69C60 (837), 0x00C69FB0 (197),
0x00C6A080 (252), 0x00C6A200 (128), 0x00C6A180 (128), 0x00C6A5D0 (125: identical, contrary to NF99's "123, another SDK build"),
0x00C75CE0 (432), 0x00C75E90 (221), 0x00C89750 (255), 0x00C89AA0 (147), 0x00C89B60 (125), 0x00C89B40 (32), plus the NAME_FIXES
findings. Not identical: 0x00C77AE0 UpdateReflectionsOutputGain (SDK 76 bytes scales by 0.25f, retail 72 doubles; same shape, name
from Translate's REL32 +0x153).

## Blocked family (q11415, q11492, q11493, q11512..q11515, q11518..q11522): 13 entries, one family
Starting from the eight owners, every split-free closure (no new split) reaches the whole software-reverb engine:
- 0x00C75CE0 / 0x00C75E90 have one binder, 0x00C6A180; 0x00C6A180 has one binder, 0x00C6A5D0 (ProcessReverb); 0x00C6A5D0 binds the
  four engine owners 0x00C69C60 / 0x00C69FB0 / 0x00C6A080 / 0x00C6A200. So the short-reverb owners cannot close without the engine.
- The engine owners bind callees whose self rows and other binders agree on the old spellings today (new splits unless they move):
  - 0x00C69C60 -> 0x00C744E0 (CFloatFilter::Clear; self + 7 landed binders on ?Reset@CDspGain@@QAEXXZ: 0x00C69B60, 0x00C69C60,
    0x00C6A460, 0x00C75670, 0x00C75CA0, 0x00C89110, 0x00C896C0);
  - 0x00C69FB0 -> 0x00C75FF0 (CFloatReflection::Process) -> 0x00C76680 (CReverbFns::AddSamples; other binder 0x00C891A0) and 0x00C764E0
    (CReverbFns::ScaleSamples; other binder 0x00C76B60);
  - 0x00C6A080 -> 0x00C75140 (CFloatDiffuser::Process, 700 bytes);
  - 0x00C89750 / 0x00C89B60 -> 0x00C76760 (CReverbFns::ScaleAddSamples).
  (0x00C74CE0, 0x00C74D30, 0x00C76460, 0x00C763E0, 0x00C770B0, 0x00C76E90, 0x00C76C20, 0x00C76B60, 0x00C75A20, 0x00C75AB0 and 0x0143CCDC
  are already split, so binding them under SDK names leaves an existing split.)
- Minimum split-free set, about 27 rows: 12 renames (the 8 owners, 0x00C75E90, 0x00C75CE0, 0x00C6A180, 0x00C6A5D0), 6 callee renames
  (0x00C744E0, 0x00C75FF0, 0x00C76680, 0x00C764E0, 0x00C75140, 0x00C76760) and about 9 binder-only respells holding the header class in
  their existing local model (0x00C69B60, 0x00C6A460, 0x00C75670, 0x00C75CA0, 0x00C89110, 0x00C896C0, 0x00C891A0, 0x00C76B60; the 7
  0x00C744E0 binders are the bulk). Headers needed: CFloatCyclicBuffer accessors (SDK GetLength / GetWriteCursor / SetWriteCursor /
  AddWriteCursor), CFloatFilter (APPLY_PASS23 already declares Clear / Process), CFloatReflection, CFloatDiffuser, CFloatEcho,
  CFloatPitch, CFloatLongReverb Process overloads, CReverbFns ring wrappers (declared). That exceeds the 40-grade family budget for
  one hand-back, so nothing of it is in rows/; NF99's five drafts stay in work/held_drafts/ (0x00C75CE0 updated for q11516). All rows
  are unlocked by anyone else and none is in UNMATCHED_ROWS / COORDINATION_CLAIMS. Suggested split: one agent takes the 0x00C744E0 /
  CReverbFns leaves first, then the owners.
- q11517 (0x0143CCDC m_pDSP) is separate: a closure block (not attempted, 0 grades), see APPLIED.

## Order of work
- The slice held 30 entries and 29 rows.txt rows (the brief said 11 and 16). Entries were not taken strictly in queue order: the
  owner closure showed the short-reverb owners cannot close without the whole engine, so the closed Translate / Commit family
  (q11477, q11498, q11499..q11511, q11414) went first, then q11516; the family entries were left blocked without grades.
- Tests: every changed row's test is unchanged (self-contained copies of the body under the old local names; they compile without the
  row and still PASS).

## Judgment calls (for the coordinator)
- 0x00C54510 and 0x00C54190 hand the +0x290 block to Commit as `reinterpret_cast<CHLRvbParams*>(&p)`: a new cast in each row, in rows
  that already reach the same block through sixteen `reinterpret_cast<...&>(p)` setter calls. Typing the block as the header
  CHLRvbParams would move the sixteen setter calls onto CHLRvbParams members and split sixteen agreed addresses; the setters are
  byte-identical SDK CHLRvbParams members (NAME_FIXES, 16 lines) and make a clean 18-row follow-up that removes every cast.
- CReverbConstants.h opt-in is an `#ifdef ... #else <original declaration> #endif` (inline ctor + dtor in place of the out-of-line dtor
  declaration); the default path is token-identical (md5 checked).
- Data symbols declared `extern "C"` to bind the SDK's undecorated `_sc_f...` names.

## Descriptive names
- Parameter and local names in the ported bodies are the landed rows' (attenuation / reference / sampleRate, cutoff, room / scale /
  segment / lineBase / extent / previous, level / mix, gain), not CodeView locals (none read). RoundHalfDownToLong and
  FloatToLongTruncate are the landed descriptive inline helpers (no symbol in retail), unchanged. Class, member, function and data
  names are SDK CodeView / symbol-table spellings.

## Rule slips
- None known. One tooling note: my first old-symbol extractor regex missed objdump's `(nx 0)` symbols (no grade spent).
