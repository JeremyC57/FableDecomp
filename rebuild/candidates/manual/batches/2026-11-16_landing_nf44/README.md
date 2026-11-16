# 2026-11-16 landing nf44: name-fix round 6, NF44 (50 landed rows, 4 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 6 continues NF42's half: tags land1115ga ..
land1116p23, 129 entries. Brief and records: `naming/2026-11-16_agentNF44/`.

## Queue (129 entries, counted from APPLIED.tsv)

| Status | Entries |
|---|---|
| done | 2 |
| partial | 10 |
| carry | 117 |

- **done:** 88 (0x00C4B010 follow-up) and 103 (CString::operator=). A fresh compile of each self row shows it already spells the
  claim.
- **partial:** 23, 29, 30, 31, 34, 35, 37, 40, 41, 79. Most binders were ported this round. What is left is in NOTES.md and
  under Open below.
- **carry:** 117, each with a fresh probe count (ok / bad binders) in APPLIED.tsv.

Most of the remaining work is full ports, not respellings. Rows that model a shared-header class locally have to move onto the
header before the binder can name the real callee.

## Rows (50; one new test)
- **RENAMES (`RENAMES_applied.tsv`):** section 1 has 4 self renames. 2 of them are file moves (`naming/batch_rerow.py`):
  - 0x0042BEAD is CTCInventoryWeapons::AddComponentToAvailableWeaponsEntries;
  - 0x00533B30 is NUISystem::CComponent::GetCurrentState, replacing the placeholder `vfunc_50`.
  - 0x005518E0 / 0x00551BC0 change their leaf only.
- **Section 2:** 66 changed relocation bindings, read from a fresh compile of each original source and each new one.
- **Families ported:**
  - the GPU scratch-texture family (19 rows), onto `NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager()`;
  - GPEngineWorldMap, GPRenderManager, GPOcclusionManager, GPShadowRenderer and GPCamera;
  - 18 of the 23 stale callers of 0x0042D5B1, onto engine/C2DVector.h, NUISystem_CUIState.h and stlport/fable_map.h;
  - CString::operator= for 0x00C31230 / 0x00C313C0, onto engine/CString.h.
- **New test:** 0x00C4B040's old test defined stand-ins the row no longer calls.
- **Recheck (`logs/recheck_part0..1.log`):** all 50 graded from the repo after landing. All 50 are RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - No funclet includes a changed row; funclet_includes_check reports 1,355 checked, 0 broken.
  - `naming/rebind.py` removed 67 binding lines and added 59.
  - bindings_multi_check is at its baseline (the one `NullCoord` entry).

## Headers (opt-in additions; default path unchanged)
- **engine/CGPUScratchBufferTextureManager.h:**
  - `..._FREEMEMORYPAGE`, `..._ENDTEXTUREUSE`: Ego_r declarations;
  - `..._SURFACEUSE`: Start/EndSurfaceUse.
- **engine/CEngineOcclusionManager.h:** `..._GETRESULT`.
- **engine/CEngineMap.h:** `FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP` declares `NEngineInternal::GPEngineWorldMap` (both PDBs).
  - It is more than an opt-in: nine body lines now read the global through `FABLE_CENGINEMAP_WORLDMAP`. Without the opt-in that
    macro expands to the old `g_pEngineWorldMap`.
  - The coordinator preprocessed the old and new header (`cpp -P`, default path). The token streams are identical.

## Open (for later rounds)
- **0x0053CFC2:** its own leaf is a local one-parameter `vector<CUIStateMap>::push_back` model. Respell it to the STLport spelling,
  then its 4 callers (0x0053B91E, 0x0053BEAC, 0x0053BF20, 0x0053C332).
- **0x00F36EE0 (entry 23):** every binder now binds StartTextureUse. The self row still spells the placeholder `?M@S@@QAEPAXH@Z`.
- **0x00B242A0:** now binds GPCamera, but it still carries a pre-existing one-method local CEngineCamera view.
- **0x0042D5B1, 5 callers left:**
  - 0x00427F31 / 0x0042819A have a local CRGBColour constructor;
  - 0x00545B10 has a local namespace std;
  - 0x00555440 / 0x005556F0 have their own leaf on a local C2DVector.
- **0x0042B6B8, 2 callers left:** 0x0059DE04 / 0x0059DE76, whose local list pushes the key by reference.
- **Mislabelled stubs:** the catalog also labels 0x005BC0C6 and 0x004DFBC6 `NUISystem::CComponent::GetCurrentState`. Both are
  13-byte placeholder free functions, not the virtual. The real one is 0x00533B30. The labels look propagated; the rows need
  their own names.
- **Pre-existing local models edited:** in 0x00B5BD30, 0x00B6D630, 0x00B869A0 and 0x00B86A30, handle fields changed from `void*`
  to `long` inside local models. No member was added; the models remain row debt.
- **Placeholder classes in recheck output:** 0x00C313C0 is still `CUnprovenRecordReader_012b4d34::OnRecord`, an older name this round
  did not change. 0x00C4B040's leaf `_Dest_val_pair_CCharString_CSymbolInfo` has no name evidence.
