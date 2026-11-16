# 2026-11-16 landing nf42: name-fix round 5, NF42 (47 landed rows, 31 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 5 continues NF40's half: tags land1115ga ..
land1116p23, 157 entries. Brief and records: `naming/2026-11-16_agentNF42/`.

## Queue (157 entries, counted from APPLIED.tsv)

| Status | Entries |
|---|---|
| done | 24 |
| done-already | 1 |
| rejected | 2 |
| partial | 8 |
| blocked | 4 |
| carry | 118 |

- **rejected:**
  - 46: an `_Rb_tree::_M_lower_bound` spelling that is not proven;
  - 74: ~CLipSyncFrameData at 0x006E54D0 is not proven, because the destructor is implicit.
- **blocked:**
  - 60 / 152: VC7.1 rejects every explicit instantiation of the dependent-return count_if with C2785, after 4 grades on 0x0065F6A6;
  - 75 / 127: authoring. Their only wrong binders are jmp-thunk rows with an `extern "C" Forward` body.
- **partial:**
  - 20, 38, 42, 50, 98, 110, 111, 112. For 38 / 98 (GPLightingManager), three more binders now bind the claim; 0x00B54310 and
    0x00B76BC0 have local C4DVector / CArray models that collide with the shared header.
  - The coordinator also counts **44, 45 and 109** as partial, though NF42 marked them done. Their claims are bound exactly, but NF42
    also renamed the binder rows 0x0042D5B1, 0x0042B6B8 and 0x00C4B010, whose own landed callers (10, in NOTES.md) still bind the
    old spellings.
- **carry:** 23 triaged with a reason, 95 with fresh probe counts.

## Rows (47; one new test)
- **RENAMES:** section 1 has 31 self renames; section 2 has 49 caller respellings.
- **New test:** 0x005B1FF4's old test linked a helper the row no longer emits.
- **Recheck (`logs/recheck_part0..1.log`):** all 47 graded from the repo after landing: 6 MATCH and 41 RELOCATION_MATCH, every test
  PASS. No funclet includes a changed row, and funclet_includes_check reports 1,355 checked, 0 broken.
- **Headers (opt-in additions only; every conditional balanced):**
  - stlport/fable_map.h `FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE`;
  - stlport/fable_containers.h `FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE`;
  - libpk/PKCBufferedReader.h `..._RETURNALLBUFFERS` (ReturnAllBuffers is descriptive);
  - engine/LSocket.h `FABLE_RETAIL_LSOCKET_THINK`;
  - engine/CEngine.h `..._SETSPOTLIGHTSHADOWMODE`;
  - engine/CEngineLightingManager.h `..._SETSHADOWMODE`.
- **Bindings:** rebind run, bindings_multi_check at baseline.

## Notes
- 0x00B32230's host-only stub gains `DisablePointLights()` so the non-MSVC branch parses.
- The tests of 0x00B32230 and 0x00BAC0F0 use their own fakes, so they pass without exercising the new spellings.
