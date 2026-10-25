# Hand chunk 413 (GFXToY .. GetSizeVal): 37 of 40 byte-exact

## What matched (37)

- **Graphics globals** -- `GFXToY(float)` 0x9E23F0 (x * render-target height / width): two named display-manager
  locals, height source first, so both `GFGetSystemManager` calls run before either dimensions call.
  `GFGet2DDistance(C2DCoordI, C2DCoordI)` 0x9E2430 (`#pragma intrinsic(sqrt)` on the double of the int sum) and the
  float `GFGet2DDistance(C2DVector, C2DVector)` 0x9E2460.
- **CJoystick** -- `~CJoystick` 0x9E4420: a local model, because engine/CJoystick.h carries a 12-byte list. It has
  64 CInputEvents plus a sentinel-only maintained-event list whose inline dtor is `clear(); free(node)`; the class dtor
  body is empty.
- **list<CMaintainedEvent>** -- `erase(iterator)` 0x9E47E0 and `insert(iterator)` 0x9E4A20 (`insert(pos, _Tp())`):
  both are STLport verbatim with malloc/free nodes and the placement `_Construct`.
- **CSymbolMap** -- `FindSymbol(name, long&)` 0x9E5170, with the landed inline `Find` (end() read once, ternary).
  `GetSymbolsFromGroupName` 0x9E5290 uses the inline `CCharString::operator==` (same block, then length plus `Compare`)
  and `push_back` into a CArray.
- **STLport algorithms on vector<pair<CCharString, CSymbolInfo>>**:
  - `__unguarded_partition` 0x9E6C80, `__make_heap` 0x9E7100 and `__partial_sort` 0x9E7B40 (inline make_heap): all
    `__fastcall` explicit instantiations, with the comparator by value.
  - `vector::reserve` 0x9E72C0 needs a same-TU noinline `__uninitialized_copy`, or the frame words swap.
  - `CArray::DeallocateMemory` 0x9E7680 is the landed 0x6FC620 template form.
  - `vector::push_back` 0x9E7AB0.
- **CMemoryFile::Open(void*, long)** 0x9E93C0 -- the four member stores written directly (an inline `Open()` reorders
  them), then `resize(length, 0)` and an inline memcpy to `GetData()`.
- **vector<unsigned char> back-inserter** -- `back_insert_iterator::operator=` 0x9E98C0, and `__copy` (random-access,
  `_Distance*`) into it, 0x9E9900.
- **CDateAndTime::ToString** 0x9E9E30 -- an RAII 1024-byte text buffer (malloc + memset; free in its dtor), so the
  free lands after the result copy.
- **NProgressDisplay** -- `StartProgress(bool x6)` 0x9E9EF0 and `DisplayProgress(text, float, bool, bool)` 0x9E9F40:
  both `__fastcall`, forwarding through the global counted pointer after a null test.
- **CConsoleCommandParameters**:
  - `GetCorrectlyTypedParameter(long)` 0x9EA6C0: a default CGenericVar (body store `Type = 0` after the string ctor)
    whose Type is set from `ParamTypes[index]`, returned by value with the implicit copy, then the inline virtual dtor.
  - Ctors `(type)` 0x9EBB30 and `(type x4)` 0x9EBCB0: these need the STLport `_Vector_base` + `_STLP_alloc_proxy`
    model, or VC folds finish == end_of_storage and always overflows. Retail homes the `__true_type` tag on the live
    first-argument slot, so `_M_insert_overflow` is a same-TU noinline stand-in.
- **CConsole (retail offsets of the landed SetHistoryMode)**:
  - `ScrollToHistoryBeginning` 0x9EAC80: STLport `size()` = distance, evaluated per branch; bidirectional `__advance`
    on the member iterator.
  - `GetRegisteredConsoleInput` 0x9EB220: the inline `FindInput` wrapper of the landed RunCommandLine; the ToLower
    temporary sits in the dead argument slot.
  - `ConsoleCommandList` 0x9EB690 and `ConsoleVariableList` 0x9EB8E0: static `__fastcall`. Each walks the input map
    in order and compares the slot-3 type string with "command" / "variable" through the inline `operator==(const
    char*)`, whose null-block arm is `strcmp(text, "")`.
- **CGenericVar containers**:
  - `__uninitialized_copy<const CGenericVar*>` 0x9EE0A0 (`__fastcall`).
  - `vector<CGenericVar>::push_back` 0x9EFDF0: here x is loaded first, so the tag lands in its dead slot and a
    declared callee is enough.
- **list<CConsoleCommandLine>::insert(pos, x)** 0x9EEB00 -- the element copy calls the out-of-line `CBase()` DEFAULT
  ctor, so CConsoleCommandLine has a user copy ctor that does not name its base.
- **map<char, CCharString>::operator[]** 0x9EFCF0 (KeyBindingMap) -- STLport verbatim, plus two load-bearing inline
  layers: `_STLP_DEFAULT_CONSTRUCTED` as `__default_constructed((_Tp*)0)` (retail re-derives the temporary's address)
  and map::insert as an inline wrapper over `insert_unique` (retail re-reads the result from the frame). The iterator
  has a user copy ctor.
- **CCodeSectionManager**:
  - Ctor 0x9F0320: three STLport maps through `_Rb_tree_base` (allocate(1) in the body) and `_M_empty_initialize`,
    then `SectionLoadingEnabled = false`.
  - On the SectionDesc tree (map<unsigned long, CSectionDesc>): `_Rb_tree::erase(iterator)` 0x9F0C90,
    `_M_create_node` 0x9F0DE0 and `_M_insert` 0x9F0FA0 (STLport 4.5 verbatim), plus `operator[]` 0x9F1470, the twin
    of 0x9EFCF0.
- **GetSizeVal** 0x9F1C80 (anonymous namespace, `__fastcall` begin/end): a digits loop, a spaces loop, then a k/m
  suffix shift. VC7.1 emits the unreferenced anonymous-namespace function; the leaf is the `?GetSizeVal@` prefix.

## Parked for Fable 5.1 (3)

- 009e24b0 GFRecalibrateFastSubFrameTimer DIFFER(122v139): retail inlines the time-stamp read (`xor eax,eax ; xor edx,edx ; rdtsc`, edx:eax spilled and reloaded) where the C++ body calls GFGetCycleCount; everything else is identical | tried: C++ reference body (1 grade, retail_asm candidate) | next: lane gap: retail_asm (owner-graded C++ body with a minimal inline-asm rdtsc, as the landed CPUAheadCounterCallback 0x9E2540)
- 009e3f60 CPixelFormat::ConvertRGBToPFDData DIFFER(179v189): retail re-reads g_D3DFormatInfo[FormatIndex].{A,R,G,B}Bits once per channel with no ebp frame; ours CSEs the bit counts into registers and spills to the frame (add order and shifts already match) | tried: reference-local expression, accessor expression, accessor statements, inline ConvertRGBToPFD(colour, CPFDColour&) (not inlined) | next: `__forceinline ConvertRGBToPFD(const CRGBColour&, CPFDColour&)` into a local CPFDColour, then memcpy
- 009f18f0 CGameEventDataPackage::GetData DIFFER(166v166): register permutation only -- retail this=ebx, size=ebp, StartPos=edx (dest staged in edx); ours size=ebx, this=edx, StartPos=esi; the IsValidPosition int64 guard and both copies already match | tried: plain body, + IsValidPosition((__int64)(first + start)), + `end` local, + `start` local | next: reach Data through a local `unsigned char*` or write the test as `size > sizeof(Data) - start` to move this into ebx

## Skipped

None. Every row is one function, there are no int3-after-noreturn tails, and no row looks cut wrong.

## Label corrections

Row labels (catalog -> real):
- 009e2460 Calc_Distance2D -> GFGet2DDistance(const C2DVector&, const C2DVector&)
- 009e3f60 Graphics_CopyPixelsToBuffer -> CPixelFormat::ConvertRGBToPFDData
- 009e4420 JoystickGui_Destructor -> CJoystick::~CJoystick
- 009e47e0 EventNode_Unlink -> std::list<CMaintainedEvent>::erase(iterator)
- 009e4a20 CInputEvent_AllocNode -> std::list<CMaintainedEvent>::insert(iterator)
- 009e5170 SymbolMap_Lookup -> CSymbolMap::FindSymbol(const CCharString&, long&)
- 009e5290 CCombatWheel_TryInsertMatchingString -> CSymbolMap::GetSymbolsFromGroupName
- 009e6c80 Partition_QuickSort -> std::__unguarded_partition
- 009e7100 MakeHeap_Start -> std::__make_heap
- 009e72c0 Vector_Reserve -> vector<pair<CCharString, CSymbolInfo>>::reserve
- 009e7680 Vector_Clear -> CArray<SSymbolEntry>::DeallocateMemory
- 009e7ab0 Vector_Append_CCharString -> vector<pair<CCharString, CSymbolInfo>>::push_back
- 009e7b40 Partition_IntroSort -> std::__partial_sort
- 009e93c0 BinaryStream_LoadFromWide -> CMemoryFile::Open(void*, long)
- 009e98c0 Vector_PushBackByte -> back_insert_iterator<vector<unsigned char>>::operator=
- 009e9900 Vector_AppendRange -> std::__copy into a back_insert_iterator
- 009e9e30 Format_DateTimeString -> CDateAndTime::ToString
- 009ea6c0 Command_CreateAnonymousObject -> CConsoleCommandParameters::GetCorrectlyTypedParameter(long)
- 009eac80 LinkedList_RotateToIndex -> CConsole::ScrollToHistoryBeginning (best PDB fit: both window iterators reset to
  begin(); no xref proof)
- 009eb220 StringMap_FindLowerBoundAndToLower -> CConsole::GetRegisteredConsoleInput
- 009eb690 CConsole_ListCommandsCommand -> CConsole::ConsoleCommandList
- 009eb8e0 CConsole_ListVariablesCommand -> CConsole::ConsoleVariableList
- 009ebb30 Vector_PushElementOrReallocate -> CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType)
- 009ebcb0 Vector_InitAndAppendFour -> CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType x4)
- 009ee0a0 _Copy_backward<CExplosionRing*,CExplosionRing*> -> std::__uninitialized_copy<const CGenericVar*, CGenericVar*>
- 009eeb00 HeroInformationScreens_CreateListNode -> std::list<CConsoleCommandLine>::insert(iterator, const T&)
- 009efcf0 CCharMap_FindOrInsert -> std::map<char, CCharString>::operator[]
- 009efdf0 Vector_PushBackPolymorphicEntry -> std::vector<CGenericVar>::push_back
- 009f0320 NQuestManager_InitializeMapsList -> CCodeSectionManager::CCodeSectionManager
- 009f0c90 CMemoryAllocatorVariableSize_EraseElement -> _Rb_tree<unsigned long, pair<const unsigned long, CSectionDesc>>::erase(iterator)
- 009f0de0 RBTree_CreateNode -> the same tree's _M_create_node
- 009f0fa0 RBTree_Insert -> the same tree's _M_insert
- 009f1470 CCodeSectionManager_RegisterSection -> std::map<unsigned long, CSectionDesc>::operator[] (RegisterSection
  is the landed 0x9F05A0)
- 009f18f0 CircularBuffer_Read -> CGameEventDataPackage::GetData(long, void*)

Callee labels found on the way:
- 0x9E76F0 "CCombatWheel::InsertElements" -> vector<pair<CCharString, CSymbolInfo>>::_M_insert_overflow
- 0xA03B30 "$E2" -> the empty out-of-line CInputEvent::~CInputEvent
- 0x9E4710 "SortTreeRecursively" -> list<CMaintainedEvent>::clear
- 0x9EF900 "VectorInsertElementRange" -> vector<4-byte scalar>::_M_insert_overflow(pos, x, __true_type, n, atend)
- 0x9EF4D0 / 0x9F1180 -> _Rb_tree::insert_unique(iterator, const value_type&) for the KeyBinding / SectionDesc maps
- 0x42A34E / 0x42971D -> _Rb_global<bool>::_Rebalance_for_erase / _Rebalance
