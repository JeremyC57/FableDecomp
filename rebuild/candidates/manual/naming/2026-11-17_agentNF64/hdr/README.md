# NF64 header overlays (round 7, slice land1117fu)

Three overlay headers. Each one adds `#ifdef` opt-in blocks only. Without its macro, each file is token-identical to the repo
header. `diff repo overlay | grep '^<'` is empty for all three.

The repo copies of fable_map.h and CInputProcessGameBase.h changed during this session (10:39, a landing that added NF62's
`FABLE_RETAIL_INPUTPROCESS_GETMOUSEMOVEMENT` among others). Both overlays were rebased onto the new repo text with a three-way
merge (no conflicts), re-checked to be token-identical without the macros, and recheck_final.log was run on the rebased overlays.
If the repo headers move again before landing, re-apply the opt-in blocks the same way.

| Overlay | Opt-in macro | Rows using it | What it adds | Evidence |
|---|---|---|---|---|
| `stlport/fable_map.h` | `FABLE_RETAIL_STLP_TREE_ASSIGN_BODY` | 27 tree `operator=` rows (21 in rows/, 6 held in work/ready) | Declares the private `_M_copy(x, p)` (declaration only, and only when NF61's `FABLE_RETAIL_STLP_TREE_MCOPY_BODY` is not defined). Adds the STLport 4.6.2 `stl/_tree.c` body of `_Rb_tree::operator=(const _Rb_tree&)`, which has these steps: skip on self-assignment; `clear()`; zero the count; copy the comparator; then either reset the header to an empty tree, or `_M_copy` the source root under the header and walk to leftmost / rightmost. | STLport 4.6.2 `_tree.c` source. The landed callers 0x006303C0, 0x00465C54, 0x004321F0 and 0x00631720 bind `??4?$_Rb_tree@...@QAEAAV01@ABV01@@Z`. Each row graded RELOCATION_MATCH and passed vland. |
| `engine/CInputProcessGameBase.h` | `FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON` | 12 CInputProcess* ProcessInput rows on the flat (generated) layout | Forward-declares `class CInputEvent`. Declares `IsEventButtonPressed`, `IsEventButtonHeld` and `IsEventButtonReleased(const CInputEvent&) const` as protected (0x00687A90 / 0x00687BD0 / 0x00687B30). Declarations only; the layout is unchanged. This is the flat-branch twin of the retail branch's `FABLE_RETAIL_INPUTPROCESS_ZTARGET` / `_MOVEMENT`. | Ego_r.pdb 0x006B2BA0 / 0x006B2C40 / 0x006B2CE0 and FableWin.pdb 0x021D3D20 / 0x021D3E10 / 0x021D3F00 both spell `?IsEventButton*@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z`. |
| `engine/CInputProcessHeroInformationScreens.h` | `FABLE_RETAIL_CINPUTPROCESSHEROINFORMATIONSCREENS_PROCESSINPUT` | 0x0068C6D0 (self re-author) | Forward-declares `CInputEvent` / `CProcessedInput`. Declares `virtual void ProcessInput(const CInputEvent&, CProcessedInput&);`. | Ego_r.pdb 0x006B83C0 and FableWin.pdb 0x021E12B0: `?ProcessInput@CInputProcessHeroInformationScreens@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z`. |

Rows also use existing repo opt-ins, unchanged:

- CTCHaste: `FABLE_RETAIL_CTCHASTE_UNSET` and NF61's `_UNSET_INLINE`.
- Vectors: `FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE`, `_CLEAR_OUTOFLINE`, `FABLE_RETAIL_STLP_VECTOR`, `FABLE_RETAIL_STLP_VECTOR_C`.
- Maps: `FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE`, `FABLE_RETAIL_STLP_MAP_CTOR_NOINLINE`.
- Others: `FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS`, `FABLE_RETAIL_CTCINVENTORYITEM_*`.

Each row lists its macros at its top.
