# Hand chunk 410 (GFBoolToCharString .. _STL::__insertion_sort): 39 of 40 byte-exact

## What matched (39)

### CCharString / CBasicString<char> free helpers
- 0099f9b0 `GFBoolToCharString(bool)`: returns "TRUE" or "FALSE" by value. It is a `__fastcall` hidden-result function in
  the CCharString TU, so the visible `(const char*, long = -1)` ctor and AllocStringData inline into the return slot.
  The `value == true` test gives `cmp dl,1`; a plain `if (value)` would give `test`. VC hoists the identical arm heads.
- 0099fa90 `GFByteSizeToCharString(unsigned long, long)`: formats a size as "<n> bytes", "<x> KB" or "<x> MB".
  - Each `return a + "suffix"` builds its left temporary in the dead `decimals` argument slot.
  - The small-size return is written last, so its block sits at the function end as in retail.
  - Retail also has one unused default-constructed CCharString local.
- 009a0460 `CBasicString<char>::Append(char, unsigned long)`: appends a fill of a character and zeroes the slack. Two
  PDB inline helpers are kept as plain loops so VC emits its rep stos fill idiom:
  - `Fill` is a plain byte loop; an explicit memset sinks the add and uses `movsx`.
  - `ClearStringEnd` aligns the zeroing to 4 bytes.

### Streaming font cache
- 009a0ec0 `CStreamingFontBank::SetAlphaMaskUVs`: two whole-object C2DVector POD copies. The implicit copy gives dword
  moves; a user operator= would go through the x87.
- 009a0f00 `InitializeStreamingFontCacheEntries`: builds the LRU chain.
  - The entries pointer is a local (`data.entries[0].x` re-reads it per store).
  - The 0x40000000 is subtracted at each use; a precomputed local reorders the stores.
  - The counted pointer is read through `.Object` (operator-> leaves a dead `add`).
  - The loop index is `unsigned short`.
- 009a0f90 `UpdateUseStreamingFontCacheEntry` and 009a1020 `InsertNewStreamingFontCacheEntry`: unlink and relink in
  the LRU list. The entry reference is formed before the frame-counter fetch; VC forwards the just-stored `older` word.
- 009a1660 `UninitializeStreamingFontTextureCache`: four `delete[]`s plus an allocator free. The free needs
  `GFFreeFromGlobalAllocator` as `__forceinline`; a visible body or plain inline stays a call.

### CBankFileManager
- 009a7a20 `GetAllRetailPaths(set<CWideString>&)`: walks Banks and inserts BasePath + each retail path into the set.
  - STLport `_M_increment` is inlined, and the operator+ temporary is homed in the dead `paths` slot.
  - insert's `pair<iterator,bool>` needs a user ctor. Without one, a POD 8-byte pair is returned in edx:eax instead
    of through a hidden pointer.
- 009a7ac0 `CBankFileManager()`: STLport 4.6 container constructors inlined in member order, then
  `UsingRetailBanks = false; FileVersion = 100;`.
  - Ctors used: `_Rb_tree_base` / `_Rb_tree` / `map` / `set` / `_List_base` / `list`, plus
    `_STLP_alloc_proxy(const allocator&, T)`.
  - There is no EH frame: the TU builds without /GX.
- 009a80b0 `BankExists(const CCharString&)`: resolves the alias, then searches either the open retail banks' entry
  indexes or the Banks map.
  - Same-TU noinline bodies for the finds and `CBasicString<char>::operator<` let VC cache the list head in ebx across
    the call.
  - Each find result is a named local compared with end() after the call.
  - There are separate `return true` and `return false` exits.

### CDefinitionManager
- 009ad580 `GetPDefClassInfo(const CCharString&)`: a CRC lookup in the class-info vector map.
  - `GetCRC()` goes into a named local.
  - The by-value key is homed in the dead `name` slot, and its address goes to LowerBound.

### STLport `_Rb_tree` (set<CWideString>, map<CCharString,...>, map<unsigned long, CIVCountedPointer<T> >)
- 009aa4c0 `_M_find`: STLport 4.6 verbatim at /O2. The loop is rotated, and the header is re-read for each return.
- 009aa900, 009aaf70, 009ab120 and 009b5160 `_M_insert`: verbatim. The right-insert arm is laid out first; 009b5160
  uses an unsigned key compare.
- 009aaeb0 `insert_unique`: verbatim. `__comp` starts true, and `__j` and the `_M_insert` result share the dead v slot.
  That sharing only happens with a same-TU noinline `iterator::operator--` body; without it an extra `push ecx` slot
  appears.

### STLport sort and heap algorithms (`__fastcall` under /Gr)
- 009b1870, 009b1a30, 009b1b00 and 009b47f0 `__unguarded_linear_insert`, over CWideString, 12-byte pairs, 8-byte
  pairs and `pair<CCharString,long>`: verbatim.
- 009b2cb0, 009b2d40 and 009b2db0 `__unguarded_insertion_sort_aux`: verbatim with the inner insert expanded. The
  catalog's Dinkumware `_Copy_impl` label is wrong.
- 009b4650, 009b46b0, 009b5920 and 009b5990 `__linear_insert`: verbatim.
  - The pair needs a user copy ctor; it then counts as `__false_type`, so copy_backward becomes the counted
    `*--result = *--last` loop.
  - The tag temporary is homed in the dead comparator slot.
- 009b68d0 `__insertion_sort<CWideString>`: `__linear_insert` must be `__forceinline` (/Ob2 keeps the plain inline
  out of line, as in its own copy at 0x009B58B0).
- 009b1c30 `__push_heap`, 009b2ea0 `__adjust_heap` and 009b3bb0 `__make_heap` over CWideString: verbatim, with signed
  `(hole-1)/2`.
- 009b3580 and 009b5800 `__unguarded_partition`: verbatim with `__iter_swap` inlined. At 009b5800 the pivot stays in
  esi only with a same-TU noinline `CBasicString<char>::operator<` body (0x00429950); otherwise the pivot is re-read
  after the call.

### STLport vector
- 009b2780 `vector<SRecord12>::_M_allocate_and_copy`: `__uninitialized_copy` must be `__forceinline`. The element must
  be a `pair<unsigned long, 8-byte>` with a user copy ctor, which gives three single-word copies; a plain 12-byte
  record copies as a block move.
- 009b4d80 `vector<SRecord12>::reserve` and 009b4e20 `vector<unsigned char>::reserve`: verbatim. The 12-byte version
  divides by the 0x2AAAAAAB reciprocal. The byte version inlines `__copy_trivial` as memmove and spills old_size to
  the `push ecx` slot.
- 009b4a20 `vector<unsigned short>::push_back`: verbatim.
  - The empty `__true_type` tag is homed on the still-live x slot.
  - VC only does that when a same-TU noinline `_M_insert_overflow` stand-in body shows the tag is never touched.

## Parked for Fable 5.1 (1)
- 009a7d40 CBankFileManager::GetRetailBankFilePath DIFFER(223v222): only the trailing `int3` VC7.1 emits after the final noreturn `call _CxxThrowException` (q.py strips it from the retail row); RELOCATION_MATCH against the 223-byte N+1 span read from Fable.exe (probe/n1.py) | tried: ResolveAlias front end + `Banks.find` named local + `CreatedRetailBanks.find/insert` + `throw CGenericException(key + " bank not found!")`, 1 grade | next: lane gap: int3-after-noreturn oracle trim (land against an N+1 row, per GOTCHAS "int3-after-noreturn rows LANDED")

## Skipped
- None.

## Label corrections
| Address | Catalog label | Correct name |
|---|---|---|
| 0099fa90 | FormatFileSizeString | GFByteSizeToCharString |
| 009a0460 | String_FillRange | CBasicString<char>::Append(char, unsigned long) |
| 009a7a20 | RBTree_ForEachNode | CBankFileManager::GetAllRetailPaths |
| 009a7ac0 | NHeroInformationScreens_CAbilityScreen_ctor (propagated, wrong) | CBankFileManager::CBankFileManager |
| 009a7d40 | CBankFileManager_FindOrCreateBank | CBankFileManager::GetRetailBankFilePath |
| 009a80b0 | GetLanguageData (wrong) | CBankFileManager::BankExists |
| 009aa4c0 | BinaryTree_FindByComparator | `_Rb_tree<CWideString,...>::_M_find` |
| 009aa900, 009aaf70, 009ab120, 009b5160 | BinaryTree_InsertNode / RBTree_Insert | `_Rb_tree<...>::_M_insert` (three different instantiations) |
| 009aaeb0 | RBTree_LowerBound (wrong) | `_Rb_tree<CWideString,...>::insert_unique` |
| 009ad580 | CVectorMap_FindByKey | CDefinitionManager::GetPDefClassInfo(const CCharString&) |
| 009b1870 | CWideString_BackupAndRestore (wrong) | `__unguarded_linear_insert<CWideString*>` |
| 009b1a30, 009b1b00, 009b47f0 | Insertion_Sort_Shift / SortedArray_InsertElement_Uint / Sorted_Vector_InsertElement | `__unguarded_linear_insert` |
| 009b1c30, 009b2ea0 | Dinkumware `_Push_heap` / `_Adjust_heap` | STLport `__push_heap` / `__adjust_heap` |
| 009b2780 | Deque_AllocateBuffer (wrong) | `vector<SRecord12>::_M_allocate_and_copy` |
| 009b2cb0, 009b2d40, 009b2db0 | `_Copy_impl<CNavigationPosition*,...>` (propagated Dinkumware, wrong) | `__unguarded_insertion_sort_aux` (three instantiations) |
| 009b4650, 009b46b0, 009b5920, 009b5990 | StdVector_InsertElement_Sort / Insertion_Sort_InsertElement_Pair8byte / Sorted_Vector_InsertElement_CCharString | `__linear_insert` |
| 009b4d80 | Deque_Reserve (wrong) | `vector<SRecord12>::reserve` |
| 009b68d0 | Vector_InsertRange_String (wrong) | `__insertion_sort<CWideString*>` |
| callee 0x0099AF70 | manifest "_Iterator_base12::_Getcont" (wrong) | `CWideString::operator<(const CWideString&) const` |
