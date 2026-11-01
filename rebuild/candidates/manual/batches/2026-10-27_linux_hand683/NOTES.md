# Hand chunk 683 (never-attempted 284-640 byte rows, 8 unnamed gap-scan rows): 8 of 10 byte-exact

All ten identities settled with two sources each (PDB symbol + vtable/caller/string evidence, or landed sibling + vtable
sharing for the no-PDB libpk rows). No row is a toolchain-library copy. vland (read-only): 8 RELOCATION_MATCH, 0 reject.

## What matched (src/, test/ PASS under g++ and VC)
- 0x0121E480 (318 B) `dynamic initializer for COpinionReactionManager::AttitudeStrings` (Leaf `_$E1`) -- PDB donor 0x0100CEA0
  `?AttitudeStrings@COpinionReactionManager@@2PAVCCharString@@A`, .CRT$XCU entry 0x01374488, user 0x007018B0. 18 CCharStrings
  "NONE".."HATE". 1 probe.
- 0x0121F2D0 (284 B) `dynamic initializer for CTCFireheartMinigame::BlockNames` (Leaf `_$E1`) -- PDB donor 0x01011AE0
  `?BlockNames@CTCFireheartMinigame@@2PAY03VCCharString@@A`, landed PlayGame 0x007E78E0 indexes `[row][column]`; .CRT$XCU
  0x01374620. CCharString[4][4] "BLOCK00".."BLOCK33". 1 probe.
- 0x00CEFAC0 (499 B) `NScript::CGameflowAssistanceScript::Main` -- vtable 0x012C5DE0 slot 2; four
  `AddSpawnedFunction(new CSpawnedFunc<...>("Tutorial_X", 0, this, &Tutorial_X), CCharString(""))` (0x00CEFCC0/0x00CEFFB0/
  0x00CF0180/0x00CF02A0). 1 probe.
- 0x00EFFE10 (480 B) `NScript::COracleMinigameTestScript::Main` -- vtable 0x012F70A0; three entity bindings (Alloc 0x00F00000/
  0x00F00A10/0x00F00BB0, flags 0), `while (!MissionSucceeded)`, SetQuestAsCompleted(name, true, true, false) + DeactivateQuestLater. 1 probe.
- 0x00ED83C0 (480 B) `NScript::CV_TalentlessBardScript::Init` -- vtable 0x012F3428 slot 3 (the one missing catalog virtual of the
  class whose Main/OnPersist are landed); clears six flags + counter, `BardWanderTime = pGlobalScriptDef->BardWanderResetTime`
  (retail +0x7c8), 11 QuestInfo name/tag pairs in retail order, `SongThingAvailable.resize(12, false)` + guarded fill loop.
  Retail `std::vector<bool>` here is the 12-byte byte vector (resize 0x00467686), not a bit vector. 1 probe.
- 0x00D50800 (448 B) `NScript::CQ_GuildTrainingDepartureScript::Main` -- vtable 0x012CF670 slot 2 (RegisterMain 0x00D50760 landed,
  CTheRealGuildmaster::Alloc 0x00D509D0, flags 1); wait for Q_GuildTrainingWoodsDeparture to start, then to end
  (`!= false` form for `setne`), DeactivateQuestLater(GetActiveQuestName(), 0). 2 probes.
- 0x00C6D150 (511 B) `CVtable_012bed7c::Update(float x, float y, float blend, float volume, const float* gains, bool crossfade,
  bool force)` (descriptive; libpk, no PDB/RTTI) -- slot 4 of vtable 0x012BED7C, shared with the derived SSE2/MMX tables
  0x012B8A64/0x012B8AA0; same signature/shape as the landed CVtable_012bede0::Update 0x00C71270 and CVtable_012bedcc::Update
  0x00C708A0. Looks up the direction's filter pair (0x00C400D0: `(4, x, y, &first, &second)`, 0x34-byte records, delay word +0x32),
  copies the delays into +0x1fc/+0x204 and +0x200/+0x208 on change, stereo (0.6 -/+ 0.4 sin) or quad (>= 4 output channels:
  (c+0.6)(0.6-s), (c+0.6)(s+0.6), (0.6-s)(0.6-c), (s+0.6)(0.6-c)) gains into +0x1e4.., volume into +0x20, RebuildGain(crossfade)
  0x00C6CF40 when anything changed or `force`. Key: a function-scope `float gain[6]` (frame 0x20) and the inline `x * 0.017453292f`
  inside sin()/cos() (a float `angle` local spills). 8 probes.
- 0x0099A6A0 (640 B) `CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long)` -- PDB
  `?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z`, vtable 0x0122D06C slot 1, called by the landed copy ctor
  0x0099ADC0. Pathname = pathname; WinFileHandle = 0; attributes = (flags & 2) ? FILE_FLAG_SEQUENTIAL_SCAN : 0 (written
  `attributes = 0` BEFORE the pathname assignment, raised by an `if`); ConvertPathToFullPath; empty -> throw
  CBBBFileException("Bad filename " + fullPath.ToCharString(), 1); switch(access) 1: GENERIC_READ/share 1/OPEN_EXISTING,
  2: Validate + RW/share 1/OPEN_EXISTING, 3: Validate + RW/share 0/OPEN_ALWAYS (AccessType = 2), 4: Validate +
  CreateDirectoryForFile + RW/share 0/CREATE_ALWAYS (AccessType = 2), default handle 0; INVALID_HANDLE_VALUE -> named
  `error = GFGetLastWindowsError()` then message "Cannot open file '" + path + "' (access type " + GFIntToCharString(access) +
  ") - " + error, throw type 2; Length = GetFileSize, SetFilePointer begin/current, Position 0, Opened, WinFileHandle. 6 probes.

## Parked for Fable 5.1 (differ/)
- 0x00C6EEF0 (507 B) `CVtable_012b8a8c::Process(const short* src, long* const* outputs)` (descriptive; slot 2 of the SSE2 gain-ramp
  mixer, ctor 0x00C550B0): DIFFER(511v507), `// Flags: /GS`. Everything matches except the current/previous record swap: retail
  `eax=[0x30]; ecx=[0x34]; [0x34]=eax; mov eax,ecx; [0x30]=ecx` (5 instr.) vs every C++ swap form's 4-instruction version (or, for
  `old = cur; cur = prev; prev = old`, a pointer forwarded in edx with a zero-extended m_step, 517). 24 probes. NOTE: the file
  contains the two same-TU `__asm` kernels (the landed retail_asm bodies of ScaleAccumulate16Ramp 0x00C6ED70 / ScaleAccumulate16
  0x00C6ECD0) because the ramp kernel's private `initial`-in-ecx convention only appears for a static same-TU callee (and needs /GS
  and kernel locals WITHOUT `__declspec(align(16))`); it must land under retail_asm/ or with the kernels replaced -- coordinator call.
- 0x00BA9830 (570 B) `CEnginePrimitiveManagerMeshBase::GetReflectionTexture(const C3DMeshMaterial*, unsigned long, bool&, const
  CGraphicDataBank*, bool)` (PDB, static __fastcall): DIFFER(570v570), byte-identical except `push ebx`: retail sinks it past the
  graphic-list lookup diamond into the flags block (pop order ebx/esi/ebp/edi), the lane pushes it with ebp/esi after the index==0
  early return. 25 probes (helpers, nesting, expression forms, flag forms). The double `(end-begin)/0x2c` size computation that
  parked the sibling PreloadReflectionTexture 0x00BEBCA0 (hand675) is SOLVED: an STLport-style vector member (start/finish/
  end_of_storage, size() = finish - start) makes VC7.1 recompute it; a begin/end pointer-pair model CSEs it. Retry the sibling.

## Skipped
- none.

## Label corrections (see NAME_FIXES.tsv)
- 0x013BAEB0 / 0x013BABD0 data labels; 0x01229E20 / 0x012295F0 (the dynamic atexit destructors, labelled CFontBankStateBlock::CreateCRC);
  0x00C550B0 / 0x00C55070 (libpk effect ctors, labelled C3DAnimationUtil); 0x00C400D0 (Animation_ComputeFrameOffset is a libpk
  direction-filter lookup); 0x00999140 (bound as CWideString::operator= -- it is CAFile::ConvertPathToFullPath); 0x00999970 (bound to
  the data symbol DebugUseReadOnlyDrives -- it is ValidatePathnameIsOnWriteableDrive); 0x0099B2C0 ToCharString (landed file says
  ConvertToCharString); 0x00C6CF40 (forge label Audio_SetFilterCoefficients; the landed sibling calls it RebuildGain).
- vland WARN RELOC_CONFLICT on `_$E2` (0x012295F0 / 0x01229E20 vs 01228930 in landed 01219f00): every dynamic-initializer TU has its
  own `$E2` atexit thunk; one symbol, many addresses -- inherent to this Leaf family, not a naming error.

## Header debt
- engine/CDiskFileWin32.h flattens Pathname to `Pathname_PStringData` and types AccessType as `unsigned char[4]`: the Open model
  (like the landed 0x0099ADC0) uses a typed local class. engine/CResource.h's flattened default differs from the landed GetFrame model.
- CVtable_012bed7c (0x00C6CA20 ctor model): +0x1e4 is `float m_gains[6]`, +0x1fc `long m_delays[6]` (the ctor calls them m_valuesA/B).
- CV_TalentlessBardScript: landed OnPersist 0x00ED85A0 models SongThingAvailable as a 0x14 bit vector; retail is a 0xC byte vector
  (RandomSpeechTimer at +0xe0).
- No PDB layout for CGraphicDataBank's graphic list (+0x1e0 std::vector<CGraphicInfo>, 0x2c each) or blank textures (+0x2b4).

## Callee names used
- 0x00C400D0 `LookupDirectionFilterPair` (descriptive, replaces the wrong Animation_ComputeFrameOffset), 0x00C6CF40
  `CVtable_012bed7c::RebuildGain(bool)` (landed sibling's spelling), 0x009FD150 `?GetBlankTexture@CGraphicDataBank@@QBEPBVCTexture@@W4ETextureType@1@@Z`,
  0x009FD3F0 `?GetBlankTexture@CGraphicInfoFrame@@QBEPBVCTexture@@XZ`, 0x0099B2C0 `?ToCharString@CWideString@@QBE?AVCCharString@@XZ`,
  0x00999140 `?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z`, 0x00999970 `?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z`.
