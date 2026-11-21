# 0x009CE370 CBankFile::WriteFileInformation(bool) -- READY (p25Q, Account B, 2026-11-21)

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x009CE370 | CBankFile::WriteFileInformation(bool is_valid) | 620 | RELOCATION_MATCH (`// Flags: /GS`, retail's cookie) | PASS (links the real row; 3,000 scenes: writable / not, packed / not, retail mode, is_valid, 6 alignments; whole call log + every argument) | 15 / 16 KILLED (1 equivalent) |

Strict check: `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv` -> RELOCATION_MATCH, behaviour PASS, no warnings.

## Text
Shared headers only: CBankFile (FILENAMEINDICES, ADDENTRYMULTIPLEFILES, PACKRUNTIMEDATA, ENTRYDATA_INLINE, STATICBLOCKS_TEXTURE_HELPERS and
the new WRITEFILEINFORMATION opt-in), CBankFileEntryUpdateData, CPackedUIntArray, CCountedPointer, CAFile / CDiskFileWin32,
CDataOutputStream / CFileDataOutputStream (retail branches). Ego_d's parameter and local names and its statement list; the source comment
decodes every Ego_d line. What closed the parked 622v620 / 624v620 drafts (pending/2026-10-22_linux_hand281, frame slot of the loop's
strength-reduced RuntimeData offset):
- Ego_d puts info_offset, data_version, max_index, rand_val and entry_count at -16..-32, above os (-48) and header (-56): VC10 /Od allocates
  in declaration order, so the five are declared first, uninitialised, and assigned later (lines 1679 / 1680 / 1681 / 1693 / 1695). With
  that declaration block VC7.1 gives the offset temporary its own slot [esp+28h] in the 0x2C frame, as retail does.
- line 1684 (19 B) is one call: Ego_r / FableWin ?IsEntryInitialised@CBankFile@@ABE_NK@Z (Ego_d lines 1931 / 1933 / 1935 / 1938 give the
  Valid && DataOffset test, then !UpdateData[index]->RequiresUpdate);
- line 1687 (47 B) is std::max<unsigned long> by reference (fastcall, FableWin ??$max@K@std@@YIABKABK0@Z), retail's pointer select at
  0x009CE48B..0x009CE497; line 1691 (15 B) is info_offset = Align(info_offset);
- line 1709 (34 B) is `if (!RetailMode) WriteRecoveryFile(...)` on one line in the `!is_valid` branch, and 1714 / 1715 the else branch.

## Labels
- `// Inferred:` Ego_d lines 1672 (282 B) and 1710 (17 B) have no release bytes (debug-only checks), left out.
- `// Inferred:` max_index's zero store before entry_count's (retail stores [esp+18h] before [esp+14h]) and `max_index < c_entry` (retail
  `cmp [esp+18h],edi ; jae`); /Od sizes are the same either way. (The other order and `c_entry > max_index` each grade 620v620.)
- No Fidelity copies, brackets or inline controls (no noinline / forceinline define is named; MakeWritable, IsEntryInitialised and Align
  are ordinary inline members, expanded here; retail keeps out-of-line copies of MakeWritable 0x009CBF90 and Align 0x009CBB90 for its
  other callers).

## Header changes (hdr/, hdr.diff) -- strictly add-only (0 lines removed)
- engine/CBankFile.h: new opt-in FABLE_RETAIL_CBANKFILE_WRITEFILEINFORMATION:
  - the Ego_r.pdb member types `CCountedPointer<CDiskFileWin32> BankFile` (+0x7C) and `CWideString Filename` (+0x84) in place of the
    flattened BankFile_Data / BankFile_Info / Filename_PStringData. The three existing offset asserts go into the #else branch, and the
    opt-in branch has the two typed asserts under the same pinned names (260, 262). pin_asserts: 0;
  - inline MakeWritable (private; Ego_d local file_pos), IsEntryInitialised (private const), Align (when FABLE_RETAIL_CBANKFILE_ALIGN's
    out-of-line declaration is not chosen);
  - declarations of GetRandomValidationValue, WritePadding(no_bytes), WriteRecoveryFile(validate, max_index, entry_count),
    WriteFileInformation(is_valid) (private) and InternalWriteEntryList(os) (public), with Ego_d parameter names.

## Mutants (mutants/): 15 / 16 KILLED
The survivor, `if (max_index < c_entry) max_index = c_entry;` -> `max_index = c_entry;`, is equivalent: c_entry only increases, so the
last initialised index is the largest.

## Names (NAME_FIXES.tsv)
None: the row's landed name is right.

## Newly flagged rows
None: CBankFile.h already existed. The landed TU-mates that model CBankFile locally (e.g. MakeWritable 0x009CBF90, WriteRecoveryFile
0x009CDF50, InternalWriteEntryList 0x009CDA70) were already flagged before this change.
