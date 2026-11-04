# Landing 2026-11-04q: chunk 754 (retry of 40 parked same-length rows, 168-231 bytes, C++ only), 13 landed

## Result
- **Landed 13 of 40** (`logs/land.log`). All 13 were graded read-only on the merged tree with no overlay first (`logs/new2.log`):
  RELOCATION_MATCH + PASS.
  - The rows: CMenuEntry::MakeUnavailable, CTCInventoryQuests::MoveToNextTestQuest, CTCWife::Murdered,
    CTCDParticleEmitter::Construct, CGameCameraManager::ResetFlags, CTCSpecialAbilities::SetupDrainLifeSpell,
    NCameraModes::CTrack::OnInitLeaf, CAIStateGroup_WorkInWarehouse::IsRunnable, CAIStateGroup_SitForMeal::Update,
    CDefinitionManager::DecompressDefinitionData, CMatrix4x4::SetInverseOfUniformScaleRotationTranslation,
    NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter and CWeaponDrawState::Initialize.
  - What fixed each one is in `agent_NOTES.md`. In summary:
    - retail class headers with PDB signatures and identities;
    - copying a landed same-TU sibling's source verbatim;
    - reading a base into a named local after construction;
    - writing members before copying them into locals.
  - CWeaponDrawState is the catalog's class name; no PDB covers that audio code.
- **Asm rules (owner, 2026-11-04):** no asm, helper or `volatile` anywhere. The only pragma is the landed sibling's
  `auto_inline(off)` around same-TU bodies (00aeb600), the documented EXTRA_RULES device.
- **Parked 27** (`pending/2026-11-04_linux_hand754/differ/`). Each has 10+ grades (`STATUS.tsv`), a `// Grade:` line and a
  passing test. The residues are register rotations, frame slots, block or scheduling order, one x87 operand order and one lea
  operand order.

## Headers (8 changed, each behind a new opt-in sub-macro or an existing retail branch; none changed in the repo since the overlay base)
- New sub-macros: FABLE_RETAIL_CGAMECAMERAMANAGER_CAMPOINTS, _CINTELLIGENTPOINTER_ASSIGN / _COPY, _CTCHEROSTATS_MARRIAGE,
  _CTCSHAPEMANAGER_TRACK, _CTCVILLAGE_CRATETASK, _CTCWIFE_MARRIAGEEND and _CVECTORMAP_UPPERBOUND.
- Two declarations were added inside FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS.
- hdrcheck2 `default=same` for all 8 headers (`logs/hdrcheck2_pre_merge.txt`). The FABLE_RETAIL_CTCVILLAGE "(cpp-err)" note is the
  same with the overlay as without it.
- **Siblings:** `includers.py` found 689 files; `ppcmp.py` shows 687 same and 2 different. Both differing files were rechecked on the
  merged tree (`logs/recheck.log`): RELOCATION_MATCH + PASS.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04q.tsv` (14). `apply_labels.py` made 13 catalog and 7 manifest renames.
  0x00968460 is GetPaymentFinishedMessage; its manifest label had been propagated across a twin.
- **Queued PRIORITY** (`land0411q`):
  - IsInWorkHours (template base);
  - EHeroExperienceStatCategory parameters;
  - CTCEnemy::AreThingsEnemies;
  - SetMarriageState (private, const CThingAICreature*);
  - NShape GetBestTrack / GetTrackPosNearest;
  - the NParticleEngine-namespaced CreateInterpolatedEmitter, which landed callers 0x00AEB6D0 / 0x00AEB850 bind without the namespace.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,642 genuine of 49,568 (94.10%); 5,806,096 function bytes (56.79%).
