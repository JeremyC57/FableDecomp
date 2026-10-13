# Batch `2026-10-13_linux_b3` — 23 commented matching candidates (unverified)

Third batch from the Linux container lane (method and grading command in
`../2026-10-13_linux_b1/README.md`; use this directory's `payload.json` / `oracle.tsv`).
Same selection rule as batch b2 (catalog-unique label, header-backed class); 46 TUs pass the host
syntax check, the purity gate and the shared-header policy check.

## The container-size rule (supersedes the per-class "editor-only member" wording in b2)

Every class whose retail offsets disagree with `engine/<Class>.h` does so by exactly 4 bytes per
std container (`std::vector`, `CArray`, `std::map`, `CVectorMap`, `std::list`) that precedes the
member, and a retail `std::vector` is `{first, last, end}` with `first` at +0. Evidence in this
batch: `CTCCombat` (two vectors before `CurrentAbilityIndex`: PDB +0x4C, retail +0x44),
`CTCCreatureGroupBoastingCrowd` (one pointer-holding member: PDB +0x30, retail +0x2C),
`CTCTurncoat` (one vector: PDB +0x4C, retail +0x48), `CCategory`/`CClassFactory` (vector `first`
exactly at the header's vector offset). The Ego_r.pdb donor is an editor build whose containers
carry one extra word. `gen_class_headers.py` can therefore derive retail layouts mechanically:
subtract 4 for each container member emitted before the target offset.

## Functions

| Address | Function |
|---|---|
| 005bf2c6 | CCategory::GetAllItemsInCategory |
| 00789c90 | CTCHeroRangedAttacks::IncrementProjectileWeaponMode |
| 007c7890 | CTCTurncoat::IsTurncoated |
| 00993c10 | CDataInputStream::SetPositionAssumeWithinSourceChunk |
| 009f2f90 | CSurface::GetDimensions (IDirect3DSurface9::GetDesc) |
| 009f9db0 | CTexture::GetDimensions (IDirect3DTexture9::GetLevelDesc) |
| 00bde160 | CLocalDetailCacheMap::OnMapVisibilityChanged |
| 004458f0 | CGamePlayerInterface::GetMouseMovement |
| 006af6d0 | CTCPhysicsBase::SetThingStandingOn |
| 007311f0 | CTCDoor::FrameUpdate |
| 00733480 | CTCCombat::GetPMeleeFlourishAbility |
| 007841c0 | CTCCreatureGroupBoastingCrowd::CurrentStateIsOffPodiumType |
| 00993f60 | CDataOutputStream::WritePixelFormat |
| 00a14eb0 | CNavigatorManager::IsNavigationMapActivated |
| 0057cb00 | CTCHeroStats::ForceRenownLevelTo |
| 0057c999 | CTCHeroStats::SetMoneyAsConfiscated |
| 00662820 | CThingCreatureBase::IsAnimCombinationAnim |
| 0088bbb0 | CAIBrain::InitCreaturePosition |
| 00a371c0 | CClassFactory::AllocateClass |
| 005d8e90 | CTCInventoryItem::GetInventoryOwner |
| 007899e0 | CTCHeroRangedAttacks::SetButtonPressure |
| 007c7810 | CTCTurncoat::SetTurncoatTimeLast |
| 009942d0 | CDataOutputStream::AlignData |

## Skipped on purpose

`CInputProcessGameBase::IsGameActionEventInQueue` (first argument unused, helper identity
unknown), `CTCTrapBase::{OnKill,UpdateTrigger}`, `CQuestManager::OnWorldDestruction`,
`CThingAICreature::IsAvailableForInteractionType`, `CGameScriptInterface::GetWaterHeightAtPosition`
(body is an init routine, not a height query), `CTCHeroStats::{GetStartingMaxStamina,GetMaxExtraMaxStamina,GetHeroStatExperience}`
(CDefPointer temporaries / switch jump chain), `CEngineResourceManager::UnloadShaderCode`
(label contradicted by a per-entry float loop), `CCategory::{RemoveItem,GetNumberOfItemsOfTypeInCategory}`
(STL find_if / pair construction), `CDataOutputStream::Write` (empty-vector pointer quirk),
`CThingGameObject::~CThingGameObject`.
