# Shared-header policy: report-only scan (Account B, 2026-11-20)

Nothing in the repo was changed. Scanner: `scan.py` (this folder), run on all 56,866 landed sources in `rebuild/src/compiled`.
The scanner works in four steps:
1. strip comments and string literals;
2. find every class or struct definition, with or without a base list;
3. keep those whose class name has a shared header in `rebuild/include/engine/` or `rebuild/include/libpk/`;
4. record each one's namespace and nesting.

## Headline numbers

| | rows |
|---|---|
| Landed rows that define at least one class that has a shared header | **28,080 (49%)** |
| ... already flagged by today's check (plain `class X {`, engine/ only) | ~22,955 |
| ... **newly flagged** by the widened check | **5,125** |
| of the new ones: a base-list class at global scope | 3,242 definitions |
| of the new ones: a base-list class inside a named namespace (mostly NScript, NUISystem) | 2,673 definitions |
| of the new ones: a `libpk/` header, which today's check never looks in | 381 definitions |

My first quick estimate (about 12,500 new rows) double-counted rows that today's check already flags for another class. The correct
figure is **5,125**.

## False positives: few

- **No row both includes the header and defines the class.** That would not compile (C2011), so every hit is a genuine local copy.
- **Samples are real models.** Six random new hits:
  - `class CTCTargeted : public CTCBase { ... members ... }`;
  - a full template copy of `CIntelligentPointer : public CBaseIntelligentPointer`;
  - `NUISystem::CTable : public CChangingStateComponent` with a padded layout;
  - three NScript level-script classes (`CQ_BanditCampScript : public CLevelScriptBase<...>` with nested entity classes).
- **Row-private namespaces are rare.** Only 12 definitions sit in namespaces like `N_00xxxxxx` or `EntityScriptBinding_...`.

## Classes behind the new flags (top; full list in `classes_new.tsv`, 1,024 classes)

| class | newly flagged rows | all rows with a local copy |
|---|---|---|
| CIntelligentPointer | 436 | 1,524 |
| CPersonalScript_EmptyScript | 121 | 152 |
| CBaseClassNonCopyable | 92 | 652 |
| NScript level scripts (CQ_MinionClifftopChaseScript, CQ_HobbeCaveScript, CQ_AmbushTradersScript, ...) | 50-73 each | |
| CSoundEventManager (libpk) | 69 | 73 |
| CArray | 67 | 577 |
| CTCBase | 52 | 1,135 |
| CSoundSystem (libpk) | 32 | 44 |
| CSoundManagerLH | 20 | 51 |

## What the change would and would not do

- **Already-landed rows are not blocked.** The check gates new landings and rechecks only, and a failure that is the same with a set's
  headers off already counts as pre-existing.
- **New rows must follow the rule.** No new local copy of a base-list class (CTCBase, CIntelligentPointer, NScript level scripts) or of
  a libpk class could land.
- **Strict includer regrades get noisier.** About 49% of landed rows would show HEADER_POLICY under strict (about 40% today), so those
  regrades would lean even more on the non-strict grade.
- **A second gap should be fixed in the same change.** The check reads only the repo's include folder. A set that adds a new shared
  header passes before landing and fails after: C3DExtentsI, CNavConnectorNode and C3DCoordI all did this today.

## Recommended next steps (each needs the owner's yes)

1. Widen the check:
   - the regex accepts a base list;
   - engine/ and libpk/ headers are both checked;
   - comments are stripped;
   - the set's overlay folder is read.
   Claim the tool change, and tell Account A first, because A's strict includer counts change.
2. Land the pending 0x009FDE80 re-port set first, or fix its three remaining `class CVolumeTexture : public CBaseClass {` rows inside it.
3. Treat the backlog as waves, by class, the way earlier phases did the C3DVector wave. Settle the two conflicting CSoundEventManager
   models (libpk/LHAudioEventPosition.h vs libpk/CSoundSystem.h) before any sound-family wave.

## Applied (owner's yes, 2026-11-20)
`tools/decomp_pipeline/verify_and_land.py` now calls `shared_header_redefinitions()`:
- it matches base-list definitions;
- it checks engine/ and libpk/;
- it strips comments and strings;
- it looks in FABLE_HDR_OVERLAY before rebuild/include, so a set that adds a new header is checked against it before landing;
- a row is exempt only through an actual `#include` of the header path.
The GENERIC_STRUCT check is unchanged.
Measured on the queued Account B sets: 4 rows newly fail strict and are being fixed inside their sets
(0x009FD9B0, 0x00A685E0, 0x00A699D0, 0x00A78800). Every other queued set passes.
