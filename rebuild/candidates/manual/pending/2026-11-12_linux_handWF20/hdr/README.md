## 0x00AD9F10

Overlay for row 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks (wf20). Each file = the CURRENT repo header + only the opt-ins below
(repair 3 pruned every dead attempt leftover; `diff rebuild/include/<f> hdr/<f> | grep -c '^<'` = 0 for all three).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL -- list operator= / pop_back declared only (out of line in retail, 0x00ADD020 / 0x00ADC790).
- stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE -- tree const find body visible but __declspec(noinline) (0x006699C0).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_REMOVELAST -- RemoveLast(unsigned long) declaration (0x00AD9470).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP -- declares ?AddGroup@CVertexGroupBlock@@QAEXK@Z (landed 0x00AD91F0) so the row can carry InsertBlock's real body.
Removed in repair 3 (never defined by the final source): FABLE_RETAIL_STLP_LIST_POPBACK_NOINLINE, FABLE_RETAIL_STLP_LIST_ASSIGN_BODY,
FABLE_RETAIL_STLP_SORT2_BODY (+ _NOINLINE), FABLE_RETAIL_CVERTEXGROUPBLOCK_HASGROUP_DECL.
Repo opt-ins the row also defines (already in the shared headers): FABLE_RETAIL_STLP_VECTOR, _VECTOR_RESERVE, _VECTOR_PUSHBACK_NOINLINE,
FABLE_RETAIL_STLP_LIST, _LIST_PUSHBACK_NOINLINE, FABLE_RETAIL_STLP_SORT2, FABLE_RETAIL_CARRAY, FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS.
- (coordinator, at landing) fable_containers.h and fable_map.h overlays rebased by 3-way merge onto the current repo headers (they keep VECTOR_RANGE_INSERT / COPY_NOINLINE of 0x00523CF0 / 0x00956060 and add only LIST_ASSIGN_POPBACK_DECL and TREE_CONSTFIND_NOINLINE; the '0 removed lines' claim predates that landing).

