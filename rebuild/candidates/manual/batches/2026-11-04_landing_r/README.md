# Landing 2026-11-04r: chunk 755 (retry of 39 parked same-length rows, 235-299 bytes, C++ only), 11 landed

## Result
- **Landed 11 of 39** (`logs/land.log`). Each was graded read-only on the merged tree first (`logs/new2.log`): 10 RELOCATION_MATCH
  and 1 RELOCATION_MATCH[/EHsc] (PKCDeferredBank::~PKCDeferredBank), all PASS.
  - The rows: NUISystem::CScrollingComponent::UpdatePosition, CPhysicsMesh::HasBoundingBoxIntersectedFromTo, CSoundManagerLH::AddSound,
    PKCDeferredBank::~PKCDeferredBank, CAIStateGroup_CreatureRangedCombat2::IsAbleToFire, CTCActionUseScriptedHook::OnInitialActivate,
    NCameraModes::CScriptedControl::OnInit, CTCShop::FrameUpdate, CombatDialogueManager::AttemptToSpeak,
    CAIStateGroup_MinionGuardFollowPlayer::Update and CAIStateGroup_UsesSeat::AllocChairSittingMode.
  - What fixed each one is in `agent_NOTES.md`. The main fixes:
    - retail class headers in place of old local models (they remove most this/temporary register swaps);
    - a named `std::map::iterator` from `find()`;
    - an early `Terminate(); return;`;
    - an `else` arm with a single return;
    - the inline PDB wrapper `CScriptModuleBase::OnInit`.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile` in any row or test.
- **Parked 28** (`pending/2026-11-04_linux_hand755/differ/`). Each has 10+ grades (`STATUS.tsv`), a `// Grade:` line and a
  passing test. The residues are load scheduling around `fstp`, x87 operand order, frame slots and callee-saved register swaps.

## Headers (16 changed; every addition behind a new or existing FABLE_RETAIL_* opt-in)
- **Nested opt-ins:**
  - CTCBase `_COMPONENT`
  - CMainGameComponent `_PLAYERMANAGER` / `_WORLD` / `_WORLDFRAME`
  - CPlayerGui `_CRIME`
  - CThing `_SCRIPTSUPERVISION`
  - CAIStateGroupBase `_AIACTIONS` / `_GETCREATURE`
  - CTCVillage `_GUARDFOLLOW`
- **New opt-ins:** FABLE_RETAIL_CAIBRAIN_GETCREATURE, _CTCSHOP, _CTCTALK, _COMBATDIALOGUEMANAGER, _CTCHASTE and _CTCCREATUREMODEMANAGER.
- **Retail-branch declarations added:** CTCTargeted, CTCBulletTime, CTCVillage::IsHeroCriminal and the C2DVector operators.
- **Merged with chunk 754** (both changed these after the chunk base):
  - CTCVillage.h and C2DVector.h merged 3-way cleanly.
  - CAIStateGroupBase.h was resolved by hand as the union of both chunks' declarations.
  - CIntelligentPointer.h: both chunks defined `operator=` under FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (GetPItem then SetPItem).
    754's landed spelling was kept. 755's row 0x00729F60 regraded RELOCATION_MATCH with it.
- `hdrcheck2`: `default=same` (`logs/hdrcheck2_pre_merge.txt`).
- **Siblings:** `ppcmp.py` over 1,253 includers shows 24 different. All 24 were rechecked on the merged tree (`logs/recheck.log`):
  RELOCATION_MATCH + PASS.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04r.tsv` (20). `apply_labels.py` made 12 catalog and 9 manifest renames.
- **Queued PRIORITY** (`land0411r`), among others:
  - SetUpPEntranceConnectedToFromUID, GFFloatToCharString;
  - CTCShop::UpdateStock (was OnInitialActivateBody);
  - CTCBase::PeekComponent (was GetGameSubsystem / GetFrameSource);
  - CTCVillage::SetGuardFollowingPlayer, CWorld::PeekMyPlayer;
  - AIAction_MoveToThingProximity / AIAction_LookAtThing;
  - CModeSitChairBored / CModeSitChairSleeping;
  - NUISystem::CManager::GetInstance (was CFrontEndManager).
- **Stale label:** 0x0129BA3C is labelled 'EPSILON' but is `?DELTA_NOTIONAL_ZERO@@3MB`. vland reported it as a RELOC_NAME warning.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,651 genuine of 49,568; 5,808,983 function bytes (56.82%).
