# 2026-11-12 landing fc -- 0x00492973 CSubtitleRenderer::SetText (wf13, after two repair rounds)

Source: `pending/2026-11-12_linux_handWF13/`. The row matched in workflow wf13 with verifier findings about the callee bodies it
carries. Two Opus repair rounds (`lane_tools/workflows/repair_and_verify_row.js`), each followed by two independent verifiers:
- Round 1: RemoveMessage 0x00493B5B is now `inline __declspec(noinline)` (a header-inline member VC7.1 emitted out of line; body =
  the landed row's); each carried body's origin is stated in the source; the `_List_base<const CMessageEvent*>` spelling of
  0x00493BC2 / 0x00493BDD is marked unproven by any PDB (retail call graph: 0x00493CCA forwards to 0x00493BC2, 0x00493CDC is
  `jmp 0x00493BDD`); the three pragmas are justified by retail address + instruction and are the TU's established spelling.
- The round-1 mechanical verifier then PROVED one carried body was a stand-in: FindAllMessages graded DIFFER(81v81) against
  0x00493EAB (`mov eax,[ebp+0Ch]` at 0x00493EE4 vs ecx).
- Round 2: one define, FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE (an opt-in the repo header already has), spells list::size() the STLport
  way (`distance(begin(), end())`). The same source text now grades RELOCATION_MATCH against 0x00493EAB, 0x00493B5B, 0x00493BC2,
  0x00493BDD and the row 0x00492973. Both verifiers confirmed all five grades; their one remaining finding (a stale NOTES sentence)
  was fixed by the coordinator.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. Reserved: 0x00492381 is only declared and called. Headers (additions
under new opt-ins, unused elsewhere, defaults unchanged): stlport/fable_deque.h, CMessageEventFilters.h, CMessageEventManager.h,
CTCHeroReceiveItems.h, CSubtitleRenderer.h. Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fc): 0x00493BC2, 0x00493BDD (STLport _List_base ctor/dtor; not in a PDB, weigh in the naming pass).
