# Landing nf305 (Account A, 2026-11-23): T1 "other" class / namespace, NF302's continuation (32 rows, 1 test, 19 table lines)

Hand-back: `pending/2026-11-23_nf305_handback/`; helpers under `naming_batch/agent_tools/nf305/` (new: mdiff.py sorts out the access /
const-only shape; callcensus.py + memacc.py find every exe call site of a target and the declaration each caller row uses). 78 verdicts
(entries 1-60 plus 206, 211, 214, 217, 218, 220, 230, 234, 237, 246, 259, 263-268, 274 pulled ahead under E2): 17 done-partial,
1 done-already, 27 skipped-port, 33 skipped-held, no rejects; no header or vtable change.

- Fixes in each row's own local model: access for AIAction_DoNothing, FindFreeGazePos, GetCamTools, ExtendData, InvalidateAll, Open,
  UpdateAbilitiesForStat / OnStatLevelChange, StopSound, Reset; `const` for ClipSphere, IsOpen, IsQuestActive, IsAbilityPotentiallyAvailable,
  IsActionListEmpty; the reference return both PDBs give for GetAnimFromName and GetCombatTypeDef. 14 of the 32 rows had no table line (found
  by the exe call-site census). Owner access self fixes (NF302 Q5) landing with their binders: 0x00918880 with 0x009189C0 / 0x00918900 /
  0x00918AC0 / 0x00918B50; 0x006D8170 and 0x006D83B0 with 0x006D8B50. 0x00892F40's test stub made const (it failed to link otherwise).
  0x00B8FF70 gains `friend class CEngineInternalPrimitiveBase;` (the real class; retail calls the private member from it: NF281 D1 / R1).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ with only the listed renames, COMDAT sets SAME, tables unchanged; one link of all 45
  objects HEAD v new: nothing HEAD resolved left unresolved, the same 4 equal-copy duplicates in both; no changed source is included by
  path anywhere. Staleness: all 33 orig/ copies equal HEAD at landing (after nf303 / nf304).
- Table: 16 deletes (the `??0CBaseObject@@QAE@XZ` line is a stale witness: 0x00EFC500's object binds the protected ctor, confirmed at landing),
  2 replaces (ClipSphere, StopSound), 1 rewitness: GetCamTools onto 0x008728C0 (the agent read the source only; compiled at landing,
  `logs/witnesses_head.log`).
- Rulings (standing brief): Q1 entry 15 is a port (the move would rebind the OnCreate call 0x005C0121 onto another wrong form; a binder may
  stay unresolved only on the PDB name). Q2 `cl /Zs` counts as the precheck of a CHANGED source only when the same clang error (outside the
  toolchain headers) also fires on its unchanged HEAD source, recorded in pre_*.log (entry 20, 0x00ED4830). Q3 native `wchar_t` is a
  compile-flag change: T3 (entry 35). Q4 the GetCamTools rewitness was compiled at landing. Q5 the friend lands.
- Queued: 27 T3 ports; NAME_FIXES_QUEUE: owners 0x0048DA30 (IsOpen), 0x00730C00 (Open), 0x00666D10 (GetCombatTypeDef); 0x009A01C0
  (ExtendData) has no landed row.

## Recheck
`logs/recheck.log` (landed tree, 32 rows + witnesses 0x008728C0 / 0x00EFC500): 34 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT; 0x008728C0 binds the public const GetCamTools, 0x00EFC500 the protected CBaseObject ctor.
