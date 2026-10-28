# Chunk 520 re-queue (CLocalDetailPrimitiveMeshZSpriteBatch .. eh vector ctor funclet): 22 of 25 byte-exact

6 MATCH + 16 RELOCATION_MATCH, no retail_asm rows, 1 parked (00c52c40 DIFFER 75v75), 2 SKIP (CRT `__finally`
funclets). Every src/ file grades clean in gall.sh (tmp/gall_final.txt). Every test prints `<addr>_TEST PASS` under g++
(tall.sh) and VC7.1 (tall.py). The vland dry run reports WINS: 22. It has one RELOC_CONFLICT, settled below from the
bytes (the landed bindings are swapped), and otherwise only RELOC_NAME / SHARED_HEADER_AVAILABLE notes.

Why the earlier attempts stalled: the 2026-10-14 parks were graded on the QFE-4035 lane, and most rows matched at once
under RTM 13.10.3077. The rest were held up by wrong propagated manifest labels (16 of the 25 labels name another class),
which sent the earlier authors after the wrong classes. 00eb4710 also needed the `int` return of
IsPlayerThreateningEntity and the guarded-return shape.

## Proven (two sources each; exact PDB / library spelling, checked with objdump -t on the built object)
- 00bfac70 `??0CLocalDetailPrimitiveMeshZSpriteBatch@@QAE@PBVCLocalDetailObjectCollectionType@@PAVCLocalDetailCacheMap@@@Z` -- Ego_r.pdb ctor + vtable store of the class; landed sibling ctor 0x00BFAA80 has the same base-init sequence.
- 00bfde80 `?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z` -- Ego_r.pdb (private virtual) + slot 1 of vtable 0x012A835C; compares IndexBuffer +0x34 / ForegroundVertexBuffer +0x38 (PDB offsets).
- 00c91360 `@oggpack_readinit@12` -- libogg bitwise.c body (memset, buffer=ptr=buf, storage=bytes) + existing bound spelling.
- 00ca0620 `@_encodepart@20` -- libvorbis res0.c body + callees local_book_besterror 0x00CA02B0 and vorbis_book_encode 0x00C96140 (landed).
- 00cba7e0 `??$swap@U?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@@std@@YIXAAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@0@0@Z` -- Ego_r.pdb std::swap instantiation + landed sibling 0x00CBA8E0 (same body shape, 16-byte pair).
- 00d9f160 `?Init@CMC_Minion@CQ_MinionCampScript@NScript@@EAEXXZ` -- Ego_r.pdb + slot 2 of vtable 0x012D6DAC stored by the landed Alloc 0x00D9F0B0 (slot 1 is the landed Main 0x00D9F270).
- 00e376d0 `??0CV_AssassinAttacksScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z` -- Ego_r.pdb members (RegionOfMostRecentKill, RegionInfo[5], RegionKillCounts) + the ctor's own member-init sequence at the PDB offsets less 0x14.
- 00e5dde0 `??0CBodyGuard@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z` -- Ego_r.pdb (AngerTimer +0x30, AttackedBySummonersTimer +0x34) + the vtable 0x012E701C store; landed CFisticuffsMember 0x00E83920 shape.
- 00e7dfe0 `??_G?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@UAEPAXI@Z` -- Ego_r.pdb CPredicate_And<CIsThingAlive,CIsVariableFalse> + Right's vtable 0x012EA43C (CIsVariableFalse) and Left's ~CIsThingAlive 0x00E241B0.
- 00e7e110 `??0CV_FishingCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z` -- Ego_r.pdb (Winner +0x68, SizeComment +0x74 retail) + the member-init order.
- 00eb4710 `?IsFeelingThreatened@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAE_NXZ` -- Ego_r.pdb (private, bool) + its callees: IsPlayerThreateningEntity, GetSquaredDistanceBetweenThings compared against the member ThreatDistSq +0x34, and IsActiveThreadTerminating.

## Probable (PDB engine names, but this is libpk's own copy of the pool)
- 00c28380 `?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z` -- Ego_r.pdb engine class + donor order inside the libpk copy (0xC27EF0 InsertFreeArea .. 0xC28380 .. 0xC28610 InternalFreeHandle .. 0xC28690 Free).
- 00c28640 `?ValidateResources@CHandleBasedMemoryPool@@QAEXXZ` -- same evidence; calls vtable slot 2 (ValidateResource 0xC29CE0) of vtable 0x012B4098.
- 00c28690 `?Free@CHandleBasedMemoryPool@@QAEXJ@Z` -- same evidence; the callees fit the PDB private trio InsertFreeArea / CombineAdjacentAreas / InternalFreeHandle.
  These collide with the engine build of the same class (0xA026B0 GetPointerFromHandle, 0xA02AD0 ValidateResources, 0xA02B30 Free). The precedent is that landed libpk pool siblings use the same PDB spellings. See "Review at landing".

## Descriptive (libpk / GameCODA audio: no PDB, no RTTI)
- 00c10c90 `?GetPhysicalPlayer@CSoundEventManager@@QAEPAXK@Z` -- GetFreeSlot 0xC21E50, then FindEntry 0xC21EB0 + YieldPhysicalPlayer 0xC216D0, then GetFreeSlot again.
- 00c20cb0 `?HasPositiveGain@CSoundEvent@@QAEJPAUCSoundEventManager@@@Z` -- CheckState(-1) then `GetGain > 0.0f`.
- 00c2d260 `?UseInstance@CInstanceManager@@QAEXIPAUSThemeRequest@@@Z` -- the spelling the landed CInstanceManager::Update already binds.
- 00c38850 `?SetScale@CInstance@@QAEXM@Z` -- stores +0x17C and forwards a float to the frame streamer (slot 5). Float proven by the bytes (tail jmp without re-storing the argument).
- 00c437c0 `?FlushQueue@CAudioObject@@QAEXXZ` -- the landed ReleaseQueue 0x00C43810 without the per-buffer release.
- 00c52c40 `?GetCyrixModelName@CCpuInfo@@QBEPBDXZ` (parked) -- Cyrix model strings; siblings GetIntelModelName 0x00C52AD0 and GetCentaurModelName 0x00C52C90 are landed.
- 00c59720 `?Play@CCodaVoice@@UAEJPAUCAudioSound@@@Z` -- slot 7 of CCodaVoice vtable 0x012B5C10.
- 00c59bc0 `?BeginSound@CAudioSource@@QAEXPAUCAudioSound@@@Z` -- the bound spelling, kept.
- 00c624c0 `?UpdateVolume@CAmbientTrigger@@QAEXPAVCAmbientSource@@@Z` -- the trigger's handle (+8) and source volume go to the handle table.

## SKIP (not functions)
- 00bfef20 -- `__finally` funclet of CRT `eh vector destructor iterator` 0x00BFEED8. It uses the parent's ebp frame and is called at 0x00BFEF13.
- 00f3fbe0 -- `__finally` funclet of CRT `eh vector constructor iterator` 0x00F3FB96. It has the same shape (`cmp [ebp-20h],0`, then calls 0x00BFEE7A).

## Label corrections (propagated manifest labels; the real identity is in the row source)
| Address | Wrong label | Real identity |
|---|---|---|
| 0x00C10C90 | CInputProcessRebootGame::ProcessInput | CSoundEventManager::GetPhysicalPlayer |
| 0x00C20CB0 | CDraw2DText::IsAlphaThresholdExceeded | CSoundEvent::HasPositiveGain |
| 0x00C28690 | CHandleBasedMemoryPool::FreeAllocation | CHandleBasedMemoryPool::Free |
| 0x00C2D260 | CEditTransactionSetEngineTheme::Redo | CInstanceManager::UseInstance |
| 0x00C38850 | CBankFile::SetBankUpdateStatus | CInstance::SetScale |
| 0x00C437C0 | CDisplayManager::DestroyDevice | CAudioObject::FlushQueue |
| 0x00C52C40 | CEngineInternalPrimitiveParticleGroup::GetRenderLayerMask | CCpuInfo::GetCyrixModelName |
| 0x00C59720 | CDataOutputStream::Write3DVector | CCodaVoice::Play |
| 0x00C59BC0 | CManager::GetCurrentScreen | CAudioSource::BeginSound |
| 0x00C624C0 | CConsoleCommandLine::DecrementCursorPosition | CAmbientTrigger::UpdateVolume |
| 0x00C91360 | CMemoryDataInputStream::OpenMemorySource | oggpack_readinit |
| 0x00CA0620 | CEnvironment::LookupLocalThemeIndex | _encodepart |
| 0x00CBA7E0 | C3DAnimFile ctor | std::swap<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>> |
| 0x00E376D0 | CInputProcessManager ctor | NScript::CV_AssassinAttacksScript ctor |
| 0x00E5DDE0 | CDarkwoodTrader ctor | NScript::CV_BodyGuardScript::CBodyGuard ctor |
| 0x00E7DFE0 | CLandscapeBackgroundSubNodeSplitBranch ??_G | CPredicate_And<CIsThingAlive,CIsVariableFalse> ??_G |
| 0x00E7E110 | CIcicleData ctor | NScript::CV_FishingCompetitionScript ctor |
| 0x00BFEF20 / 0x00F3FBE0 | CFrameData::ReleaseTexture | CRT eh vector dtor / ctor iterator `__finally` funclets |
Callee catalog labels also wrong (RELOC_NAME notes): 0xAC1F60 `Nop_ret0` (CIndexBuffer::OnPostRelocation), 0xC20720
`EmptyStub` (CSoundEvent::CheckState), 0xC16890 `TailJmp_GetNextRegionOnRouteTo` (CName::GetString), 0xC442F0
`CMarkedBuffer::GetMark` (CircularQueue::GetInfo), 0xC62440 `CRegion::SetMiniMapRegionExitTextOffsetY`
(CAmbientTrigger::GetHandleTable), 0xC46F40 `TurnToPointer` (CAmbientSource::GetVolume), 0xCA02B0
`Audio_ComputeTimeFrequencyMap` (local_book_besterror), 0xE241B0 `CScriptThingFunctor` (~CIsThingAlive).

## RELOC_CONFLICT settled from evidence
`?GetSquaredDistanceBetweenThings@@YIMABVCScriptThing@@0@Z` -> 0x00CBE583 here, 0x00CBE512 in landed 0x00DDC6C0.
The landed bindings for this pair are swapped:
- Retail bytes: 0x00CBE512 (two things, two `[eax+18h]` position calls) ends in `fsqrt`, and so does 0x00CBE551 (one thing plus a vector). 0x00CBE583 (two things) has no `fsqrt`.
- FableWin.pdb symbol order: GetDistanceBetweenThings, GetDistanceBetweenThingAndPosition, GetSquaredDistanceBetweenThings, GetSquaredDistanceBetweenThingAndPosition. That maps to 0xCBE512, 0xCBE551, 0xCBE583, 0xCBE5C0.
- The row's own member is named ThreatDistSq.
So 0x00CBE512 = GetDistanceBetweenThings and 0x00CBE583 = GetSquaredDistanceBetweenThings. Landed 0x00DDC6C0 (CWaypointSpeedCheck::Main) and 0x00DA0D90 (FindFurthestSpawnPoint) must swap their declarations (NAME_FIXES.tsv).

## Header debt
- engine/CHandleBasedMemoryPool.h is the engine layout (Ego_r). libpk's copy has a different vtable (0x012B4098: slot 0 MoveResource 0xC28220, slot 1 CanMoveResource 0xC29CD0, slot 2 ValidateResource 0xC29CE0, no dtor) and a 0x10-byte Dinkumware vector with `_Myfirst` at +4. It is modelled locally (tmp/pool_model.inc), which gives SHARED_HEADER_AVAILABLE warnings on 00c28640 / 00c28690.
- engine/CMovableResource.h, CScriptThing, CGameScriptInterface.h and CIndexBuffer are data-only headers (`void* __vftable`, no virtuals). Rows that call through or derive from their vtables (00bfac70, 00bfde80, 00d9f160, 00e5dde0, 00e7dfe0, 00eb4710) model them locally, the same way as landed precedent (CHTEAssaultGuard_Main_00d6aea0).
- CGameScriptInterface slots are padded locally (v001..), with the real slot names only at the slots called (264/265 GetHealth/ModifyThingHealth, 70 GetHero). This is the precedent of the landed script rows.
- libpk structures (CSoundEvent, CSoundEventManager, CInstance, CInstanceManager, CAudioObject, CircularQueue, CCodaVoice, CAudioSource, CAmbientTrigger) have no headers. Their member names are descriptive.
