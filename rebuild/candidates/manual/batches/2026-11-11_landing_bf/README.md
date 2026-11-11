# 2026-11-11 landing bf -- agentNF7: 132 landed rows moved off private stand-ins onto the shared headers

Source: `naming/2026-11-10_agentNF7/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 132 RELOCATION_MATCH,
tests PASS (six print their own catalog PassPattern -- PARITY_OK_ / BEHAVIOR_OK / OK_<addr> / TEST_PASS_SETBOASTASFAILED -- which
the recheck harness reports as FAIL; each was run and prints its pattern).
- CQuestManager PClass (0x013B89FC): 71 rows now reach it through the shared CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get();
  1 left (0x00892D80). Self row 0x01228CA0 (was Release_013b89fc) is the `dynamic atexit destructor for
  'CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass'` (Ego_r name set + body; VC7.1 local symbol `_$E2`), rebuilt from the static
  member's definition, renamed.
- GetDef<CThingBaseDef> 12, CIntelligentPointer family 30 rows (vtables 8 / 5, GetPItem 18, SetPItem 2), CVectorMap::LowerBound 16 rows,
  ~CCombatActionBase 3, SetCurrentAction 3, AIAction_MoveToThingProximity 1 (0x0097EF60, now protected as in the PDBs).
Header: engine/CTBaseSingletonNoDefaultAlloc.h opt-in FABLE_RETAIL_SINGLETON_SET (row 0x004A6550), hdrcheck2 default=same.
reloc_bindings.tsv recomputed for the 132 rows (rebind.py); bindings_multi_check at baseline.
Left (STATUS.tsv / NOTES.md): rows that model CDefinitionManager / CDefClassBase / CDefPointer / CTCBase / CThing locally need per-row
rewrites -- GetDef ~50, GetSubDef<CAppearanceDef> 38, GetDefinitionManager / PeekDefinitionManager 95 (PDB returns
const CGameDefinitionManager&), the rest of CIntelligentPointer (needs a complete CBaseObject-derived CThing), LowerBound, ~CCombatActionBase,
SetCurrentAction; NullChar, the CCharString functions, AddTC and the AI state-group targets were not reached.
