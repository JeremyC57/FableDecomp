# Chunk 716 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 716) A 17-byte `mov eax,[esp+4] ; add eax,N ; mov [esp+4],eax ; jmp [IAT]` COM slot is a STDMETHODIMP_(ULONG) AddRef
  `return InterlockedIncrement(&m_cRef);` tail-calling the import with `this` rewritten in place (0x00CA7AE0 / 0x00CA7C10 /
  0x00CA9060); the near-twins are told apart by m_cRef's offset and by the QI's IID in slot 0 of the same vtable.
- (chunk 716) `mov [esp+8],offset IID ; add [esp+4],N ; jmp <non-virtual member>` is a STDMETHODIMP forwarder to an embedded
  helper with a constant argument (CMediaPosition::GetIDsOfNames -> m_basedisp.GetIDsOfNames(IID_IMediaPosition, ...), 0x00CAD2E0).
- (chunk 716) A slot shared by >100 vtables with no RTTI is a base-class virtual the templates inherit: CActiveEntityScriptBase
  slots 5/6 (0x00CE1090 / 0x00CE10A0) sit in every CActiveEntityScript<T> table.
- (chunk 716) Dynamic-initializer rows in `.text$yc` can start unaligned right after the previous initializer's `ret` (same TU, no
  padding): the `.CRT$XCU` entry proves the start; Ego_r keeps the same odd placement (0x0121DF1C / Ego_r 0x01010A15).
- (chunk 716) An initializer that calls a static function with no `mov ecx` before the atexit push is the global's inline ctor
  calling a static member (gIME: `CIME() { ResetToDefaultSettings(); }`), and an atexit thunk `mov ecx,obj ; jmp F` is an inline
  dtor `{ F(); }` (0x01219E90 / 0x012288B0).
- (chunk 716) An initializer with `push 0Ch ; call malloc ; [eax]=eax ; [eax+4]=eax` is an STLport 4.6 list's `_List_base`
  ctor on a global; its `_$E2` thunk jumps to the out-of-line `~_List_base` (the `~list() {}` folds away), which names the
  thunk target (0x0121F850 -> 0x008F9F50).
- (chunk 716) Grading the TU's other emitted COMDATs against neighbouring retail addresses (the ??_G, the ctor, the `_$E2` atexit
  thunks) proves those rows' names at no extra cost (0x00DCF6E0, 0x00DCD160, 0x0122A200, 0x012288B0 all RELOCATION_MATCH).
- (chunk 716) finish.py splits line 1 on `::`: a dynamic initializer of a namespaced object reads
  `NS::`dynamic initializer for 'X''()`; a static data member's PDB display name keeps its qualification inside the quotes.

Review at landing:
- hdr/fable_ime.h: new opt-in FABLE_RETAIL_CIME branch (default text unchanged; both landed includer rows rechecked
  RELOCATION_MATCH + PASS with the overlay).
- Two landed sources for one address: rebuild/src/compiled/00/40/CIME_LoadIMESettings_00405350.cpp and
  NUISystemCIME_LoadIMESettings_00405350.cpp (recheck.py skipped the first).
- Landed 0x0122A200 / 0x012288B0 have `// Leaf: CEntry>_const&>`, which names no symbol (vland grades the largest function);
  0x008F9F50 is a free `__fastcall SortTreeRecursively_00436900` stand-in (Leaf SortTreeRecursively): re-author from this chunk's
  TUs (the `_$E2` leaves already match).
- vland RELOC_CONFLICT / RELOC_NAME on `_$E2` for 0x01219E90 / 0x0121F850 are the expected per-TU `_$E` reuse plus the old
  placeholder labels of 0x012288B0 / 0x0122A200.
- Coordinator decisions in NOTES.md: linkage of VALUE_DISPLAY_DURATION / gIME; the 0x013B85E0 string's name.
- No row needed more than 3 grades; every extent was right.
