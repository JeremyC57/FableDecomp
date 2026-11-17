# NF72 overlay headers (round 7, land1117fu)

Each file here is a copy of the repo header (the copy as taken is in `orig/`, and every `orig/` file matched the repo when this was written)
plus NF72 opt-ins only. Every opt-in sits under `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>`, so the default path is unchanged. Token identity
was checked with `tokid.sh` (preprocess with and without the overlay, normalise `__LINE__` static-assert names, then `cmp`). For
CBankFileAsync.h the check was repeated with FABLE_RETAIL_CBANKFILEASYNC_DATA + _ASYNCUPDATEPENDING defined (2205 lines, identical),
because the default path holds no class.

| header | opt-ins added | rows that use them | default path |
|---|---|---|---|
| engine/CBankFile.h | FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA (PackedDataOffset / PackedDataSize as CPackedUIntArray; private `void PackRuntimeData();` 0x009CD740); FABLE_RETAIL_CBANKFILE_ALIGN (`unsigned long Align(unsigned long) const;` 0x009CBB90). the diff names FILENAMEINDICES only in a comment (the existing repo opt-in that PACKRUNTIMEDATA needs) | 009cd740, 009cfb70, 009cc240, 009d5260 | token-identical (103 lines) |
| engine/CPackedUIntArray.h | FABLE_RETAIL_CPACKEDUINTARRAY_PACK (Initialise(const void*, unsigned long, long, unsigned long) 0x00A62D80, `unsigned long Sizeof() const` 0x00A62A60, Clear() 0x00A629D0) | 009cd740 | token-identical (10 lines) |
| engine/NGameText_CDataBank.h | FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_CTOR (`CGroupEntry(CCountedPointer<CBankFileAsyncData>&, const CDataBankGroupEntry*)` 0x009C8060, declaration only) | 009c80f0 | token-identical (230 lines) |
| engine/CAIStateGroup_CreatureFindCover.h | FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_UPDATE (retail class `: public CAIStateGroup_CreatureUseCoverBase` with `virtual void Update();` 0x0097CCD0 and protected `bool IsStillRunnable();` 0x0097CB10, sizeof 0x4c; the flattened default model is kept in the `#else`) | 0097ccd0 | token-identical (29 lines) |
| engine/CBankFileAsync.h | FABLE_RETAIL_CBANKFILEASYNC_UPDATEPENDING_BODY (public `void ProcessDisposalList();` 0x009D6730 + private `void RestartAbortedReads();` 0x009D5A90, both Ego_r.pdb + FableWin.pdb; declarations only) | 009d68b0 | token-identical (default empty; DATA path 2205 lines) |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_SORT_FASTCALL_BODY (inside the existing SORT_FASTCALL branch: __lg, the __fastcall helper declarations, `__final_insertion_sort` marked `__forceinline` under `_MSC_VER`, and the __fastcall sort body) | 00a84160 | token-identical (1782 lines) |

Rebase note: the repo fable_containers.h changed during the session (12:49, NF71's FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED).
The overlay was rebuilt with `patch` from the current repo copy (my hunk applied at offset 7, clean). `orig/` now holds that current copy.

Review flag: FABLE_RETAIL_STLP_SORT_FASTCALL_BODY uses a raw `__forceinline` inside `#if defined(_MSC_VER)` because FABLE_FORCEINLINE is
not defined at that point in fable_containers.h. With plain `inline`, VC7.1 did not expand __final_insertion_sort (70 v 115 bytes).
It is an inline-budget control (the owner's FORCEINLINE rule, 2026-11-08), not asm.

## Static-assert pinning (coordinator rule, landing fz)
Each overlay was run through `tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py <base> $W/hdr <relpath>`, with the
`orig/` copy (= the repo file at merge base) as `<base>`:
- **engine/CBankFile.h: pinned 37.** The PACKRUNTIMEDATA / ALIGN opt-ins sit above the 37 layout asserts. Each became
  FABLE_STATIC_ASSERT_AT(<orig line>, ...); the preprocessed typedef names match the repo's (fable_static_assert_266, _268, ...).
- **engine/CAIStateGroup_CreatureFindCover.h: pinned 0.** The one assert the tool reports is the new `sizeof == 0x4c` inside the
  FINDCOVER_UPDATE opt-in. It is not a moved assert.
- **CPackedUIntArray.h, NGameText_CDataBank.h, CBankFileAsync.h, fable_containers.h: pinned 0.** No assert moved.

Every row that uses an overlay is regraded in recheck_final.log, after the pin.
Observation: during the session the repo CBankFile.h showed the coordinator's pins as FABLE_STATIC_ASSERT_AT(246..284) at lines 266..304.
At the final check it was plain again, with FABLE_STATIC_ASSERT at 266.., identical to `orig/`. This overlay pins to the current repo lines
(266..). If the coordinator's final pin numbers differ, theirs win for those 37 lines at the three-way merge.
