# NF180 header overlay

## stlport/fable_containers.h
- Copy of the repo header (orig/stlport/fable_containers.h, byte-equal to the working tree at the start of this session) plus ONE add-only
  nested opt-in inside the FABLE_RETAIL_STLP_SORT_FASTCALL_BODY block:
  `FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE` -- declares STLport's __final_insertion_sort as an out-of-line __fastcall template
  (declaration only) instead of the __forceinline body. Used by row 0x00CB7780 (std::sort<pair<CCharString,CScriptInfo>*, ..>, which
  calls the landed 0x00CB70E0 out of line). The macro name is new (no other header or row defines it).
- Default path token-identical: without the macro the preprocessor emits exactly the old text (the `#else` arm is the unchanged body).
- pin_asserts.py: the header has no FABLE_STATIC_ASSERT lines; nothing pinned.
- Header users: every landed includer of stlport/fable_containers.h (grep -rlE 'fable_containers\.h' rebuild/src/compiled) is unchanged,
  because none defines FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE (grep count 0).
- No other running agent overlays this header (NF176: engine/CMeshDataBank.h; NF177: CBankFile / CEngineStaticMapFileBlockManager /
  CTCWeapon / fable_retail_banks; NF178: stlport/fable_map.h; NF179: none).
