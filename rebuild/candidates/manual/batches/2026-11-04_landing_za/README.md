# Landing 2026-11-04za: chunk 765 (82 tiny groups-ledger rows: adjustor / vcall thunks and small members), 81 landed

## Result
- **Landed 81 of 82** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): 6 MATCH and
  75 RELOCATION_MATCH, all PASS. 2-3 grades per row (`STATUS.tsv`).
  - **51 NUISystem adjustor thunks (+4 / +0x18):** CMenuEntry, CDynamicList, CParticleEffect, CSpellContainer, CScrollBarOutside,
    CDraggable, CClickable, CMouseCursor and others.
    - A thunk row only needs a translation unit that defines the class's ctor or dtor. CComponent 0x00535AB0 / 0x00535AC0 and
      CWorld 0x004AE8A0 reuse the landed ctor / dtor bodies. Only the thunk is graded.
  - **The vcall{0} thunk 0x0043E420** (`??_9@$BA@AE`, no class).
  - **About 30 small members:** CThing / CTCBase / CThingManager accessors, CGameCameraManager::GetAngleXY / GetAngleYZ,
    CTCWallMount Get/PeekObjectAttached, CActionPlayAnimation::OnActivate, CTCHero::FrameUpdate and others.
  - The genuine-original count rose by 29: compiler-generated thunks are counted separately from original functions.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile`.
- **Not landed: 0x00686D90** `?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ`. Its bytes match, but the landed
  `CTCBase_PeekThingSearchTools_006afd40.cpp` already holds that symbol for 0x006AFD40.
  - 0x006AFD40 is really `?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ`: Ego_r's order lines up with retail
    0x006AFD30 / 0x006AFD40 / 0x006AFD60, all four callers are CTCPhysics* methods, and one landed caller already binds it.
  - Queued (`land0411za`): respell 0x006AFD40, then land `pending/2026-11-04_linux_hand765/differ/00686d90.cpp` unchanged.

## Headers (21 changed + 32 new)
- Every change sits behind a new FABLE_RETAIL_* sub-macro or a new retail branch, for example CThing `_MANAGERACCESS`,
  CThingManager `_ACCESSORS`, CTCBase `_SERIALISEVERSION`, CGameCameraManager / CCamera `_ANGLES` and CComponentContainer `_CTORS`.
  The full list is in `agent_NOTES.md`.
- The 32 new headers include 15 NUISystem classes, CTCHero, CTCBob, CUserProfileManager and the four NTCScriptedControl actions.
- hdrcheck2: `default=same` for all 21 changed headers (`logs/hdrcheck2_pre_merge.txt`). `ppcmp.py`: all 1,004 includers same.
- The agent also regraded 6 landed siblings on its mirror: all still match.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04za.tsv` (81). `apply_labels.py` made 77 catalog and 77 manifest renames.
- **Slot names proven:** CTCScriptedControl::CActionBase vtable 0x01265504 (OnActivate / OnAdd / OnCleanup), C3DAnimationStateBase
  slots 2-5, CIDrawWorld slots 0-3, CTCBase slot 9 FrameUpdate, and CThingGameObject slots 38 / 39 GetThingStandingOn.
- **Queued PRIORITY** (`land0411za`, 29), among others:
  - 0x006AFD40 CTCPhysicsBase::GetThingSearchTools;
  - 0x0051E2D0 / 0x0051E2E0 CThingManager::GetSerialiseVersion / IsSerialisingGameState;
  - 0x00833230 / 0x008332E0 IsMessageEventWithinSightRange / IsMessageEventWithinHearingRange;
  - 0x004A6AB0 `??1CWorld@@UAE@XZ` (its file is misnamed NHeroInformationScreensCBase).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,788 genuine of 49,568; 5,832,285 function bytes (57.06%).
