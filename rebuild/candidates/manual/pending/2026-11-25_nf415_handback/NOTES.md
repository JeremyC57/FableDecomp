# NF415 hand-back (2026-11-25): _s_Empty / pooled wide literals, the widened baked-address scan, STRING_VAS.tsv and the purity patch

## Result
- **13 rows, 13 ACCEPT, 0 HOLD** in four batches; locks kept as NF415 (004fdab0 004a05c0 | 0041735a 00451e11 0045228f 0088f850 00891490
  00c3e290 | 00a66550 | 00692380 007ea2d0 009c3510 00a62980). Rows the scan found but NF415 did not edit are listed in HOLD_ROWS.tsv
  (never locked, never edited, so not in any RESULTS.tsv).
- **Headline finding:** NF413's "27 data casts" are **all vftable stores** (`self->vtbl = (void*)0x01230BA0;` and kin), not floats or
  tables. The widened scan also found **six baked strings that no detector saw** because they are bare integers, not casts
  (`ProfileEnter(0x122d70e)`, `CCharString2 s(0x122eea4, -1)`, `return 0x12b5b78;`): fixed in b2.

| batch | rows | what |
|---|---|---|
| b1 | 004fdab0, 004a05c0 (ONE unit) | task 1: file-static `s_Empty[] = { 0 }` -> `L""` (both rows), `s_HeroStateExt` -> `L".hs"` (004a05c0); 2 table deletes |
| b2 | 0041735a 00451e11 0045228f 0088f850 00891490 00c3e290 | baked strings passed as bare integers -> the literal, cast to the stand-in's existing `int` (no callee / self / table change) |
| b3 | 00a66550 | `*reinterpret_cast<const float*>(0x0122DEDC)` -> `return 0.0f;` (`__real@00000000`) |
| b4 | 00692380 007ea2d0 009c3510 00a62980 | baked .data / IAT VAs -> the table-bound symbol: `&ACTION_PAUSE_ON_THIS_FRAME`, `&TARGETING_CURSOR_POSITION_NAME`, `CloseHandle(...)`, `EnterCriticalSection(...)` |

Each batch: rows.txt, rows/, tests/, orig/rows + orig/tests (HEAD copies, byte-equal to the repo at hand-back), RESULTS.tsv, CHECKS.tsv,
UNITS.tsv, TABLE_CHANGES.tsv (no SELF_SYMBOLS.tsv: no self changes) (b1: 2 deletes; others header only), LITERALS.tsv, logs/,
evidence/comdat_<a>.txt; b4 also DATA.tsv (the data symbols each row now references / drops) and MUTANTS.tsv (its mutant controls).
head/ new/ mut/ hold payload.json + oracle.tsv: lane input, the oracle carries retail bytes, NOT for the repo.

## Task 1: _s_Empty (b1)
- Retail 0x0122D70C = `00 00`, preceded by `00 00` (the NUL + pad after the string ending at 0x0122D709), followed by the pooled narrow
  "" at 0x0122D70E: the pooled wide empty literal. Evidence: (a) vland's literal_check pairs the new `??_C@_11LOCGONAA@?$AA?$AA@`
  relocation with 0x0122D70C as equal at every site (gates `ok`); (b) NF233 already moved 0x0042EA8F's `_g_emptyWide` onto the same
  literal, and landed rows such as 0x00997F80, 0x004A00E0 and 0x00427216 already spell L"" against 0x0122D70C; (c) the codex pass-11 NAME_FIXES record ??_C@_11LOCGONAA@?$AA?$AA@ for
  0x0122d70c. L".hs" at 0x01236A74 = `2E 00 68 00 73 00 00 00`, NUL-preceded.
- Census (`grep -rnw` over rebuild/, tools/, docs/, refs/, ghidra_out/, uncut): `s_Empty` / `s_HeroStateExt` are defined and used ONLY
  in the two rows and in 004fdab0's HEAD test (replaced). No header (rebuild/include), oracle other than reloc_bindings, integration
  file or other test declares either: **no header declaration to remove.** Other mentions are history (old batches, MULTINAME indexes,
  the NF413 hand-back and T3 line 4022). Found, not touched: rebuild/runtime/openretailbank/bake/CBankFileManager_OpenIniFile_009a8170_bake.cpp
  declares `extern "C" fable_u8 g_FableOpenIniEmptyWide_0122D70C;` (pushed by its asm_bake oracle) and its _test.cpp defines it as L"":
  another model of 0x0122D70C, in an `__asm` bake (not a landed row, no table line).
- TABLE_CHANGES (b1): delete `_s_Empty 0122d70c 004fdab0` (live witness, plus 004a05c0 which uses the same symbol) and
  `_s_HeroStateExt 01236a74 004a05c0` (live witness). No stale-witness lines. apply_table --dry: {'delete': 2}, misses 0, multi 1 -> 1, new [].
  Land both rows together (UNITS.tsv: one unit 004fdab0): the table is symbol-keyed.
- Mutating L"" to L"x" changes no behaviour (`wcscmp(x, x)`, `compare(.., 0)`), so the new tests are linked and define wcscmp, the
  STLport compare, operator!= and operator+ under the row's spellings and inspect the pointed-to text. Mutant: all literals FAIL; a
  second mutant of L".hs" alone (logs/vland_mut2.log) also FAILs.

## Task 2: the widened scan (evidence/widened_HEAD*.tsv; tools/scan415.py, cls415.py)
- Scope: every landed row source and its test (113,883 files) + every rebuild/include header (3,007), comments and string / char
  literals stripped; every hex token 0x00400000..0x01FFFFFF (optional u/L suffix) that lands in a non-code section. 266 hits: 63 in rows,
  203 in tests, 0 in headers.
- Cast forms seen in rows: `(void*)0x..` (vftable stores, data args), `(void**)`, `(DWORD*)`, `(unsigned int*)`, `(DWORD)` in a macro,
  `*(T**)0x..` / `*(const CCharString*)0x..` / `*(unsigned long*)0x..` dereferences, `(CloseFn*)` / `(EnterFn*)` / `(FreeFn*)` IAT slot
  pointers, `reinterpret_cast<const float*>(0x..)`, bare integers as arguments / return values / member stores, `#define X 0x..`.
- Row hits by target: 39 vftables (HOLD, RULINGS q1), 6 strings (b2), 1 pooled float (b3), 4 table-bound .data / IAT symbols (b4),
  7 other .data / IAT targets with no exact spelling (HOLD_ROWS.tsv gives each reason), 4 false positives (0x01400000 is a dirty-flag
  constant in 0x00C41E40 / 0x00C41FB0).
- b3: VC7.1 RTM compiles `return 0.0f;` to `fld dword ptr [__real@00000000] ; ret` (/O1, /O2, /O2 /Op all checked), byte-identical to
  retail 0x00A66550 `D9 05 <0x0122DEDC> C3`; 0x0122DEDC sits in retail's float pool (3.0f, 1.0f, 0.0f, -1.0f at 0x0122DED4..E0).
  vland skips `__real@` symbols, so gates415 checks the COMDAT's bytes against its name and retail at the VA itself.
- b4 names: ACTION_PAUSE_ON_THIS_FRAME / TARGETING_CURSOR_POSITION_NAME (Ego_r.pdb + FableWin.pdb dynamic initializers; table lines;
  defined by the landed initializer rows 0x0121E900 / 0x0121E700); CloseHandle / EnterCriticalSection from Fable.exe's import directory
  (tools/iat415.py: slot 0x0143FE30 = KERNEL32!CloseHandle, 0x0143FE0C = KERNEL32!EnterCriticalSection; sanity: the operator new /
  delete / malloc / wcscmp thunks name their slots correctly) and the existing `__imp__` table lines; EnterCriticalSection comes from
  the shared engine/Win32Types.h. The HEAD pointer objects `?g_pfnEnter@@3QAP6GXPAX@ZA` / `_g_pCloseSlot` (holding the baked slot,
  no table line, no other user in rebuild/) are gone.
- Grade note (RULINGS q2): 00c3e290, 00a66550, 009c3510, 00a62980 go MATCH -> RELOCATION_MATCH: their only relocated field was the
  baked immediate (HEAD had no relocation there at all), so RELOCATION_MATCH is the honest grade of the fixed row.

## Gates (tools/gates415.py = NF413 gates.py + the checks below; runbatch.sh; all four batches re-run at the final HEAD)
- vland HEAD / new / mutant at one LANDVERIFY_WORK path per batch (lv/batches_<b>_one -> _h/_n/_m): 13/13 MATCH|RELOCATION_MATCH +
  PASS both sides; new logs 0 LITERAL_DIFF / LITERAL_UNCHECKED; literal_check on each new object returns every string VA equal.
  WARN sets HEAD v new identical for all 13 rows (no WARN added or removed; evidence/warn_diff.txt).
- Section compare (evidence/comdat_<a>.txt): every HEAD section pairs with a new one; the leaf is byte-equal once the fixed fields are
  masked; new sections only `??_C@` literals (b1, b2) or `__real@00000000` (b3, bytes == its name == retail); HEAD relocations to the
  static data symbols swap at the same offsets to `??_C@` (addend 0); the dropped HEAD data sections are accepted only as GONE-DATA (b1:
  every symbol table-bound to a LITERALS VA and byte-equal to retail and the new literal, padding zero; b4: a 4-byte pointer equal to the
  baked VA, listed in DATA.tsv); b4's added relocations go to the DATA.tsv symbol, table-bound to exactly the HEAD immediate.
- No new relocation targets a non-`??_C` symbol bound to a string VA (stray check; control: the HEAD b1 objects report `_s_Empty` /
  `_s_HeroStateExt`, evidence in b1/evidence/stray_control.txt).
- purity.baked_string_addresses and the widened scan: [] for every new row and test; no __asm / naked / _emit / volatile.
- Mutant control: every row's mutant (string first character / float 0.0f -> 0.5f / b4 MUTANTS.tsv: the other CCharString global,
  FlushFileBuffers, LeaveCriticalSection) FAILs its new test; b4 mutants also raise RELOC_CONFLICT.
- /FORCE link (link377.py; evidence/link_b<N>.txt): HEAD v new duplicates 0 -> 0 for every batch; unresolved HEAD = new for b1 (30/30),
  b2 (29/29), b3; b4 linked with the two initializer definers and VS2003 kernel32.lib: 7 -> 7, and `__imp__CloseHandle@4` /
  `__imp__EnterCriticalSection@4` resolve in KERNEL32.dll (without the import library they are the only NEW_UNRESOLVED, both table-bound:
  evidence/link_b4_nolib.txt). RESOLVED_TO_GONE lists only map size lines (`000000xxH`) and the dropped g_pfnEnter.
- apply_table --dry: b1 {'delete': 2} misses 0; b2-b4 {} misses 0 (re-run after HEAD moved at 04:45: still 0 misses).
- reserved_check (RESERVED_REFS="", so the second account's branch was not read; rerun with the default refs at landing): 0 / 0 mentions, all batches.
- HEAD drift: HEAD moved during the work (NF412's GetClassTypeName landing, 04:45: reloc_bindings / self_symbols + 92 forwarder rows);
  none of my rows, tests or headers changed, and every batch was re-run on the new HEAD (evidence/table_md5_at_final_runs.txt).

## Task 3: STRING_VAS.tsv and the purity patch
- STRING_VAS.tsv: 563 VAs (559 narrow, 4 wide) from the NF410 / NF411 / NF413 / NF414 hand-back LITERALS.tsv and NF415 b1 / b2.
  Verified for each: .rdata; bytes at the VA == the encoded text + terminator; start of string: preceding NUL (560) or 4-aligned after a
  non-text dword (3, all after floats: 0x01275988, 0x012780DC, 0x01288C48); no VA spelled both narrow and wide; 0 bad (tools/mkstrvas415.py).
  The empty literals (0x0122D70E "", 0x0122D70C L"") read as both widths from bytes alone; the width is the spelling's.
- purity_string_vas_patch.py (separate file, NOT applied): adds a second pass to baked_string_addresses that flags any hex token in
  code whose value is in rebuild/oracles/string_vas.tsv (FABLE_STRING_VAS overrides), keeping pass 1. The coordinator commits
  STRING_VAS.tsv as rebuild/oracles/string_vas.tsv. Run on a patched copy over HEAD (evidence/purity2_census.tsv, purity_patch.diff):
  **6 rows, exactly the b2 rows** (0041735a 00451e11 0045228f 0088f850 00891490 00c3e290), which pass 1 misses; after b2 lands: none.
  26 landed tests also hit (self-contained copies comparing integers; tests do not affect the grade; T3 hygiene line).
- **Order:** commit the patch and rebuild/oracles/string_vas.tsv WITH or AFTER b2: before b2 lands, the patched detector grades those
  six rows `baked_string` (the dashboard count would read 6). b1 is one unit (004fdab0 + 004a05c0); b3 and b4 rows are independent.
- Gaps a hex key cannot see: the data-symbol form (a table symbol standing in for a literal, as _s_Empty): evidence/table_string_symbols.tsv
  lists 88 reloc_bindings lines bound to string VAs (69 real, 10 vtable false positives, 8 short / unsure, 1 pooled ""); and 21 symbols
  bound to the pooled 0.0f 0x0122DEDC. Both are T3 lines.

## Names
- No new names. All symbols used are existing table spellings: ??_C / __real COMDATs, ?ACTION_PAUSE_ON_THIS_FRAME@@3VCCharString@@B,
  ?TARGETING_CURSOR_POSITION_NAME@@3VCCharString@@B, __imp__CloseHandle@4, __imp__EnterCriticalSection@4. Stand-in callees and selves
  (ProfileEnter, PrepShadow, CCharString2, CDeedReactionName, Str, T::Get, Inner::Query, CCriticalSectionLock_Methods) are unchanged and
  stay on their T3 lines; b2's `(int)"..."` casts exist only to keep those stand-ins' signatures and go with the ports.

## Tools (tools/)
pe415.py (PE reader), iat415.py (import directory), scan415.py (widened scan), cls415.py (classify targets), holds415.py, tablestr415.py,
mkstrvas415.py, purity2_census.py, stray415.py, gates415.py, mkpayload.py (NF413's + float / MUTANTS.tsv mutants), runbatch.sh,
linkgate415.sh, mkb1.py / mkb2.py (row edits); link377.py, nf387.py, r10cmp.py, objsec356.py, cmpcomdat.py copied from NF414.
Budget: no git, no `wineserver -k`, no repo writes (python -B; apply_table only --dry). One vland mutant run was outside the batch path
(lv/b1_mut2, control only). A VC7.1 ICE (C1001) on calling a declared-only explicit specialization from a test is worked around by
calling the undefined member template (same mangled name): GOTCHAS line suggested below.

## GOTCHAS one-liners (suggested)
- (NF415) VC7.1 RTM emits `fld dword ptr [__real@00000000]` for `return 0.0f;` (not fldz): a `fld [0x0122DEDC]` row is a float literal, and the 21 table symbols bound to 0x0122DEDC are literals modelled as data.
- (NF415) Baked strings also hide as bare integers (`f(0x122d70e)`, `return 0x12b5b78;`): no cast pattern sees them; key the detector on known string VAs (rebuild/oracles/string_vas.tsv).
- (NF415) A lane test that calls a declared-only explicit specialization (`template<> void C::F<T>(T&);`) ICEs VC7.1 (C1001, P2 main.c 148); declare only the member template and call it: the implicit instantiation names the same symbol the row defines.
