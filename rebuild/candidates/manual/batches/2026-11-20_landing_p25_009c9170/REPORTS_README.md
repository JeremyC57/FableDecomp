# Re-port amendments: the databank.cpp text-entry rows (p25N, 2026-11-20)

**Landing order 1 of 2.** These five landed rows move from local models onto the shared header, using Ego_d's text. 0x009C9170
CDataBank::Validate (`batches/009c9170`, order 2 of 2) copies them verbatim.

**Order inside this batch** follows the Fidelity dependencies: 0x009C7D00 and 0x009C7BE0 (and 0x009C7A40, which depends on nothing),
then 0x009C8630, then 0x009C9070.

| row | landed file | grade now | Fidelity copies in the file | diff |
|---|---|---|---|---|
| 0x009C7A40 CTextEntry::GetText | NGameTextCTextEntry_GetText_009c7a40.cpp | RELOCATION_MATCH (22) | none | `src_009c7a40.diff` |
| 0x009C7BE0 CDataBankTextEntry::IsLoaded | CDataBankTextEntry_IsLoaded_009c7be0.cpp | RELOCATION_MATCH (30) | none | `src_009c7be0.diff` |
| 0x009C7D00 CDataBank::GetTextInfo | NGameTextCDataBank_GetTextInfo_009c7d00.cpp | RELOCATION_MATCH (39) | none | `src_009c7d00.diff` |
| 0x009C8630 CDataBank::BeginLoadingText | CDataBank_BeginLoadingText_009c8630.cpp | RELOCATION_MATCH (43) | 0x009C7D00 | `src_009c8630.diff` |
| 0x009C9070 CDataBank::GetPTextEntry | NGameTextCDataBank_GetPTextEntry_009c9070.cpp | RELOCATION_MATCH (245) | 0x009C7BE0, 0x009C7D00, 0x009C8630 | `src_009c9070.diff` |

- **Strict vland:** passes (`vland.log`). Run with `--recheck-landed` (all five rows are landed), LANDVERIFY_STRICT_HEADERS=1,
  FABLE_HDR_OVERLAY=`hdr/`, a fresh LANDVERIFY_WORK; behaviour PASS on all five; no warnings.
- **No flag, no pragma, no Inferred line.** Every row uses the same define block as 0x009C9170, and every Fidelity copy is
  byte-identical to the row file it copies (checked by script).
- **Retail lengths:** three old Retail lines were short. 0x009C7A40 is 22 bytes (the line said 20), 0x009C7D00 is 39 (said 37) and
  0x009C8630 is 43 (said 42). Each body runs to its `ret 4`, and the repo oracle (auto-re-candidates.tsv) already uses these lengths.

## Header (`hdr/`, `hdr.diff`; add-only, no `-` lines)
`engine/NGameText_CDataBank.h` gets a new nested opt-in, **FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY**, inside GROUPTEXT:
- **CTextEntry : CResource** (ghidra_out/rtti_bases.tsv: `.?AVCTextEntry@NGameText@@ : CResource@0`). The members are
  struct_layouts_egor.tsv's (SpeechBank +0x1C, Narrator +0x20, SoundName +0x24, Text +0x28, Tags (opaque, 12 bytes in retail),
  PFileData +0x38, Owner +0x40).
  - Asserts are pinned with FABLE_STATIC_ASSERT_AT(68/69/70).
  - The existing `class CTextEntry : public CIVCountedPointeeBase` is kept unchanged as the `#else` branch, so landed includers are unaffected.
- **CDataBankTextEntry** gets `friend class CDataBank;` plus private `BeginLoading(unsigned long, CDataBank&) const` (0x009C8340) and
  `BuildText() const` (0x009C8FB0), declared the same way CDataBankGroupEntry's pair already is.
- **CDataBank::Validate()** is declared.
- **newflags.py hdr/:** no newly flagged rows.

## Text (Ego_d.pdb, databank.cpp, line-table file id 432; /Od line sizes in `evidence/od_lines.txt`)

**GetTextInfo**
- Parameter is `index`.
- Lines 140 / 142 / 144 are 16 / 22 / 2 bytes, and this text gives the same.
- Ego_d lines 137..139 (370 / 300 / 318 bytes) are debug checks with no retail code.
- Ego_d names the types `NGameText::CDataBank::EDataType { TYPE_TEXT = 0, TYPE_GROUP = 1, TYPE_NARRATOR_LIST = 2 }`. That is one source
  only (not in Ego_r's types or the name lists), so the literal stays, as in the header's GetGroupInfo. Your call if you want the
  enumerators.

**BeginLoadingText**
- Ego_d locals are `entry_index` and `info`.
- Lines 406 / 407 / 409 are 15 / 6 / 16 bytes, the same as this text.
- The body is GetTextInfo, then `info->BeginLoading(entry_index, *this)`, the same shape as the landed BeginLoadingGroup 0x009C8220.

**IsLoaded**
- Lines 767 / 769 / 771 / 775 are 12 / 57 / 4 / 2 bytes. Ours are 15 / 57 / 4 / 2: the outer test goes through CIVCountedPointer::IsNull.

**GetText**
- Ego_d has a debug version (lines 877..904: two text_copy scopes and a static seed).
- Retail is only the last statement, `return Text;` (line 903). The debug code is compiled out of the release build.

**GetPTextEntry** (`evidence/getptextentry_order.txt`)
- Ego_d locals are this, index, wait, text_info.
- Ego_d has 16 line records (162..187), and this text gives 16, in the same order.
- Sizes: ours 15 / 10 / 8 / 23 / 15 / 12 / 8 / 16 / 27 / 8 / 15 / 17 / 12 / 11 against Ego_d's
  15 / 10 / 8 / 27 / 15 / 12 / 8 / 25 / 22 / 8 / 15 / 26 / 12 / 20. Ego_d is a VC10 build, so the counted-pointer and base-adjust
  lines differ.
- Two other spellings are also byte-equal at /O2, and Ego_d rules both out:
  - The landed GetPGroupEntry shape, with a `bool loaded = ...` expression, is one 94-byte line (`order_a_expression.cpp`). Ego_d's
    line 167 is 27 bytes, which fits the IsLoaded() call.
  - `if (!IsLoaded() && !wait) BeginLoadingText(index); else {...}` gives 17 records including an `else` record
    (`order_b_else.cpp`). Ego_d has 16 records and no `else`. The not-loaded BeginLoadingText is the fall-through at line 183.
- The two `order_*.cpp` files are probe TUs: the older draft names and a group-entry body. They are kept for the line records only.

## Same-TU bodies (`evidence/result.txt`)
Each Fidelity copy is load-bearing. With the copy declaration-only:

| row | copy removed | result |
|---|---|---|
| 0x009C8630 | GetTextInfo | DIFFER(32v43) |
| 0x009C9070 | IsLoaded | DIFFER(232v245) |
| 0x009C9070 | GetTextInfo | DIFFER(236v245) |
| 0x009C9070 | BeginLoadingText | DIFFER(221v245) |

In 0x009C9070, BeginLoadingText is called on the not-loaded path and expanded on the first-load path (0x009C90D5..0x009C90F0), as retail does.

## Define block
This is the databank.cpp rows' common block (0x009C9170's; see that README for the define ablation).
- Its counted-pointer release spelling is the landed TU-mate **0x009C9280**'s: FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES +
  FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER. It is not a new fitted choice.
- None of these five rows expands a release.

## Tests
- **New `test/<addr>.cpp`** link the REAL row objects. The same-TU copies run for real; the out-of-line callees are `/alternatename`
  stand-ins that log every call.
  - 0x009C7A40: three texts. Checks the hidden result, one copy-constructor call from +0x28, and that the entry is unchanged.
  - 0x009C7BE0: no entry, no pending read (IsFinished not called), pending, finished.
  - 0x009C7D00: types 0 / 1 / 2 over eight indices. Checks the type call's this and index.
  - 0x009C8630: text and non-text types, null records. Checks BeginLoading's this, index and bank.
  - 0x009C9070: 13 scenarios: group / narrator types, no record, not loaded with and without wait, read pending / finished, first load,
    TouchResource with no list / same frame / other frame.
    - Checks the whole call sequence and arguments, MakeFreeSpace(0) on the ResourceList at +0x168, ListInsert(Head.Prev, &Head), the
      LastUsedFrame store, and the AddRef on the returned pointer.
- **Mutants:** 2 + 5 + 4 + 3 + 9 = 23/23 KILLED (`mutants/*.out`).
- **Landed tests:** the landed self-contained tests in rebuild/tests/00/9c (models; 0x009C9070's embeds the old source) still pass.
  The new tests should replace them.

## NAME_FIXES.tsv
| address | fix |
|---|---|
| 0x009C7BE0 | the landed file defined the global `?IsLoaded@CDataBankTextEntry@@QBE_NXZ`; now namespaced, as both PDBs and the landed 0x009C9280 call |
| 0x009C8630 | the landed file defined the global non-const `?BeginLoadingText@CDataBank@@QAEXK@Z`; now `?BeginLoadingText@CDataBank@NGameText@@QBEXK@Z` |
| 0x009C8340 | callee spelling `?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z` (Ego_r 0x00B16820 / FableWin 0x02F744F0). The landed 0x009C8340 defines the public, pointer form `QAEXKPAV..`, so that row needs a respell before these link together. |
| 0x009C7A40 / 0x009C9070 | catalogue: return types (CCharString with a CBankFile* parameter; a CGroupEntry pointer), and 0x009C7A40's stale compiled_source path |

## Reported, not touched
Landed **0x009C8890 GetPGroupEntry** is not my row. Ego_d's table for it (lines 191..216) has exactly the same record sizes as
GetPTextEntry's, so its text has the same problems:
- the 94-byte `bool loaded = ...` line, where Ego_d expects `group_info->IsLoaded()` (FableWin.pdb has
  ?IsLoaded@CDataBankGroupEntry@NGameText@@QBE_NXZ);
- the not-loaded early return in place of the fall-through;
- parameter names id / allow_pending / entry, where Ego_d has index / wait / group_info. BeginLoadingGroup 0x009C8220 likewise uses
  id / entry, where Ego_d has entry_index / info.

Under this batch's define block, 0x009C8890 grades the same. Its landed FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES without
RELEASE_MEMBER would delete twice once a release is expanded (probe s1); it expands none, so the bytes are unaffected.
