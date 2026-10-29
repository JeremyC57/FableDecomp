# Naming pass wave 7 (2026-10-29): 308 rows / 313 files respelled

Decisions (DECISIONS_W7.tsv, 77 addresses, every spelling checked against both PDB name lists): 58 proven in both PDBs,
5 in FableWin only, 10 probable, 4 descriptive. Inputs: chunks 562-563 NAME_FIXES, the multi-name addresses and the
landed-row RELOC_CONFLICTs left after wave 6.

Multi-name addresses (no ICF fold; misnamed callers): 0x004378C0 CPlayerGui::UpdateMiniMapMarkers(bool) (24 callers),
0x013B8AB8 / 0x013B8AC0 CMovie::PEngine (CIEngine*) / CMovie::MovieTexture, 0x0070B8A0 / 0x0070BA00 STLport
`_List_base::clear` (CAnimSet, CDelayedAnimation), 0x00527910 `_Rb_tree::_M_erase`.

Conflicts settled: 0064e019 reaches 0x004FC180 through PeekWorldMap, so it calls the const GetRegion (binding of the
non-const one to 0x004FB490 kept); CTCBase 0x00686A70 GetWorld() const returning CWorld& / 0x00686A80 PeekWorld() const;
CIntelligentPointer<T>::operator= per pointee (0x004A9760 CThing, 0x0048BD20 CThingPlayerCreature, 0x0050F440
CThingAICreature, 0x00C16950 CThingCreatureBase probable); CTCFollowed 0x006D4D70 IsFollower / 0x006D4DB0 IsAIFollower.

Scripts (scripts/, composed by scripts/tools/compose7.py: hand scripts, then families on their file lists): q7a-q7e,
rg7, ip7 (queue fixes, minimap / region / CMovie rows, CTCBase accessors), fam_umm7 (22), fam_tcb7 (21), fam_ref7 (11),
fam_gpi7 (45, GetPItem typed wrappers), fam_ccp7 (7), fam_gmc7 (31, `const CPlayer& GetMainPlayer() const`), fam_cst7 (75,
PDB const on GetPItem / GetDefGlobalIndexFromName / GetMainPlayerNumber / GetRegionNumberMapIsIn), fam_ipo7 (24,
CIntelligentPointer operator= instances), fam_tcd7 (1). No header changes.

Bindings: 111 retired, 13 added (incl. 5 anonymous-namespace hash pairs); NO-BIND stand-ins not applied. Catalog names
corrected: 0x004378C0, 0x004374F0 CPlayerGui::IsOpen, 0x00661E80 CThingCreatureBase::IsInMelee, 0x00449700
CPlayerManager::GetMainPlayerNumber. RENAMES.tsv: 8 file-rename proposals, not applied. DEFERRED.tsv: 44 entries.
recheck.log: the coordinator's full recheck of the 308 rows after applying.
