# NF379 option S tool patches: test log (2026-11-24)

The diffs are against the repo's current files. They were tested in a scratch copy and never applied to the repo.

## Diffs

| file | patch | what |
|---|---|---|
| `a_land_scripted.diff` | (a) | UNITS.tsv `kind` column (found by header name) marks option S members. S members are never binders, so each must grade its own new symbol. They grade in their own vland call with `LANDVERIFY_STRICT_LEAF=1`. Non-S rows keep exactly today's call. Includers of every landed file (thin or fold, transitive) re-grade in a third call. A failing includer holds and restores the row it includes, and that row's unit. Writes `logs/includers.tsv`; `logs/recheck.log` concatenates the three calls. |
| `d_verify_and_land.diff` | (d) | `LANDVERIFY_STRICT_LEAF=1`, in `parity_of` only, so the grade step is the only one affected. The graded symbol must equal the leaf; an anonymous-namespace `...?A0x` leaf may be a prefix. Funclet slices are exempt. `LEAF_MISMATCH` skips the pragma / flag sweep. |
| `c_naming_status.diff` | (c) | `owner_source(src)`: a thin row resolves to the file it includes; any other row to itself. `includers_map(ent)` / `includers_of(src, ent)`: every landed row that includes `../src/compiled/<src>`, thin **or fold** (0x00BEB490), transitive. NF376's version missed fold rows. |
| `c_recheck.diff` | (c) | `recheck.py <out> --with-includers <addr / src>...` adds the includers. Without the flag the output is byte-identical to today. |
| `e_s_unit_check.diff` (new file, also `s_unit_check.py`) | (e) | `coordinator/s_unit_check.py [root] [--objdir DIR] [--quiet]`. Checks every S unit for: owner, flags / compiler, full leaves, and the explicit members' retail address order in the owner text. Compiler-emitted members (??_G, implicit or inline ??0 / ??1, templates) get a distinct `implicit` status; with `--objdir` they must be DEF symbols of the unit object. Handles nested scopes and operators. Exit 1 when any unit fails. |

`patch -p1 --dry-run` on a fresh copy of `tools/decomp_pipeline`: all five apply cleanly. Applied, each file is byte-equal to the tested file (`work/fresh`).

## Setup
- Scratch root `root/` holds real copies of `tools/`, `rebuild/src/compiled`, `rebuild/include`, `rebuild/oracles`, the catalog, and `candidates/manual/{COORDINATION_CLAIMS.tsv,naming/*.tsv}`. `tests` and `manifest` are read-only symlinks.
- `root_u/` runs the unpatched repo tools on the same `rebuild/`.
- `FABLE_ROOT` points at the copy, and `S` at a fake S (`fakeS/`, for locks and `land_` dirs).
- T0 proves the include path resolves to the scratch tree: a planted `int x = ;` in the scratch owner makes thin 0x00B80F90 grade `SRC_FAIL`.
- The repo was checked untouched (`find -newer marker`: no files) after the first pass, after the land simulation and at the end.
- Test states (`tools/state379.py`):
  - `pristine`: the repo text.
  - `pre_s`: NF376 b1 `orig/` over 0x00B80AD0 / F90 / FE0, and the pre-landing bindings (NF376's three table lines reverted).
  - `pre_nf373`: NF373 b2 `orig/` over 0x00736590 / 0x00736AD0.
- Each b80 run passes `--release-t3 nf376`: the repo's T3 line `nf376 0x00B80FE0 test retext` otherwise holds FE0.
- Runner: `tools/run_tests379.sh`. Log: `tests/run.log`. Batch outputs: `tests/<T>/batch/`.

## Results
| test | tool | input | result | expected |
|---|---|---|---|---|
| T1 positive S unit | patched | NF376 b1 (kind S), pre_s | `landed 3, s_members 3`, table `{'delete': 2, 'replace': 1} misses 0`, T3 nf376 closed; F90 / FE0 sources = run rows | lands |
| T2 NF376 negative (F90 RESULTS symbol wrong), kind S | patched | pre_s | `landed 0, held 3, recheck_failed 3`; F90 `landing recheck ('RELOCATION_MATCH','PASS') symbol ?GetWindVector...` (graded symbol != claimed); **sources restored**: sha1 of the three = pre_s (c643c86c / b24d98c2 / 74d28fe7); bindings untouched | held, restored |
| T2u same negative | unpatched | pre_s | `landed 3` (the hole NF376 found) | control |
| T3 two-column UNITS.tsv, negative | patched v unpatched (T3u) | pre_s | both `landed 3`; batch dirs identical (RESULTS, rows, held, recheck.log, T3_CLOSED, TABLE_CHANGES), bindings identical | as today |
| T3n `note` third column whose values are `S` | patched | pre_s | `landed 3`, batch identical to T3u: the kind is found by header name, never by position | as today |
| T4 owner-only run, planted store-order change in FE0's body (owner 0x00B80AD0) | patched | pristine | AD0 graded MATCH, but includers 0x00B80F90 RELOCATION_MATCH OK and 0x00B80FE0 `DIFFER(112v112)` FAIL, so AD0 is held (`includer 00b80fe0 regrade`) and **restored** (sha1 = pristine b2f93b91); `logs/includers.tsv` written | held |
| T4u same | unpatched | pristine | `landed 1` (planted text landed, sha1 04ff33a3) | control |
| T5 non-S unit (NF373 b2 unit 0x00736590 / 0x00736AD0, two-column UNITS, `--release-t3 nf289`) | patched v unpatched (T5u) | pre_nf373 | both `landed 2, t3 closed 1`; batch dirs byte-identical; bindings identical; summary JSON identical (no extra keys: no S member, no includer) | unaffected |
| T6 strict leaf, vland | patched | thin F90 with a wrong Leaf (QBE) | non-strict: `DIFFER(112v69)` after the full sweep (25 compile WARNs, 12 s); strict: `LEAF_MISMATCH(?GetWindVector...)` in 1 compile (2 s) | strict catches, no sweep |
| T6 strict leaf, `parity_of` unit test (`tests/T6_strict/parity_unit.txt`) | patched | F90 object | full UBE leaf: RELOCATION_MATCH both modes. Wrong QBE: DIFFER non-strict, LEAF_MISMATCH strict. Substring `Get`: DIFFER non-strict (it graded GetWindVector), LEAF_MISMATCH strict. Bare `GetGravityVector`: RELOCATION_MATCH non-strict, LEAF_MISMATCH strict (S members need full leaves). | |
| T7 s_unit_check | new | scratch root and the repo (read-only) | repo: `1 checked, 0 failed`, exit 0; with `--objdir` (T1 objects) OK. Negatives FAIL with exit 1: owner order swapped (`order 00b80ad0 00b80fe0 00b80f90`), F90 flags differ (`'/O2 /Oy /W3 /GS' != owner ''`), thin Leaf QBE with `--objdir` (`leaf not defined in the unit object`), bare Leaf (`not a full symbol`). NoiseDef unit: `implicit 0045c5f6 00455ef0 0045c5da (not verified)`, and with `--objdir` `(DEF ok)`. A unit with no object under `--objdir` is reported, not failed. | |
| T8 recheck.py | patched v unpatched | 4 args incl. a src path | no flag: payload.json, oracle.tsv and stdout byte-identical. `--with-includers 00b80ad0 00736590` adds 00b80f90 00b80fe0. `--with-includers 00beaaa0 00b260b0` adds 00b2ab10, 00beb490 (fold), 00beb4d0. | |
| L1 land simulation of NF379 b1 / b2 | patched | pristine + b2 CATALOG_EDITS, NF379 claim closed in the copy | b1 `landed 2, s_members 2`; b2 `landed 3, s_members 3, t3 closed 1` (nf309); s_unit_check after: 3 units OK, `--objdir` DEF checked; landed == run rows | lands |

L1's first try was held by `open claim`: NF379's own claim line, which is still open (`evidence/landsim_try1_claimheld.log`). The coordinator closes it before landing.

## Known limits
- **An includer that already fails at HEAD** (not MATCH / RELOCATION_MATCH + PASS) holds every landing that edits its owner. The tool does not compare against HEAD; the reason names the includer, so the coordinator can tell.
- **Summary JSON:** a non-S run that edits a file with includers (an EH-funclet parent) now re-grades those funclets. Its summary JSON gains `s_members` / `includers` keys, and the run costs one more vland call.
- **(b) build_candidates cache** was not done, as briefed (it needs a Windows build test).
- **Compile cost:** about 150 VC7.1 compile invocations for the tool tests (T0's and T6's sweeps took 50 of them).
