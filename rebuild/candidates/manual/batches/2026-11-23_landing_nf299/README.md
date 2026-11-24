# Landing nf299 (Account A, 2026-11-23): T1 "other" made-up names, NF296's continuation (38 rows, 34 table lines)

Hand-back: `pending/2026-11-23_nf299_handback/`; helpers under `naming_batch/agent_tools/nf299/` (new: `drefs.py` finds where a donor
function takes a target's address, `acc.py` prints the access label in effect at a declaration line; `rec.py` / `mkpay.py` / `tblafter.py`
fixed to the brief's layouts). 46 of 835 entries reached: 11 done, 7 done (table only), 7 done-partial, 9 skipped-port, 12 skipped-held
(the NF297 / NF298 stay-off set, both since landed), no rejects; no header, flag, catalog, test or vtable change.

- Unit respells onto the PDB spellings (each in both PDB name sets plus a donor FableWin call, names only): `Init` -> private `Initialise`
  (5 binders), `CancelAbilities` -> `CancelMove` (also 0x0072BA40 / 0x0072B420, left bare by NF296's delete), `PushSourceDefIndex` ->
  `PushNewSourceDef`, `PlaySound` -> `PlaySoundW`, `GetVillage` -> `GetPVillage` const, `ThreadProc` -> private static `BeginThread` (the
  donor ctor takes its address), `IsMeleeWeapon` -> `IsProjectile` const (the old stand-in was 0x005DAA00's real name: one symbol on two
  addresses removed), `WriteBuffer` / `WriteData` -> `WriteCompressed` (6 rows), `ReleaseVolume` / `ReleaseResources` -> `Uninitialise(bool)`
  (7 rows), `DeallocateMemory` -> `Clear`, `PeekWorldMap` protected, `UnassignString` access (3 rows), `GetPWeapon` -> `GetPWeaponUsed`,
  `FindCombinationAnim` -> `GetFirstCombinationAnimFromType` (3 rows), `StopSample` -> `StopAttachedSound`; owner 0x0077CCA0 and binder
  0x0077D140 declare `DoStopRunning` protected (owner `// Leaf:` now the full mangled symbol).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ under explicit old -> new symbol maps; link: every made-up spelling HEAD left
  unresolved is gone, no new unresolved or duplicate. Staleness: all 38 orig/ copies equal HEAD at landing.
- Table: 27 deletes, 4 adds, 2 conditional rewitnesses (both confirmed by compiling the witness at landing: 0x006BA030 binds
  `?WriteData@CDataOutputStream@@QAEXPBXK@Z`; 0x0099BE70 defines its own public `?UnassignString@CWideString@@QAEXXZ` copy, queued T3), plus
  the coordinator's delete of the stale `?GFGetIME@@YAPAUCIMECalls@@XZ` line (witness 0x00851860 binds `?Get@CIME@NUISystem@@SIAAV12@XZ`;
  only a comment names GFGetIME): `logs/coordinator_table.tsv`, `logs/witnesses_conditional_head.log`.
- Queued: 11 T3 lines (9 skipped-port, 0x0077CDE0's `CTCHighlightItem_Methods` class port, the 0x0099BE70 / BF30 / BFF0 same-TU copies);
  NAME_FIXES_QUEUE: owner rows 0x00771620 (`PushNewSourceDef` bool return), 0x00A67460 (`Uninitialise`), 0x00A629D0 (`Clear`, ICF caveat),
  and the `ForwardToVolumeLevel` symbol defined at both 0x009FCD40 and 0x00A69550. `Uninitialise` / `Clear` stay unresolved until those owner
  rows are respelled (HEAD left the made-up spellings unresolved too).
- Rulings on NF299's questions (in the standing brief): the 0x00666EA0 `friend class CThingCreatureBase;` lands (it follows the `#undef`,
  so it names the real class, NF281 D1); a full mangled `// Leaf:` replacing a bare one lands when the recheck follows it; `virtual` on a
  local base model is allowed under R1 when the TU emits no table or RTTI for it and objsec is ALL_EQ; census-based skipped-port
  classification is allowed with a committed census script and a 1-in-10 open sample.

## Recheck
`logs/recheck.log` (landed tree, 38 rows + the 3 witnesses): 41 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
