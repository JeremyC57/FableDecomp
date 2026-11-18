# 2026-11-18 landing egod2: 2 rows from the in-house Ego_d author egod2a (1,654 bytes)

Owner: "Have a single opus 5.5 high agent working on more near misses that ego_d is likely to help match."
Rows: `../../pending/2026-11-18_egod2/` (14 reserved, now released; hand-back files in `return/`).

| row | name | bytes | what closed it |
|---|---|---:|---|
| 0x007243C0 | CTCPhysicsStandard::UpdateVelocityForImpassability | 617 | the old local-model draft rewritten on shared headers with Ego_d's locals and scopes; `final_force.Clear()`, `cell_offset` member by member, `to_impass = C3DVector(x, y, 0) + cell_offset`, existing DOT_ORDERED_XYZ |
| 0x00B40260 | CEngineMap::PeekHasWaterFast | 1,037 | the landed twin PeekWaterDepth 0x00B40BD0's text and opt-ins plus Ego_d's nested t0 { t1 { t2 } }; `// Flags: /d2inlT1267` with an Inline-budget line (bisected: T1266 leaves the first PeekThemeId site a call) |

- Dry run on the repo headers and landing, `LANDVERIFY_STRICT_HEADERS=1`: both RELOCATION_MATCH + PASS (`logs/`). Warnings: RELOC_NAME
  0x00880200 (manifest label `NUISystem::CSpellContainerList::AddChild` is wrong; bound GetGroundHeight), RELOC_ALIAS `__CIfmod` (import thunk).
- Headers: two additions under new opt-ins, no assert moved, default paths unchanged (assert_names_check 0 mismatches):
  `FABLE_RETAIL_CTCPHYSICSBASE_GROUNDHEIGHT` (engine/CTCPhysicsBase.h) and `FABLE_RETAIL_CWORLDMAP_IMPASSABILITYAROUND`
  (engine/CWorldMap.h: GetImpassabilityFactorAround 0x0063E3C0 + g_BlockNeighbourOffsets 0x01385978, Ego_r + reloc_bindings).
- Purity: no `__asm` / `naked` / `_emit` / `register` in either row or test. Funclets 1,355 / 0 broken; bindings_multi_check at baseline.
- `/d2inlT1267` is high next to the twin's T576; the author bisected it as the smallest value and had no c2 inline trace tool.
