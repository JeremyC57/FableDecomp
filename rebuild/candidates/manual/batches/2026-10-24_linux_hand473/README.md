# Hand chunk 473 (Vector_ConstructElements_Unrolled .. TreeNode_EraseRange): 33 of 40 byte-exact

All 40 rows are `_global` catalog rows. The 33 matches are all RELOCATION_MATCH, and every test PASSes under g++ and
VC7.1. calls.py shows no import mismatch. Sources write STLport as `_STL` (they were authored before the 2026-10-25
`std` rule); the coordinator converts and regrades them at landing.

## What matched (33)
- **STLport vector `_M_insert_overflow`** (0049b480 CTexture, 004bb2e0 CQuestManager::CActivateQuestInfo, 004bb400
  pair<CCharString,CPersistInfo>, 004d1940 pair<float,CCharString>, 004ea5a1 CEngineGraphic, 00517ba0
  CNavigationPosition, 0051a430 CLevelLoader::CObject::CLevelFiles): reallocate, uninitialized-copy the prefix, fill
  n copies, copy the suffix, destroy and free the old block. Shape: STLport 4.6 verbatim over inline (not noinline)
  helper templates. VC7.1 itself inlines the prefix `__uninitialized_copy`, the `_Copy_Construct` and the fill loop,
  then runs out of inline budget and calls the out-of-line suffix copy, which is exactly retail's mix (0049b480).
  004ea5a1 comes from a size-optimised TU, where the helpers retail inlined are `__forceinline`.
- **STLport vector `_M_fill_insert`** (0049b5a0 CTexture, 004ea446 CEngineGraphic, 004ed161 CShopItemDef, 004edb3e
  CPrizeScoreDef, 004edfdc CMultiStaticMeshEntryDef, 004f9df8 CCardPositionsDef, 004fa277 CCardDef, 005149f0 unsigned
  short): the in-place arm (x_copy, then the elems_after split) plus the call to `_M_insert_overflow`. Shape: STLport
  4.6 verbatim. Every helper retail calls out of line is a same-TU noinline body under `auto_inline(off)`, so the
  `__false_type()` tags are homed in the dead `n` slot. The defs TUs (004ea446, 004ed161 ..) are `optimize("s")`, so
  the helpers retail inlined are `__forceinline` and the `_IsPODType()` / `_TrivialAss()` tags keep the
  `xor eax,eax ; stosb` fill. The def rows need the real copy ctor and dtor callees (`_Construct<CCardDef>` 0x4EC08A,
  ~CCardPositionsDef 0x4F9CFC).
- **vector<CMapInfo>::_M_range_insert<const CMapInfo*>** (00519cd0): the forward-iterator range insert. Shape: the
  STLport 4.6 member template verbatim. The same-TU noinline helpers never touch their tag, so the tags are homed on
  the still-live `first` argument slot.
- **CReplaceSerialise<New,Old>::ConvertPreviousUnknownVersionUp** (00514790 Level2/1, 00515e90 Level3/2, 00517ef0
  CFileFormatLevel/Level3, 005196e0 MapHeader5/4, 0051a5a0 MapHeader6/5): read the old version, convert it up and
  copy it into the new type. Shape: the landed 0x00518040 / 0x0051C2F0 template (`result(new T)`, a null `held`, the
  version byte vs `TOld::GetClassVersionNumber()`, an inline `ConvertFromOld`, held and result released in reverse
  order).
- **STLport algorithms**:
  - 0048cc60 `__find_if<CThing**, binder2nd<pointer_to_binary_function<...>>>` (/Gr): the random-access 4x
    unrolled loop. Shape: the noinline stand-in homes the tag in the dead argument slot.
  - 004b8e40 `__find<CCharString*>`: the random-access unrolled find.
  - 0051af80 `_Rb_tree<long, pair<const long, list<long>>>::erase(first, last)`: the whole-tree clear fast path,
    otherwise per-node erase. The leaf prefix matches both erase overloads; the range overload is the larger symbol.
- **CWorld**:
  - 0049e080 RenderUpdate. Shape: epsilon squared is kept on the x87 stack across both tests, the forward copy is
    declared before the up copy, and the shared Update call is duplicated into both arms.
  - 004a05c0 SaveGameState: resolve the profile path for a bare file name, take the memory-pool shutdown guard, save
    the world state, then either the hero state or delete the stale hero-state file. Shape: an explicit goto layout for
    the inline CWideString `== L""`, in this order: the wcscmp null arm, then the size + MemCmp_Unsigned16 arm, falling
    into the empty arm.
  - 004a0940 TeleportHeroToHSP (/Gr): copy the holy sites, `find_if` the one with the script name, and teleport the
    main player's character to it. Shape: the same CopyHolySites template model as the landed CPlayer 0x488B20, and
    named locals for the script interface and the player manager so they load before the arguments.
- **CWorldMap**:
  - 005079c0 LoadGameState: binary modes read the count, resize and transfer each region by name; text modes parse
    REGION/END_REGION blocks through a nested CStringParser and CPersistContext. Shape: a same-TU noinline
    TransferBinaryIn stand-in keeps the count in ebp. Inline const-char helpers give a lea-addressed "END_REGION"
    temporary. The CStringParser implicit dtor inlines the comment list clear.
  - 004fe7f0 GetSoundThemesUsedInCurrentRegion: collect the cell sound themes of every map in the current region into
    a local set<char>, then add the SwitchID of each resolvable SOUND_THEME def. Shape: an STLport pair with ctors plus
    the `set::insert` conversion inside loops gives retail's `and esp,-8` frame. A same-TU noinline `GetDef` stand-in
    keeps the def in edi.
- **CThing**:
  - 004c88e0 SerialiseThingInfo(bool, CCharString&, CSerialiseInfo&), static. Shape: BeginSerialising's two context
    set-ups are inline, and the parser's compiler-generated dtor stores no vtable.
  - 004c8a30 GetSubTypeFromScript, static. Shape: an inline CStringParser(const char*, filename = "") ctor, the inline
    `CCharString::operator==` with the `strcmp(text, "")` null arm, and the definition manager as a named local.
- **NGraphicAppearance::CTCGraphicAppearance::GetMeshObject** (004bc4f0). Shape: `interpolation == 1.0f` picks the
  overload and the common scale argument is pushed before the branch. MSVC lays an overload set out in reverse
  declaration order, so the by-reference GetRHSet() is declared first to land in slot 72.
- **CConversationAttitude::Init** (004cd670): 42 table assignments, generated from the retail disassembly.

## Parked for Fable 5.1 (7)
- 004b2ca0 CQuestManager::GetActiveQuestSectionNames DIFFER(856v832): ours builds an `and esp,-8` frame (ebp frame pointer) for the 8-byte insert_unique result returned through a hidden pointer inside the loops, under both RTM and QFE; retail keeps a plain 0x24 esp frame with `this` in ebx and the vector finish pointers in ebp/edi | tried: the shared STL model, a POD pair result, a non-POD pair returned directly, an aggregate `_Rb_insert_result` (best, in differ/) | next: discard the insert result inside a void noinline wrapper so no 8-byte temporary lives in the loop; else lane gap: aligned frame for a hidden-pointer pair return
- 004cea60 CSubtitledMessage::GetMessageFromTextIndex DIFFER(332v332): the same instructions, but retail keeps the text entry and speechIndex in esi and the speech bank in edi (ours swaps them) and pops edi later | tried: const-qualified declarations, a ternary vs if-assign speechIndex (if-assign is best, in differ/) | next: lane gap: register allocation (esi/edi swap)
- 004ff750 CreateRelevantPersonAndUpdateStats DIFFER(361v336): ours keeps the created creature in a `push ecx` stack local; retail keeps it in esi and homes it in the dead `init` argument slot | tried: an out-of-line push_back, a same-TU noinline push_back stand-in (best, in differ/), an inline helper, explicit instantiation | next: a same-TU noinline stand-in for the creature-create call so VC7.1 treats the result as uncaptured (the 005079c0 TransferBinaryIn trick)
- 004917b5 FormatTextWithGameActionNames DIFFER(-v797): not attempted; 797-byte ebp-frame body in an unaligned TU | tried: none | next: author from the retail disassembly in a later wave
- 004cebb0 GetMessagesFromTextName DIFFER(-v771): not attempted; a /Gr static with a jump-table mode switch that calls the parked 0x4CEA60 | tried: none | next: author with 004cea60's model once that row matches
- 00501150 LoadLevel_ParseObjects DIFFER(-v756): not attempted; `ret 20h`, counted-pointer and set construction | tried: none | next: author from the retail disassembly in a later wave
- 00502b00 AddToRelevantBuilding DIFFER(-v906): not attempted; `ret 0Ch`, list walks | tried: none | next: author from the retail disassembly in a later wave

## Skipped
None. No row holds two functions.

## Label corrections
- 0048cc60 Vector_ConstructElements_Unrolled -> `_STL::__find_if<CThing**, binder2nd<...IsThingScriptNamed>>`.
- 0049b5a0 Std_Vector_Insert_Range_CTexture -> vector<CTexture>::_M_fill_insert.
- 004a0940 NQuestManager_ActivateQuestWithFilters (also seen as CWorld::LoadHSP) -> CWorld::TeleportHeroToHSP (Ego_r PDB).
- 004a05c0 CWorld_SaveGameState -> CWorld::SaveGameState(const CWideString&, bool, bool).
- 004b2ca0 WorldMap_BuildLoadSectionsList -> CQuestManager::GetActiveQuestSectionNames (Ego_r PDB). The landed
  UninitializeDay/NightSection call it "BuildLoadSections".
- 004b8e40 VectorFind -> `_STL::__find<CCharString*>` (random-access).
- 004ed161 Pair_StringPair -> vector<CShopItemDef>::_M_fill_insert.
- 004edb3e CCardDef -> vector<CPrizeScoreDef>::_M_fill_insert. 0x4EDC9B is the CPrizeScoreDef copy ctor.
- 004fa277 ShoveHaPenny -> vector<CCardDef>::_M_fill_insert. 0x4EC08A is `_Construct<CCardDef>`.
- 0x4F9CFC is ~CCardPositionsDef and 0x430300 is ~CParentDefClassBase.
- 004fe7f0 CollectSoundThemeSwitchDefs -> CWorldMap::GetSoundThemesUsedInCurrentRegion (Ego_r PDB).
- 005079c0 LoadGameStateInternal_ParseRegions -> CWorldMap::LoadGameState(CPersistContext&) (Ego_r PDB).
- 005149f0 vector_insert_erase -> vector<unsigned short>::_M_fill_insert.
- 00514790 / 00515e90 / 00517ef0 / 005196e0 / 0051a5a0 CopyConstructor -> CReplaceSerialise<...>::ConvertPreviousUnknownVersionUp.
  0x512BC0 is CBaseSerialise<COldFileFormatLevel1,1>::ConvertPreviousUnknownVersionUp.
- 0x9C9070 is CDataBank::GetPTextEntry; the landed 0x5BD8A0 source uses another name.
- 0x9B9C00 is CStringParser::SkipToItem.
- 0x9B9BA0 "NextItemIs" is probably CStringParser::SkipPastString, and 0x9BA3E0 is probably ReadAsStringUntilPastString
  (both descriptive, not from the PDB).
- 0x4BBF00 is GetRenderPositionForThing.
- 0x4D1DB0 is ~CSubtitledMessage.
- 0x4ADCB0 is the CWorld-TU copy of the holy-site copy template, which the landed CPlayer model calls CopyHolySites.
- 0x13B89D0 / 0x13B89D1 are file-static save flags; g_SavingQuestScreenState / g_SavingHeroState are descriptive names.
