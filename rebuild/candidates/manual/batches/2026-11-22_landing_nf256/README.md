# Landing nf256 (Account A, 2026-11-22): L28, L57, the last two CEngineSkyRenderer builders (4 rows); the GPLandscapeRenderer address proven

Agent NF256 (L28, L57, entry 3 done; L129 proven + planned; L37 not started). Hand-back: `pending/2026-11-22_nf256_handback/` (NOTES =
the L57 offset proofs, the L37 order and the L129 family plan).
- **L28 0x0049C490:** binder 0x0063E480 (CWorldMap::GetHeightZBelowPos) onto the shared headers: its local CWorld / CMainGameComponent /
  CThingSearchTools / CPhysicsMeshObject / CTCPhysicsBase / C2DBoxI / vector models and its same-TU strong stub (calling an undefined
  `CollectThingsInRoughArea`) are gone; 0x0049C490's define block + `ROUGHAREA_NOINLINE_PASS27` (as 0x0079A890) + CThingFilter_HasPhysicsMesh.h;
  2 forms (the filter a temporary in the call). 0x0049C490 2 -> 1; 0x00A12450 moves to the PDB reference form, 4 -> 4 (0x00875E60 still
  binds the pointer form). Its new pick-any GetAllThingsInRoughArea copy 583 = 583 EQ; the one pk.py NEQ is the LayerInfo relocation with
  addend 4 (retail 0x0137DADC = 0x0137DAD8 + 4), correct. cmpobj v 0x0049C490's HEAD object: 44 common COMDATs, all equal.
- **L57 0x00733EE0:** 0x00899DB0 -> `?IsPlayerCreatureFlourishEnabled@CGameScriptInterface@@UBE_NXZ` (both PDBs, slot 152) on
  CGameScriptInterface.h: IS_THING_ALIVE, HasTC(TCI_COMBAT), the non-const GetTC, `CTCCombat::IsMeleeFlourishValid`; offsets proven (Dead
  +0x91 bit 0, combat flag +0x24 bit 16, TCI_COMBAT = 48; its LowerBound copy EQ 0x0040F020). 0x00733EE0 2 -> 1. Test rewritten on the
  shared headers (it linked the stand-in name), PassPattern kept.
- **Sky builders:** 0x00B61CD0 / 0x00B61FF0 onto the header's private `?BuildBaseBandMesh@CEngineSkyRenderer@@AAE...` /
  `?BuildStarFieldVB@CEngineSkyRenderer@@AAEXXZ`, 2 -> 1 each; 0x00B61FF0's local CWideString / CAFile / CDiskFileWin32 models -> headers;
  its `??_7CDiskFileWin32@@6B@` v 0x0122D06C: 16 slots, 7 OK, 9 untabled, 0 conflicts.
- **L129 (0x01436EA8):** both PDBs put `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A` at GPResourceManager + 0x10 (Ego_r
  3:7bdf4 -> 3:7be04, FableWin 4:8807c -> 4:8808c); the table binds GPResourceManager / GPLightingManager / GPCamera / GPEngine at
  0x01436E98 / 9C / A0 / A4 in PDB order. Proven; the 91-referencer family is planned, not changed.
- Table: two stand-in lines deleted. Link (4 new + 3 HEAD objects): 0 LNK2005 / LNK4006 (control 1).
- **Recheck** (`logs/recheck.log`, landed tree): the 4 changed rows + 0x0049BE90, 0x0049C490, 0x00B620A0, 0x009D0380, 0x00733EE0 and the
  witnesses 0x0064BE45 / 0x00875E60 (11 rows): 11 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator ruling on NF256's question
- **D2** (`??1CAFile@@UAE@XZ`, 5 B pick-any in 0x00B61FF0 v table owner 0x0099A300 7 B): land as debt, as nf254 D1: landed 0x009D0380
  already defines the byte-identical copy at HEAD, so no new body enters the build; neither the leaf nor `~CDiskFileWin32` references it.
Rule slips (one control graded before its clang precheck; HEAD control compiles under q.py's inner 300 s timeout only): noted.
Open: L37 (order in NOTES; 0x00ADC310's catalog name is wrong: vector<unsigned long> + map<unsigned long,long>, not the
pair<CCharString, CArray> of its twin 0x009D3CA0), L129's family; 0x0049BE90's local CThingFilter_HasPhysicsMesh (HEAD debt).
