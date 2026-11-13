# acctB hand-back, author workflows bwf1 / bwf2 (second harvest): 8 rows matched, 54,069 bytes

Same lane and layout as `2026-11-12_acctB_bwf2a` (author workflow on acctB's reserved rows, all agents Opus; overlays based on
`FullDecomp` at `bb0dc27e9`). Nothing is landed. `STATUS.tsv` lists address, grade, length, class and name as matched.

| address | name (as matched) | bytes |
|---|---|---|
| 0x004678E2 | `CInventoryDef::Transfer` | 5422 |
| 0x005A9CBE | `CTCInventoryTrade::ConstructTradeDescriptionWeapon` | 4714 |
| 0x00B87870 | `CEngineScreenEffectDisplacementRenderer::ApplyDisplacement` | 4944 |
| 0x005A04F7 | `CTCInventoryTrade::ConstructNormalShopList` | 9469 |
| 0x0073B130 | `CTCCreatureModeManager::RegisterModes` | 6673 |
| 0x00B9ECE0 | `CEnginePrimitiveManagerStaticMeshesBase` constructor | 8921 |
| 0x00BCFF10 | `CEnginePrimitiveManagerAnimatedMeshesBase` constructor | 8140 |
| 0x00D6D670 | `NScript::CQ_HangingTreeEvilScript::Conclusion` (catalog label `CQ_HangingTreeEvil_Execute`) | 5786 |

## Verification state

- **Coordinator (acctB) re-verification, done:** read-only `vland.py` with `LANDVERIFY_STRICT_HEADERS=1` and each row's overlay:
  all eight RELOCATION_MATCH + PASS (`coordverify/<addr>.vland.log`); purity scan clean; no oracle bytes in this folder.
- **Workflow verifier lenses / repair:** still running for these rows; a row that a repair changes is re-staged.
- One `RELOC_CONFLICT` warning to settle at landing:
  `00b87870   WARN RELOC_CONFLICT: ?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA -> 01396dd8 here, 01396de8 in landed 00bbc460`
- `notes/<addr>/NOTES.md` carries each row's name fixes with evidence and its made-up names; `hdr/<addr>/` its overlay.
