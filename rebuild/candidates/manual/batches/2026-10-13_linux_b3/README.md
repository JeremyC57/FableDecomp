# Batch `2026-10-13_linux_b3` — 23 commented matching candidates (21 landed)

Third batch from the Linux container lane (method and grading command in
`../2026-10-13_linux_b1/README.md`; use this directory's `payload.json` / `oracle.tsv`).
Same selection rule as batch b2 (catalog-unique label, header-backed class); 46 TUs pass the host
syntax check, the purity gate and the shared-header policy check.

## Lane grading result (2026-10-13, Wine + DDK 13.10.4035 `cl.exe`, `/O2 /Oy /W3` + lander sweep)

Graded in the Linux container with `tools/decomp_pipeline/linux_lane/` and landed with `verify_and_land.py --land`:
**21 of 23 rows are byte-exact (MATCH / RELOCATION_MATCH) with behaviour PASS** and now live in
`rebuild/src/compiled/`, `rebuild/tests/` and the catalog. 7 rows needed a second shape (retail-vector objects, two un-shared deltas, an inline `Clamp(long&)` under the "s" pragma, a sequencing local, an extern epsilon, a `<` that the first draft had as `>=`).

Still open (DIFFER, recorded in `tools/decomp_pipeline/crawl/gen_tried.txt`):

- 00a371c0 CClassFactory::AllocateClass — DIFFER(40v42): element address now matches; retail keeps `pop esi` inside each exit after the indirect call, our build hoists it above the branch
- 005d8e90 CTCInventoryItem::GetInventoryOwner — DIFFER(42v43): retail ends with `call GetThing ; ret`, our build emits the sibling-call `jmp`

## The container-size rule (supersedes the per-class "editor-only member" wording in b2)

Every class whose retail offsets disagree with `engine/<Class>.h` does so by a fixed amount per
std container that precedes the member: 4 bytes for a `std::vector` / `CArray` / `std::map` /
`std::set` / `CVectorMap`, and 8 bytes for a `std::list` (retail keeps only the head pointer; see
`../2026-10-13_linux_b4/README.md` for the table and the CTCVillage proof). A retail
`std::vector` is `{first, last, end}` with `first` at +0. Evidence in this batch: `CTCCombat`
(two vectors before `CurrentAbilityIndex`: PDB +0x4C, retail +0x44), `CTCCreatureGroupBoastingCrowd`
(PDB +0x30, retail +0x2C), `CTCTurncoat` (one vector: PDB +0x4C, retail +0x48), `CCategory` /
`CClassFactory` (vector `first` exactly at the header's vector offset). The Ego_r.pdb donor is an
editor build whose containers carry extra words. `gen_class_headers.py` can therefore derive
retail layouts mechanically from the PDB member types.

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
