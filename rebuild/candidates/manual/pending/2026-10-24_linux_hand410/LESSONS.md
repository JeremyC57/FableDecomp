# Chunk 410 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU noinline callee body keeps values in registers across the call.** Retail keeps a value in a callee-saved
  register across a call to `operator<` or `find`; with only an extern declaration, VC7.1 re-reads it after the call.
  Fix: put the callee's real body in the TU under `#pragma auto_inline(off)` + `__declspec(noinline)`, so VC knows the
  callee only reads memory.
  - 009b5800: the pivot stays in esi.
  - 009a80b0: the list head is cached in ebx.
- **The same trick lets a dead argument slot be reused.**
  - 009aaeb0 `insert_unique`: `__j` and the `_M_insert` result share the dead `v` slot only with a same-TU
    `iterator::operator--` body; otherwise an extra `push ecx` slot appears.
  - 009b4a20 `push_back`: the empty `__true_type` tag is homed on the live `x` slot only with a same-TU
    `_M_insert_overflow` stand-in body.
- **`__forceinline` where /Ob2 leaves a plain inline out of line.**
  - `__linear_insert` inside `__insertion_sort<CWideString>` (009b68d0; retail also has it out of line at 0x009B58B0).
  - `__uninitialized_copy(..., __false_type)` in `_M_allocate_and_copy` (009b2780).
  - `GFFreeFromGlobalAllocator` in UninitializeStreamingFontTextureCache (009a1660).
- **STLport pair element: user copy ctor vs plain struct.** A 12-byte vector element modelled as
  `pair<unsigned long, 8-byte>` with STLport's memberwise user copy ctor copies as three `mov` words; a plain 12-byte
  struct copies as a block move (009b2780). The same user copy ctor makes copy_backward pick `__false_type`, giving the
  counted `*--result = *--last` loop (009b46b0, 009b5920).
- **Non-POD vs POD pair return.** A `pair<iterator,bool>` result is returned through a hidden pointer only when the
  pair has a user ctor (STLport's does); a POD 8-byte pair comes back in edx:eax (009a7a20).
- **Name the find result.** `it = m.find(k); if (it != m.end())` compares with end() after the call; inline
  `m.find(k) != m.end()` loads end() before the call (009a80b0, 009ad580 `GetCRC()` local).
- **int3-after-noreturn.** A body ending in `throw` (a final `call _CxxThrowException`) gets one extra 0xCC from VC7.1.
  q.py strips it from the retail row, so the row grades DIFFER(N+1vN). Grade against the N+1 span with
  `probe/n1.py` and land against an N+1 row (009a7d40).
- **CGenericException model:** `class CGenericException : public CExceptionBase` (1 byte), ThrowInfo 0x013692F8,
  ctor `CGenericException(const char*) {}` (009a7d40).
- **`value == true` on a bool gives `cmp dl,1`;** a plain `if (value)` gives `test dl,dl` (0099f9b0).
- **Byte fill loop vs memset.** A plain byte fill loop becomes VC's rep stosd/stosb fill idiom with byte replication
  (`mov bl,al ; mov bh,bl`); an explicit `memset` sinks the dest add and uses `movsx` (009a0460).
- **Test pitfalls.**
  - VC7.1 rejects local structs as template arguments (TCC_FAIL); hoist them to global scope (009aaf70, 009ab120,
    009b5160 tests).
  - g++ on 64-bit: a test that sizes a malloc from a "max size" computed on 32-bit types can overflow; clamp it
    (009a7ac0 test).

Review at landing:
- 009a7d40 (`CBankFileManager::GetRetailBankFilePath`) is in differ/ only because of int3-after-noreturn. It is
  RELOCATION_MATCH against the 223-byte N+1 span; land it against an N+1 oracle row, like the 7 rows in the GOTCHAS
  "int3-after-noreturn rows LANDED" entry.
- Local models with no engine header yet: CBankFileManager, CBankInfo, CCharString / CBasicString<char>, CWideString,
  CGenericException, CIVCountedPointer and the STLport containers are modelled locally, because the lane needs
  self-contained sources.
  - `rebuild/include/engine/CDefinitionManager.h` and `CStreamingFontBank.h` do exist: retype 009ad580 and 009a0ec0
    onto them with `retype_landed.py`.
- The same-TU noinline stand-ins are separate symbols in the lane .obj, not the real out-of-line functions; the
  landing must keep them as same-TU bodies:
  - 009b4a20's `_M_insert_overflow` (the real 0x005135D0 reallocates).
  - 009aaeb0's `operator--`.
  - 009a80b0's finds and `operator<`.
  - 009b5800's 0x00429950.
- The 12-byte element is modelled two ways: 009b4d80 (`vector<SRecord12>::reserve`) as a plain record, and 009b2780
  (`_M_allocate_and_copy`) as a `pair<unsigned long, 8-byte>`. Both match; unify them on the pair at landing (it is
  the same vector instantiation).
- Catalog rows with several instantiations sharing one label:
  - 009b2cb0, 009b2d40 and 009b2db0: three `__unguarded_insertion_sort_aux`, all labelled `_Copy_impl<CNavigationPosition*,...>`.
  - 009aaf70, 009ab120 and 009b5160: three `_M_insert`, all "RBTree_Insert".
  - 009b46b0 and 009b5920: two `__linear_insert`, both "Insertion_Sort_InsertElement_Pair8byte".
