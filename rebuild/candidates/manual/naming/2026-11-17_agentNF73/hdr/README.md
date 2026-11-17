# NF73 header overlay (round 7)

Every file is a copy of the CURRENT repo header (re-diffed at hand-back) with `#ifdef` opt-ins added. Without the macros each
header compiles to the repo text's meaning. Rows include the overlaid header FIRST (before any repo header that includes it by a
relative path), because MSVC resolves a quoted include from the includer's own directory before the overlay; every overlaid header
has a macro include guard, so the later repo-relative include is skipped.

| Overlay file | Opt-in added | Used by |
|---|---|---|
| stlport/fable_string.h | `FABLE_RETAIL_STLP_CHAR_TRAITS_COMPARE_INLINE`: STLport 4.6.2 char_traits.h `__char_traits_base::eq` / `compare` bodies inline (the repo declares compare only, out of line 0x00403C60). With `FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH` a `text == L""` test folds to `(finish - start) & ~1` with no compare call (retail 0x004092D3..0x004092DE; control grade without it: DIFFER 560v534 with a call to compare). The `#else` arm is the repo line, token-identical. | 0x004091C0 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE`: `__declspec(noinline)` on the STLport erase(first, last) body (mirror of ERASE_NOINLINE): visible, out of line (vector<CWideString>::erase 0x00412450). Pure addition before the repo `#ifdef ..._ERASE_RANGE_OUTOFLINE`. | 0x00596CCE, 0x00597006 (through the GetProfileNames body) |
| engine/CFrontEndManager.h | `FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY`: the body of the landed GetProfileNames 0x005955AB (same text as rows/005955ab.cpp), `inline __declspec(noinline)`, after the class in the DESTRUCTOR branch; includes engine/CAFile.h and fable_resource_directories.h. The includer defines BUILDCOMPONENTS, CCHARSTRING_ENDSWITH, CAFILE, CAFILE_SCANNEXTFILE, CAFILE_SPLITPATH_WIDE and the STLport PUSHBACK_NOINLINE / OVERFLOW_OUTOFLINE / ERASE_RANGE_NOINLINE (comment in the block). Pure addition. | 0x00596CCE, 0x00597006 |
| engine/CCountedPointer.h | `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE`: ReleaseData with the _RELEASEDATA body (Data zeroed before Info), visible but `__declspec(noinline)`; the destructor is `{ ReleaseData(); }` as for _RELEASEDATA / _DECL_ONLY. Two repo lines change only by an added alternative: the destructor's `#if defined(RELEASEDATA) \|\| defined(DECL_ONLY)` gains `\|\| defined(RELEASEDATA_NOINLINE)`, and the ReleaseData `#if defined(DECL_ONLY)` becomes `#elif` after the new `#if defined(RELEASEDATA_NOINLINE)` arm: without the macro both evaluate exactly as the repo text. | 0x00597006 |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_INITSTRUCTURE`: `CMainGameComponentInit InitStructure;` (+0x161D0, Ego_r.pdb member type; includes engine/CMainGameComponentInit.h) in place of the `unsigned char InitStructure[0x10]` pad (the `#else` arm is the repo line). Same size and offset. | 0x00416953 |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_STATICMAPEXPORT`: public const ConvertWorldToStaticMapPath(const CWideString&) (not with LOAD_ORIGINAL, which declares it), ConvertWorldToStaticMapPath(.., bool, bool), ConvertWorldToISMThingyPath(..) and (.., bool, bool): Ego_r.pdb / FableWin.pdb spellings, retail 0x0049DDD0 / 0x0049D550 / 0x0049DEC0 / 0x0049D6B0 (the landed rows' catalog names; Ego_r address order matches). Declarations only. | 0x00416953 |
| engine/CAFile.h | `FABLE_RETAIL_CAFILE_COPYFILE`: `static void __fastcall CopyFileW(CAFile&, CAFile&)` (Ego_r.pdb 0x00B009F0 ?CopyFileW@CAFile@@SIXAAV1@0@Z, retail 0x00998E70). Declaration only. | 0x00416953 |

No default path changed. Macro names are member-specific so the coordinator can merge them with NF70 / NF72 overlays.

Static asserts (coordinator rule, 2026-11-17): pin_asserts.py was run on all seven overlays and pinned 0 asserts. The only plain
FABLE_STATIC_ASSERT in them (CCountedPointer.h, CCPPointerInfo size) stays on repo line 59, above the opt-in; every assert below an
insertion (CMainGameComponent.h, CFrontEndManager.h) is already a FABLE_STATIC_ASSERT_AT form. No regrade was needed.
