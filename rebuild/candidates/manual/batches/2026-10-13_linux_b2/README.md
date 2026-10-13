# Batch `2026-10-13_linux_b2` — 42 commented matching candidates (unverified)

Second batch from the Linux container lane (see `../2026-10-13_linux_b1/README.md` for the
method and the grading command; substitute this directory's `payload.json` / `oracle.tsv`).
Targets were restricted to manifest rows whose (module, name) label is unique in the catalog
and whose class has an `engine/<Class>.h` header, so every member access is PDB-named unless the
file says otherwise. All 84 TUs pass the host syntax check, the purity gate and the shared-header
policy check.

## Retail-vs-PDB layout findings used here (each is stated in the file that relies on it)

| Class | Retail offset evidence | Conclusion |
|---|---|---|
| `CBankFile` | Alignment +0x98, HeaderSize/FileInformationSize/FileSize +0xA0/+0xA4/+0xA8, BankFile_Data +0x7C, ProgressCallback/WarningFlag +0xEC/+0xF4 | shipped build lacks the editor-only `UpdateData` CArray (+0x38, 0x10 bytes) and `SymbolCRCIndices` (0x14 bytes before +0x110) |
| `CTCHeroStats` | Stamina/MaxStamina/ExtraMaxStamina at +0x58/+0x5C/+0x90 | 8 bytes smaller than the PDB from the money block on |
| `CTCInventoryTrade` | CategoryIdent +0x170 (PDB +0x19C) | 0x2C bytes smaller before it |
| `CTCInventory` | ContextSensitiveFadeTimes vector at +0x160 (PDB +0x188) | 0x28 bytes smaller before it |
| `CLandscapeBackgroundTreeNode` | SubNode +0xA0 (PDB +0xA8) | 8 bytes smaller before it |
| `CNavigatorManager` | QuadTrees list +0x24 (PDB +0x30) | 0xC bytes smaller before it |
| `CQuestManager` | RegisteredQuestNames vector +0x28 (PDB +0x34) | 0xC bytes smaller before it |

Where a retail layout disagrees with the generated header the TU declares a `<Class>_Retail`
view at the retail offsets instead of including the header; `gen_class_headers.py` should pick
these up as reconciliation evidence once the batch lands.

## Relabelled

- `0x00CACBE0` is not `CMovableResourceMemoryPool::CanMoveResource`: it is a `__stdcall`
  COM helper that stores an interface pointer into an out-parameter and AddRefs it
  (`E_POINTER` on null). Authored as `AcquireInterfaceReference`.

## Functions

| Address | Function |
|---|---|
| 00a028b0 | CHandleBasedMemoryPool::InternalSetResource |
| 0049d8c0 | CWorld::EventIsWorldEvent |
| 00a0aa30 | CRenderManagerCore::SetVertexFormat (IDirect3DDevice9::SetFVF) |
| 005bc450 | CItem::IsSameItemTypeAsDefIndex |
| 009cbb90 | CBankFile::Align |
| 009cc4c0 | CBankFile::GetUnusedSpace |
| 009cbd90 | CBankFile::WriteBankWarning |
| 009cbf10 | CBankFile::GetBankPathname |
| 00643b3b | CTCPerceiveThing::IsAwareOfThing |
| 00666830 | CThingCreatureBase::UnsheatheAllWeapons |
| 006d6340 | CTCHeroExperience::ApplyPenaltyForDeath |
| 008023c0 | CTCTargetingPlayer::SetAsTargetNextThingInDirection |
| 0048da30 | CFadeInFadeOutBase::IsOpen |
| 00a0a9d0 | CRenderManagerCore::Uninit |
| 00bddd70 | CLocalDetailCacheMap::Render |
| 006b0130 | CTCPhysicsBase::SetAsMoving |
| 009cb710 | CStreamingFontDataBank::GetLastModifiedTime |
| 00633b10 | CPlayer::InitModeQuestCompletionUI |
| 00789b00 | CTCHeroRangedAttacks::ResetProjectileTargetingZoomParameter |
| 00970760 | CAIStateGroup_CreatureUseCoverBase::IsLowCover |
| 009e3940 | CPixelFormat::IsCompressed |
| 009fc4f0 | CResourceList::MakeFreeSpace |
| 00a0d2d0 | CAInputProcess::RemoveFromInterface |
| 00cacbe0 | AcquireInterfaceReference (relabelled, see above) |
| 00733580 | CTCCombat::RemoveFromCombat |
| 0057a730 | CTCHeroStats::SetMaxStamina |
| 0059b6f4 | CTCInventoryTrade::GetCategoryIndexForTradeType |
| 00581d2a | CTCInventory::GetContextSensitiveFadeAlpha |
| 007449b0 | CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing |
| 0077db90 | CTCHeroCentre::OnHeroWalkedThroughDoor |
| 0088b8c0 | CAIBrain::SetAllStateGroupsEnabled |
| 00bf6130 | CLandscapeBackgroundTreeNode::GetPatch |
| 00bf6160 | CLandscapeBackgroundTreeNode::GetSplitBranch |
| 00638310 | CThingMapManager::AddGameMap |
| 00658561 | CTCVillage::SetCropTaskAsAcceptedByThing |
| 007f5070 | CTCStockItem::FrameUpdate |
| 00a14e80 | CNavigatorManager::IsNavigationMapLoaded |
| 00a76240 | CNavBlockedQuadTreeNode::operator= |
| 004af3c0 | CQuestManager::GetRegisteredQuestFromIndex |
| 006d6470 | CTCHeroExperience::ForceSpendableExperienceTo |
| 00a23f00 | C3DAnimationInfo::IsCombinationAnimation |
| 00a23f50 | C3DAnimationInfo::IsLoaded |

## Skipped on purpose (label or layout contradicted by the bytes)

`CTCInventory::SetQuickAccessAssign`, `CParentDefClassBase::GetInstantiationName`,
`CSprite::GetHeight`, `CTCVillage::PayFines`, `CTCHeroStats::HasMarriageInState`,
`CDrawMiniMap::IsActive`, `CTCVillage::GetBestCrateTaskForThing`,
`CQuestManager::OnQuestStartScreenDisplayed`, `LHHttp2::GetDocumentSize`,
`CThingCreatureBase::{SetLastActionHandedness,PerformBlockedReaction,IsPerformingASneakingIncompatibleAction}`,
`CTCHeroExperience::IsAbleToBreakDownDoors`, `CThingPhysical::CanBlockNavigatingCreature`
(all read offsets the PDB header assigns to a different member), plus the destructors
(`~CChunkedFileRawDataChunk`, `~CTCMapwho`: the parity extractor keys on the leaf name, which a
decorated destructor symbol does not contain) and `CEditControlCentre::ActivateThemeDensityDisplay`,
`CTCInventoryClothing::{AddClothingMenuEntry,OnItemDeselected}`, `CEngineWaterRenderer::OpenStaticMapForUse`,
`CTCHeroStats::GetMaxExtraMaxStamina` (by-value temporaries whose codegen cannot be pinned without the compiler).
