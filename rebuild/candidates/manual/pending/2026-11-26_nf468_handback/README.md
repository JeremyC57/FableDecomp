# NF468 hand-back (2026-11-26): lane-wide `/D_USE_ANSI_CPP` proof (owner approved, applied)

## Proof over all 57,160 landed rows
Each row was compiled as is and with the flag, then compared on the COFF header, every non-debug section, the full symbol table, the grade and the exact `.drectve`.

| verdict | rows |
|---|---:|
| IDENTICAL | 44,713 |
| DRECTVE_ONLY: exactly the old text minus `/DEFAULTLIB:"libcp"` | 12,441 |
| /Zi rows differing only in debug sections, as two plain compiles also do | 5 |
| does not compile at HEAD (0x00A7A5E0, an existing C2664) | 1 |

- **0 grade changes.** Every negative control was caught.
- **Drift:** 179 rows changed during the run, and 9 failed while the disk was full. All were recompiled and are clean.
- **Tests:** 655 rows were sampled through vland. Row compiles with the flag are identical. On the `/Od` test compile, the flag breaks 0x00CAE9B0's link (`new(std::nothrow)` is libcp-only), so **tests do not get the flag**.

## Applied (coordinator)
`libcp_flag.patch`:
- `verify_and_land.row_defines()` is used in `parity_of`, the single funnel for every lane row compile;
- the same rule goes in `build_candidates.ps1`'s row compile;
- both skip `/MD`, `/MDd`, `/MT` and `/MTd` rows.

A smoke test on 0x00401C00 gives RELOCATION_MATCH PASS, with `.drectve` now `LIBC OLDNAMES`. Docs: BUILDING.md, LIBRARY_SIGNATURES.md and GOTCHAS.md.

The Windows half has not been run; only the include trees were compared.

## Follow-ups
- 0x00A7A5E0 does not compile at HEAD and needs a repo fix.
- The CAIStateGroup units held on libcp (r473d: MinionWander, CreatureGoUnderground) can now be re-gated.
