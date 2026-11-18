# NF78 header overlay (round 7)

Five repo headers are overlaid. Base copies are in `../orig/`; at the final recheck each one was still byte-identical to the repo
header (`cmp`). Every change is an `#ifdef` opt-in with a member-specific macro. With no opt-in defined, each header preprocesses to
the same tokens as the repo copy: the blocks are added, and one existing declaration and one access label sit in the `#else` arm of
their new opt-in.

| Header (copy of rebuild/include/...) | Opt-in | Used by | What it adds |
|---|---|---|---|
| stlport/fable_map.h | `FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY` | 0x004F97B3 | STLport 4.6.2 `_tree.h` `_Rb_tree(const _Rb_tree&)`. It is `FABLE_FORCEINLINE` (EXTRA_RULES 2026-11-08 inline-budget preference 3). Grades: default, `/d2inlS22` and `/d2inlS60` all kept it out of line (18 v 126 bytes). It also declares the private `_M_copy` when neither `MCOPY_BODY` nor `TREE_ASSIGN_BODY` does. |
| stlport/fable_map.h | `FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY` | 0x004F97B3 | STLport `_map.h` `map(const map& x) : _M_t(x._M_t) {}`. Do not define it together with `MAP_COPY_OUTOFLINE`. |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_LIST_ASSIGN_BODY` | 0x00ADD020 | STLport `_list.c` `list::operator=` (body after the class) and `erase(first, last)` declared only (0x00ADC4E0). Do not define it together with `LIST_ASSIGN_POPBACK_DECL`. |
| engine/CHitParameters.h | `FABLE_RETAIL_CHITPARAMETERS_COPY_CTOR` | 0x00812170 | Declares `CHitParameters(const CHitParameters&)`, Ego_r.pdb 0x00807C70 (public). It needs `VECTOR_MEMBERS` + `DAMAGEMAP` + `AUGMENTATIONS`. |
| engine/CPersistTraits.h | `FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYIN_NOINLINE` | 0x007ADBF0 | In `BOOL_CLASS`, the `CPersistTraits<bool>::TransferBinaryIn` body (the landed 0x00403EB0 text, `value=is.ReadEBOOL();`) is visible but `__declspec(noinline)`. Without the opt-in the existing declaration stays (`#else` arm). |
| candidates/world_map_theme.h | `FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED` | 0x0063A970 (+ its test) | In the legacy local CMap model, the five theme members (AddThemeDefIndexToPalette, Set/GetEngineThemeAt, Set/GetEngineBlendAt) become protected, as both PDBs give them, and CWorldMap becomes a friend (engine/CMap.h has the same `CWORLDMAP_FRIEND`). The `#else` arm keeps the original `public:`. |

**pin_asserts.py** (base `/home/user/FableDecomp/rebuild/include`, run after every edit): it pinned 1 assert in total, in
candidates/world_map_theme.h. `FABLE_STATIC_ASSERT(sizeof(C2DCoordI) == 8)` became `FABLE_STATIC_ASSERT_AT(101, ...)`, its original
line. The other four headers pinned 0: their asserts were already `_AT` (fable_map.h 699; CHitParameters.h 102..111 and 236), or the
edit was above none.

**Link note for the coordinator (BOOL_BINARYIN_NOINLINE):** the in-class body is an inline (COMDAT) definition of
`?TransferBinaryIn@?$CPersistTraits@_N@@QAEXAAVCMemoryDataInputStream@@AA_N@Z`. The landed 0x00403EB0 row defines the same function
out of class, which is not inline. That is harmless per-TU. A whole-program link that takes both objects may need the 0x00403EB0
definition marked `inline`. The retail 0x00403EB0 is itself a COMDAT, so an inline header body is the likely original.
