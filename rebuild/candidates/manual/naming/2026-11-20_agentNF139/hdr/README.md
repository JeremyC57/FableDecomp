# NF139 header overlay

## engine/CBankFile.h
- Base: the repo working-tree copy of `rebuild/include/engine/CBankFile.h` at the start of NF139 (2026-11-20), kept in `orig/engine/CBankFile.h`.
- Added (add-only, one `#if` block right after the `FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE` declaration):
  `FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE` (active only with `FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA` and without
  `FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE`): the inline bodies of `GetEntryDataSize` / `GetEntryDataOffset` (Ego_r.pdb 0x00B19F40 /
  0x00B19F70, FableWin.pdb 0x02F91CC0 / 0x02F91EA0 `?GetEntryData{Size,Offset}@CBankFile@@QBEKK@Z`; out-of-line retail copies
  0x009CC240 / 0x009CC270), which ReadEntryData 0x009CC2A0 expands in place. Same text as the landed 0x009CC240 body and the header's
  GetEntryDataType.
- Without the new macro the header is token-identical to the base (the block is the only change).
- Users: rows/009cc2a0.cpp (+ its test) and rows/009cc240.cpp (now the nf88 pick-any scaffolding; it drops ENTRYDATASIZE for the new macro).
- pin_asserts.py (`engine/CBankFile.h`): pinned 0 (every assert already carries its `FABLE_STATIC_ASSERT_AT` line).
- Landed includers (`grep -rlE 'engine/CBankFile.h' rebuild/src/compiled`): 117 rows, listed in hdr/CBankFile_includers.txt. None defines `FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE`
  (the macro is new), so the opt-in changes none of them. NF137 / NF138 overlays have no CBankFile.h.
