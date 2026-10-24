# Hand chunk 379 (_global .. _global): 37 of 40 byte-exact

All 40 rows are catalogued as `_global`. Their real owners are shown below: STLport instantiations of the inventory TUs, NInventory::CTCInventoryBase / CCategory, CTCInventoryClothing, CTCInventoryStats, CTCInventoryItem statics and CAnimationSet. Line 1 of each source carries the real name. 7 rows are MATCH and 30 are RELOCATION_MATCH. Every test prints PASS under both g++ (tall.sh) and VC7.1 (tall.py).

## What matched (37)

- **STLport `__find_if`, random access (9).** The 4x unrolled trip loop is followed by a 3/2/1 remainder switch. They are `__fastcall` /Gr template TUs (first in ecx, last in edx), use the size TU, and take the predicate BY VALUE on the stack.
  - 005ba23a / 005ba2a3 search 12-/24-byte keyed records. A one-member predicate keeps its key in esi, so every test is `cmp [ecx],esi`. The stride is `push 0Ch/18h ; pop edi`.
  - 005c0653 / 005c06c3 search CCategory with CIsCategoryWithDefIndex (const and non-const twins). The retail CCategory is 0x2C (`push 2Ch ; pop edi`) and the compared field is GlobalDefIndex +0x1C.
  - 005c0965 uses `const_mem_fun_ref_t<bool,CItem>`. It has no frame pointer because ebp holds the stride.
  - 005c1153 / 005c11fa use `bind2nd(mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex), type)`. The ebp frame is there for the slot-relative calls.
  - 005c176a / 005c1818 use CIsSameItemType. The out-of-line `operator()` is addressed in its own by-value slot (`lea ecx,[ebp+8]`), and `last` is spilled to [ebp-4].
- **STLport vector internals (9).**
  - 005b3325 is `vector<CDefPointer<const CTattooDef>>::push_back`. `_Construct` must be `__forceinline` in this size TU, and the `__false_type` temp goes in x's high byte.
  - 005ba5ad / 005c0932 are `_Vector_base(n, a)` for 20-byte elements. They use the real `_STLP_alloc_proxy` end-of-storage (`lea edi,[esi+8] ; mov ecx,edi`), and allocate is out of line.
  - 005baa6b is the 16-byte twin with allocate INLINED: the zero register doubles as the null result.
  - 005bb4e6 is the `_M_insert_overflow` for a 4-byte class element. `(max)` works through references, so old size and the allocate result share [ebp-4].
  - 005c135e is `__copy_backward<CItem*>`. `result` is decremented in its argument slot because the assignment's left operand is evaluated first.
  - 005c1f42 is `vector<CItem>::operator=` and 005c2831 is `vector<CItem>::_M_fill_insert`. Both need same-TU helper bodies so VC sees that the tag and x_copy references do not escape; the tag temps go in the high byte of a dead argument slot.
  - 005d85de is the `vector<long>` copy ctor. `get_allocator()` is evaluated first (right-to-left) and homed at [ebp+0Bh].
- **NInventory::CTCInventoryBase (3).**
  - 005bd33c / 005bd3a0 are PlayInventoryMovementSound / PlayInventoryNoMovementSound. The sound player is fetched BEFORE the def pointer copy. The copy's add-ref is null-tested, but its release is hand-written with no null test.
  - 005bf4a3 is AddItem. The category end() is read into a local AFTER the lookup, and the `&&` tail shares `xor al,al`.
- **NInventory::CCategory (1).** 005bda1d is GetHighlightedItemText.
  - The inline `CWideString == L""` must be written as two caller tests that `goto` the empty arm: no data → `wcscmp(L"",L"")`, else the out-of-line wstring compare. A forced-inline version graded 170v168; a plain inline one graded 140v168.
  - GetCRC runs on an explicit `CCharString("TXT_OBJT_EMPTY")` temporary.
- **CTCInventoryClothing (1).** 005b425b is ResetMannequin (name inferred). It copies the hero's worn appearance modifiers onto the mannequin.
  - Early returns keep the CLongList in the function block, so its word is not packed with the GetInterface key: frame 0x10 matches, where nested ifs packed to 0x0C.
- **CTCInventoryStats (3).**
  - 005cc772 is GetXString (static fastcall). The arguments must be IMPLICIT conversion temporaries (`"TEXT_..."` → CCharString, `L"_"` → CWideString), which are built first and then pushed with a fresh `lea`. Explicit temporaries graded 225v240. It also needs `typedef unsigned short wchar_t`.
  - 005cd6b7 is ConvertMap. The loop is a for loop over a cached `end()` with a found flag, so the match block lands after the epilogue. The pair ctor is `__forceinline` (a plain inline stays a call: 167v176).
  - 005cdc1c is Copy. It uses the landed OnCreate GetInterface model: `push 68h ; pop edi` serves both the test and the key, the key is homed in the dead `other` slot, and the screen base is reached through a null-safe base conversion.
- **CTCInventoryItem statics (9).** These are static `__fastcall` def-cache getters.
  - 005d9320 is ClearCachedDefs.
  - 005d95c0 / 005d9630 are GetDescription / GetDetails. `operator->` at both uses gives retail's double field load; a plain local merges the loads (107v110). VC7.1 has no NRVO, so the test returns an unnamed temporary.
  - 005d96a0 is GetEngineGraphic: a 14-byte packed result copied dword/dword/dword/word.
  - 005d9730 is GetStartingRightHandedSet. CRightHandedSet has user ctors, so its hidden result is in ecx; a POD result graded 146v88. C3DVector's inline user copy ctor makes the outer copy non-trivial, and that copy inlines with the destination in esi.
  - 005d9a10 / 005d9aa0 are GetHairCardDef / GetTattooCardDef. They need the same-TU out-slot-only bodies of GetDef/GetSubDef; declared only, they graded 166v137.
  - 005d9b90 is GetContextSensitiveType, also with a same-TU GetSubDef.
  - 005d9c60 is IsClothing. GetInventoryType is inlined, and `== 0x12` is duplicated into both of its returns.
- **CAnimationSet (2).**
  - 005dc020 is FindFirstAnimWithName. GetCRC is inlined, with each accessor under its own null test. LowerBound's same-TU lower_bound body keeps the key in edi, homed in the dead `name` slot.
  - 005dc290 is GetNoAnims, which counts the equal-key run that passes the filter. The loop is entered under the end() test.

## Parked for Fable 5.1 (3)

- 005bc3ed (anonymous namespace)::GFGetGraphicFrame DIFFER(66v67): retail rounds with bare `fld dword [ebp+0Ch] ; fistp dword [ebp+8]` (the float goes in the dead `frame` slot, the int in the dead `scale` slot); ours calls `__ftol2` | tried: `(long)` cast of the scaled float in two naming/parameter-order forms (the /QIfist form gives `fistp qword`) | next: lane gap: retail inline-asm `fistp dword` (a `retail_asm` candidate under the CLAUDE.md exception, which this lane forbids)
- 005cc530 RoundToDecimalPlaces DIFFER(47v59): retail `fld [esp+0Ch] ; fistp dword [esp+4] ; fild dword [esp+4]` (the product is spilled into the dead `value` slot) vs our `call __ftol2` + `fild` | tried: pure C++ `(float)(long)(value*scale) / scale` with the scale loop kept in st0 | next: lane gap: retail inline-asm `fistp dword` (a `retail_asm` candidate, same idiom as 005bc3ed)
- 005cdbbe CTCInventoryStats::GetStatsPersonalityValueRenown DIFFER(95v94): our frame is `sub esp,0Ch`; retail's is `push ecx ; push ecx`, with the fidiv divisor spilled into the dead GetInterface key slot [ebp-4] and 0.0f at [ebp-8]. Every other instruction matches | tried: direct `return (float)a/b` (92v94: the 0.0f folds to a constant), `value =` with a named maxLevel local (95v94), `value =` with no named local (95v94), const vs non-const LowerBound key forms (worse) | next: scope the GetInterface key in an inner block or a helper returning the component, so the key's word dies before the divide (retail calls the non-const LowerBound 0x0040F020)

## Skipped

None. No row holds padding followed by a second function.

## Label corrections

- 005ba23a LinearSearch_12ByteStride and 005ba2a3 LinearSearch_24ByteStride are STLport `__find_if` over 12-/24-byte keyed records.
- 005c0653 std_find_Unrolled_EWeaponClass and 005c06c3 std_find_EWeaponClass_Dup are `__find_if<CCategory*, CIsCategoryWithDefIndex>` (const / non-const). Their forwarder "_Umove<EWeaponClass*>" (0x005C08C0 / 0x005C08D3) is STLport `find_if`.
- 005c0965 BinarySearch_FindWithPredicate_Stride_0x14 is `__find_if<const CItem*, const_mem_fun_ref_t>`. 005c1153 (…_WithArg) and 005c11fa std_find_Unrolled_Pair are `__find_if` with `binder2nd<const_mem_fun1_ref_t<bool,CItem,long>>`.
- 005c176a CItem_FindMatchingItemType and 005c1818 std_find_CItem_ByPredicate are `__find_if<CItem*, CIsSameItemType>` (non-const / const).
- 005ba5ad Vector_Initialize_20Bytes, 005baa6b Vector_Initialize_16Bytes, 005c0932 Vector_Initialize and 0x005D82F3 LuaStack_Initialize are STLport `_Vector_base(size_t n, const allocator&)`.
- 005b3325 Vector_PushBack is `vector<CDefPointer<const CTattooDef>>::push_back`. Its callee 0x005B3275 Tattoo_ReallocateAndCopy is `_M_insert_overflow`.
- 005bb4e6 Vector_ReserveAndShift_Uint is `vector<4-byte class>::_M_insert_overflow`.
- 005c135e `_Copy_backward<CArray<unsigned short>*>` is `__copy_backward<NInventory::CItem*>`. Its callee 0x005C0C24 "CEnginePrimitive2DText::operator=" is `CItem::operator=`.
- 005c1f42 CActiveFile_CopyAssign is `vector<NInventory::CItem>::operator=`.
- 005c2831 STL_VectorInsertRange is `vector<NInventory::CItem>::_M_fill_insert`.
- 005d85de lua_stackspace is the `vector<long>` copy constructor. Its callee 0x00405BA0 Std_Move_Backward is STLport `__copy_trivial`.
- 005bd33c PlaySoundForMeshAnimation and 005bd3a0 CAnimateThing_PlaySoundAtMeshBank are `CTCInventoryBase::PlayInventoryMovementSound` / `PlayInventoryNoMovementSound`. Their callee 0x004C7A80 "CAnimateThing::GetMeshBank" is `CThing::GetSampleBank`.
- 005bf4a3 Inventory_AddItem is `NInventory::CTCInventoryBase::AddItem(const CItem&)`.
- 005bda1d NParticleMorphs_GetDisplayText is `NInventory::CCategory::GetHighlightedItemText`.
- 005b425b Appearance_UpdateModifiers is CTCInventoryClothing's mannequin reset (name `ResetMannequin` is inferred). Its callee 0x00706800 "CArray_InsertNode" is `GetAppearanceModifierList`.
- 005cd6b7 CTCInventoryStats_AggregateCreatureGroupStats is `CTCInventoryStats::ConvertMap`. Its callee 0x005D86B2 "Lua_CreateClosure" is `list<pair<CWideString,long>>::push_back`.
- 005cdc1c CTCInventoryBase_CopyAndSyncInterface is `CTCInventoryStats::Copy(const CTCInventoryBase*)`.
- 005cdbbe ETCInterface_CalculateRatio is `CTCInventoryStats::GetStatsPersonalityValueRenown`.
- 005d9630 GetDescription is `CTCInventoryItem::GetDetails`, and 005d9aa0 GetHairCardDef is `CTCInventoryItem::GetTattooCardDef`.
- 005dc020 CVectorMap_LookupOrGetEnd is `CAnimationSet::FindFirstAnimWithName`, and 005dc290 AnimationDef_CountMatchingPreloads is `CAnimationSet::GetNoAnims`.
- 0x009CC410 is `CDataBank::GetTagFromCRC`, and 0x00404280 is STLport `operator==(const wstring&, const wchar_t*)`.
