# Batch `2026-10-13_linux_b4` — 28 commented matching candidates (unverified)

Fourth batch from the Linux container lane (method and grading command in
`../2026-10-13_linux_b1/README.md`; use this directory's `payload.json` / `oracle.tsv`).
Same selection rule as b2/b3, plus five CTCVillage functions and one CTCHeroStats function that
the refined container rule below made consistent. 56 TUs pass the host syntax check, the purity
gate and the shared-header policy check.

## The container-size rule, refined (supersedes b2/b3 wording for lists)

Relative to Ego_r.pdb (`engine/<Class>.h`), the shipped binary's containers are:

| Container | PDB size | Retail size | Shift per instance |
|---|---|---|---|
| `std::vector` | 0x10 | 0x0C `{first, last, end}` | -4 |
| `CArray`, `std::map`, `std::set`, `CVectorMap` | 0x10 / 0x14 | 4 less | -4 |
| `std::list` | 0x0C | 0x04 (head pointer only) | -8 |

Proof in this batch: `CTCVillage` has 17 lists and one set before `Initialised` (PDB +0x104);
17 x 8 + 4 = 0x8C, and the retail bytes read `AmountOfFineAlreadyPaid` at +0xA0 (PDB +0x12C),
`HeroEnemyOfGuardsUntil` at +0xD4 (PDB +0x160), `CurrentIsHeroCriminal` at +0xE4 (PDB +0x170) and
`CrateTasks` at +0x60 (PDB +0xD0, the 15th list: 0x28 + 14 x 4). `CPlayer` reads
`InitialPlayerModeSet` at +0x216 (PDB +0x21E: one list, `PlayerModes`, before it). `CTCHeroStats`
reads the `Marriages` list head at +0x1D8 (PDB +0x1F0: six vectors/maps before it).
`CClassFactory::ClassList` is at +0x18 (PDB +0x1C: one vector before it). Every earlier shift
(b2/b3) is consistent with this table.

## Functions

| Address | Function |
|---|---|
| 00a02680 | CHandleBasedMemoryPool::ResizePool |
| 00a37130 | CClassFactory::DeleteAllClasses |
| 00bf7370 | CLandscapeBackgroundTreeNode::SetVisibilityFlagAndUpdateParentChain |
| 0059b679 | CTattooDef::IsConflicting |
| 00637ae0 | CThingMapManager::GetMapsMemoryFootprint |
| 006ceef0 | CTCWife::WantsNiceGift |
| 0082a270 | CMarkerInfo::IsPulsingOnMap |
| 009f9d80 | CTexture::GetPixelFormat (IDirect3DTexture9::GetLevelDesc) |
| 00a3abb0 | CCompressorZlib::FinishDecompressStream (zlib inflateEnd) |
| 00488a80 | CPlayer::InitialiseIfUninitialised |
| 00576c0b | CTCInventoryExperience::ProcessExperienceActionAssign |
| 005bf16e | CCategory::UpdateItemStatsForThing |
| 005bf9b5 | CCategory::GetAllInventoryItemsOfTypeInCategory |
| 006afe20 | CTCPhysicsBase::CalculateRadius |
| 00851860 | CVirtualKeyboard::Cancel |
| 00993f00 | CDataOutputStream::WriteNullTerminatedWString |
| 009959f0 | CDataOutputStream::WriteString |
| 009fbf20 | CResource::SetResourceSize |
| 0049d4b0 | CWorld::IsDiskSpaceForSaveGame |
| 0057ad1d | CTCHeroStats::GetNumberOfCurrentSpouses |
| 0057ac29 | CTCHeroStats::HasMarriageInState |
| 00666d10 | CThingCreatureBase::GetCombatTypeDef |
| 009fbfb0 | CResource::ListRemove |
| 0065c760 | CTCVillage::PayFines |
| 006588dc | CTCVillage::IncreaseHeroEnemyOfGuardsUntil |
| 0065bfce | CTCVillage::IsHeroCriminal |
| 00658e09 | CTCVillage::IsThereACratePutDownTask |
| 00658ea0 | CTCVillage::GetBestCrateTaskForThing |

## Skipped on purpose

`CTCInventoryClothing::OpenInventory`, `CEditWorldMap::IsMapChangeable`,
`C3DMeshFilePrimitiveChunk::SetMaterialIndex`, `CEngineLandscapeRenderer::UnregisterSubPatchForZSorting`,
`CManager::AnimHasEvent`, `CTCInventoryItem::OnCreate`, `CTCInventory::{StartGameRendering,ConstructScreenPC,ProcessButtonYReleased}`
(offsets the rule cannot yet place or callees whose identity is unknown),
`CTCInventoryTrade::AddItemOfTypeToInventory` (keeps `this` in edx across a call: a custom calling
convention the verifier cannot reproduce from C++), `CEnvironment::GetLocalEnvironmentThemeIndexFromGlobal`
(`__fastcall` with a stack argument, body unclear), `CTCHeroStats::GetMaxExtraMaxHealth`,
`CThingPhysical::ValidateInitialisation`, `CCategory::RemoveItem`, the two `GFTransferVectorField_*`
template bodies and `CThingGameObject::~CThingGameObject`.
