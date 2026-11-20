# NF184 header overlay

## stlport/fable_map.h
- Copy of the repo header (orig/stlport/fable_map.h, byte-equal to the working tree at the start of this session) plus TWO add-only
  opt-ins.
- `FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY` (row 0x009A7D40, q4393): an `#ifndef` / `#endif` pair (plus a comment) around the
  existing FABLE_RETAIL_STLP_MAP insert_unique(const V&) body, the FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY pattern. With it the TU
  calls the owner row's COMDAT (set<CWideString> insert_unique 0x009AAEB0) instead of emitting a different pick-any copy (fable_string.h
  has no CWideString::operator<, so the copy compared converted pointers; it also pulled _M_decrement 0x004294E2, 64 v 57 bytes).
- `FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY` (row 0x009A80B0, q4391): one block at the end of the file, before the closing
  include-guard `#endif` (after the last FABLE_STATIC_ASSERT_AT):
  `FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY`. It is an explicit specialization of std::less<CCharString>::operator() with the
  STLport body (`return x<y;`), compiled under `#pragma optimize("s", on)` / `#pragma optimize("", on)`. It is marked
  `Fidelity: scaffolding, not retail`. Paired with the existing FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS (fable_string.h), the pick-any
  COMDAT this emits grades RELOCATION_MATCH at its owner row 0x0042AAB3 (41 bytes, a size-optimised TU). Used by row 0x009A80B0
  (q4391). Why not declaration-only: see NOTES.md q4391 (DIFFER 140v145).
- Default path token-identical: without the macro the preprocessor emits exactly the old text.
- pin_asserts.py: pinned 0 (the header's asserts use explicit FABLE_STATIC_ASSERT_AT line numbers and the block sits after them).
- Header users: 0 landed rows define FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY or FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY
  (grep of rebuild/), so all 961 landed includers of stlport/fable_map.h are unchanged.

## fable_retail_banks.h
- Copy of the repo header (orig/fable_retail_banks.h, byte-equal to the working tree at the start of this session) plus two opt-ins.
  Both are for row 0x009A7D40 CBankFileManager::GetRetailBankFilePath (q4393).
  - `FABLE_RETAIL_CBANKFILEMANAGER_CREATERETAILBANK`: add-only, right after the private ReadQuotedString declaration. It declares
    `void CreateRetailBank(const CWideString&)` (private: Ego_r.pdb 0x00AF4D70 + FableWin.pdb 0x02FD2B40
    ?CreateRetailBank@CBankFileManager@@AAEXABVCWideString@@@Z; retail 0x009A7720, landed). Declaration only.
  - `FABLE_RETAIL_CBANKFILEMANAGER_CREATEDRETAILBANKS`: the existing `unsigned char CreatedRetailBanks[0x0C]` line wrapped as the
    `#else` arm (the FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP pattern). With the macro the member is its Ego_r.pdb type,
    std::set<CWideString> (struct_layouts_egor.tsv: CBankFileManager +56 set<CWideString,...> CreatedRetailBanks). Same 12 bytes, same
    offset (+0x28).
- Default path token-identical: without the two macros the preprocessor emits exactly the old text.
- pin_asserts.py: pinned 0. The FABLE_STATIC_ASSERT_AT lines carry explicit numbers; the two plain FABLE_STATIC_ASSERTs (lines 27 / 29)
  sit above the edits.
- Header users: 0 landed rows define either macro (grep of rebuild/), so all 24 landed includers of fable_retail_banks.h are unchanged. No repo header
  includes it, so the root-level overlay resolves cleanly.
- No other agent overlays this header or the other two (re-listed at hand-back: NF180 stlport/fable_containers.h; NF181 none; NF182
  engine/CMaintainedEvent.h, CTCFlashOnHit.h, CSimpleMemoryPool.h; NF183 engine/CDynamicObjectHeap.h; NF185 engine/CGameScriptInterface.h,
  CScriptConversation.h).

## engine/CAnimInit.h
- Copy of the repo header (orig/engine/CAnimInit.h, byte-equal to the working tree at the start of this session) plus one nested opt-in
  inside the FABLE_RETAIL_CANIMINIT_CTORS block: `FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY` (row 0x007502D0, q6391 / q6462).
  - An `#elif` arm declares the argument constructor `inline` in the class.
  - A block after the class defines it with the same body (`{Init(...);}`) under `#pragma optimize("y", off)` +
    `#pragma optimize("s", on)`, then `#pragma optimize("", on)`. It is marked `Fidelity: scaffolding, not retail`.
  - Callers still expand it in place (0x007502D0 RELOCATION_MATCH).
  - The pick-any ??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z COMDAT it emits grades RELOCATION_MATCH at its owner 0x005B33F4
    (49 bytes, size-optimised frame-pointer TU). The plain /O2 copy is 59 bytes, as at the NF173b revert.
- Default path token-identical: without the macro the preprocessor emits exactly the old text. The new `#elif` arm and the
  `#if defined(...)` block are skipped.
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT lines).
- Header users: 0 landed rows define FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY (grep of rebuild/), so all 19 landed includers of
  engine/CAnimInit.h are unchanged.
- Debt, not touched: the other landed rows that emit the /O2 ??0CAnimInit copy (NF173 names 0x008EACB0) could take the same opt-in.

## Note on the two scaffolding opt-ins
- Both FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY and FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY end with `#pragma optimize("", on)`. That
  restores the command-line settings, so it would also cancel a size-optimised row's own `#pragma optimize("s", on)` for the code after
  the include. Neither user row has one (0x009A80B0 and 0x007502D0 are /O2 TUs), but a future size-TU user should restate its pragma
  after the include.

## Not overlaid
- q4190 (row 0x0083EEE0) needed no header change: two EXISTING stlport/fable_containers.h opt-ins (FABLE_RETAIL_STLP_FILL_CHAR,
  FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22) make its _M_fill_insert<char> copy byte-match the owner 0x004110D0.
  A trial `FABLE_RETAIL_STLP_VECTOR_FILL_INSERT_DECL_ONLY` overlay and a trial std::less declaration-only opt-in were both reverted.
