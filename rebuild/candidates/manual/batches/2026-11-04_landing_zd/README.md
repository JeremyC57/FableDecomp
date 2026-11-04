# Landing 2026-11-04zd: chunk 767 (61 tiny groups-ledger rows), 51 landed

## Result
- **Landed 51 of 61** (`logs/land.log`): 8 MATCH and 43 RELOCATION_MATCH, all PASS. Each was graded read-only on the merged tree first
  (`logs/new2.log`).
  - The rows include:
    - 9 `vector deleting destructor` adjustor thunks;
    - 6 atexit destructor stubs (`no_text_entry`, `interpolated_theme`);
    - small members such as CMiniMapDisplay::RemoveAllMarkers/RemoveMarker, CString::AllocCopy, CDynamicOutputPin::QueryInterface,
      CEngineResourceManager::OnReleaseDefaultPoolResources and CGraphicInfoFrame::CFrameData::ResetDebugMemoryInUse.
  - Every row is plain VC7.1 C++, with no `__asm`, `naked`, `_emit`, `volatile`, helper or pragma. Most matched on the first grade.
- **Slot names proven from the vtables:**
  - 0x00B3AFE0 is slot 11 of 0x012A1B3C.
  - 0x009FD4D0 is slot 9 of 0x0129C89C.
  - 0x00CA96F0 is IPin slot 0 of 0x012BF760.
  - 0x007FD3C0 is OnInitialActivate; the catalog had vfunc_6.
- **Parked 10** (`pending/2026-11-04_linux_hand767/`):
  - **Seven rows whose bytes already reproduce but whose names cannot be proven:**
    - 0x00ADD5C0: CLHFile has no PDB symbols and nothing references it.
    - 0x00C11700, 0x00C14700, 0x00C14710, 0x00C2D9B0, 0x00C47EA0 and 0x00C48740: libpk has no PDB/RTTI. 0x00C167C0 is bound under
      eight spellings.
    - These wait on a libpk naming pass.
  - **0x00D29880 CHookCoastTeleporter::Main:** a 1,319-byte quest body, not tiny. Not attempted.
  - **0x0122A2F0:** the atexit stub for the scope-43 static `interaction` (`?CL@`). It needs its 0x4B0-byte owner function authored
    first.
  - **0x0132D470:** a length-0 `.rdata` address. It is a ledger error, not code.

## Headers (13 changed, every new sub-macro reads `default=same`; 9 new headers)
- **New sub-macros:**
  - FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK
  - FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
  - FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
  - FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRELEASE
  - FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_BANKS
  - FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE
  - FABLE_RETAIL_CGAMESCRIPTTHING_UNIQUEID
  - FABLE_RETAIL_CMINIMAPDISPLAY
  - FABLE_RETAIL_CRESOURCELIST_REMOVERESOURCE
  - FABLE_RETAIL_CSTRING_ALLOCCOPY
  - FABLE_RETAIL_CSYMBOLMAP_SLONGMAP
  - FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
  - FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
- **Includers:** `ppcmp.py` found 10 of 3,937 that differ. All 10 were rechecked on the merged tree (`logs/recheck.log`):
  RELOCATION_MATCH + PASS.
- **Watch: FABLE_RETAIL_CMINIMAPDISPLAY.** The unlanded `pending/2026-11-03_linux_hand729` uses the same macro. This landing gives it
  the same class shape (virtual dtor, sizeof 0xC0), so landing 729 means merging the two declaration lists.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zd.tsv` (54 labels: 46 catalog renames, 45 manifest renames).
  - They cover every landed row and three newly proven callees:
    - 0x00CD2B1C `_M_find`
    - 0x0082C770 `remove_if`
    - 0x00ABED70 `vector<C3DVertex2>::push_back`
- **Five libpk rows were landed under the placeholder spellings their landed callers already bind**: 0x00C36220, 0x00C3B840,
  0x00C3B960 and 0x00C44B40. This keeps one address, one name.
- **Queued (`land0411zd`):**
  - FindAlertEvent 0x0090F110 is really `AAEPBV` (private, const return).
  - StopSpeaking 0x00864580 is `?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z`.
  - 0x00440730 is the shared `_List_base::clear` body.
  - **PROPOSAL only, one source:** EndShutdown 0x00F36D30, SetOccupier 0x0087C390, IsOccupied 0x0087C3F0 and SetParams 0x007FD3F0.
- **Checks:** `bindings_multi_check.py` prints 0. `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,840 genuine of 49,568; 5,839,207 function bytes (57.13%).
