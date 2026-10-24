# Hand chunk 376 (_Rb_tree<UID, CPersistedDeadEntity>::_M_insert .. vector<pair<float, CWideString> > copy ctor): 40 of 40 byte-exact

All 40 rows grade RELOCATION_MATCH (38) or MATCH (2: 0x0052DEC0, 0x0055E7F0) through gall.sh. All 40 tests pass
under g++ (tall.sh) and under VC7.1 (tall.py). No row was parked and none was skipped. The chunk spans five retail TUs:
- the CThingManager / thing-class TU (0x00529xxx-0x0052Exxx)
- the NUISystem component TU (0x00530xxx-0x0053Cxxx)
- the text / front-end TU (0x00542xxx-0x0054Fxxx)
- the redefiner / hoverable / observable TU (0x00556xxx-0x0055Cxxx)
- the photojournal and online-scoreboard TU (0x0055Cxxx-0x00566xxx)

## What matched (40)

STLport containers of CThingManager and NThingManager::CEntityRuntimePersistence:
- 0x00529150 / 0x005292F0 / 0x005293D0 / 0x0052E0E0 `_Rb_tree::_M_insert` for four maps and sets:
  - map<UID, CPersistedDeadEntity>
  - UIDToThingMap
  - NPCNameMap
  - set<NUISystem::EEvent> (CObserver::EventsToObserve)

  All four are STLport's `_M_insert` verbatim. `_M_create_node` is the null-tested placement copy, which is
  duplicated in both arms. Writing the pair's memberwise copy ctor gives the word moves.
- 0x00529230 `_Rb_tree::_M_copy` (dead-entity map): STLport's recursive-right / iterative-left structural copy with
  an inline `_M_clone_node`.
- 0x0052A010 `_Rb_tree::equal_range` (UIDToThingMap): the arguments are evaluated right to left, so the inlined
  `_M_upper_bound` walk runs before the out-of-line `_M_lower_bound` call.
- 0x0052B8B0 NPCNameMap `map::operator[]`: STLport's
  `lower_bound; if (end || k < key) insert(i, value_type(k, _Tp()))`. `_Tp()` is built first, and the hint iterator
  goes through its user copy constructor.
- 0x0052B5E0 / 0x0052BD10 `__copy` (const and non-const instantiations, byte twins); 0x0052BBE0 `__uninitialized_copy`;
  0x0052BD70 `__destroy_aux`. These cover the CEntityScriptInfo records:
  - CCharString + CArray<unsigned char> (12-byte STLport vector) + long + bool, 24 bytes.
  - The explicit instantiations are compiled `/Gr` (`__fastcall`).
- 0x0052DEC0 `__find` over set<EEvent> (MATCH): the iterator's ++ is written as STLport's static
  `_M_increment(node)` returning the node. A member `_M_increment()` swaps esi/edx.
- 0x00566CD0 vector<pair<float, CWideString> > copy constructor: STLport `_Vector_base(n, a)` with the
  `_STLP_alloc_proxy` end of storage, then `__uninitialized_copy` with `__false_type`.

Thing classes:
- 0x0052AC10 CThingBuilding::Alloc, 0x0052AC90 CThingHolySite::Alloc, 0x0052AD50 CThingPhysicalSwitch::Alloc: each is
  `return new (false) T;`.
  - Thing types 3 / 7 / 12, through CThing's static `operator new(size, bool)` 0x004C7380 and the CThingPhysical(long)
    ctor 0x005296B0.
  - Retail CThingGameObject is 0xD0 bytes: PThingStandingOn at +0xC0, the flags at +0xC8.
  - The building's bitfield clear sits in the ctor body. It is scheduled before the derived vtable store.

Engine:
- 0x0052ADA0 CCompressorZlib::CCompressorZlib(): `CACompressor(GetWorkspaceSize())` then `Init()`. The retail object
  is 0x58 bytes (the PDB says 0x5c).

NUISystem:
- 0x0052E530 / 0x0052E580 CUIScreenUnitConverter::UnConvertVector / ConvertVector:
  - `static C2DVector __fastcall (C2DVector)`.
  - Each axis is written as `r = v / div; r *= mul;` in two statements. The one-expression form lets VC reassociate
    the two globals first.
- 0x00530F30 CComponent::FindAllChildren(CCharString, CRetailList<CComponent*>&):
  - Slot 5. Name at +0x20, children at +0xB0.
  - The CCharString `==` is inline over Compare 0x00411570. The by-value name is copied per recursive call.
- 0x00533CC0 CComponent::AddChild(CCountedPointer<CComponent>&): `Children.push_back(child)`, then
  `child->SetParent(this)` (slot 51), then `child->Initialise()` (slot 43).
- 0x0053CFC2 vector<map<unsigned long, CUIState> >::push_back (CList::ScrollingChildrenStates):
  - An explicit instantiation in an `optimize("y", off)` + `optimize("s", on)` TU, emitting only push_back.
  - The odd address is real: /Os does not pad.
- 0x00542730 CIconText::ClearIcons (slot 148):
  - `Icons.clear()` is `erase(begin, end)`, which reaches `__copy_ptrs` and then the out-of-line `__copy` 0x00543700.
  - Then `for_each(handles, mem_fun_ref(&CEnginePrimitiveHandle::Clear))` 0x0043F360, with the functor returned
    through a hidden pointer.
- 0x0054E350 SetPAcceptButtonRecursive: `GetChildren()` (slot 53) is re-called for every use, and SetPAcceptButton
  is slot 7.
- 0x0054F4B0 CText::GetCharWidthW:
  - The CSystemManager font manager goes through a POINTER accessor `GetFontManager()`. This puts the
    GFGetSystemManager call between the temporary construction and the pushes.
  - The implicit `const char*` -> CCharString conversion gives the `lea`.
- 0x00556470 CRedefinerList::Cancel and 0x005565C0 CRedefinerList::ResetKeysToDefaults(bool):
  - Static members over s_pActiveList 0x013B8AC4 and g_pSoundSystem 0x013B8394 (SetGroupVolume is slot 46).
  - The two consecutive `if (s_pActiveList)` tests produce retail's threaded null tests.
  - FadeList is inline.
  - The accept button is reached through the CObserver base (slots 2/3) and the colour through slots 13/14.
- 0x005578A0 AreAllowedToCoexist(EGameAction, EGameAction): a switch with two case groups. I checked the jump table by
  hand. The byte index is 8->0, 26->1, 31->0, 45->0, 86->1, every other action ->2. The dwords
  {0x005578B6, 0x005578C8, 0x005578D5} pair with ours ($L283 +0x16, $L286 +0x28, $L280 +0x35).
- 0x00559040 CDraggableInto `??_G` and 0x0055ABA0 CDraggable::~CDraggable:
  - The `delete` of an owned STLport action list is inlined: clear 0x005356A0 + sentinel free.
  - 0x005359A0 is that list's own `??_G`, pulled in by a `delete` helper.
- 0x0055BB40 CHoverable::GIsAnyAlreadyHovered:
  - Written as `component->GetHoverPriority() >= hovered->GetHoverPriority()`. VC calls the right operand first and
    emits `cmp a,b`.
  - Slots: 129 GetHoverPriority, 132 CanBeHovered, 142 IsHovered.
- 0x0055CBE0 CObservable::SetOwnedEvent(CObserver*, EEvent) (slot 1):
  - It copies the Observers list: the STLport list copy ctor inlined, then the out-of-line `_M_insert_dispatch`
    0x0055CE90.
  - It then calls ProcessEvent on the owner when it observes the event.
  - It matched on the first grade with the GOTCHAS recipe: a same-TU noinline `_M_insert_dispatch` that never writes
    the head word (landed 0x00A84470).

Photojournal and scoreboard:
- 0x0055CFB0 CTCPhotojournalCapture::AddDescription(filename, description):
  - Static `__fastcall`. It appends the UTF-16 caption and then its character count to the screenshot file.
  - The local CDiskFileWin32 view is the landed WriteRecoveryFile runtime model, with an inline slot-9 GetLength.
- 0x0055DE40 CTCPhotojournalCapture `??_G`: an inline virtual dtor calling RemoveFromGlobalTCs, then the member dtors,
  pulled in by a `new T` factory.
- 0x0055E5F0 MD5 (anonymous namespace, /GS): md5_init / md5_append / md5_finish (Deutsch md5.c, extern "C"
  `__fastcall`), then `_snprintf("%02x")` per digest byte.
- 0x0055E7F0 CTCHeroOnlineScoreboard::IsScoreboardEmpty (MATCH): the map walk with the static `_M_increment`.
- 0x0055F090 / 0x0055F140 StdDecryptString / StdEncryptString (anonymous namespace):
  - Shape: `vector<char> buffer(size [+32], 0)`, then DecryptBuffer/EncryptBuffer inlined into CryptoMatic 0x0055E2C0,
    then `std::string result(&buffer[0], &buffer[0] + length); return result;`.
  - Matched with same-TU stand-ins for:
    - the vector fill ctor (0x00567560, which itself grades RELOCATION_MATCH from the same body)
    - `_M_range_initialize` (landed 0x00567D00)
    - `_M_throw_length_error` (landed 0x00565A30)
  - With these, VC knows neither local's address escapes. The local string then reuses the vector's frame slot and
    both start pointers stay in registers (frame 0x10 instead of 0x1c).
- 0x005472A0 GFRandomNoRepeat(range, previous, seed): `while (range > 1 && result == previous)`, an inline GFRandom
  (`seed * 0x24a1 + 0x24df`, GFROR13 0x00497890), and OutputDebugStringW after 100 retries.

## Parked for Fable 5.1 (0)

None.

## Skipped

None. No row holds two functions. No row looks cut wrong: 0x0053CFC2 starts on an odd address because its /Os TU does
not pad, and the explicit instantiation reproduces it from its first byte.

## Label corrections

Chunk rows (catalogued -> actual):
- 0x00529150 CRedBlackTree_Insert_With_Clone -> `_Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity> >::_M_insert` (CEntityRuntimePersistence::PersistedDeadEntities)
- 0x00529230 CCreatureAction_TreeClone -> the same tree's `_M_copy`
- 0x005292F0 CRedBlackTree_Insert_Compact -> `_Rb_tree<UID, pair<const UID, CThing*> >::_M_insert` (CThingManager::UIDToThingMap)
- 0x005293D0 ListNode_InsertSpline -> `_Rb_tree<UID, pair<const UID, pair<CCharString, CWideString> > >::_M_insert` (CThingManager::NPCNameMap)
- 0x0052A010 BinaryTree_InsertLowerBound_Uint64 -> UIDToThingMap `_Rb_tree::equal_range`
- 0x0052AC10 / 0x0052AC90 / 0x0052AD50 unscoped Alloc -> CThingBuilding / CThingHolySite / CThingPhysicalSwitch::Alloc
- 0x0052ADA0 ConvertPathToFullPath -> CCompressorZlib::CCompressorZlib()
- 0x0052B5E0 / 0x0052BD10 _Move<CRegister*,CRegister*> (propagated) -> `__copy<const CEntityScriptInfo*>` / `__copy<CEntityScriptInfo*>`
- 0x0052B8B0 CParticleMorphs_Load -> NPCNameMap `map::operator[]`
- 0x0052BBE0 CCharStringVector_CopyAssign -> `__uninitialized_copy<const CEntityScriptInfo*, CEntityScriptInfo*>`
- 0x0052BD70 _Copy_backward<CPixelFormat*,CPixelFormat*> -> `__destroy_aux<CEntityScriptInfo*>`
- 0x0052DEC0 BSTIterator_FindNextNodeWithValue -> STLport `__find` (set<EEvent> iterators)
- 0x0052E0E0 CRedBlackTree_Insert_Int -> set<NUISystem::EEvent>::_M_insert
- 0x0052E530 / 0x0052E580 ConvertX -> NUISystem::CUIScreenUnitConverter::UnConvertVector / ConvertVector
- 0x00530F30 ParticleMorphs_Find -> NUISystem::CComponent::FindAllChildren
- 0x00533CC0 Vector_PushBack_Or_Reallocate -> NUISystem::CComponent::AddChild
- 0x005359A0 EventList_RemoveAndCleanup -> list<CCountedPointer<NUISystem::CAction> > `??_G`
- 0x0053CFC2 UIScrollList_AddChild -> vector<map<unsigned long, CUIState> >::push_back (CList::ScrollingChildrenStates +0x17C)
- 0x00542730 GetPackedNormalFromTargetVertex -> NUISystem::CIconText::ClearIcons
- 0x0054F4B0 GetCharWidthW -> NUISystem::CText::GetCharWidthW; 0x00556470 Cancel / 0x005565C0 ResetKeysToDefaults -> NUISystem::CRedefinerList::
- 0x00559040 CHoverable_Destructor -> NUISystem::CDraggableInto `??_G`
- 0x0055ABA0 CHoverable_VectorDeleting_Destructor -> NUISystem::CDraggable::~CDraggable (`??1`)
- 0x0055BB40 GIsAnyAlreadyHovered -> NUISystem::CHoverable::GIsAnyAlreadyHovered
- 0x0055CBE0 LinkedList_FilterByPredicate -> NUISystem::CObservable::SetOwnedEvent
- 0x0055CFB0 AddDescription -> CTCPhotojournalCapture::AddDescription (static __fastcall)
- 0x0055DE40 CTCBase_Destructor -> CTCPhotojournalCapture `??_G`
- 0x0055E5F0 MD5 -> `(anonymous namespace)::MD5` (Ego_r.pdb `?MD5@?A0xf010c0d0@@YI?AVCCharString@@ABV2@@Z`)
- 0x0055E7F0 GUI_FindNonEmptyNode -> CTCHeroOnlineScoreboard::IsScoreboardEmpty
- 0x0055F090 CryptoDecrypt_ToStruct -> `(anonymous namespace)::StdDecryptString`
- 0x0055F140 CryptoEncrypt_ToStruct -> `(anonymous namespace)::StdEncryptString`
- 0x00566CD0 Vector_Copy_Uint -> vector<pair<float, CWideString> >::vector(const vector&)

Callees:
- 0x00543700 `_Copy_impl<CActionInputControl*>` -> `__copy<CIconText::CEmbeddedIcon*>`
- 0x0043F360 `_For_each<pair<long,CCharString>>` -> `for_each<..., mem_fun_ref_t<void, CEnginePrimitiveHandle> >`
- 0x0053CB9B list_construct_from_list -> the map<unsigned long, CUIState> copy ctor
- 0x0053CEF6 CActiveFile::Read_CArrayElements -> vector<map<...> >::_M_insert_overflow
- 0x009E2C80 CMessageEventFilter operator() -> CFontManager::GetFont
- 0x0055CE90 LinkedList_CopyElements -> list<CObserver*>::_M_insert_dispatch
- 0x00A3A7D0 AddChildPrimitive -> CCompressorZlib::GetWorkspaceSize (static)
- 0x00A3AEA0 CPreallocTexturePool -> CACompressor::CACompressor(long)
- 0x005296B0 -> CThingPhysical::CThingPhysical(long)
- 0x00A3DF00 COpinionTransientOffset::COpinionTransientOffset -> md5_init (`@md5_init@4`)
- 0x00567560 Vector_Fill_Byte -> vector<char>::vector(size_type, const char&, const allocator&)
- 0x00567D00 CTCElectrocutedEffect::InitFromStringRange (propagated; already landed as CNetString_Retail::RangeInitialize)
  -> basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&)
- 0x0099AF10 CCharString::operator_char_const* / 0x0099B220 CCharString::GetLength (propagated) -> also called on
  CWideString here (AddDescription); the landed rows name them CWideString::operator const wchar_t* / GetLength

Catalogue gaps (functions missing from rebuild/manifest/functions.tsv; fn.py attributes them to the previous row):
- 0x0055E4F0 DecryptBuffer
- 0x0055E510 EncryptBuffer
- 0x0055F1F0 DecryptString(CCharString)
- 0x0055F2D0 EncryptString(CCharString)
