# Chunk 531 (re-queue) -- agent 531 notes

20 rows, 0x00BCF4F0..0x00E81B40. Result: 11 matched (1 MATCH + 7 RELOCATION_MATCH in src/, 3 RELOCATION_MATCH graded
`retail_asm` in retail_asm/), 5 DIFFER parked in differ/, 4 SKIP. Every matched row passes its behaviour test under g++ and
VC7.1, and passes the vland dry run with no RELOC_CONFLICT or RELOC_DUPLICATE.

## Names

### Proven (two sources each, exact PDB spelling)
- 0x00BD2920 `?Update@CEngineInternalPrimitiveAnimatedMesh@@UAEXABVCEnginePrimitive@@K@Z` -- Ego_r.pdb + the vtable slot of the animated mesh primitive; the CEnginePrimitiveAnimatedMesh desc layout matches the copy.
- 0x00BD2BC0 `?Update@CEngineInternalPrimitive2DAnimatedMesh@@UAEXABVCEnginePrimitive@@K@Z` -- Ego_r.pdb + the 2D desc layout; it calls Initialise 0x00BCF4F0.
- 0x00BCF4F0 `?Initialise@CEngineInternalPrimitive2DAnimatedMesh@@QAEXXZ` -- Ego_r.pdb, in donor order after SetupMesh2D 0x00BCF430 + called by Update 0x00BD2BC0. Parked as DIFFER.
- 0x00BE1C10 `?Update@CEngineInternalPrimitive2DViewport@@UAEXABVCEnginePrimitive@@K@Z` -- Ego_r.pdb + Ego_r CEnginePrimitive2DViewport layout. Parked as DIFFER.
- 0x00BE2B70 `?GetMappingDirectionBlend@CEngineLandscapeMeshBuilder@@QAEMW4LANDSCAPE_TEXTURE_MAPPING_DIRECTION@@ABVC3DVector@@@Z` -- Ego_r.pdb signature (thiscall, enum + const C3DVector&, float) + caller BuildBlendingTables 0x00B673B8. Parked as DIFFER.
- 0x00BE3110 `?BuildSharedIndexBuffers@CEngineLandscapeMeshBuilder@@AAEXXZ` -- Ego_r.pdb (private) + the CIndexBuffer/GetIndexBufferMemPool callees and the power-of-two patch loop.
- 0x00BF7DF0 `?GetDesiredLODBand@CLandscapeBackgroundTreeNode@@QAEJXZ` -- Ego_r.pdb QAEJXZ + members (BoundingBox, FirstBand +0xe0, LastBand +0xe2) + callers ProcessBackgroundLoading 0x00BF8030 / UpdateStaticLOD 0x00BF8810.
- 0x00D226E0 `?SendBodyguardsToTheRightPlace@CQ_BreakSiegeScript@NScript@@QAEXVCCharString@@0@Z` -- FableWin.pdb (Ego_r only has the CV_HauntedHouseScript method of that name) + the "BodyGuard" literal 0x012CB3F0 + callers 0x00D21D37/0x00D21F88/0x00D21FE5 in CQ_BreakSiegeScript code.
- 0x00E3AC70 `?Init@CBordelloLady@CV_BordelloScript@NScript@@EAEXXZ` -- Ego_r.pdb + vtable 0x012E3AC8 slot 2, stored by Alloc 0x00E3AB90 at 0x00E3ABC5.
- 0x00E81B40 `?GetBiggestFishWeight@CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@AAEMXZ` -- Ego_r.pdb AAEMXZ + members (CompetitionEntry +0x5c, fish types) + the PlayCutscene 0x00E80D60 call.
- 0x00BEEA00 `??$__adjust_heap@...CEngineTextureDecal...@@YIX...@Z` (full symbol in differ/00beea00.cpp) -- STLport 4.6 __adjust_heap (retail STL is STLport) + callers __make_heap 0x00BEEE40 / 0x00BEEEF6 in the CVectorMap<pair<long,long>,CEngineTextureDecal> sort TU. Parked as DIFFER.

### Probable
- 0x00C1CDA0 `?PreMultiply@CMatrix3x4@@QAIXABV1@@Z` -- FableWin.pdb names CMatrix3x4::PreMultiply (spelled `QAEXABV1@@Z`) and the body mirrors the landed PostMultiply 0x00C1CF20; the register convention follows the landed PostMultiply (QAI). See Review at landing.
- 0x00C0CB50 `?Insert@CString@@QAEHHD@Z` -- libpk has no PDB; the body is MFC CString::Insert(int, TCHAR) line for line, built on the landed CopyBeforeWrite / AllocBuffer / Release siblings.
- 0x00C0C270 `?ConcatInPlace@CString@@QAEXHPBD@Z` -- MFC ConcatInPlace line for line, with its three assertion strings; landed ConcatCopy 0x00C0C220 / GetData 0x00C0B8C0. Parked as DIFFER.

### Descriptive (no PDB coverage)
- 0x00C44460 `?Enqueue@CircularQueue@@QAE_NPAUCSoundBuffer@@_K@Z` -- the landed caller CAudioObject::PlaySource 0x00C43F80 declares this callee; the landed DequeueNext 0x00C44320 / DequeueItem 0x00C44270 share the layout.
- 0x00C3FF90 `?LookupAzimuthElevation@@YAJMM@Z` -- libpk audio direction-table lookup; caller 0x00C400D0; the table 0x012B5E10 is spelled `g_DirectionFilterTable_012b5e10`.
- 0x00C3FF70 `RoundHalfDownToLong` (stand-in callee of 0x00C3FF90, not a row) -- a bare `fld` / `fistp dword` helper.

## Label corrections (catalog/manifest label -> what the body is)
- 0x00BF7DF0 CEngineLandscapePatch::DebugCalcInForgroundRange -> CLandscapeBackgroundTreeNode::GetDesiredLODBand
- 0x00C1CDA0 CEngineClothSimulationMeshBase::ConvertVectorToLocal -> CMatrix3x4::PreMultiply
- 0x00C44460 CCreatureAction_FireMissileWeapon::ProcessAnimFrame -> CircularQueue::Enqueue
- 0x00C3FF90 Animation_ComputeFrameOffset -> LookupAzimuthElevation (descriptive)
- 0x00C0CB50 CCharString_InsertAt -> CString::Insert; 0x00C0C270 CCharString_InsertData -> CString::ConcatInPlace
- 0x00D226E0 CScriptBase_InitializeBodyGuardAbility -> CQ_BreakSiegeScript::SendBodyguardsToTheRightPlace
- 0x00E3AC70 CScriptGameResourceObjectScriptedThingBase_HandlePoliticianDialogue -> CBordelloLady::Init
- 0x00E81B40 CScriptBase_AskForFish_Impl -> CFishCompOrganiser::GetBiggestFishWeight
- 0x00BE2B70 BuildBlendingTables_ComputeBlendFactor -> CEngineLandscapeMeshBuilder::GetMappingDirectionBlend
- 0x00BE3110 module doubled (CEngineLandscapeMeshBuilder::CEngineLandscapeMeshBuilder::) -> CEngineLandscapeMeshBuilder::BuildSharedIndexBuffers
- 0x00BEEA00 `_Adjust_heap` (Dinkumware, Ego_r name set) -> STLport `std::__adjust_heap`
- 0x00C3C5D0 manifest CCreatureAction_FireMissileWeapon::FrameUpdate -> CSoundBuffer::AddRef (landed binding from 0x00C43E20 already spells it)
- 0x00C3C4A0 manifest NCreatureMode::CModeBlock::GetAnimFilter -> CSoundBuffer::GetDesc (landed spelling from 0x00C43390; descriptive)
- 0x00A14510 manifest C2DVector::Dot -> C2DVector::AccurateNormalise (Ego_r; landed binding already correct)
- 0x00B3B0A0 manifest CEngineInternalPrimitiveBase::AddChildPrimitive / label ReturnConst_1 -> CEngineResourceManager::GetIndexBufferMemPool
- 0x00A242B0 / 0x00A24280 manifest CTCInventoryAbilities::GetAbilityLevel -> CMeshDataBank::PeekAnimatedMesh / PeekStaticMesh
- 0x00AC1F00 manifest CVertexBufferWin32::DoUnlock -> CIndexBuffer::Unlock
- 0x00BD8D00 / 0x00BD8A10 manifest LTextTreeWalkThrough_Assign / BuildTreeArray -> STLport _Rb_tree::operator= / _M_erase (BoneScalings map)
- 0x00E80D60 manifest CTCCarryable_Initialize -> CFishCompOrganiser::PlayCutscene; 0x00E81A60 CheckForFish -> CheckForRod
All spelling fixes for landed code are in NAME_FIXES.tsv (15 rows).

## Header debt
vland warns SHARED_HEADER_AVAILABLE (local class models where `engine/<Class>.h` exists; strict mode would reject):
- 0x00BD2920 / 0x00BD2BC0: CIVCountedPointeeBase, CMeshDataBank, CEngineResourceManager, CEngineInternalPrimitiveBase
- 0x00BE3110: CIndexBuffer, CEngineResourceManager
- 0x00BF7DF0: CEngineCamera, CEngineLandscapeRenderer, CLandscapeBackgroundTreeNode
- 0x00D226E0 / 0x00E3AC70 / 0x00E81B40: CScriptThing, CGameScriptInterface (the padded CGSI vtable is generated from ghidra_out/cgsi_slots_aligned.tsv)
Not converted: each conversion needs a re-grade and the budget was spent on matching. The member names and offsets already
follow the PDB layouts, so the switch should be mechanical.

## Stand-in bodies (same-TU noinline bodies of real retail callees; not rows)
- src/00be3110.cpp: `CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables` (real callee 0x00BE2CD0), so the call and its argument setup are emitted as in retail.
- src/00e3ac70.cpp: `Compare` (__fastcall; real callee 0x00411570, landed), as in the landed sibling scripts.
- retail_asm/00c3ff90.cpp: `RoundHalfDownToLong` (real callee 0x00C3FF70), the `fld`/`fistp dword` helper that stays out of line at the first call.

## Review at landing
- 0x01436EA8 is spelled `?g_pLandscapeRenderer@@3PAVCEngineLandscapeRenderer@@A` here (0x00BF7DF0 reads LODZBandsZ +0x498 through it via GetLODZBandAtZ); landed spellings also include `g_pEngine_01436ea8` (two struct types) and `g_pEngineLandscape`. Pick one.
- 0x01436EA0 `?g_pEngineCamera@@3PAVCEngineCamera@@A` vs landed `g_pEngineCamera_01436ea0`, `g_pLightingManager_01436ea0`, `?g_pEngineCamera@@3PAUCEngineCameraView@@A`.
- 0x013D2880 `?g_SSEAvailable@@3_NA` (matches one landed binding; others say DAT_013d2880).
- 0x0143E908 `?g_UnprovenWideString_0143E908@@3VCWideString@@A` (kept the landed descriptive spelling).
- 0x0143BB2C `?g_pfnAssertHook@@3P6IXPBD@ZA` vs landed `g_pfnLogSink_0143bb2c`, `g_pfnLogSink`, `g_pfnErrorReport` (same type).
- 0x0143BB58 `?g_audioSystem@@3VCAudioSystem@@A` (= the 0x00C43390 binding; 10 other spellings landed for this address).
- 0x01436E98 `g_pResourceManager`, 0x01436E7C `g_pEnginePrimitiveRenderer2D`, 0x01436E1C `g_pDisplayManager_01436e1c`, 0x0139C338 `g_MappingDirectionVectors_0139c338` (differ rows) -- each also has other landed spellings.
- 0x00C1CDA0 PreMultiply: FableWin.pdb spells `QAEXABV1@@Z` (thiscall), but retail passes `this` and the matrix in registers like the landed PostMultiply 0x00C1CF20 (QAI). The same question applies to both.
- 0x00C3C4A0 respelled to the landed `?GetDesc@CSoundBuffer@@QBEPBUSBufferDesc@@XZ` (re-graded, still RELOCATION_MATCH); 0x00C3F510 binds it as `?GetDescriptor@COwner@@...`.
- 0x00E81B40 now builds with `/Zc:wchar_t` so 0x0099B800 is spelled `??4CWideString@@QAEABV0@PB_W@Z` as in the PDBs (re-graded, still RELOCATION_MATCH).
- Queue items are modelled as CSoundBuffer (from PlaySource); some landed audio code calls the same objects CItem.
