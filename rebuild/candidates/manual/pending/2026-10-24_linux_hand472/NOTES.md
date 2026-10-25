# Hand chunk 472 (_Sort_heap<std .. _global): 35 of 40 byte-exact

All 35 sources in src/ grade RELOCATION_MATCH through gall.sh (grades.txt) and every test prints `<addr>_TEST PASS`
under g++ (tall.sh) and VC7.1 (tall.py). Five rows are parked in differ/; none skipped. Owner rule 2026-10-25 applied:
every source, test, parked file and `// Leaf:` line now uses namespace `std` (no `_STL` left); all 35 were regraded
after the conversion. Call targets checked with calls.py: malloc 0xBFEA0E / free 0xBFEA14 / scalar new 0xBFEA1A as retail,
and no modelled symbol binds two retail addresses (the two `""` literals of 00402130 / 004138d0 are now modelled separately,
see below).

## What matched (35)

### STLport 4.6 vector growth family (24 rows)
- `_M_insert_overflow(pos, x, const __false_type&, n, atEnd)`: 004117c0 (pair<float, CSubtitledMessage::CCameraTag>, 28-byte),
  00412330 (CWideString), 004125a0 (CDateAndTime), 004413c0 (CEnginePrimitiveHandle), 0046aecc (CSoundBankEntry),
  00477169 (CEnvironmentThemeDef), 0047800e (CEngineLocalDetailObjectDef), 0047c030 / 0047c140 / 0047ccd0 (CHeroLogBook
  objective / entry / quest). Shape: `len = old + max(old, n)` through the const-reference `max` (old size homed in a frame
  or `push ecx` slot), `allocate = n ? malloc : 0`, the null-checked placement `_Construct`, the trivial or inline/out-of-line
  `_M_clear`. Lesson: VC7.1 homes an empty tag on the LIVE `atEnd` argument slot only when the tag-taking helpers have same-TU
  template bodies with the tag parameter unnamed; `inline` versus `__forceinline` on the copy/fill helpers decides which of
  them the /Ob budget keeps out of line (00412330 inlines the head copy and calls the tail copy; 004125a0 needs all forced).
- `_M_fill_insert(pos, n, x)`: 00411db0, 004126e0, 004333b3, 00442350, 004676ef (unsigned char: POD helpers, `rep stosd ;
  rep stosb`), 00469012, 0046ad59, 0046c022, 00472c2a, 00476212. Shape: STLport verbatim; `x_copy` homed in the dead `x`
  slot or in the frame, the inline `__copy_backward`/`fill` loops, only the `_M_insert_overflow` call materialising its tag
  (`xor eax,eax ; stosb`) when the class has no user tag constructor.
- `operator=`: 00411b90, 00477ab7. Shape: `_M_allocate_and_copy` / `__copy_ptrs` inlined, tags homed on the live `x` slot.

### STLport heap (1 row)
- 00993130 `std::sort_heap<pair<long, CSoundBankRecord>*, CSoundBankEntryLess>` (/Gr): `while (last - first > 1)
  pop_heap(first, last--, comp)` with `pop_heap -> __pop_heap_aux -> __pop_heap` inline (the implicit pair copy inlined) and
  `__adjust_heap` 0x00992B80 out of line; explicit instantiation at the end of the TU.

### Win-module startup / settings globals (8 rows)
- 004017b0 GFConfigDetection (/GS /Gr): early returns for LoadLibrary / CreateConfigDetection, then the success path NESTED
  inside `if (initVideoCardInfo(..))` -- that moves the shared MessageBox failure block and the return to the end.
- 00402130 FableCheckMyDocumentsWritePermissions (/Gr): `__forceinline` STLport wide `operator==` with an index-loop
  `char_traits::length`, the CWideString `== L"..."` written as an if/return pair (a ternary widens the && arm to `mov ebx,1`).
- 004023f0 GetWindowTitle (/Gr): no NRVO in VC7.1; retail's order (result temp, ~tag, delete bank, ~title) comes from an
  `auto_ptr<CDataBank>` declared between title and tag and `reset(new ...)`, and `return title + L" ";`.
- 00406350 GetValidProfileName: each Replace a `__forceinline` helper with two NAMED string locals.
- 004138d0 GFParseCommandLine (/Gr): the parser's file name is an explicit `CCharString("")` (constructed after the later
  arguments are pushed); `EqualsNoCase(const CCharString&)` called with an implicit conversion so the literal temps get
  their frame slots in source order.
- 00415070 NLocalisation::GetLanguage, 00415640 NLocalisation::GetUIGraphics (jump tables verified by hand against
  0x004157E8 and the byte table), 0042f5a9 CNewFrontendGameComponent::CompileDefsPlatformSpecific (`optimize("s")` +
  `("y", off)` frame-pointer TU).
- 00415e85 GFLogMemoryUse (/GS /Gr): the biased /Os frame `lea ebp,[esp-N]` comes from `#pragma optimize("s", on)` ALONE
  (adding `("y", off)` gives `mov ebp,esp`).

### Input / GUI (3 rows)
- 00431c30 NUISystem::CUIIconsDef::Transfer: 32 `CPersistContext::Transfer<unsigned long>` calls, speed TU.
- 00446720 GetActionName (/Gr): switch on the controller type, 12-case mouse switch; jump-table offsets verified by hand
  (0x40, 0x58, 0x70, 0xf1, 0xf8, 0xff, 0x106, 0x88, 0x9d, 0xb2, 0xc7, 0xdc) with each case's literal pair.
- 0047dc69 PCLoad (/Gr): frame-pointer size TU (`optimize("y", off)` + `("s", on)`, as the landed CloseInGameMenuPC);
  globals 0x013B87A8 (yes/no dialog) / 0x013B87A4 (mouse pointer) go to the manager as their CObserver sub-object
  (`neg/sbb/and`); the menu pointer local is zeroed BEFORE the hero fetch (`xor ebx,ebx` ahead of GetMainPlayer; declared
  after it, the zero lands just before the bitset test); the menu is dereferenced without a null test, as retail does.

## Parked for Fable 5.1 (5)
- 00405220 TransferIMEColour DIFFER(290v290): retail loads the colour dword once for the return packing (`mov ecx,[esp+14h] ; movzx edx,ch ; movzx ecx,cl`), ours loads G and B as bytes | tried: initialiser list, B-first and A-first constructor bodies (19 forms in a sweep.py probe sweep, 4 counted grades) | next: every form that gives the dword-load tail misses the head's `shr eax,8 ; mov edx,eax ; shr edx,8` split -- try a union/bitfield colour type, else lane gap: register split
- 0040cec0 CUserProfileManager::CUserProfileManager DIFFER(981v977): the 50-name fill's CWideString() temp at [esp+20h] / ref at [esp+18h] (retail 18h/1Ch), the defaults release `mov edi,[eax+4] ; dec edi ; cmp edx,ebx` (retail `dec dword ptr [ecx+4] ; jne`), `mov ecx,[eax+60h]` before the modes push (retail `mov eax,[eax+60h]` ... `mov ecx,eax`), and the inline CDisplayModeDesc copies' load order | tried: plain vector model; C2DExtentsI user copy ctor (fixes the in-place argument build and the loop registers); real `_Vector_base`/`_STLP_alloc_proxy`/`_Deque_base` (fixes the push/lea scheduling); field-wise compares (the PDB has no CDisplayModeDesc::operator==); CCountedPointer operator-> + a named `system` local (fixes AddDeviceResetCallback, 989v977) | next: CDefPointer release through the landed `DecRef(); if (GetRefCount()==0) Delete()` model with a `long` count, the d.cpp named-local AddDeviceResetCallback, and `vector(size_type n, const T& v = T())` to reorder the fill's frame slots
- 00437d10 CPlayerGui::FormatStringForArea DIFFER(572v583): ours enregisters the running line width in edi (spilling `area` and using a separate fild temp), retail keeps the width in [esp+10h] and `area` in edi | tried: plain body (564v583), inline helper returning by value (614v583), named word reference + newWidth (572v583), const-reference helper (572v583) | next: a same-TU noinline stand-in for the font's GetStringExtents (chunk 422: makes the width local "captured"), else lane gap: register allocation
- 00441020 std::vector<unsigned int>::_M_fill_insert DIFFER(276v277): retail keeps n in ecx and pushes edi after the capacity test; ours holds n in edi pushed at entry | tried: by-value fill_n value (needed for `rep stosd`), const& x_copy (279v277), _IsPOD tag copy, same-TU fill body, tag without ctor, reordered locals, /Oa, /Ow | next: lane gap: register allocation (scheduling-only)
- 00446d10 CGamePlayerInterface::IsControlInputSharedBetweenGameActions DIFFER(496v473): ours keeps the action key and the lower-bound entry in separate frame words (0x2C frame vs 0x24) and walks `lea ebp,[entry+4]` instead of ebp = entry with +4/+8 offsets | tried: plain, `inline` and `__forceinline` _M_insert_overflow, `__forceinline` overflow + push_back (this file) | next: the key as a block-scoped local whose slot the entry reuses (`{ EGameAction key = ...; entry = LowerBound(key); }`), else lane gap: frame packing

## Skipped
None (no row holds two functions).

## Label corrections
Catalog / manifest labels for chunk rows:
- 00993130 `_Sort_heap<pair<unsigned long,C3DHelperPoint>*, CKeyPairCompareLess<..>>` -> `std::sort_heap<pair<long, CSoundBankRecord>*>` (0x24-byte element; C3DHelperPoint is 16 bytes).
- 004117c0 "Std_Vector_Insert_Range" -> vector<pair<float, CSubtitledMessage::CCameraTag>>::_M_insert_overflow.
- 00411db0 "Std_Vector_Insert_Range_Dispatch" -> same vector's _M_fill_insert.
- 00412330 "Std_Vector_Insert_Range_Copies" -> vector<CWideString>::_M_insert_overflow (one COMDAT, 28 callers).
- 004333b3 "Std_Vector_InsertRange" -> vector<CCharString>::_M_fill_insert.
- 00442350 "Vector_InsertRangeWithRealloc" -> vector<CEnginePrimitiveHandle>::_M_fill_insert.
- 0046c022 "Vector_Insert" (CAppearanceModifierGraphics::CEntry) -> vector<CReverbEnvironmentEntry>::_M_fill_insert.
- 0047c140 "Vector_Reserve_And_Assign" -> vector<CHeroLogBook::CHeroLogBookEntry>::_M_insert_overflow.
- 0047ccd0 "Std_Vector_Insert_Range" -> vector<CHeroLogBook::CQuestDesc>::_M_insert_overflow.
- 00431c30 "InitialiseDefaultCameraTags" -> NUISystem::CUIIconsDef::Transfer (vtable 0x01230F54 slot 18).
- 00446d10 "ProcessCombatInput_MapInputsToActions" -> CGamePlayerInterface::IsControlInputSharedBetweenGameActions (Ego_r.pdb).
- 0040cec0 "CUserProfileManager_Constructor" is right; behavioural-only labels (right behaviour, STLport names missing):
  004125a0, 004126e0, 004413c0, 004676ef, 0046ad59, 0046aecc, 00472c2a, 00476212, 00477169, 0047800e, 0047c030, 00441020.

Propagated manifest labels on callees (wrong):
- 0x0099AED0 "CCharString" = CWideString::CWideString(); 0x0099B510 "~CCharString" = ~CWideString; 0x0099EAE0
  `_Cons_val<..EHeroMorphType..>` = ~CCharString; 0x0099E480 "GetDefaultVal<CAnimationSet>" = CCharString::ToWideString.
- 0x00992510 "~CEditFractal" = ~CSoundBankRecord; 0x00992630 "UpdateCleanupFlags" = CSoundBankRecord::operator=;
  0x00992B80 `_Adjust_heap<pair<long,EHeroAbility>*..>` = __adjust_heap<pair<long, CSoundBankRecord>*>.
- 0x009C85A0 "CCompressedMemoryDataInputStream" = NGameText::CDataBank::CDataBank(); 0x009C7A40 "GetBankHandle" =
  NGameText::CGroupEntry::GetText; 0x009CC410 FUN = CDataBank::LookupValue (one landed file calls it GetTagFromCRC).
- 0x0099CFE0 "PeekWaterHasType" (landed as CEngineMap::PeekWaterHasType) = CSystemRegistry::ReadBOOL.
- 0x009F2080 "$E2" = CMemoryAllocator::DumpLog (an empty `ret 4` in retail).
- 0x009A4EC0 "AddChildPrimitive" = GFGetSystemManager.
- 0x00449970 "Catch@0040d959" = CPlayerManager::GetMainPlayer; 0x00487DC0 `_Cons_val<..EHeroMorphType..>` = CPlayer::GetCharacterThing.
- 0x004083E0 "GetIconsDef" = CUserProfileManager::GetConfigOptionsDefaultsDef() const.
- 0x0040DDC0 `_Copy_impl<C2DLineF*,C2DLineF*>` = __uninitialized_copy<CDisplayModeDesc*> (16-byte copy, fastcall, `ret 8`);
  0x0040F4A0 FUN = deque<ETutorialCategory>::_M_initialize_map; 0x004124C0 FUN = vector<CDisplayModeDesc>::_M_insert_overflow.
- 0x00477BFC / 0x00473023 "OnReadFinished" (0x00477BFC also landed as "CActiveFile::OnReadFinished" / "StdList_Dtor") =
  vector<CEngineLocalDetailObjectDef> / vector<CEnvironmentThemeDef>::_M_clear.
- 0x0047C800 `_Move_backward<pair<unsigned long,CDefString>..>` (landed "DestroyAux_SNamedListRecord18") = __destroy_aux<CHeroLogBook::CQuestDesc*>.
- 0x00A0CB80 "GetAnimFilter" = the parsed item's (CWParsedItem) string getter.
- 0x009BA790 "ReadAsString" = CStringParser::ReadAsSymbol; 0x004046B0 "SortTreeRecursively" = ~CStringParserCommentList;
  0x00430300 "OnDie" = ~CParentDefClassBase; 0x009FC550 "~CMemoryBlock" = ~CResource; 0x00431020 "UpdateShadowScene" =
  CPersistContext::Transfer<unsigned long>; 0x0043368D "GFGetPreMainMemoryUsed" = CStartupDefinitionManager::Get.

Errors noticed in ALREADY-LANDED sources:
- rebuild/src/compiled/00/40/CUserProfileManager_GetIconsDef_004083e0.cpp: file/catalog name "GetIconsDef" is wrong (its
  own line 1 says GetConfigOptionsDefaultsDef; it returns the CONFIG_OPTIONS_DEFAULTS_DEF pointer).
- 0x0099CFE0 landed as CEngineMap::PeekWaterHasType (it is CSystemRegistry::ReadBOOL); 0x009F2080 landed as "$E2".
- 0x009CC410 is LookupValue in one landed file and GetTagFromCRC in another.
- 0x00477BFC landed twice under wrong names (CActiveFile::OnReadFinished, StdList_Dtor).
- Landed sources disagree on 0x009E1DA0: GFVXToAX (CRenderManager2D_Draw2DBoxOutline_009de140) versus GFAXToVX_009e1da0
  (CSubtitleRenderer_FormatTextForVWindow_0048f50c), while CSubtitleRenderer_DrawTextEmbeddedGraphics_0048f230 puts
  GFVXToAX at 0x009E1E10 -- check against FableWin.pdb at landing.
- global_SavedConfirmed_0047db73.cpp models the hero lookup with invented placeholder types (A / BSrc / C / D / Wrapper):
  it is g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCharacterThing() and the in-game-menu interface 0xC6
  (the CThing/CPlayer models of 0047dc69 here or of the landed NPlayerGui ClearSoundEffects 0x0064B668 fit it).
- Three landed asm_bake rows now have compiler-emitted C++ here: 004017b0, 00402130, 004023f0.
