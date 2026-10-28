# Hand chunk 538 (CWorld .. CIconText): 5 of 8 byte-exact (retry chunk)

All 8 rows were parked DIFFER within 8 bytes. 5 now grade RELOCATION_MATCH under RTM, and 3 stay parked. Every callee,
class and member in src/ and differ/ now uses its PDB spelling. Each external symbol of the 5 src objects was checked with
`objdump -t` against FableWin.pdb and Ego_r.pdb. The only symbols outside the PDB lists are:
- CRT symbols;
- STLport helpers (the PDBs use Dinkumware);
- the descriptive globals listed under Descriptive.

## What matched (5)

### Proven
- **CWorld**
  - 004a6550 `CWorld::PostInit(CDisplayEngine&)` builds the managers with progress strings and gives the world to the 3D engine.
    - Applying the PDB names fixed the 4-instruction eax/edx swap at SetWorld, which had survived about 20 forms. The names were `NProfileTimer::StartProfile`, `NProgressDisplay::DisplayProgress(text, float, bool, bool)`, `CDisplayEngine::Get3DEngine()` returning `CIEngine&`, and the reference-parameter CGameScriptInterface/CQuestManager ctors.
    - The quest manager must be one plain `g_pQuestManager.Reset(new CQuestManager(*this, *PScriptInfoManager, *PGameScriptInterface))`. The parked form spelled out `operator new` + placement new + a null check. Going through `.Data`/`.ptr` instead of the inline `operator*` still leaves 2 instructions.
- **CWorldMap**
  - 005064c0 `CWorldMap::PostRegionLoad(long)` matches with:
    - a same-TU body of `CThingSearchTools::GetAllThings<CThing, list<CThing*>, CThingFilter_HasTC>` (0x004ACD70);
    - CThing's real flag bitfields at +0x91, so the filter tests `IsIncludedInThingScans() && filter(*thing)`.
  - 005066e0 `CWorldMap::CWorldMap(...)` matches with a same-TU body of `vector<unsigned short>::resize` (0x00517900).
- **CThingManager**
  - 00521240 `CThingManager::LoadEntitiesBinary(...)` matches with:
    - same-TU bodies of `_Rb_tree<CCharString>::_M_find` (0x004B9370) and `vector<CThing*>::_M_insert_overflow` (0x00436AC0);
    - `CLevelLoader::GetLoadAllSectionName()` as an inline by-value return, so the temp is built in the return slot;
    - a permutation sweep of the opening stores: interval, version, level, hasProcess, ticks, CurrentSectionName = "".
- **NUISystem::CIconText**
  - 005424b0 `NUISystem::CIconText::~CIconText()` is the PDB name. The oracle/manifest label "CText::~CText" is wrong. It matches with:
    - `virtual __forceinline ~CEnginePrimitiveHandle() { Clear(); }`;
    - the inner-copy `CDefPointer::Release` form;
    - the CDefString to CCharString conversion as an inline by-value return.

## Parked for Fable 5.1 (3)
- 004a34f0 CWorld::AutoLoadRegionLeaving DIFFER(564v564): instructions identical; only two frame slots swap (retail heroState/fullPath share esp+8 and path takes +0xc, ours the reverse) | tried: PDB-named callees (StartFileScan, GetDebugSaveDir, protected Internal helpers, const operator=), the AutoLoadInternal 0x004A3730 inline (fixes heroState/path but swaps questStart/name), ~30 v34 scope/order/flag/helper forms | next: lane gap: stack-slot packing order (VC7.1 slot assignment for temporaries that do not overlap); try the AutoLoadInternal inline with the questStart/name declarations swapped
- 00502620 CWorldMap::LoadLevel DIFFER(561v559): retail map offset esi / this edi / new result ebp, ours ebp/esi/edi, and one `add` per Maps access through the `mapInfo` reference | tried: PDB names (*LevelWADBank, GetFileModifiedTimeAsSomeWackyWin32BigNumber, static fastcall GetThingFilenameFromLevelFilename) — these lost the register flag (568v559); retail ReleaseData form of the same-TU Reset; a `CMapInfo& mapInfo` for the first statement (brings the temp-destruction flag back into ebx as in retail, 561v559); const/pointer/levelName forms, index types, stamp via temp, ctor forms, no same-TU body | next: keep the mapInfo variant and look for the form that puts the map offset in esi before `this` (e.g. compute `map * sizeof(CMapInfo)` into a local first, or an inline CArray accessor taking `unsigned long`)
- 00542780 NUISystem::CIconText::AddIcon DIFFER(682v682): retail loads FrameWidth (+6) and FrameHeight (+8) back to back before the first fild/fstp; ours loads the height after storing the width | tried: ~50 v78/v78b forms (C2DVector ctor/assign/set, local int/long/ushort copies, placement new, frame GetWidth/GetHeight accessors) | next: lane gap: x87 load scheduling; try a `C2DCoordI` (or CGraphicInfo frame-size struct) copied by value then converted

## Skipped
None.

## Label corrections
- 005424b0 oracle/manifest "CText::~CText" -> `??1CIconText@NUISystem@@UAE@XZ`.
- 009d8240 "LoadingStage" / catalog `$E2` -> `NProfileTimer::StartProfile`.
- 00501150 "LoadLevel_ParseObjects" -> `CWorldMap::LoadLevelFromFiles`.
- 00999320 is `GetFileModifiedTimeAsSomeWackyWin32BigNumber` (returns `unsigned __int64` in edx:eax). It is not GetFileModifiedTime, which returns a 28-byte CDateAndTime.
- 0049c770 "PeekAllThings" -> `CThingSearchTools::PeekClassList`.
- 0099a2f0 "CBaseObject::CBaseObject" -> `CBaseClassNonCopyable::CBaseClassNonCopyable` (protected).
- 0099b800 "CCharString::AssignFromWide" -> `CWideString::operator=(const wchar_t*)`.
- 009a4ec0 "GetStaticAddr_013ca618" -> `GFGetSystemManager`.
- 004b9370 "std::map_FindNode" -> set<CCharString> `_Rb_tree::_M_find`.
- 0050be40 "DeleteViaVtable0" -> `CCountedPointer<CAFile>::DeleteData`. The body is folded and shared across types.
- CASuspendableProcess slot 1 is `RunSuspendableProcess`, not "ResumeTimeslice". Slot 2 is `SuspendSuspendableProcess`.
- 00515ad0 "std::vector<CMapInfo>::vector" -> `CArray<CMapInfo>::CArray(unsigned long)`; 00993b10 "CDataStream::~CDataStream" -> `CMemoryDataInputStream::~CMemoryDataInputStream` (vland RELOC_NAME).
- The full list is in NAME_FIXES.tsv (59 rows).

## Probable
- 00999760 is the `StartFileScan` overload with `wchar_t*` (landed name "CAFile_FindFirstFile").
- 0041a1a0 is `NResourceDirectoryNames::GetDebugSaveDir` (one label says "GetProfileDirectory").

## Descriptive
- The globals have no PDB data symbol, so their names are descriptive and match the landed names:
  - g_LoadTickInterval 0x01375714
  - g_CCharString_013bab08 ("XXX_ALL")
  - g_DefStringTable 0x013CA828
  - g_pDriveManager 0x013CAA28
  - g_bLoadFromAnyDirectory 0x013B8646
  - g_pSoundManager 0x013B8394
- The `CASoundManager` slot 46 name `RefreshCategory(long, float, float)` is descriptive. The sound library has no PDB symbols.
- `CIEngine` slots 0-6 are padding. Only slot 7 (`SetWorld`) is named from the PDB.

## Header debt
- vland reports SHARED_HEADER_AVAILABLE for CIEngine, CDisplayEngine (004a6550); CClass, CTCVillage, CThingSearchTools, CWorld, CWorldMap (005064c0); CRegion, CThingMapManager (005066e0); CDataInputStream, CASuspendableProcess, CLevelLoader (00521240); CIVCountedPointeeBase, CObserver (005424b0). These use local models because the shared headers are data-only (no vptr or methods) or use a non-retail layout. They are warnings only, and vland shows no RELOC_CONFLICT, RELOC_DUPLICATE or RELOC_IDENTITY.
- engine/CWorldMap.h, CDataInputStream.h, CMemoryDataInputStream.h and CASuspendableProcess.h are data-only (no vptr or methods). The exact-mangled members had to be local models.
- In the PDB, `CCharString::operator=` and `CWideString::operator=` return `const X&` (ABV0). About 900 landed files declare `CCharString& operator=` and 150 declare the CWideString one with a non-const return. This only matters where the symbol is UND, but it is a one-retail-address/two-names problem.

## Review at landing
- 004a6550: `g_pQuestManager` (0x013B89FC) is a `CCountedPointer<CQuestManager>` here, because retail calls the thiscall Reset 0x004A9A10 with `ecx = 0x013B89FC`. Landed models declare it `CQuestManager*` (Data is at +0, so the reads agree). The mangled data symbols differ (`3V?$CCountedPointer@...` vs `3PAVCQuestManager@@A`). Settle one spelling for the address.
- The STLport helper spellings in NAME_FIXES (0x004B9370, 0x00436AC0, 0x005149F0, 0x00517900, 0x00440990, 0x0043F360) come from our objects. They cannot be checked against the PDBs, which use Dinkumware.
