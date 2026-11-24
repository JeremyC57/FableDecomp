# Landing nf344 b1 / b3 (Account A, 2026-11-24): /d2inl flag changes the NF340 audit called for

Agent NF344 measured the 29 /d2inl rows flagged in `naming/audits/2026-11-24_pk_all_nf340/SUMMARY.md`; 0x00D45060 and 0x00CE7670 were
skipped because NF343 holds them. Each row was built at its catalog flags, with /d2inl removed, and at 3-8 other values it has evidence for.
Both checkers ran on every build, and the NF340 gate compared each build against the catalog build. Hand-back:
`pending/2026-11-24_nf344_handback/` (NOTES.md, FLAG_CHANGES.tsv, evidence/); tools: `naming_batch/agent_tools/nf344/`.

**Catalog CompilerFlags changes landed (6 rows).** The rows' `// Flags:` comment lines follow; the line count is unchanged.

| row | old | new | why |
|---|---|---|---|
| 0x00B47BC0 | /d2inlT29 | /d2inlT39 | repairs the leaf-called 0x00B49DF0 `_M_insert_overflow<CManagerLocalLight*>` (pk_all 1/1 to 2/0) |
| 0x007677E0 (b3, R1) | /d2inlT59 | /d2inlT60 | repairs 0x007696E0 / 0x007698C0 (pk_all 14/8 to 16/6) |
| 0x00B2C8E0 | /d2inlS100 /d2inlT200 | none | leaf unchanged; the one changed unaddressed copy takes the 25-byte body found at 0x00412880, and the old 94-byte body occurs nowhere in Fable.exe |
| 0x00E4AA00 | /d2inlT200 | none | object identical without it |
| 0x00AFCC30, 0x00B03CA0 | /d2inlT1537 /d2inlS15 | /d2inlT1537 | S15 has no effect (object identical); their resize copy 0x00AFE910 stays NEQ debt |

**Landing.** Recheck on the landed tree with the new catalog flags: 6 / 6 RELOCATION_MATCH PASS (`logs/recheck.log` in each batch). No
leaf-called copy turned NEQ in any change.
- Three T3 lines reserve 0x00AFCC30 / 0x00B03CA0 / 0x00B2C8E0: the two `pickany_d2inlS15` debt lines and nf283's CDiskFileWin32 unit. The
  flag change does not fix that debt, so the lines were parked for the land_scripted run and restored verbatim afterwards.

**Rulings (coordinator):**
- **R1 (b3), accepted.** T60 passes the gate. The one unaddressed copy it changes, `__uninitialized_fill_n<AttackParticleAndEndPoints>`, is
  found in neither form in Fable.exe and is called only from 0x0076B760, a copy that was already NEQ.
- **D1 (b2: 0x0064B1AC, 0x00E4FA20), held.** Codex pass 27's RESERVED.tsv still lists both rows, and neither was among the 100 rows of that
  pass's landing. Dropping the flag changes nothing in either object, so holding costs nothing. The run dir is kept as
  `pending/2026-11-24_nf344_handback/held_b2`.

**Kept as they are:** the flag repairs a leaf-called copy in 0x005414A0, 0x006116E0, 0x007E4B60, 0x0090E5D0, 0x00B3FC40 and 0x0091F140,
and a non-leaf copy in 0x00BD9CE0 and 0x00BDA150. Nothing is gained in 0x006C6040 and 0x00803D60. No setting both kept the leaf and
repaired a copy in the other nine rows; their copies stay as per-copy debt (T3 nf340).
- 0x00B1F200's comment still says its twin carries /d2inlS15, which this landing made stale. Fix it with that row's next change.

**Leaf-vs-table census:** `naming/audits/2026-11-24_leaf_table_nf344/LEAF_TABLE_CONFLICTS.tsv` has 624 landed rows whose own leaf is bound
only at another address. Hints:
- 140 folded / twin bodies;
- 335 cases where two landed rows grade the same symbol (an identity claim; hold both as a unit);
- 143 where the bound address is a landed row with a different leaf (the table line is likely stale);
- 6 plain research.

None of these is settled by two sources, so all are queued as research.
