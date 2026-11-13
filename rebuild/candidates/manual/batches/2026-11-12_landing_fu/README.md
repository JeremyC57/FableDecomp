# 2026-11-13 landing fu -- 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks (wf20, after repair; flag /Gr)

Source: `pending/2026-11-12_linux_handWF20/`. The row matched in workflow wf20 (three author passes) with four verifier findings.
An Opus repair (`lane_tools/workflows/repair_and_verify_row.js`) fixed all four without changing the code: the header overlay is
pruned to the four opt-ins the source really uses (the dead attempt leftovers SORT2_BODY, LIST_ASSIGN_BODY, LIST_POPBACK_NOINLINE,
HASGROUP_DECL are gone); the carried inline HasMoreBlocks body now cites where retail expands it (0x00AD9F66..0x00AD9F99; no
out-of-line copy in retail); two name fixes are handed back with evidence (the landed 0x00ADB570 is
`std::list<CVertexGroupBlock>::size`, not `SortTreeRecursively`; 0x00A96DA0 is `~CVertexGroupBlock`).
Both verifiers confirmed bytes, test, purity and the landing-verifier dry run. Their one finding: the fable_containers.h overlay
had gone stale against a landing made while the repair ran. Rebased at landing by 3-way merge; the two landed rows that use the
blocks it would have dropped (0x00523CF0, 0x00956060) were rechecked: RELOCATION_MATCH, PASS.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. Headers (additions under opt-ins with no other user; defaults
unchanged): stlport/fable_containers.h (LIST_ASSIGN_POPBACK_DECL), stlport/fable_map.h (TREE_CONSTFIND_NOINLINE),
CVertexGroupBlock.h. Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fu): 0x00AD92E0, 0x00ADB570, 0x00A96DA0. Workflow wf20 is ten of ten landed.
