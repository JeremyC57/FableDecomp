# Landing nf311 (Account A, 2026-11-23): T1 "other" class / namespace / form, NF308's continuation (6 rows, 11 table lines)

Hand-back: `pending/2026-11-23_nf311_handback/` (`unreached.txt` lists the 48 entries not opened; `evidence/held_18/` the measured entry-18
unit); helpers under `naming_batch/agent_tools/nf311/` (the census now reads the full spellings from `reloc_bindings.tsv`: ENTRIES cuts them
at 160 characters). 164 verdicts: 2 done, 5 done-partial, 153 skipped-port (vtable stops 108 / 128 among them), 4 skipped-held, no rejects;
no header, test, flag or catalog change.

- Entry 20: owner 0x006D5050 + binder 0x006D5180, RemoveEmoteIcon protected (IAE; the owner's full Leaf replaces a prefix Leaf that had
  hidden its public QAE self). Entry 119: 0x00701990's local CReactionMatch base makes TransferBinaryIn `virtual` (its `__vftable` member
  becomes the implicit vptr); the own override QAE -> UAE (NF303 Q1). Entry 138: 0x00835AC0's local CThingPhysical makes
  DrawPreparePrimitivesForRendering `virtual` (pad 0x64 -> 0x60), the override `protected: virtual` (the PDB MAE). Entry 85: 0x00709B10 +
  0x00708A30, the local C3DAnimationInterpolator member model gets a `virtual` destructor (pad 0x14 -> 0x10).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ under the listed renames, COMDAT sets SAME, no `??_7` / `??_R` symbol; links: every
  reference HEAD resolved stays resolved, 0 duplicates (0x00835AC0's base call is unresolved before and after, after only on the PDB name:
  0x006A64D0 has no landed row). Staleness: all orig/ copies equal HEAD at landing (after nf310).
- Table: 6 deletes (3 with the rows; 3 stale witnesses compiled by the agent: entries 74, 116, 118), 2 rewitnesses (entry 85 onto 0x00BD9250
  and entry 35 onto 0x005F05E5, both compiled at landing), 1 replace (entry 136). Coordinator (Q4): `??1ScriptMethods@@UAE@XZ` and
  `??_7ScriptMethods@@6B@` move from the stale 0x008976F0 (its object binds none of its ScriptMethods lines) onto 0x00D408B0, which binds
  both (`logs/coordinator_table.tsv`, `logs/witnesses_head.log`).
- Rulings: Q1 entry 18 lands in the continuation now that NF310 has landed and released 0x009689C0 (re-derive line 73 against the landed
  row). Q2 the three local `struct char_traits` rows are one T3 unit. Q3 confirmed: 108 / 128 are ports. Q5 / Q6 NF299 Q3 extends to a member
  model (a `virtual` destructor) and to an access label the PDB gives the override, under the same conditions (no table / RTTI emitted,
  offsets kept, ALL_EQ). Q7 compiled at landing. Q8 the STLport V spellings stay unqueued until a second source exists.
- Queued: 153 T3 ports + the char_traits unit.

## Recheck
`logs/recheck.log` (landed tree, 6 rows + witnesses 0x00BD9250 / 0x00CDFF50 / 0x00D408B0 / 0x008976F0 / 0x005F05E5): WINS 11, 0 DIFFER, 0 RELOC_CONFLICT.
