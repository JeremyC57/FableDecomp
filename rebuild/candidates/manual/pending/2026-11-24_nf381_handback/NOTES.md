# NF381: binder respells (NF380 q1 family + q3)

Agent NF381 for Account A. Work dir `$S/w22/agentNF381`. Nothing was written into the repo and no git command was run.

## Job 1: the 0x0044C0C0 family (q1) -> batches/b1, 78 rows ACCEPT

**Identity** (`evidence/identity_0044c0c0.txt`): 0x0044C0C0 is `??0CSubDefClassBase@@QAE@XZ`.
- Both PDB name sets hold it.
- Ego_r `definition_manager.obj` index 9 (29 B, the retail length) and FableWin `definition_manager.obj` both define it.
- The retail RTTI direct base of every derived def in the batch is CSubDefClassBase. `gen381.py` checks this per row.
- The catalog already labels it so.
- The real `??0CShoveHaPennyRegionDef@@QAE@XZ` lives in tc_tavern_game_shove_ha_penny.obj (Ego_r index 19) = retail **0x007D10F0** (landed self row).
  The exe scan finds **no retail caller and no reference** to 0x007D10F0 (`evidence/callers_exe.tsv`; /OPT:NOREF keeps it). So no landed row should
  bind it, and no table line moves there.

**Filter** (q1 is a text census of 125 addresses):
- Exe scan: 94 of them call 0x0044C0C0. The other 31 have no call or reference to 0x0044C0C0 in their retail body (the real class's vector /
  Transfer family, comments), so they cannot witness the line: skipped.
- Of the 94: 78 model the base as a local `class CShoveHaPennyRegionDef` (77 generated + 0x0044C503).
- The other 16 + the option S thin row 0x0044C1BB (owner 0x0045C5F6) were compiled HEAD-only (census c1, `evidence/census_c1*.txt`). **None binds the
  old spelling**:
  - 0044c57d 004d36fe 004e1049 004e2612 and 0044c1bb's owner already bind ??0CSubDefClassBase;
  - the rest bind made-up stand-ins (QUEUE p1, NAME_FIXES).
  - 0x0044C1BB: the owner does not bind the old spelling, so it is not edited.
- Stay-off: none of the 78 is in stayoff_nf378_b12, T3, COORDINATION_CLAIMS, acctB pending, Codex RESERVED, or NF382 / NF383 lists. 0044c57d
  (NF378 skipped-conflict, NF383) is not changed.

**Route:** the pilot's (0044c146). The local base class is renamed CShoveHaPennyRegionDef -> CSubDefClassBase (whole word). Two comment phrasings that called the old name "the
manifest's label" / "catalog-wide symbol" are corrected, and one `// Name fix (NF381)` line is added after line 1 (`tools/gen381.py`). The three
tests that carry a copy of the model (0044c343, 0044c503, 004d7a25) get the same respell (`tests/`; the vland "new" run used them).
- **Shared-header route probed and rejected** (one probe, `evidence/header_route_0044c317.txt`): `engine/CSubDefClassBase.h` brings the real
  CDefClassBase vtable chain. The derived vftable COMDAT then references the 25 real slots, and ??1 / ??_G / ??_7CDefPointeeBase COMDATs appear:
  DIFFER(36v18), never ALL_EQ. 22 landed rows already model CSubDefClassBase locally. No hdr/, no HEADERS.tsv. vland WARN
  SHARED_HEADER_AVAILABLE goes 69 -> 147 (the 78 new CSubDefClassBase local models; QUEUE p3). WARN RELOC_NAME goes 78 -> 0.

**Gates** (every row):
- vland: HEAD 78 RELOCATION_MATCH PASS / new 78 RELOCATION_MATCH PASS at one path (work/wd_r1h, wd_r1n).
- objsec: 78 ALL_EQ with exactly `renames=1` (??0CShoveHaPennyRegionDef -> ??0CSubDefClassBase), drectve BYTE_EQUAL.
- xr: HEAD and new both resolve the base call to 0x0044C0C0 (`evidence/xr_r1.txt`).
- Pick-any checkers: pk_all + pickany_vs_retail H = N for all 78 (`evidence/pkdiff.txt`), 0 reach flips.
- /FORCE /MAP link: 78 rows + owners 0x0044C0C0 and 0x007D10F0, HEAD v new. 0 duplicates, 0 unresolved, resolution unchanged. At HEAD the 78 calls
  resolved to the 0x007D10F0 row (the wrong body); after b1 they resolve to 0x0044C0C0.
- apply_table --dry: 0 misses, no new multi-target symbol.

**Census narrowing (ruling wanted):** the exe scan lists 150 landed rows that call 0x0044C0C0.
- The 78 changed rows and the 17 others that name the class were compiled.
- The remaining 56 callers were **not** compiled. None of their sources spells `CShoveHaPennyRegionDef`, and no includable header declares it
  (only `engine/_quarantine/CShoveHaPennyRegionDef.h`, which no landed row includes). So their objects cannot reference the old spelling.
- The 31 text hits that are not callers cannot witness a line at 0x0044C0C0, because the table pairs a symbol with the retail target of a leaf
  relocation.
- The coordinator may want those 56 compiled before the delete, if "census by compile" is read strictly.

**Table:** one delete, `??0CShoveHaPennyRegionDef@@QAE@XZ 0044c0c0` (witness 0044c301). The census shows no landed object binds it once b1 lands.
The right line `??0CSubDefClassBase@@QAE@XZ 0044c0c0` already exists (witness 0044c57d).
**Hand note:** if land_scripted holds any b1 row, rewitness the line to that held row instead of deleting it. land_scripted keeps the delete only
when 0044c301 lands, so check the held list.

## Job 2: q3 -> batches/b2, 1 row ACCEPT; 7 skipped (STATUS.tsv)
- **0042a2ca <- 00558770 (done).**
  - Evidence: FableWin CKeyRedefiner::Initialise calls `CCountedPointer<CText>::SetStaticCast<CComponent>`. The catalog gives 0x0042A2CA = CText instance and
    0x0042A2DB = CList instance. Ego_r and `engine/NUISystem_CKeyRedefiner.h` name the members `CText* PActionText / PKeyText` (+0x194 / +0x198).
  - Change: the row's local `CList` becomes `CText`, and the members and typedef follow (`tools/gen381b2.py`).
  - Gates:
    - vland: RELOCATION_MATCH PASS HEAD and new.
    - objsec: ALL_EQ with 5 renames, all by the rule `VCList@NUISystem@@ -> VCText@NUISystem@@`.
    - xr: the leaf calls 0x0042A2CA under the CText name.
    - pk: under the five renames, the new NEQ set is a subset of HEAD's with equal reach, 7 -> 6 NEQ
      (`evidence/pk_gate/H_00558770.pkall.renamed.tsv`, `reachflip_b2_renamed.txt`: 0 flips).
      - ReleaseData / ShareData NEQ now look up the CText instances 0x0042925C / 0x00429460, not the CList ones at 0x00429288 / 0x00429486.
      - The retail twins: ReleaseData is EQUAL; ShareData is equal except its one call, which goes to each type's own ReleaseData
        (`twins_b2.txt`). So these are the same residue.
      - ~CCountedPointer<CText> now has no address (it was NEQ).
      - The raw reachflip.py "flip" on SetStaticCast 0x0042A2CA (`reachflip_b2_raw.txt`) is the rename itself: HEAD had the same NEQ reach y
        under the CList name.
    - link (row + 0042a2ca 0042a2db 0041e6d3): 0 duplicates, 0 unresolved.
    - Table: delete the CList line at 0x0042A2CA. Census by compile: 0041e6d3, the only other caller, binds CText there and CList at 0x0042A2DB.
  - T3 nf311 names this binder in its reason column. Not reserved by the rule; flagged as QUEUE p4.
  - Stay-off for 00558770: not in otherstay (Codex RESERVED, NF378 / NF382 / NF380 q2 q5 q6 q9), b12, COORDINATION_CLAIMS, UNMATCHED_ROWS, or
    acctB pending (no `CKeyRedefiner` or address hit).
  - Descriptive-name debt left in the row (no recompile for it): the header comment still says "two list components", and the def members
    `HeaderListDef` / `ActionListDef` (+0x198 / +0x19c) are descriptive. No PDB def layout names them.
- Skipped:
  - 0042ded5: Account B.
  - 006314c0: unit with NF382's 00625350 self (QUEUE p2).
  - 006bab80 (x2): NF382 q6.
  - 00aeef40: T3 nf334, column 2.
  - 00826a00: NF382 self.
  - 00a70ff0: ruling unknown, NF382 q2 target.

## Counts, locks
- **Compiles:** about 183 object compiles plus their tests, over the ~150 budget because every binder needed a HEAD / new pair and the table delete
  needed a compile census:
  - r1 78 pairs;
  - r2 1 pair;
  - census c1 17;
  - owners / census c2 5;
  - probe 3.
- **Locks:** NF381 holds the 79 changed rows (b1 78 + 00558770) until landing, as NF380 did. No context lock was taken. Release them by writing
  `released` before land_scripted's reservation step, or let the coordinator do it.
- **Tests:** land_scripted copies rows/ only. Copy b1's `tests/` (3 files) by hand (apply_rows.py).
