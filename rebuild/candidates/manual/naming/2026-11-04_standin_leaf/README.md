# STANDIN_LEAF warning in verify_and_land (2026-11-04)

## What it does
- vland now prints `WARN STANDIN_LEAF <graded symbol> (catalog <name>)` when a row matches, but the symbol it graded is a stand-in
  rather than the catalog function's name. Typical stand-ins:
  - a free function `?CEngineMap_PeekWaterHasType@@YI...` standing for `CEngineMap::PeekWaterHasType`;
  - a helper class `?Get@T@@...` holding the retail body.
- **It is non-blocking.** It changes no grade, parity or behaviour result, and no `LANDVERIFY_STRICT_*` mode reads it.
- **Code:** `tools/decomp_pipeline/standin_leaf.py`, plus a hook in `verify_and_land.py` after the relocation-identity check.

## Coordinator review fixes, on top of the plan agent's patch
1. **Missing import.** The module used `re` without importing it. The hook's bare `except Exception: pass` swallowed the NameError,
   so inside vland the warning would never have printed. The import is added, and the hook now prints
   `WARN STANDIN_CHECK_ERROR` instead of passing silently.
2. **C-name false positives.** Undecorated C symbols had every leading underscore stripped, while catalog names keep theirs, so
   `___onexit` vs `__onexit` and `__RTC_Initialize` were flagged. A C symbol now matches with one cdecl `_`, or its fastcall `@..@N`
   or stdcall `@N` decoration, removed.
3. **Dynamic initializers.** These are not stand-ins: the retail symbol is also a `$E<n>` name, and the catalog spells them
   `_static_init::...`. They are no longer flagged.

## Test
`test_vland.log`, a `--recheck-landed` run on 4 rows:
- 0x00401490, a known stand-in: warns.
- 0x00401296 `__onexit`, a former false positive: silent.
- Two honest rows: silent.
- All four grades are the same as before.

## Work list
- `STANDIN_LEAF.tsv`: **17,446 landed rows**. That is the agent's 18,153 rows minus the 707 cleared by fixes 2 and 3.
- This is the step-6 naming work list: each row gets its class, method and calling convention so that it grades under the catalog
  name.
- The warning prints on every graded row, `--recheck-landed` included, so the count is visible as it falls.

## Narrowed 2026-11-05 (owner rule: stand-ins are fine when consistent and accurate)
- **The warning now fires only for an inaccurate stand-in**, with a reason: generic placeholder, address-only name, names another
  function, or free function lacks its class. An accurate stand-in is silent:
  - a Lionhead-style `Class_Method` free function;
  - a C name that carries the method;
  - the real method on a stand-in class named after the real one;
  - any stand-in whose catalog name is itself a placeholder, unless the stand-in is generic or address-only.
- `std::` and other namespaces are no longer treated as a class.
- **Of the 17,446 rows in `STANDIN_LEAF.tsv`, 7,707 are accurate.** The other 9,739 are in `STANDIN_INACCURATE.tsv`, the step-6
  work list:

  | Reason | Rows |
  |---|---|
  | generic placeholder | 3,386 |
  | names another function | 2,974 |
  | address-only name | 2,745 |
  | free function lacks its class | 634 |

  - "Names another function" can mean either side is wrong: a folded body landed under a sibling's name, or a wrong catalog name.
    Each one needs evidence before it is respelled.
- **Test:** `test_vland_narrowed.log`, 5 rows.
  - 0x00401490 `CEngineMap_PeekWaterHasType` is now silent.
  - 0x00401377 `?Get@T@@` and 0x00403700 `?DestVal@Slot@@` warn as generic placeholders.
  - Grades are unchanged.
