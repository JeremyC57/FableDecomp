# 2026-11-10 landing ay -- agentNF4: 68 stand-in binder rows moved onto the shared CIntelligentPointer / CParentDefClassBase models

Source: `naming/2026-11-10_agentNF4/` (ROWS_TODO.tsv = the 161 binder lines agentNF3 left; STATUS / NOTES / LESSONS / RENAMES).
68 rows re-authored (69 binder lines); vland --recheck-landed: 68 RELOCATION_MATCH, tests PASS (22 new self-contained tests).
- ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10: 27 rows (13 CIntelligentPointer<T> deleting dtors, action / deleting dtors, ~CTCDiceGame).
- ??0CBaseIntelligentPointer@@QAE@XZ 0x00A01B10: 5 rows (four std::_Construct rows in the STLport 4.6 spelling, 0x009207A0).
- ??0CParentDefClassBase@@QAE@XZ 0x00430370: all 26 (def ctors, map / multimap ctor rows under `optimize("s",on)`, Alloc rows).
- ??1CParentDefClassBase@@UAE@XZ 0x00430300: 11 of 12 (def dtors, deleting dtors).
33 renames with rerow.py (mostly catalog stand-ins -> the PDB `scalar deleting destructor`; real identity changes: 0x00456010
CCreatureGenerationFamilyDef::~CCreatureGenerationFamilyDef, 0x00459ABE CBrainDef::~CBrainDef, 0x00459B73
CPlayerInventoryDef::~CPlayerInventoryDef, five CCreatureAction_* dtors, std::_Construct). 0x009207A0 IsRunnable is protected (MAE)
and carries /d2inlS20 (ps1 CompilerFlags added).
Headers (opt-ins, base 423d4a070, hdrcheck2 default=same): CSpecialAbilitiesFireballSpellDef, CTrollBattleDef, CEnvironmentDef,
CEnvironmentThemeDaySetDef, CCreatureAction_PickupGameObject / _BriarRoseConcentrateInto / _ActivateQuest.
reloc_bindings.tsv fixed by hand: 39 stale spellings removed, 8 added, no conflict.
Left (STATUS.tsv, 92 binder lines in 72 rows): ambiguous action owners, state groups needing retail CThingCreatureBase / CThingObject
headers, std_Destval stand-ins, camera / AI temporaries, STL algorithm instances, invented CRetailTree / CAggregate rows, 0x00454B4C
(= ~CThingBaseDef, needs a CThingBaseDef.h opt-in). Header debt: CEnvironmentDef.h derives from CDefClassBase by default (retail type
data: CParentDefClassBase); CCardPositionsDef.h declares a virtual dtor retail def classes lack. Queued land1011ax: 10 name fixes.
