# Chunk 767 -- the rest of the tiny rows (61 rows)

**51 matched (8 MATCH, 43 RELOCATION_MATCH), 10 parked.** Every matched row: VC7.1 RTM C++, no `__asm`/`naked`/`_emit`/`volatile`;
behaviour test passes under g++ and under the lane's VC7.1 `/Od` test build. Read-only vland: `WINS: 51` (vland_result.txt; warnings only:
SHARED_HEADER_AVAILABLE on the nine reused adjustor-thunk definer TUs, RELOC_NAME `??_E` vs the catalog's "scalar deleting destructor"
label, and the TU-local `_M_find` instantiation name). hdrcheck2 on `hdr/`: 13 CHANGED, all `default=same`, every pre-existing macro
`same`; 9 NEW headers (hdrcheck2_result.txt).

## Results
| address | name | grade | attempts |
|---|---|---|---|
| 007fd3c0 | CTCDPhysicalObstruction::OnInitialActivate | MATCH | 1 |
| 00810fc0 | NEntityEvents::CEventHitBase::GetPHitBy | RELOCATION_MATCH | 1 |
| 00810fd0 | NEntityEvents::CEventHitBase::GetPWeaponUsed | RELOCATION_MATCH | 1 |
| 00827880 | CMap::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 0082a300 | CMiniMapDisplay::RemoveAllMarkers | RELOCATION_MATCH | 1 |
| 0082a380 | CMiniMapDisplay::RemoveMarker | RELOCATION_MATCH | 1 |
| 0084d2b0 | NCameraModes::CBase::GetPWorldSeed | RELOCATION_MATCH | 1 |
| 00850630 | NCameraModes::CPhysicsLeaf::Reset | MATCH | 1 |
| 0085ca00 | NCameraModes::CPhysicsLeaf::Reset | MATCH | 1 |
| 0087c3e0 | CTCSinglePersonOccupiable::GetOccupier | RELOCATION_MATCH | 1 |
| 008ab960 | CScriptGameResourceObjectScriptedThingBase::SetImplementation | RELOCATION_MATCH | 2 |
| 008ab970 | CScriptGameResourceObjectMovieBase::SetImplementation | RELOCATION_MATCH | 1 |
| 008ba7e0 | CCreatureAction_RemovePhysicalShieldSpell::PerformAction | RELOCATION_MATCH | 1 |
| 008cff20 | CGameScriptThing::GetPThingUniqueID | MATCH | 1 |
| 008fcfb0 | CAIStateGroupBase::PeekDefinitionManager | RELOCATION_MATCH | 1 |
| 0090f1b0 | CAIStateGroup_MinionReactToAlert::IsRunnable | RELOCATION_MATCH | 2 |
| 009112f0 | CAIStateGroup_MinionGuardReactToBodyFoundMessage::IsRunnable | RELOCATION_MATCH | 2 |
| 00919a00 | CAIStateGroup_BuyFromSeller::Cleanup | RELOCATION_MATCH | 1 |
| 0094bf50 | CAIStateGroup_SellToBuyer::Cleanup | RELOCATION_MATCH | 1 |
| 00994450 | CFileDataOutputStream::SetPosition | MATCH | 1 |
| 00994460 | CFileDataOutputStream::GetPosition | MATCH | 1 |
| 009cb730 | NGameText::CDataBank::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 009d4a40 | CDefStringTable::SetTableFilename | RELOCATION_MATCH | 1 |
| 009e5350 | CSymbolMap::DeallocateUnusedMemory | RELOCATION_MATCH | 1 |
| 009fc3c0 | CResourceBank::RemoveResource | RELOCATION_MATCH | 1 |
| 009fd4d0 | CGraphicInfoFrame::CFrameData::ResetDebugMemoryInUse | MATCH | 1 |
| 00a013d0 | CGraphicDataBank::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00abceb0 | C3DMeshFileVerticesChunk::AddVertex | RELOCATION_MATCH | 1 |
| 00add5c0 | CLHFile_00add5c0::IsNull() const | PARKED DIFFER(name) | 1 |
| 00af2160 | NParticleEngine::CParticleDataBank::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00b2ad80 | CEngineSurfaceCompositionManager::ChangeSceneForTextureBank | RELOCATION_MATCH | 1 |
| 00b2f5d0 | CEngineSurfaceCompositionManager::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00b3aad0 | CEnginePrimitiveAttachmentManager::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00b3afe0 | CEngineResourceManager::OnReleaseDefaultPoolResources | RELOCATION_MATCH | 1 |
| 00b83d20 | CEngineSimulationManager::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00b8c790 | CEngineBillboardGenerator::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00b8fb50 | CEnginePrimitiveTypeInfo::`vector deleting destructor' | RELOCATION_MATCH | 1 |
| 00c0bf90 | CString::AllocCopy | RELOCATION_MATCH | 1 |
| 00c11700 | CUnproven_00c11700::GetFaderOutput() const | PARKED DIFFER(name) | 1 |
| 00c14700 | CUnproven_00c14700::~CUnproven_00c14700() | PARKED DIFFER(name) | 1 |
| 00c14710 | CUnproven_00c14710::~CUnproven_00c14710() | PARKED DIFFER(name) | 1 |
| 00c2d9b0 | CUnproven_00c2d9b0::~CUnproven_00c2d9b0() | PARKED DIFFER(name) | 1 |
| 00c36220 | CSequenceTrack::Play | RELOCATION_MATCH | 1 |
| 00c3b840 | CUnprovenTransferRecord_00C3B850::~CUnprovenTransferRecord_00C3B850 | RELOCATION_MATCH | 1 |
| 00c3b960 | CUnprovenTransferRecord_00C3B970::~CUnprovenTransferRecord_00C3B970 | RELOCATION_MATCH | 1 |
| 00c44b40 | CObject_00c44b40::Method_00c44b40 | RELOCATION_MATCH | 1 |
| 00c47ea0 | CUnproven_00c47ea0::~CUnproven_00c47ea0() | PARKED DIFFER(name) | 1 |
| 00c48740 | CUnproven_00c48740::~CUnproven_00c48740() | PARKED DIFFER(name) | 1 |
| 00c701d0 | CVtable_012bedcc::~CVtable_012bedcc | RELOCATION_MATCH | 1 |
| 00c70bb0 | CVtable_012bede0::~CVtable_012bede0 | RELOCATION_MATCH | 1 |
| 00ca96f0 | CDynamicOutputPin::QueryInterface | MATCH | 1 |
| 00cbf9cf | GetScriptActiveStatus | RELOCATION_MATCH | 1 |
| 00d29880 | NScript::CQ_EndGameScript::CHookCoastTeleporter::Main() | PARKED DIFFER(not attempted) | 0 |
| 01229150 | CTCTavernGame::SetStateAfterInstructions'::`5'::`dynamic atexit destructor for 'no_text_entry'' | RELOCATION_MATCH | 1 |
| 01229160 | CTCTavernGame::GetText'::`2'::`dynamic atexit destructor for 'no_text_entry'' | RELOCATION_MATCH | 1 |
| 01229390 | CEnvironment::RenderUpdate'::`5'::`dynamic atexit destructor for 'interpolated_theme'' | RELOCATION_MATCH | 1 |
| 01229dd0 | CTCBetting::GetText'::`2'::`dynamic atexit destructor for 'no_text_entry'' | RELOCATION_MATCH | 1 |
| 01229de0 | CTCOracleMinigame::GetTextWithNarrator'::`2'::`dynamic atexit destructor for 'no_text_entry'' | RELOCATION_MATCH | 1 |
| 01229e10 | CTCFireheartMinigame::GetText'::`2'::`dynamic atexit destructor for 'no_text_entry'' | RELOCATION_MATCH | 1 |
| 0122a2f0 | `dynamic atexit destructor for 'interaction'' | PARKED DIFFER(not attempted) | 0 |
| 0132d470 | InvalidDecode_HaltStub | PARKED DIFFER(not code) | 0 |

## Header overlay (`hdr/`)
Changed shared headers (new sub-macro only, default path untouched):
- engine/C3DMeshFileVerticesChunk.h -- FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK: `: CChunkedFileChunk`, typed `CArray<C3DVertex2> VertexList` +0x10, AddVertex.
- engine/CAIStateGroupBase.h -- FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER (`IBEABVCGameDefinitionManager@@XZ`).
- engine/CArray.h -- FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY.
- engine/CEngineResourceManager.h -- FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRELEASE (private virtual override, `EAEXXZ`).
- engine/CEngineSurfaceCompositionManager.h -- FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_BANKS (the three CGraphicDataBank members
  +0x8C/+0x398/+0x6A4, ChangeSceneForTextureBank; sizeof 0x10DC unchanged).
- engine/CEnvironment.h -- FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE.
- engine/CGameScriptThing.h -- FABLE_RETAIL_CGAMESCRIPTTHING_UNIQUEID.
- engine/CMiniMapDisplay.h -- FABLE_RETAIL_CMINIMAPDISPLAY (new retail branch: CMarkerInfo, CIsMarkerBoundToThing, virtual dtor, typed
  `std::list<CMarkerInfo> Markers` +0x20, sizeof 0xC0). **Collision:** the pending chunk-729 overlay
  (rebuild/candidates/manual/pending/2026-11-03_linux_hand729/hdr/engine/CMiniMapDisplay.h) uses the same macro with the same class shape
  (adds GetNonAutoMarkers 0x0082A390); merge the declaration lists if both land.
- engine/CResourceList.h -- FABLE_RETAIL_CRESOURCELIST_REMOVERESOURCE.
- engine/CString.h -- FABLE_RETAIL_CSTRING_ALLOCCOPY (AllocCopy, AllocBuffer, inline Init over `_afxPchNil`).
- engine/CSymbolMap.h -- FABLE_RETAIL_CSYMBOLMAP_SLONGMAP (typed SLONGMap CVectorMap; CCompareString).
- engine/NCameraModes_CBase.h -- FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS.
- stlport/fable_containers.h -- FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE (declaration-only member template `remove_if`).
New headers: engine/CDefStringTable.h, engine/CGPUScratchBufferTextureManager.h, engine/CGraphicDataBank.h, engine/CGraphicInfoFrame.h
(+ nested CFrameData), engine/CManagedTextureOwner.h, engine/CTCDPhysicalObstruction.h, engine/CTCSinglePersonOccupiable.h,
engine/NCameraModes_CPhysicsLeaf.h, engine/NEntityEvents_CEventHitBase.h. SIBLINGS.tsv: no landed includer defines any new macro.

## Header debt
- CGraphicDataBank.h / CGPUScratchBufferTextureManager.h / CManagedTextureOwner.h are opaque (bases and members not modelled; only the
  called members declared). The generated CGraphicDataBank.h sits in engine/_quarantine (same include guard; sizeof 0x30C agrees);
  pass-12 pending candidates (verified_pending/00b2d9f0, 00821010, 00e92a50, 00e8a2b0) carry their own CGraphicDataBank /
  CManagedTextureOwner / CDefStringTable overlays: reconcile when those land.
- CGraphicInfoFrame.h models CGraphicInfoFrame with a raw vptr word (CBaseClass base not modelled).
- libpk / GameCODA rows still use local class models (no shared header exists for CSequenceTrack, CAmbientCube, the transfer
  records, CObject_00c44b40, the voice-effect CVtable_* classes).
- 0x0139C900 (`_afxPchNil`) naming decision D4 is still HOLD; CString.h's ALLOCCOPY branch uses the spelling 40 landed rows bind.

## Callee names used (all bound or proven)
?clear@?$_List_base@VCMarkerInfo@CMiniMapDisplay@@..@std@@QAEXXZ 0x00440730; ??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@?$list@..
0x0082C770; ?push_back@?$vector@VC3DVertex2@@..@std@@QAEXABVC3DVertex2@@@Z 0x00ABED70; ?ChangeScene@CGraphicDataBank@@QAEXXZ 0x009FD0C0;
?BeginShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ 0x00F37170; ?AllocBuffer@CString@@QAEXH@Z 0x00C0BDA0; ?_afxPchNil@@3PADA
0x0139C900; ??1CString@@QAE@XZ 0x00C0BA40; ?UpdateDistances@CAmbientCube@@QAEXABVC3DVector@@M@Z 0x00C45810; ?M@C00c61c50@@QAEXXZ
0x00C61C50 (landed placeholder); ??1CVtable_012beda4@@UAE@XZ 0x00C6E630; ?ScriptActiveStatuses@@3V?$map@..@std@@A 0x0143E910 and its
_M_find 0x00CD2B1C; ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z 0x00864580; earlier rows: see each source's Callees line.

## Parked rows (residue, next idea)
- 0x00ADD5C0 -- name: CLHFile/LHA have no PDB symbols, no reference to the address; bytes reproduce (`return First == 0;`, MATCH under a
  placeholder). Next: find the header class whose out-of-line null test this is (a counted-pointer `IsNull`/`operator!`).
- 0x00C11700, 0x00C14700, 0x00C14710, 0x00C2D9B0, 0x00C47EA0, 0x00C48740 -- name: libpk forwarders/destructors (`add ecx,N ; jmp X`) whose
  owner class has no PDB/RTTI name and no usable landed spelling (0x00C11700 is bound as the free-function placeholder
  ?cons_ctor@@YINPAUPair@@@Z; 0x00C167C0, the target of 0x00C47EA0 / 0x00C48740, is bound under eight spellings). Each reproduces
  (RELOCATION_MATCH under a placeholder, differ/). Next: name the owners in a libpk naming wave, then land these unchanged.
- 0x00D29880 CHookCoastTeleporter::Main -- not attempted: a 1319-byte quest Main does not belong in this tiny-row chunk. Next: its own
  chunk, Aeon LUAGameflow EndGame port + the landed CQ_EndGameBossBattleScript Main 0x00D2FB20 as models.
- 0x0122A2F0 -- the atexit stub of PracticeAgainstApprentice's static `interaction` (Ego_r scope 43): its symbol needs the real owner
  (0x4B0 bytes) to get the block number; an abridged owner cannot reach scope 43 honestly. Next: author the owner.
- 0x0132D470 -- not code: the address is in .rdata and the row length is 0. Next: drop the catalog row.

## GOTCHAS one-liners
- VC7.1 local-static scope numbers (`?N?` in the static's name): function scope `?1?`; inside one if-block `?4?`; after
  `if (a) { ...; return; }` at function scope `?4?`; unbraced if + block then the static `?4?`; unbraced if alone `?3?`. The `?d?` digit is the MS number encoding of the undname scope `N` (digit N-1; N > 10 is hex A-P + `@`, so scope 43 is `?CL@`).
- The lane's behaviour test is built with VC7.1 `/Od` too: Dinkumware `list::remove_if` takes only its typedef'd binder (no member template)
  and `std::_Exit` does not exist; write tests that compile under both g++ and VC7.1.
- A `bool` `void`-caller tail call: VC7.1 emits `jmp` for `member.BoolFn();` in a void function (0x00B3AFE0), and `add ecx,N ; jmp` for any
  member forward with the arguments left in place (by-value 4-byte predicate included, 0x0082A380).
- `IsRunnable`-style `return Find() != 0;` gives neg/sbb/neg; `if (Find()) return true; return false;` gives retail's test/setne.
