# Landing nf289 (Account A, 2026-11-23): T1 standin_alias, NF287's continuation (4 rows, 1 header opt-in, 13 table lines)

Hand-back: `pending/2026-11-23_nf289_handback/`; helpers (tri4.py stale-witness scan, zsrun.sh, pdbq.sh, objbind.py with NO_OBJ) under
`naming_batch/agent_tools/nf289/`. 65 of 175 ENTRIES reached (1-60 plus five pulled forward by the stale-witness scan); 110 go to NF291.

- **0x004166A8 CMainGameComponent::CreatePlayers:** new add-only opt-in `FABLE_RETAIL_CPLAYERMANAGER_FLAT_CREATEPLAYERS` in
  `engine/CPlayerManager.h`'s flat branch (one declaration, `?CreatePlayers@CPlayerManager@@QAEXXZ` 0x0044A530, both PDBs); equiv_check over
  the header's 2,112 includer rows: 0 differ, the planted-token control flags 493.
- **0x00644424:** onto the existing `FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR` opt-in (GetMultiplayerColour 0x00449B60, and on the
  same line GetPlayer 0x004498C0), with a `class CRGBColour;` forward declaration the opt-in's `::CRGBColour` needs (ruling Q2: accepted).
  GetMultiplayerColour still links unresolved as at HEAD: its owner row 0x00449B60 defines only a stand-in name (T3 self port).
- **0x00B92070** ClipBoundingBox 0x00B4CED0 and **0x009F35D0** GetWidth 0x00415BD0: the PDB members declared in the rows' own local models.
- Proof: line counts unchanged; objsec ALL_EQ under the intended renames (`.drectve` included); no tables; links: the stand-ins unresolved
  at HEAD now resolve, 0 duplicates. Tests unchanged.
- **Table-only (stale witnesses):** 0x00AC96E0, 0x0066B120, 0x004C7A80, 0x00CD3640, one line of 0x008F9CA0, and 0x00EE2DB0 (ruling Q1:
  the coordinator compiled witness 0x00EE3EE0's HEAD object, `logs/witness_00ee3ee0_head.log`; it binds
  `?PlayCutscene@CV_TravellingHeroesScript@NScript@@QAEXVCCharString@@@Z`, Ego_r's spelling, never the stand-in). 11 deleted, 2 added.
- 55 skipped-port (stand-in local classes, `_Retail` / `_Calls` types, self ports, Methods structs on CTCBase, flat-header opt-ins not yet
  written, a ctor port), queued T3.

## Recheck
`logs/recheck.log` (landed tree): 4 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
