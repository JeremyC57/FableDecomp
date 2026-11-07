# Chunk 855 (2026-11-08): six never-attempted rows + chunk 854's two WIP rows — 3 of 8 matched (landed `batches/2026-11-08_landing_f/`)

Matched (src/, test/): 0x00CA2BB0 floor1_encode, 0x00490BE0 CSubtitleRenderer::GetGraphicFromEmbeddedTag, 0x0052F900 NUISystem::CComponent::UpdateColour.

| Parked row (differ/) | Best | Lines | Grades | Residue |
|---|---|---|---|---|
| 0x00A68DE0 CVolumeTexture::LoadFromDataStream | 923v919 | 134 | 113 | `this` spill slot and the width/Z register roles |
| 0x007602F0 NShape::IsShapeConvex | 1135v1126 | 179 | 157 | register colouring only (calls and constants line up) |
| 0x00D0F640 CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre | 1893v2101 | 330 | 97 | per-site ~vector<CScriptThing> / ~CScriptThing inline split; kills/things slots swapped |
| 0x00507070 CWorldMap::BuildStaticMapGenerationVisibilityInfo | 1628v1432 | 403 | 112 | retail calls vector<long>'s copy ctor 0x00514160 for two copies, inlines the third |
| 0x00642AA0 CMinimapData::PopulateMinimapThemeList | 1308v1340 | 447 | 142 | frame 0x120 v 0x110 and the first loop's registers |

- Names: all eight proven (LABELS.tsv); descriptive: the two 0x00507070 globals (`g_bBuildRetailStaticMaps` 0x013B864A,
  `g_bEnableMapOptimisations` 0x013B864B), chunk 854's `CGameDefinitionManager::PGlobalDef` / `PeekGlobalDef`, the `EVolumePool` enumerators.
  NAME_FIXES.tsv: 0x0099EE20 GetAsUpperCase / 0x0099ED10 GetAsLowerCase (queued land0811f).
- Open: 0x005136B0 and 0x00514E20 are byte-identical `vector<1-byte>(n)` bodies both landed under the same `vector<unsigned char>` symbol;
  which one has the other element type is unproven.
- hdr/: see hdr/README.md; only the four opt-ins for the matched rows + CPlayerGuiDef.h were landed. The 17 root `fable_*.h` copies exist only so
  the overlay's fable_string.h resolves (do not land). 156 landed includers of the edited engine headers rechecked (SIBLINGS.tsv).
