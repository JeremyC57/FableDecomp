# Hand chunk 476 (CGameCameraManager .. GetBestAttackerForPlacementVector): 20 of 40 byte-exact

All 20 are RELOCATION_MATCH under gall.sh, with behaviour tests passing under g++ (tall.sh) and VC7.1 (tall.py). The call
targets were checked with calls.py, and every import is retail's own (malloc 0xBFEA0E, free 0xBFEA14, operator delete
0xBFE9BC, memmove 0xBFEAE6, the /GS cookie). STLport code is in namespace `std`.

## What matched (20)

- **CGameCameraManager** -- 0069a1a0 `InitSpecialCameraList`: clears the special camera list and appends every
  speciality camera mode def that the manager def names. Three forms were needed together:
  - same-TU `__declspec(noinline)` stand-ins for `GetDef` and `vector::_M_insert_overflow`, so VC7.1 knows they write
    only `out` / read only `x` and keeps the def pointer in a register;
  - the global index read into a named local before the call, which fixed the argument push order;
  - a `mutable` RefCount with const AddRef/Release.

  Local retail layout: World +0x74, SpecialCameraList +0x11C, the index +0x128, PDef +0x134.
- **CIDrawEnvironmentParams::CThemeChannel and its vector** (the catalog had these as "CFeatDef" rows):
  - 006b6010, the implicit memberwise copy constructor (`rep movsd` blocks, then three inline STLport vector copies);
  - 006b6990 `_M_insert_overflow`. `__uninitialized_fill_n` must be `__forceinline`;
  - 006b6b30 `_M_fill_insert`, STLport 4.6 verbatim.
- **CEnvironment** -- 006b7380 `~CEnvironment`. An empty user destructor over a `__declspec(novtable)` class (retail
  stores no vtable of its own), with every member in its retail layout:
  - two CEnvironmentGameTickState blocks of 0xC20;
  - CIDrawEnvironmentParams at +0x1894;
  - the scoped pointers at +0x1960/+0x1964.
- **vector<CGameMapCell>** (7-byte cells):
  - 006b7e20 `operator=`;
  - 006b8050 `_M_fill_insert`. The `x_copy` local triggers `/GS`, so the source carries `// Flags: /GS`.
- **CPersistTraits<CFaction>** -- 006b91a0 `TransferOut`. One `out += "lit" + s + "lit";` statement per line, with each
  set walked by the inline `_Rb_global<bool>::_M_increment`. It matched first time.
- **STLport algorithm instances** (/Gr TUs):
  - 006bb040 `__introsort_loop` over the faction vector map, `pair<unsigned long, CCountedPointer<CFaction>>`;
  - 006bf570 `__introsort_loop` over the registered-music vector map;
  - 006bd140 and 006c32b0 `_Rb_tree::_M_insert`, for map<CCharString, C2DVector> and
    map<CCharString, CCountedPointer<...>>;
  - 006c4270 `__find_if` over CIntelligentPointer<CThing> with CIsThingDead (the 4x unrolled random-access form).
- **NHeroExperience** -- 006d76c0 `GetWeaponModifiedExperienceMultiplierForThing`:
  - the multiplier is a float local initialised to 1.0;
  - both weapon pointers are fetched first;
  - each weapon is guarded by IsDead + HasTC and looked up through the const interface map (0x4365B0).
- **GFSerialiseMapBinaryOut / In<CComboMultiplierData, long>** (006dc350 / 006dcfa0). These are the map template
  instances, not CHeroExperienceDef methods.
  - STLport map construction needs the real `_Rb_tree_base` split: the header is allocated in the base constructor and
    `_M_node_count(0)` is set in the derived class, otherwise count=0 is stored before the malloc.
  - `CPersistTraits<long>` carries its PDB `long DefaultVal`. An empty traits class gets a 1-byte slot and the frame
    comes out 4 bytes too big.
- **vector<CAttackHistoryCombo>** -- 006dccc0 `_M_fill_insert`, with `x_copy` in a 16-byte frame local.
- **NSpeechGainManager** -- 006e3440 `UpdateDialogueElement`:
  - the inline `GFFloatAbs` clears the sign bit through memory (`*(unsigned long*)&f &= 0x7fffffff; return f;`);
  - the speaker pointer is named after the alive test, so HasTC and the lookup share one GetPItem;
  - the new gain is one ternary stored into `info.OutputGain` and passed from there. A named newGain local, or an
    unnamed difference, spills both values to the frame.
- **vector<CAnimInit> / vector<CConversationLine>** -- 006e5100 / 006e6bc0 `_M_insert_overflow`, STLport 4.6 verbatim.
  The `__false_type` tags are homed in the dead argument slots.

## Parked for Fable 5.1 (7)

- 006a3a70 std::vector::_M_insert_overflow DIFFER(285v285): register colouring only, retail fill_len/second destination/destroy cursor in ebx and new_finish in edi, ours swapped | tried: forceinline copy ctor, STLport __destroy chain, trivial same-TU stand-ins | next: lane gap: register colouring
- 006a3e00 std::__introsort_loop DIFFER(277v277): register colouring only, retail first->ebx / last->edi, ours first->edi / last->ebx | tried: hoisted cut, same-TU noinline partition/partial_sort bodies, /Gr instead of __fastcall | next: lane gap: register colouring
- 006b0020 CTCPhysicsBase::GetTurnSpeedAngleYZ DIFFER(271v258): retail keeps the second wrapped angle on the x87 stack (fcom 0.0 / fst [esp+4] / fcom 1.0) and joins at one fsubr [esp]; ours re-reads it from the frame and tail-duplicates fld/fsub | tried: inline GetAngleYZ (called out of line), __forceinline GetAngleYZ + WrapAngle, both RH sets fetched first, named angle locals | next: lane gap: x87 allocation across an inlined wrap (try WrapAngle taking `float&`)
- 006bb390 CGameTimeManager::IsTimeBetween DIFFER(254v265): retail multiplies minutes by 1/60 then 1/24 and hours by 1/24 separately; ours folds 1/60*1/24 into one constant or factors (m+h)*1/24 | tried: private float overload, direct member reads, /60.0f/24.0f, split minutes statement | next: one statement per term (`t = m * c60; t *= c24; t += h * c24;`) or the constants as non-const globals
- 006d8830 CTCHeroExperience::AddSuccessfulMultistrikeHit DIFFER(321v321): retail stores |amount| (`fst [esp+10h]`) before UpdateComboMultiplier's `push 1`, ours pushes first | tried: plain map find, __forceinline find, STLport _M_find, list/map iterators with user copy ctors (fixed the shrink-wrapped pushes) | next: lane gap: x87 store vs push scheduling (or `|amount|` through a named float function argument)
- 006e31e0 NSpeechGainManager::OnDialogueStart DIFFER(596v596): the stored existing offset's squares are summed y,z,x in retail (`fld [esp+24h]` first), z,y,x in ours | tried: plain inline operators, __forceinline operator-/GetMagnitude, list iterator copy ctor + named gain local, reordered magnitude terms (no change) | next: lane gap: single-term x87 commutation
- 006e8ce0 GetBestAttackerForPlacementVector DIFFER(298v294): the first inlined Normalise squares x before y and stores inv with `fstp` + reload where retail keeps it (`fst [esp+38h] ; fmul st(2)`); prologue `fchs ; fstp` order (fixed by declaring `best` first) | tried: X*X+Y*Y+Z*Z, Z*Z+Y*Y+X*X, `*this *= 1.0f / GetMagnitude()` (fixed the second Normalise), copy-constructed flat vector + declaration order | next: lane gap: x87 allocation of a zero-Z inlined Normalise (try the flat vector as an `operator*=` temporary)

## Skipped

13 rows, not attempted (large bodies in the 241..1024 pool, parked early by tractability):
- 006b16b0, 006b1a20, 006b1d30: the sound-theme loaders. They have CCharString temporaries with construction flags,
  def lookups and virtual calls.
- 006b2260: the atmosphere update. It has intelligent-pointer temporaries and 40 calls.
- 006b2ca0: the camera matrix, 205 x87 instructions.
- 006b3b80: the thunder update.
- 006b3e60: a function-local static CVectorMap with a guard and atexit.
- 006b9610: CFactionManager::LoadGameState. It has an inline CRC-32 key hash, the vector-map LowerBound 0x6B8A80 and set
  assignments 0x6B9E00.
- 006bd9b0: CMusicManager state assignment. It has chains of inline pointer-field null tests.
- 006bdc60: the region music update.
- 006e2ed0: lip-sync processing.
- 006e3b90: the phoneme animation update.
- 006e3ec0: the GUI tree pane init, x87 heavy.

STATUS.tsv has a one-line shape note for each.

## Label corrections

Catalog label -> real name, with the evidence:

- 006a3a70 "Vector_ReserveAndFill" -> `std::vector<pair<long, CIntelligentPointer<CThing>>>::_M_insert_overflow`
  (STLport).
- 006a3e00 "Heap_SiftDown" -> `std::__introsort_loop` over the same (key, intelligent pointer) vector map.
- 006b0020 "ComputeAngleHeading" -> `CTCPhysicsBase::GetTurnSpeedAngleYZ() const`.
- 006b6010 "CFeatDef_CopySpecializedData" -> `CIDrawEnvironmentParams::CThemeChannel::CThemeChannel(const CThemeChannel&)`.
- 006b6990 / 006b6b30 "CFeatDefVector_*" -> `std::vector<CThemeChannel>::_M_insert_overflow` / `_M_fill_insert`.
- 006b7380 "CGameWorld_Destructor" -> `CEnvironment::~CEnvironment`.
- 006b7e20 / 006b8050 -> `std::vector<CGameMapCell>::operator=` / `_M_fill_insert`.
- 006b91a0 "Faction_ExportToString" -> `CPersistTraits<CFaction>::TransferOut`.
- 006b9610 "CFaction_LoadFromPersist" -> `CFactionManager::LoadGameState(CPersistContext&)`, from the `ret 4` and the
  "Faction"+index context transfer.
- 006bb040 "Heap_PopRecursive" and 006bf570 "Heap_SortWideStringElements" -> `std::__introsort_loop`.
- 006bb390 "Game_IsTimeOfDayInRange" -> `CGameTimeManager::IsTimeBetween(long, long) const`.
- 006bd140 / 006c32b0 "RBTree_Insert" / "TreeSet_InsertStringKey2" -> `std::_Rb_tree::_M_insert`.
- 006bd9b0 "CMusicManager_CopyAssign" -> a CMusicManager state assignment with a bool force argument (`ret 8`; not
  operator=). The name is descriptive.
- 006c4270 "FindFirst_DeadThing" -> `std::__find_if<CIntelligentPointer<CThing>*, CIsThingDead>`.
- 006d8830 "Experience_ProcessMultiHitCombo" -> `CTCHeroExperience::AddSuccessfulMultistrikeHit`.
- 006dc350 / 006dcfa0 "CHeroExperienceDef_Write/ReadTreeToStream" ->
  `GFSerialiseMapBinaryOut/In<CComboMultiplierData, long, less<long>>`.
- 006dccc0 "PairVector_Resize" -> `std::vector<CAttackHistoryCombo>::_M_fill_insert`.
- 006e5100 / 006e6bc0 -> `std::vector<CAnimInit>` / `std::vector<CConversationLine>::_M_insert_overflow`.
- 0069a1a0 "CGameCameraManager_PopulateListFromData" -> `CGameCameraManager::InitSpecialCameraList`.
- Callee 0x0066AE10 -> `CTCSoundPlayer::IsAttachedSoundPlaying(unsigned long) const`. The landed
  `rebuild/src/compiled/00/66/CGameScriptThing_IsSamplePlaying_0066ae10.cpp` calls it `CGameScriptThing::IsSamplePlaying`
  instead. The evidence for the real name:
  - 006e3440 and 006e31e0 call it on the TC 0x10 component, with the same `this` as
    `CTCSoundPlayer::SetSpecificSampleVolume`;
  - Ego_r.pdb CTCSoundPlayer has PAttachedEmitter (CIntelligentPointer<CThingSoundEmitter>) at +0xC;
  - its callee 0x836870 is `CThingSoundEmitter::IsSamplePlaying`.
