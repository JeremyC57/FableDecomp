# Plan phase 4 (2026-11-05): header migration, five waves, 1,335 rows

This README covers the five `2026-11-05_plan_p4_*` directories. Each holds that wave's PASS, DEFERRED and NOTES, plus its
coordinator regrade in `logs/`. Every changed row passes gate.py and vland `--recheck-landed`, with the same parity as its landed
source and no new warning.

| Wave | Directory | Rows | vland |
|---|---|---|---|
| w1, C3DVector leftovers | `c3dvector_rest` | 597 | 91 MATCH + 506 RELOCATION_MATCH |
| w1m, local C3DVector method names onto the header's | `c3dvector_methods` | 177 | 13 + 164 |
| w2, CVectorMap leftovers | `cvectormap_rest` | 263 | 7 + 256 |
| w3, rows with two local value classes, from their landed source | `multiclass` | 265 | 4 + 261 |
| w4, CScriptThing | `cscriptthing` | 33 | 33 RELOCATION_MATCH |

## Notes by wave
- **w1:**
  - All 530 READY_PHASE4 rows from phase 3 (7 of them went to w1m).
  - 59 rows drop a local C2DCoordI, which the header already brings in.
  - 10 rows drop a local free `operator-` / `operator+` whose body is the header's member-wise computation.
- **w1m:** local `GetLengthSquared` / `GetLength` / `Length` / `MagnitudeSquared` become the header's `GetSquaredMagnitude` /
  `GetMagnitude` / `GetSquaredMagnitude2D`.
  - These are FableWin.pdb names (0x01907260, 0x01B52910) and the PDB-derived header's methods.
  - Each local body is the same sum of squares, in a different source operand order (`MAPPED.tsv`). The compiled bytes are
    identical on every row, so the coordinator accepted it as the same computation.
  - The methods are inline and emit no symbol.
  - Normalise mapped on no row.
- **w2:** the new `FABLE_RETAIL_STLP_BITSET_INDEX` in `stlport/fable_containers.h`.
  - **What it models:** STLport 4.6 `bitset::reference` and both `operator[]`s, inline.
  - **Why inline:** retail tests the bit inline with no call:
    - 0x0062D032: `test dword ptr [ebx+0x24],0x10000` (CWorld::EACreatureBlock, UsingComponents +0x20, bit 0x30);
    - 0x0044BFA1: `mov al,byte ptr [ecx+0x38]; test al,al; jns` (bit 0xC7).
  - **The default build is unchanged:** every addition is inside `#ifdef`, and 56 rows define it.
  - **Other includers:** all 5,747 pass gate.py with the overlay (`logs/gate_includers.tsv`).
  - **Other fixes in the wave:**
    - 79 rows read `Data.end()` through the CArray base;
    - 30 pointer-pair maps read as begin() / end();
    - 26 local same-TU LowerBound bodies were removed.
- **w3:** all 189 phase-3 w1 multi-class rows and 76 of the 86 phase-3 w3 ones.
  - 0x004FEEC0 is deferred: its callee respell would add a 13th spelling for 0x00687540 `AddMessage`, against the stand-in
    consistency rule.
- **w4:** `engine/CScriptThing.h` is used as it is. It already declares `~CScriptThing` (0x004AA840) and the copy (0x004ABE90) out
  of line.
  - 33 rows that call both functions out of line were migrated.
  - **One respell:** in 0x00EEA7C0, `?IsAlive@CScriptThing@@QBE_NXZ` becomes `UBE_NXZ`, the spelling in both Ego_r and FableWin.
    - Queue line 4613 is closed.
    - The old binding is retired from `rebuild/oracles/reloc_bindings.tsv`.
  - **Left alone for now:**
    - 35 "scope" rows that pass but call only one of the two functions or neither. They go to phase 5.
    - The 65 inline-depth and 17 two-view rows.
    - The 121 CActiveEntityScript<T> `??_G` rows, which are still blocked by W29 D1: retail expands `~CActiveEntityScriptBase` and
      VC7.1 RTM calls it.
