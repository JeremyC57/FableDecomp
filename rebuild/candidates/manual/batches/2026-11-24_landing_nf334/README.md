# Landing nf334 (Account A, 2026-11-24): T1 `other` tracker batch, 31 rows onto PDB spellings (44 table lines)

Hand-back: `pending/2026-11-24_nf334_handback/` (NOTES.md first); tools: `naming_batch/agent_tools/nf334/`.
- **Scope:** 329 free T1 `other` entries (callers respelled onto the PDB name).
- **Results:**

  | status | entries | notes |
  |---|---:|---|
  | done | 32 | |
  | done-partial | 6 | |
  | rejected | 1 | |
  | skipped-port | 150 | 79 by census under NF299 Q4, with a 1-in-10 sample opened by hand |
  | skipped-held | 136 | |
  | not reached | 4 | |

  31 rows changed. No header change.

## Proof
- **Agent, per row:**
  - HEAD / new RELOCATION_MATCH at one work path;
  - objsec ALL_EQ under the rename map, `.drectve`, DEF / UND and COMDAT sets equal;
  - link HEAD v new.
- **Coordinator:**
  - all 31 `orig/` copies equal the repo;
  - recheck on the landed tree: 31 / 31 RELOCATION_MATCH PASS (`logs/recheck.log`).
- **`ref_scan.py` over the 32 replaced old spellings** (`logs/ref_scan_REFS.tsv`):
  - 24 NONE;
  - 0x0042EC7C did not compile in the scan; compiled alone it does not reference its symbol;
  - the 4 TOO_MANY spellings were narrowed by exact-signature grep, and the candidates compiled: one (`??0CTCPhysicsStandard@@QAE@PAVCThing@@@Z`)
    is still referenced, by 0x004D297B;
  - 3 REFS: 0x00AF59A0 still binds `DestroyDecals`, and 0x0069A920 still binds `PopulateListFromData` / `PopulateSwitchList`.
- **Table change:** for those 4 spellings the agent's `replace` became an `add` of the PDB spelling (its changed witness) plus a
  `rewitness` of the old line to the row that still binds it. No row is stranded. The agent's file is kept as `TABLE_CHANGES_agent.tsv`.
  Applied: 28 replace, 8 delete, 4 add, 4 rewitness, 0 misses, no new multi-target symbol. The 3 rows still on old spellings are queued
  as T3 `nf334 ... self respell`.
- **Q3 (accepted):** 0x004B26B0 is one of NF328's pick-any `~CScriptThing` definers. On the landing object its copy is still selection 2
  and BYTES_EQUAL with retail, so it lands.

## Rulings and queue (coordinator)
- **D1 (33 entries, destructor family):** binders call `X_Destruct` / `??_G` row-stub stand-ins, and the shared headers are flat. This is
  a class port; no in-place form is sanctioned. Queued as T3 `nf334 ... port`.
- **D2 (entry 52):** 006a0c90 / 006a0cc0 / 006a0cf0 / 006dfce0 bind 0x006A0C50 as the ScriptedThingBase copy constructor. Whether that is
  a folded body (NF104) or wrong binders is identity research, after `label_trust.py`. Only the stale table line was fixed.
- **D3 (17 entries):** the binder calls a different function or a free stand-in at the target. Queued as T3 `nf334 ... identity research`;
  run `label_trust.py` first.
- **Q1 (entry 174):** a template-instance COMDAT renamed onto the PDB nested type changes the COMDAT set, so it is a port (NF300 Q2), as
  the agent rejected it.
- **Q2 (entry 42):** the owner's self fix comes first (NF302 Q5). It stays open in the tracker.
- **NAME_FIXES:** 0x00997F80 `?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ`, whose target is the T3 line nf285. Queued as `nf334`.
