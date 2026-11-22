# NF231 header overlay (2026-11-22): two add-only, member-specific opt-ins (entry 2, row 0x0042EBB6 Shutdown)

Base copies: `orig/engine/CNewFrontendGameComponent.h` and `orig/engine/CStartupDefinitionManager.h` (the repo tree as of this run, nf230
landed). Other overlays (`ls $S/w22/*/hdr/engine/{CNewFrontendGameComponent,CStartupDefinitionManager}.h`): the first only in the
landed NF229 / NF230 / NF47 / NF76 copies, the second nowhere (auth20 has neither). `grep -rlwE` of both new macros over rebuild/ and every
`$S/w22/*/hdr`: hits only in this hand-back, so every existing includer compiles the same tokens as before. No layout change.

- `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES` (engine/CNewFrontendGameComponent.h, inside the RUN branch; needs
  FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN): types the two opaque `unsigned char[0xc]` members as `std::vector<CWideString> FileNames`
  (+0x6C) and `std::vector<CDateAndTime> FileTimes` (+0x78), and includes stlport/fable_containers.h + engine/CDateAndTime.h under the
  same macro. Sources: Ego_r.pdb (struct_layouts_egor.tsv: `vector<CWideString,std::allocator<CWideString>_> FileNames`,
  `vector<CDateAndTime,std::allocator<CDateAndTime>_> FileTimes`) and Ego_d.pdb (field list 0x57888 of CNewFrontendGameComponent 0x57889:
  FileNames type 0x7D2B = std::vector<CWideString,std::allocator<CWideString> >, FileTimes type 0x1D9E0 =
  std::vector<CDateAndTime,std::allocator<CDateAndTime> >, both private; resolved with codex pass26 egod_type.py). Retail: 12-byte STLport
  vectors at +0x6C / +0x78 (Shutdown 0x0042EC31 / 0x0042EC3E). Same sizes and offsets as the opaque arrays (the class's pinned
  `FABLE_STATIC_ASSERT_AT(89, sizeof == 0x148)` holds). User: rows/0042ebb6.cpp (and its test).
- `FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_SET` (engine/CStartupDefinitionManager.h, inside the FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
  branch): `static void __fastcall Set(CStartupDefinitionManager* manager);` (declaration). Ego_r.pdb 0x00441A90 and FableWin.pdb 0x01B31560
  ?Set@CStartupDefinitionManager@@SIXPAV1@@Z (public static __fastcall); retail 0x004336B0 `push ecx ; mov ecx,0x013B8760 ; call
  0x00433D3A ; ret` (hands the pointer to the counted pointer PClass 0x013B8760). The landed 0x0042F5A9 already binds this spelling
  (compiled census work/bind_004336b0.tsv). User: rows/0042ebb6.cpp (and its test).

Proof:
- pin_asserts.py rebuild/include hdr: engine/CNewFrontendGameComponent.h pinned 0; engine/CStartupDefinitionManager.h pinned 3 (its three
  plain `FABLE_STATIC_ASSERT` at lines 32 / 72 / 73 became `FABLE_STATIC_ASSERT_AT(32 / 72 / 73, ...)`, keeping their typedef names).
- assert_names_check.py rebuild/include hdr: mismatches 0.
- equiv_check.py (rebuild/include v work/inc_new = rebuild/include + this overlay, after pinning) over the 43 landed rows whose source or test
  includes either header (work/incl_e2.txt; no header under rebuild/include includes either): 0 differing (work/equiv_e2.tsv is empty).
  No landed row defines either new macro, so none changes. The user row includes both overlaid headers itself (NF213 lesson).
