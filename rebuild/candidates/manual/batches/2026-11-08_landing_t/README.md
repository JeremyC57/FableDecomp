# Landing 2026-11-08t: chunk 870 (libpk, Opus high), 16 of 17 rows

All 16 RELOCATION_MATCH (0x00C28640 MATCH) + PASS. libpk is Lionhead in-house code (owner, 2026-11-08: treat as game code);
zlib 0x00C02470 left the chunk for the vendor route. Hand-back: `pending/2026-11-08_linux_hand870/`.

| Row | Device |
|---|---|
| 0x00C08D40 CSystem::LoadBank | same-TU `#pragma auto_inline(off)` bodies of PKCAutoDelete::GetChecked / Release (their mod/ref keeps the bank in esi); shared libpk/PKCProtectionInstance.h |
| 0x00C0E1F0 CString::Left | shared engine/CString.h, new additive opt-in `FABLE_RETAIL_CSTRING_LEFT` (MFC strcore.cpp Left + afx.inl default ctor); extent cut to 208 |
| 0x00C174A0 LoadBankFile (descriptive) | real same-TU bodies of the private-convention static and its caller 0x00C17640 |
| 0x00C18E20 / 0x00C19340 PKCTRRedblackTree::AddValue / RemoveValue | plain C++ |
| 0x00C1A510 / 0x00C1A870 PKCDefaultFileManager::OpenFile / ReleaseFile | same-TU map Insert / Find bodies; shared lock header |
| 0x00C1D320 CMatrix3x4::Invert | flat E11..E43 model |
| 0x00C28400 / 0x00C28640 / 0x00C28690 / 0x00C28870 libpk CHandleBasedMemoryPool Defragment / ValidateResources / Free / PartialDefragment | plain C++ (Defragment extent cut to 310) |
| 0x00C313C0 CUnprovenRecordReader_012b4d34::OnRecord | field store before the flag OR |
| 0x00C32CD0 PKCTRArray::AddValue | plain C++ |
| 0x00C3BF30 CAsyncFileReader::ReadRemote | loop-counter updates swapped (frame slots) |
| 0x00C42850 CPKRiffReader_00c427d0::Open | separate early returns, zero stores per arm |

- **Extents:** 0x00C0E1F0 (368 -> 208) also holds MFC CString::SpanIncluding 0x00C0E2C0 and SpanExcluding 0x00C0E310; 0x00C28400
  (373 -> 310) also holds two unreferenced handle-slot accessors 0x00C28540 / 0x00C28560. Four manifest rows added (boundary),
  BOUNDS_FIX.tsv rows written.
- **Parked (1):** 0x00C1D1C0 InvertMatrix3x4, 8 lines: c2 merges the three m.E41 loads into one CSE temp and the x87 operand
  order follows it (599 grades, every operand order / permutation tried).
- **Names:** labels in `ghidra_out/labels_landing_2026-11-08t.tsv`; 17 respellings queued (tag land0811t), e.g. 0x00C426C0 is
  StreamReader::Open, 0x00C31D80 / 0x00C31D90 are the PKCAutoFile ctor / dtor, the CRACBuffer accessor placeholders 0x00C4BF90..0x00C4C2D0.
