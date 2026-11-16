# 2026-11-16 landing nf37: name-fix round 3, NF37 (83 landed rows, 39 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 3 continues NF35's half: tags land1011fj ..
land1113fz, 340 entries. Brief and records: `naming/2026-11-16_agentNF37/`.

## Queue (340 entries)

| Status | Entries |
|---|---|
| done | 45 |
| partial | 12 |
| blocked | 5 |
| deferred | 1 |
| carry | 277 |

- **done:**
  - **Data globals:** the `g_Max*` / `g_Memory*` globals (now `long`, descriptive); `CAFile::m_errorCallback` through the
    header's DoErrorCallback in five rows; 0x0125916C, which is the literal 8.0f.
  - **Self rows moved onto shared-header opt-ins:** about 30, including the CInputProcessGameBase queries,
    CTCEnemy::IsAlliedWithFaction and DetachZBuffer.
  - **Counted-pointer instances.**
  - **Caller-only fixes.**
- **partial:** each note names the rows still wrong. For IsEventGameAction, 12 callers are fixed and 4 are left.
- **blocked:**
  - 1 authoring: the wrong caller is a compiler-generated deleting destructor;
  - 4 port, after one or two failed grades each.
- **deferred:** q194; its callers 0x00B9E950 / 0x00BD1F70 were held by NF38.
- **carry:** 277 never reached, plus q286, which was reached but needs 19 NUISystem rows re-modelled off a local counted pointer.

## Rows (83; 17 tests changed, PassPattern kept)
- **RENAMES section 1:** 39 self renames. Three rows outside the queue had to follow: 0x008EF910 UpdateCoinSounds(bool) and the
  deleters 0x00429F43 / 0x0069BBA0.
- **RENAMES section 2:** 62 caller respellings.
- **Recheck (`logs/recheck.log`):** all 83, graded from the repo after landing, plus 0x00B2D330, the one landed user of the
  RenderManagerCore macro. 23 MATCH and 61 RELOCATION_MATCH, every test PASS.
- **Headers (opt-in only; every overlay copies the current repo header and only adds lines; conditionals balanced):**
  - CFaction.h `FABLE_RETAIL_CFACTION_ISALLIEDWITHFACTION`;
  - CGamePlayerInterface.h `..._GAMEACTIONQUERIES`;
  - CInputProcessGameBase.h `FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS`;
  - CRenderManagerCore.h `FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY` on the flattened layout. The retail branch already
    carried the macro; its user 0x00B2D330 takes the retail branch and was rechecked.
- **Bindings:** rebind run, bindings_multi_check at baseline, funclet_includes_check reports 1,355 checked, 0 broken.

## Found, not fixed
- 0x0076D460 binds a wrong spelling of CFaction::IsAlliedWithFaction at 0x006B9460.
- 0x00972490 and 0x00B991D0 include a shared header under a renaming macro and model the class locally. 0x00F37170 and 0x00689130
  already had local models.
