# operator= naming pass (CCharString / CWideString), 2026-10-28

## Result
- `transform.py` applied to the 2,107 files in `FILES.txt` (1,114 sources, 992 tests, `rebuild/include/fable_string.h`).
  2,624 one-line edits. All byte-neutral; idempotent (a second run makes 0 edits).
- Affected rows: 1,461 with an edited file + 932 that only include `fable_string.h` (directly or via the 18 headers
  that include it) = 2,393 rows. Rechecked 2,414 rows (dry run `--recheck-landed`, never `--land`; `009cdf50` with `--qfe`):
  2,409 MATCH/RELOCATION_MATCH + PASS. The other 5 give the same grade and identical code bytes in a baseline run on the
  unchanged tree (`PREEXISTING.tsv`, `EXCLUDED.tsv`); the edited one of those, `00404720`, is left out of FILES.txt.
- Verifier: `vland_ov.py` = `vland.py` with INCLUDE pointed at `incl/` (a full copy of `rebuild/include` carrying the
  new `fable_string.h`: quoted includes inside repo headers would otherwise pick the old header). Payloads: `mkpayload.py`.
  Logs: `runs/*.log`; per-row results: `results.tsv`.
- `objdump -t` (all 2,414 objects, `symscan_after.txt`; sample `objdump_sample.txt`): the plain-global spellings emitted are
  now `??4CCharString@@QAEABV0@ABV0@@Z` (596 rows), `??4CWideString@@QAEABV0@ABV0@@Z` (137), `??4CCharString@@QAEABV0@PBD@Z` (95).
  Old spellings left: `??4CCharString@@QAEAAV0@ABV0@@Z` in 00404720 only; `...PBG@Z` in 12 wchar rows (flag-limited).

## Overloads (PDB spellings, MAP.tsv)
| retail | PDB symbol | evidence |
|---|---|---|
| 0099efb0 | `??4CCharString@@QAEABV0@ABV0@@Z` | Ego_r 00afa810, FableWin 02f61830; refcount +0x0d |
| 0099efe0 | `??4CCharString@@QAEABV0@PBD@Z` | Ego_r 00afa840, FableWin 02f61860; alloc 0x11 |
| 0099f040 | `??4CCharString@@QAEABV0@D@Z` | Ego_r 00afa8c0, FableWin 02f61890; byte arg |
| 0099b7d0 | `??4CWideString@@QAEABV0@ABV0@@Z` | Ego_r 00b027e0, FableWin 02f65c60; refcount +0x0c |
| 0099b800 | `??4CWideString@@QAEABV0@PB_W@Z` | Ego_r 00b02ce0, FableWin 02f65c90 (+0x30 after ABV0 in both); `cmp word [text],0` |
| 0099b860 | `??4CWideString@@QAEABV0@_W@Z` | Ego_r 00b02d70, FableWin 02f65cc0; word arg |

## Hand-back
- `BINDINGS.tsv`: 4 reloc_bindings rows to respell (AAV0/PAV0 -> ABV0); every first_caller now emits the new spelling.
- `NAME_FIXES.tsv`: the six addresses with every wrong spelling bound today (placeholders such as `?Assign@CCharString@@QAEXABV1@@Z`,
  `@FableWideStringAssign_0099B7D0@12`, `?Load@CBank@@QAEXH@Z`, namespaced/struct models), the 0099efb0 row's own free
  stand-in `@CCharString_operator_assign@12`, and the wrong catalog/forge label `CCharString::AssignFromWide` at 0099b800.
- `BLOCKED.tsv`: 351 declarations not edited by rule: WCHAR_FLAG 258 (33 source rows: PB_W/_W unreachable without /Zc:wchar_t),
  STRUCT_KEY 58 + NAMESPACED 23 (28 source rows: need the model moved to `class` at global scope, which changes other mangled
  names in the TU), MACRO_ALIAS 5, ACCESS_private 4, UNKNOWN_PARAM 3 (all test-only).
- 0040d400's test edit must travel with the header edit (its test defines `CWideString::operator=` out of line).
