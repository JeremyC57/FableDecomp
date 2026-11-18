# 2026-11-18 landing p24: Codex pass 24 hand-back (28 rows, 21,463 bytes)

Owner: "Codex did as much as it could. The single handoff ZIP is at this drive link. It covers the 28 matches and the 72 parked
rows". Package: the 100 parked Ego_d near misses (`../../pending/2026-11-18_codex_pass24/`). SHA256SUMS: all files verify; the
returned row set equals the reserved one.

## Headers
Codex returned 135 headers (full snapshots + `hdr/PATCHES.json`), based on the package's include tree. Merged three-way against the
package base (`header_merge.log`): 106 files unchanged here since the base take Codex's text; 28 are new; one, CTCPhysicsBase.h
(changed here by landing egod2), merges cleanly. Gates: no conflict markers, `assert_names_check` 0 mismatches, and whole-tree
equivalence over all 56,640 landed rows (`equivalence.tsv`): only the five known `__FILE__` rows and two tests whose `fixtureAbort(__LINE__)`
code changes differ (the 1,355 `cpp-error-new` lines are the funclet rows, whose `../src/compiled` include is relative to the
include folder and so does not resolve from a scratch copy of it).

## Rows
`payload.json` (sources and tests exactly as returned; module / name from each `// Leaf:`), oracle from Fable.exe in scratch.
- First dry run with the merged headers as an overlay: 26 PASS, 2 SRC_FAIL (`logs/vland_dry_overlay_first_try.log`) -- 0x006BD900 and
  0x006DDD30 include fable_string.h, which Codex changed; with an overlay both copies are included (`#pragma once` sees two files).
  With the merged headers installed: 28 RELOCATION_MATCH + PASS, strict headers (`logs/vland_dry.log`); landed the same (`logs/vland_land.log`).
- 16 warnings (`logs/warnings.txt`): 12 RELOC_NAME (catalog labels the landed spellings correct, e.g. 0x0129BA3C is
  DELTA_NOTIONAL_ZERO, 0x0051F010 CThingManager::DoesNPCNameEntryExistForThing), 3 RELOC_ALIAS (import thunks); no RELOC_IDENTITY / CONFLICT.
- Purity: no `__asm` / `naked` / `_emit` / `register` in any row or test. Funclets 1,355 / 0 broken; bindings_multi_check at baseline.
- `wchar_t` in the new fable_string.h opt-ins follows the header's existing CWideString spelling (retail leaves such as
  ?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z mangle native wchar_t).

## Not landed
72 rows parked with Codex's closest source, test, PROBES.md and NAMING_EVIDENCE.md in `../../pending/2026-11-18_codex_pass24_open/<addr>/`
(UNMATCHED_ROWS.tsv points at them; its PARKED.md orders them by remaining instruction hunks, closest first: 0x0066A9E0, 0x00AE3C30,
0x00B0D360, 0x006E89B0, 0x00C52C40, 0x00F3ABF0). The reservation is released (`RELEASED.tsv`); Codex's top-level files are in `return/`.
22 name fixes are queued (`NAME_FIXES_QUEUE.tsv`, CODEXP24).

## Environment note
After the session's machine was replaced, Wine 9.0's wineserver aborted at start (`free(): invalid pointer`) because `/run/user/0`
did not exist; `mkdir -p /run/user/0 && chmod 700 /run/user/0` fixed it.
