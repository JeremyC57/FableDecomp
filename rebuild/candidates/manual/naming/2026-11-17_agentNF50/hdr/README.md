# NF50 header overlay (round 7)

Two full copies of repo headers, each with one `#ifdef`/`#elif` opt-in added. Without the opt-in macro each copy is
token-identical to the repo header (`diff` shows only the added lines). Re-diff against the repo before merging.

| Overlay file | Copies | Added opt-in | Used by |
|---|---|---|---|
| `stlport/fable_map.h` | `rebuild/include/stlport/fable_map.h` | `FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE`: a new `#elif` branch on `_Rb_tree::_M_find` (the member template), giving the same STLport 4.6.1 body under `__declspec(noinline)` (MSVC only). | 0x0059DB82, 0x0059E6BD, 0x0059FAFE (CTCInventoryTrade refresh rows; retail calls the map<CComponent*,CList*> `_M_find` 0x005B1A43 out of line and keeps the map pointer in `edi` across the call, which the declaration-only `FABLE_RETAIL_STLP_TREE_MFIND_DECL` does not reproduce: 293 v 281 bytes). |
| `engine/CVertexGroupBlock.h` | `rebuild/include/engine/CVertexGroupBlock.h` | `FABLE_RETAIL_CVERTEXGROUPBLOCK_ISSUBSETOF`: declares `bool IsSubsetOf(const CVertexGroupBlock&) const; // 0x00AD8C30` (FableWin.pdb / Ego_r.pdb `?IsSubsetOf@CVertexGroupBlock@@QBE_NABV1@@Z`) inside the existing `FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS` block. | 0x00AD8C30. |

Unrelated-includer check: the landed repo rows 0x00AD9770 (CVertexGroupBlockList::RemoveAllSubsets) and 0x00AD9F10
(CVertexGroupBlockOptimiser::AddBlocks), which include both overlaid headers without either new macro, were regraded through
vland with `FABLE_HDR_OVERLAY` set (`work/recheck_unrelated.log`). Every row in the final recheck also compiles through the
overlay, and most of them do not define either new macro.
