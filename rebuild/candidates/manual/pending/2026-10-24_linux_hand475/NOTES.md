# Hand chunk 475 (std::vector .. CGameCameraManager): 31 of 40 byte-exact

All 31 src/ rows grade RELOCATION_MATCH through gall.sh (grades.txt). All 31 tests print `<addr>_TEST PASS` under g++ (tall.sh)
and the lane VC7.1 (tall.py). calls.py shows no import mismatches: malloc 0xBFEA0E, free 0xBFEA14, memmove 0xBFEAE6,
operator new 0xBFEA1A, __ftol2 0xBFEA70, and the /GS cookie 0x139C8A8 with its check at 0xBFE9F9.

## What matched (31)
- **STLport vector (11 rows)**:
  - `_M_fill_insert`:
    - 005e32f0 pair<ulong,CAnimationEntry>
    - 00617fd0 unsigned __int64 (POD: every tag is __true_type and the copies fold to memmove; x_copy needs `sub esp,8`)
    - 00618860 CUIMiniMapMarker
    - 00618990 CUIRegionInfo (x_copy is a `rep movsd` frame copy)
  - `_M_insert_overflow`:
    - 006182f0 CUIMiniMapMarker
    - 0066e370 C3DVector (the lane VC7.1 will not auto-inline a 4-argument loop helper, so the inlining is spelled out:
      `__forceinline` __uninitialized_fill_n and head copy, with the tail copy left out of line)
  - `operator=`:
    - 00669bb0 vector<const CAnimationEntry*> (memmove)
    - 00674520, 006753f0, 00675980 (non-POD, with out-of-line __uninitialized_copy / _M_allocate_and_copy as retail's TU has them)
  - 005fd9c0 CTCInventoryMap::AddLogBookSaveInfo: four inline push_backs; the growth members are same-TU noinline stand-ins
    that never touch the tag, so the `__false_type()` temporaries land in the dead argument slot.
  - SHAPE: STLport 4.6 verbatim in `namespace std` (`_Vector_base` + `_STLP_alloc_proxy`). Out-of-line helpers are same-TU
    `__declspec(noinline)` bodies under `auto_inline(off)`, so VC7.1 knows their clobbers and homes temporaries in dead
    argument slots.
- **STLport algorithms and trees (4)**:
  - 005e3b50 `__introsort_loop` and 00623c80 `__unguarded_partition` / 006243d0 `__adjust_heap` over CTestQuest: /Gr TUs
    written as explicit `__fastcall`. The 20-byte divide is `imul 0x66666667`. For `__adjust_heap`, topIndex lives in the
    `push ecx` slot.
  - 006969d0 `_Rb_tree::_M_insert` (map<CCharString, CCountedPointer<CActionEventBase>>): the implicit pair copy ctor inlines
    into `_M_create_node`.
- **CWorld game-event actions (2, /GS)**:
  - 00629de0 EAAnswerQuestion and 0062ce60 EALoadGameFromInGameMenu: a CMemoryBuffer with a 0x20-byte inline Data array
    forces the cookie, so use `// Flags: /GS`.
  - The interface fetch is the one-ternary out-parameter store (`*out = (T*)((it==last || type<it->type) ? last : it)->component`),
    which loads straight into ecx.
  - The chosen save name is a NAMED local copy: `lea`/push after the ctor. A temporary would push the ctor's eax.
  - The local `vector<CWideString>` destructor inlines only as `__forceinline ~vector() { _Destroy(..); }`.
- **CTCVillage (2, /Os frame-pointer TU: `#pragma optimize("y", off)` + `("s", on)`)**:
  - 00658716 GetTimes:
    - A `const CGameTimeManager*` local taken from `PeekWorld().PGameTimeManager` flips retail's ebx/edi tie. About 30
      statement or helper permutations did not.
    - `best`, declared after the zero stores, is homed in the dead endTime slot.
  - 0065ce9b OnSerialise:
    - set<long>::insert's pair needs a user ctor, so it returns through a hidden pointer instead of edx:eax.
    - OnSerialise must be `virtual` to get the PDB's `UAE` mangling.
- **CThingCreatureBase (2)**:
  - 00661d70 IsThingInFrontCone: one named C3DVector is normalised in place; comparing against `(float)cos(angle)` parks the
    cosine in a dead x87 slot.
  - 00664e50 IsThingInViewAngle: angles are turns, and NormaliseAngle is fully inlined five times. The offset is a full
    C3DVector whose dead Z store keeps its frame slot.
- **Sound / persistence / defs (4)**:
  - 0066ba80 CTCSoundPlayer::PlayAttachedDialogue: a NAMED CIVCountedPointer is homed in the dead textID slot, and an
    inline two-argument overload holds `found`.
  - 00676740 CPersistContext::Transfer<CRandomAppearanceMorph>: a two-case switch with returns, and the default value's dtor
    `__forceinline` at each exit.
  - 006768f0 CCreatureDef::~CCreatureDef: the implicit member-wise destructor over STLport maps and vectors.
  - 0067f9b0 CPhysicalPrimitiveInit::StatRegisterClass: three `push_back(CCountedPointer<..>(new T))`.
- **CTCCreatureOpinionOfHero (1)**: 006821e0 SetOpinionDeedMask. GetDef and the map `operator++` are same-TU noinline
  stand-ins, the scratch vector's zero fill is `rep stosd`, and the def pointer is homed in the dead defIndex slot.
- **Input (2)**:
  - 0068d5f0 PreProcessStickVector (anon-namespace `__fastcall`): the X result stays on the x87 stack across the Y
    computation, and the sign is an inline `v < 0 ? -1 : 1` long used by `fimul`.
  - 0068eb20 NInputProcess::IsThingToKeepStrafing: out-parameter vs pointer-returning GetInterface forms chosen per lookup;
    three set-if bools in bytes 1..3 of one frame word.
- **CGameCameraManager (2)**:
  - 00699bf0 ~CGameCameraManager: the default-def release goes through the pointee's inline `ReleaseTemporary()` (DecRef,
    then GetRefCount). Grade 1 looked like a "volatile candidate" dead load; it was this two-accessor form.
  - 0069a010 InitManualCameraList: same-TU noinline GetDef / `_M_insert_overflow` stand-ins, and a NAMED block-scope
    CCharString.
- **CTCQuestCompletionUI (1)**: 005e4860 SaveInitialValues. A same-TU noinline LowerBound body (it only reads *type) lets the
  interface keys share the dead questName slot, giving a one-`push ecx` frame.

## Parked for Fable 5.1 (7)
- 005eabc0 CTCTavernGame::PositionMesh DIFFER(495v511): retail spills Y/Z of the Forward^Up temporary to [esp+20]/[esp+24] and keeps X in st0 (ours keeps X and Y on the stack), and hoists `mov edx,[edi]; lea eax,[esp+10]; push eax` above the setter `je` | tried: member vs free operator*/^, copy ctor on/off, statement-wise += sum, named GetRight, inline RotateAboutUp/Forward, ternary/helper setter pairs, same-TU LowerBound stand-in (60 normalised lines left) | next: lane gap: x87 temp allocation + branch head-merge
- 00619a10 std::_Rb_tree::_M_copy DIFFER(301v299): frame only; retail keeps __top in x's dead argument slot (`push ecx` frame), ours opens `sub esp,8` | tried: forceinline twin clone/vector copy ctor (plain STLport keeps both vector copies as calls, 193v299), tag user ctor, 4 grades | next: lane gap: inliner (retail expands the vector copy ctor only at the loop clone)
- 006668a0 CThingCreatureBase::GetPossibleCombatAnimations DIFFER(686v673): ours gives the CDefPointer<const CAppearanceDef> its own slot (`sub esp,40`), where retail packs it with the spilled modeFilter (`sub esp,3c`), so every frame offset is off by 4; def release is `dec [ecx+4]; jne` in retail vs ours re-reading Object and `cmp [ecx+4],edi` | tried: same-TU noinline operator= (fixed the edi/ebp _M_finish caching, 272 to 185 lines), block / do-while / if / inline-helper (GetCombatAnimationSet) / full-expression-temporary / dtor-less / raw-pointer def holders, declaration orders, Release() member forms, named DEFAULT CCharString | next: find the def-holder form VC7.1 packs (perhaps the real GetCombatAnimationSet returning a reference from another TU), else lane gap: frame packing
- 006726b0 std::vector::_M_fill_insert DIFFER(311v307): ecx/edx swap in the inlined __uninitialized_copy cursor; the if-branch fill iterates ebp instead of a copy in eax; the else-branch fill reloads x_copy in the loop where retail hoists it | tried: plain inline / implicit instantiation (258v307), __forceinline helpers, 4 grades | next: lane gap: register allocation (retail inliner)
- 00681e60 CTCCreatureOpinionOfHero::SetPersonalityOverride DIFFER(626v626): one SIB byte, retail `lea ecx,[eax+ecx+10h]` vs ours `lea ecx,[ecx+eax+10h]` | tried: forceinline map ++, early-return bad-def path, cached list end, pointer fill loop, old-reference-first declarations (flip the SIB but hoist the oldDef load) | next: lane gap: operand order of a scaled-index lea
- 00693140 GetActionAnimSetList DIFFER(269v269): the loop's anim-set local is homed in the dead GetTC key slot [esp+10h], retail uses the dead `action` slot [esp+24h] | tried: function / if / for-body declarations, cast rvalue, local copy of action, function-scope key, GetInterface(T**) form | next: lane gap: frame slot choice
- 00698540 CGameCameraManager::UpdateEarthquake DIFFER(597v603): retail copies g_framesPerSecond to a temp before the loop and converts once in the preheader (`fdiv [fpsF]`), ours `fidiv [g]` (a named long local reproduces the hoist but takes its own slot, +4 frame); retail keeps falloff on the x87 stack across the !Global call (`fld 1.0 ... fstp st0; call`), ours spills it; the final add loads cameraPos.y/z first; the cameraPos copy goes through `mov eax,esi` | tried: const extern / cast / local fps (8 placements), same-TU falloff body, ternary / scope / double falloff, 6 strength groupings, free operator+, accessor Get/SetPosition, pack on/off (48 normalised lines left) | next: lane gap: x87 register allocation + slot packing of the hoisted temp

## Skipped
- 005fddd0 (catalog ConstructQuestsScreen_PastQuestsPC, 704 bytes): one function, not attempted. It is a quests-screen GUI
  builder with about 12 callees whose labels are propagated and unidentified (0x618100, 0xA01B50, 0x436AC0, 0x4B8050,
  0x44C6B0, 0x9AD390, 0x42B312, 0x41E5F2, 0x41DB1D, 0x61E6D0, deleter 0x428B10). Status SKIP means "not attempted" here,
  not a two-function row.
- 0062fe60 (catalog CParticleMorphs_ctor, 832 bytes): one function, not attempted. It is a large constructor with
  interleaved dead temporary stores; judged intractable in this wave.

## Label corrections
- 005e32f0 "std_Vector_Resize": STLport vector<pair<unsigned long, CAnimationEntry> >::_M_fill_insert (resize is 0x005E39E0).
- 005e3b50 "std_sort_IntrosortPartition20Byte": std::__introsort_loop over pair<unsigned long, CAnimationEntry>.
- 005e4860 "QuestCard_SetupDialog": CTCQuestCompletionUI::SaveInitialValues (PDB).
- 005fd9c0 "CHeroLogBook_AddLogEntry": CTCInventoryMap::AddLogBookSaveInfo (PDB).
- 00617fd0 "Vector_InsertRange": vector<unsigned __int64>::_M_fill_insert.
- 006182f0 "Vector_ReallocateForInsert": vector<CTCInventoryMap::CUIMiniMapMarker>::_M_insert_overflow.
- 00618860 / 00618990: vector<CUIMiniMapMarker> / vector<CUIRegionInfo>::_M_fill_insert.
- 00619a10 "TreeNode_Clone": _Rb_tree<ECategory, pair<.., vector<CHeroLogBookEntry> > >::_M_copy.
- 00623c80 "std_Partition_TestQuest": std::__unguarded_partition<CTestQuest*, CTestQuest, CTestQuestNameCompare>.
- 006243d0 "_Adjust_heap<...>" (Dinkumware spelling): STLport std::__adjust_heap.
- 00629de0 "CPixelShader_OnTextureUpdateEvent": CWorld::EAAnswerQuestion.
- 0062ce60 "CWorld_LoadGameState_Handler": CWorld::EALoadGameFromInGameMenu.
- 00658716 "GetSchoolTimes_ConvertFloatToInt": CTCVillage::GetTimes.
- 0065ce9b "Player_TransferPersistence": CTCVillage::OnSerialise.
- 00661d70 "CVertexBufferWin32_DistanceSqr": CThingCreatureBase::IsThingInFrontCone (descriptive name).
- 00664e50 "CVertexBufferWin32_CalculateViewingAngle": CThingCreatureBase::IsThingInViewAngle.
- 006668a0 "Animation_FilterAndCompile": CThingCreatureBase::GetPossibleCombatAnimations (PDB).
- 00669bb0 "Vector_Copy": vector<const CAnimationEntry*>::operator=.
- 0066ba80 "Sound_InitializeFromTextEntry": CTCSoundPlayer::PlayAttachedDialogue (PDB).
- 0066e370 "Vector_InsertRange": vector<C3DVector>::_M_insert_overflow.
- 006726b0 "Vector_InsertN_ulong": vector<CRGBColour>::_M_fill_insert.
- 00674520 / 006753f0 / 00675980 "Vector_Assign_*": operator= of vector<pair<EBodyLocation, CWoundMorphs::CEntry> >,
  vector<CVectorMap<unsigned long, CTextureMorph> > and vector<CArray<CBodyPartMesh> >.
- 00676740 "CRandomAppearanceMorph_Serialize": CPersistContext::Transfer<CRandomAppearanceMorph>.
- 006768f0 "CMemoryBlock_Destructor": CCreatureDef::~CCreatureDef.
- 00681e60 "SetPersonalityOverride_BuildOpinionTable": CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool).
- 006821e0 "CViewBase_SetOpinionDeedMask": CTCCreatureOpinionOfHero::SetOpinionDeedMask(long).
- 0068eb20 "IsThingToKeepStrafing": NInputProcess::IsThingToKeepStrafing.
- 006969d0 "TreeSet_InsertNode": _Rb_tree<CCharString, pair<.., CCountedPointer<CActionEventBase> > >::_M_insert.
- 00698540 "CGameCameraManager_UpdatePointLightAccumulation": CGameCameraManager::UpdateEarthquake (PDB name set
  `?UpdateEarthquake@CGameCameraManager@@AAEXXZ`).
- 0069a010 "CGameCameraManager_PopulateSwitchList": CGameCameraManager::InitManualCameraList (PDB).
- Callees:
  - 0x47E186 "AddLearnedExpression": the interface-0xC6 save-name getter, CTCHeroSaveGames::GetSaveGameNames (descriptive name).
  - 0x5DC020 "CVectorMap_LookupOrGetEnd": CAnimationSet::FindFirstAnimWithName.
  - 0x5E1EB0 / 0x5E1420: vector<const CAnimationEntry*>::push_back / _M_insert_overflow.
  - 0x662A00: CThingCreatureBase::GetRandomLoadedAnimFromName.
  - 0x686A80 "GetDisplayEngine": CTCBase::PeekWorld() const.
  - 0x49D870 (coverage "GFGetPreMainMemoryUsed"): GFGetCurrentFrame.
  - 0x4107C0 "GetCellBoundingBox": CPersistContext::Transfer(const char*, bool&).
  - 0x4AEF50 "IsBinary": CPersistContext::IsSaving.
  - 0x661287 "TransferIn": std::copy into a back_insert_iterator<vector<long> >.
  - 0x514230: set<long>::insert.
  - 0x49D850 / 0x487DC0: CWorld::GetPlayer / CPlayer::GetHeroCreature.
  - 0x49D9B0 "CountReferences": CWorld::GetSampleBank.
  - 0x7F1840 "Math_Distance3DExp": a `__fastcall(const C3DVector&, const C3DVector&)` quake distance falloff,
    exp(k * squared distance) (descriptive name).
  - 0x682310: a ClearPersonalityOverride-shaped helper (descriptive name).
- Landed EASkipCutScene: its +0x5EC member is TextBoxButtonB's sound criteria, not "TextBoxButtonASoundCriteria"
  (A = +0x5E8, B = +0x5EC in retail; PDB +0x624/+0x628).
