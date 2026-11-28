# 2026-11-28 landing: Codex pass 30

Source: Codex pass 30 hand-back zip (sha256 `39fd416b…`), checked on the Linux lane (VC7.1 RTM 3077).
Codex reported 53 fully verified rows, 43 with a matching primary body but emitted-copy / link holds, and 4 differing.

## What landed
- **45 rows** (`rows_landed.txt`), each RELOCATION_MATCH or MATCH with its test passing, purity genuine or retail_asm,
  names from a PDB, the GameCODA SDK libraries, or std/compiler spellings.
- **152 respell units** (`units_landed.txt`, batches `_units`, `_units_anon`, `_units_crc`), each regraded with its
  own `// Flags:` and `// Leaf:` and Codex's test. `_units/rows.txt` still lists the 28 units backed out below.
- **282 headers** under `rebuild/include` (220 changed, 62 new), applied with base and final hash checks against
  Codex's HEADER_SNAPSHOT. No asm in any header.

## Backed out (`backed_out.tsv`)
7 rows and 28 units (the GameCODA cluster and others), with closure in both directions: landing them would have
split names against landed rows that need a port first. After the back-out the naming tracker shows 0 new splits.

## Not landed: 55 rows (see `pending/2026-11-27_codex_pass30/RESERVED.tsv`, one reason per row)
- 4 primary bodies differ (DIFFER, incl. 0x00CBFB7D 74,776 v 74,778 B).
- 0x004199C0 sits inside an instruction; the real entry is 0x00419880 (the catalog address change needs the owner).
- 7 backed out (above).
- The rest are Codex's emitted-copy / link holds. Most are blocked by strong definitions in older landed rows
  (the R3 LNK2005 debt in EXTRA_RULES), 4 by weak copies that differ from retail (pick-any: 43 NEQ in 8 rows),
  the others by name or owner holds. They stay reserved for Codex.

## Checks
- Grades: 96 RELOCATION_MATCH + 4 DIFFER on the 100 rows, as Codex said. A regrade from the repo of the 197 staying
  rows and units plus the 6 changed-input rows is all MATCH / RELOCATION_MATCH with tests passing (`logs/recheck_from_repo.txt`).
- Header equivalence: 6 rows' preprocessed input changed (`logs/equivalence_differ.tsv`); each grades the same on both trees.
- No strong duplicate definitions in any landed row object. 46 vftables checked, none short.
- pk_all on the fully verified set: 004199C0 (held) and 0064B8B3 (one copy not leaf-bound, booked as debt).
- Coordinator checks: bindings 1 known (NullCoord), funclets 0 broken, s units 0 failed, descriptive names 0 FAIL.
- Seven owner approvals Codex claimed were put to the advisor (owner delegated): portability findings are header debt
  already present at HEAD (asserts, `_stricmp`), not blocking. 0x004014A0, 0x005D9B90, 0x005933C8 and the 3 particle
  units landed; 0x00CBFB7D held (DIFFER). 0x004014A0 carries `// Fidelity: scaffolding, not retail` (FableWin marks
  CCRC::Calc inline in main.obj and Engine_Water_Sea_Generator.obj).

## Catalog audit (no full rebuild)
57,384 entries, 0 missing sources, 0 duplicate addresses. 3 tests never print PASS (004DF3EB, 009FCCF0, 004DA5E2);
350 pass lines are not literal in their test; about 4,110 catalog names differ from the source leaf
(`logs/catalog_name_vs_leaf_audit.tsv`, crude, pre-existing naming work).
