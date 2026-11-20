# 0x009C9170 NGameText::CDataBank::Validate (p25N, 2026-11-20)

**Grade:** RELOCATION_MATCH (268/268). No flag, no pragma, no inline device, no Inferred line. Shared headers only.

**Landing order 2 of 2.** Land after `batches/2026-11-20_databank_text_reports`. The five `// Fidelity:` copies in this file are
those rows' bodies verbatim (checked by script), under the same define block.

- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, a fresh LANDVERIFY_WORK; behaviour
  PASS; no warnings. The row is in the catalogue and not landed, so no shadow root was needed.
- **Base:** a new port. The pending drafts (hand222 / hand760 / hand763) and my own first draft were local models; the best was
  DIFFER(273v268).

## What made it match
1. **GetPTextEntry 0x009C9070's body in the TU.** It is called, and with its body visible VC7.1 allocates registers as retail does:
   - `this` in ebp;
   - the outer index in ebx, homed at [esp+18h];
   - in the inner loop, the text pointer in ebx across the string calls (0x009C9213).
   Declaration-only, the result is DIFFER(278v268), and the local-model drafts had ebp and ebx swapped.
2. **GetText 0x009C7A40 and GetTextInfo 0x009C7D00 bodies in the TU.** Both are expanded:
   - GetText becomes `lea edx,[ebx+28h]` plus the copy constructor.
   - GetTextInfo leaves only its GetEntryDataType call (0x009C91FA), with the result unused, which is the "dead" call the drafts
     could not explain.
   Declaration-only: GetText DIFFER(267v268), GetTextInfo DIFFER(268v268).
3. **The counted-pointer release spelling of the landed TU-mate 0x009C9280:** FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES +
   FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER. The group temporary's release is 0x009C91CE..0x009C91E2: count loaded, pointer copied
   to ecx, decrement stored, slot-1 Delete. One TU, one spelling. Alternatives (`evidence/release_*.cpp`):
   - the default spelling gives DIFFER(269v268);
   - RELEASE_DELETES with the default pointer Release deletes twice: DIFFER(290v268).

IsLoaded 0x009C7BE0 and BeginLoadingText 0x009C8630 are in the file only because GetPTextEntry's copy needs them to compile to its
retail self. For this row they are byte-neutral. GetPGroupEntry 0x009C8890 is not needed; adding its landed body leaves the row
matching (`evidence/with_getpgroupentry.cpp`).

## Text oracle (Ego_d.pdb, databank.cpp, file id 432)
- **Locals:** this { index { group_entries { c1 { str, pentry, ptext_entry } } } }, all used as named. `ptext_entry` is GetTextInfo's
  unused result.
- **Line sizes at /Od** (`evidence/od_lines.txt`):

| Ego_d line | Ego_d bytes | ours | note |
|---|---|---|---|
| 221 | 50 | 41 | the outer `for`; Dinkumware size() |
| 224 | 21 | 21 | `if (GetEntryDataType(index) == 1)` |
| 227 | 73 | 43 | group_entries = GetPGroupEntry(index, true)->GetTextEntries(); EH-state stores |
| 228 | 40 | 31 | the inner `for`; Dinkumware size() |
| 230 | 26 | 26 | ptext_entry = GetTextInfo(..) |
| 231 | 36 | 29 | pentry = GetPTextEntry(.., true); EH state |
| 232 | 19 | 19 | `CWideString str = pentry->GetText();` |
| 233 | 25 | 18 | inner `}`: destructors |
| 235 | 5 | 5 | outer `}` |

- **Ego_d lines 223 (287 bytes) and 226 (291 bytes)** are debug checks. Retail has no code for them, so they are not in the source.
- **GetTextEntries():** FableWin.pdb ?GetTextEntries@CGroupEntry@NGameText@@QBEABV?$CArray@J@@XZ, the header's inline accessor.

## Header (`hdr/`, `hdr.diff`; add-only, no `-` lines)
This is the same overlay as the re-port batch: the new FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY opt-in in engine/NGameText_CDataBank.h.
- CTextEntry : CResource with the Ego_r members (Text +0x28, PFileData +0x38). Asserts are pinned _AT(68/69/70).
- CDataBankTextEntry's private const BeginLoading / BuildText, with CDataBank as friend.
- `void Validate();`.
- **newflags.py:** no newly flagged rows.

## Define ablation (`evidence/ablation.txt`, the final source, one define removed at a time)
Every define is load-bearing:
- RELEASE_DELETES: DIFFER(242v268).
- CIVCOUNTEDPOINTER: DIFFER(287v268).
- RELEASE_MEMBER: DIFFER(290v268).
- All the others: SRC_FAIL.

FABLE_RETAIL_CARRAY and FABLE_RETAIL_CCHARSTRING (in landed 0x009C8890's block) are byte-neutral, so they were dropped from all six
files (`plus_CARRAY_CCHARSTRING.cpp`: RELOCATION_MATCH).

## Test (`test/009c9170.cpp`, the payload test; passes in vland)
- **Setup:** links the REAL row object. The same-TU GetPTextEntry, IsLoaded, GetTextInfo and BeginLoadingText run for real.
  GetPGroupEntry, the CWideString copy constructor / destructor, and the bank / resource callees are `/alternatename` stand-ins that
  log every call. The group and text fixtures have vtables whose Delete logs.
- **Five scenarios:**
  - two groups with loaded texts;
  - index 0 skipped and an empty group;
  - a group and a text deleted when their counts reach zero;
  - a first load (empty record, so BeginLoading + BuildText + MakeFreeSpace, then loaded on the second visit);
  - no groups.
- **What it checks:** the whole call sequence with this and arguments; GetPGroupEntry(index, true); each string copied from the text's
  +0x28 and destroyed once; the releases; and that bank, records, groups and texts end as they started.
- **Mutants:** 8/8 KILLED (`mutants/009c9170.out`).

## NAME_FIXES.tsv
- Catalogue module CDataBank becomes NGameText::CDataBank. The symbol is both PDBs'.
