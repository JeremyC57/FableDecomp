# Hand chunk 707 (CCreatureAction_RiseUp .. CVirtualKeyboard): 10 of 10 byte-exact

All ten rows are RELOCATION_MATCH: VC7.1 RTM 13.10.3077, QTAG=c707, FABLE_HDR_OVERLAY=hdr. Every leaf is in `objdump -t`. calls.py
confirms every relocation target, and const_check finds the 4 float constants correct (0 mismatches). Behaviour tests PASS under g++
(tall.sh) and VC7.1 (tall.py). Extents: every row ends in `jmp`/`ret` followed by int3 padding up to the next 0x10 boundary, so
there are no extent fixes and no mid-function fragments.

## What matched (10)
- **CCreatureAction_RiseUp::FinishAction 0x008B6A90**
  - What it does: if the action has not been performed yet, it performs it. Then an AI creature (CClass::Type 1) gets
    `SuspendAI(false, 0)`, which undoes Initialise 0x008B6A60. Last, the protected `CCombatActionBase::FinishAction` runs as a tail jump.
  - Name: Ego_r + FableWin `?FinishAction@CCreatureAction_RiseUp@@UAEXXZ`; vtable 0x01268C9C slot 4.
  - Shape: the creature is a named local inside the `if`, so GetPItem runs before the two argument pushes. The one-line cast form
    gave 63v63 with three instructions moved.
  - Byte twin: 0x008B6A20 is `CCreatureAction_GoUnderground::FinishAction` (vtable 0x0127A894 slot 4, chunk 706). Two symbols, no conflict.
- **CCreatureAction_ScreamerDie::FrameUpdate 0x008B8B80**
  - What it does: sets the appearance alpha to `(unsigned char)((1.0f - GetPercentageComplete()) * 255.0f)`, then calls the base
    FrameUpdate.
  - Name: both PDBs; vtable 0x0127B374 slot 56.
  - Shape: the appearance pointer comes from FableWin's inline `GetTCGraphicAppearance()` (CThing +0x64) and is loaded before the
    percentage call. Constants 1.0f is at 0x0122DED8 and 255.0f at 0x01230014.
- **CCreatureAction_BalverineLungeAttack::FinishAction 0x008CC8A0**
  - What it does: calls slot 84 `CTCPhysicsBase::SetAsPushableByHero(true)` (FableWin) through the inline `GetPhysicsTC()` (+0x60),
    then `RemoveTC((ETCInterfaceType)200)` (200 = CTCHitOnCollision), then the base FinishAction.
  - Name: both PDBs; vtable 0x01283A54 slot 4.
- **NCreatureMode::CModeDrained::OnActivate 0x008D68A0**
  - What it does: an AI creature (Type 1) gets `SuspendAI(true, 0)`; a player creature (Type 2) gets
    `GetPlayer().AddMode((EPlayerMode)31, false)`. The `CModeStandBase::OnActivate` tail is duplicated into both arms.
  - Name: both PDBs `?OnActivate@CModeDrained@NCreatureMode@@UAEXXZ`; vtable 0x01268234 slot 5.
  - EPlayerMode 31 has no enumerator name in any name set.
- **CAIStateGroup_SkillPractice::Update 0x0092B3A0**
  - What it does: `if (!PracticeSkills() || IsStopping()) { Terminate(); return; }`, then
    `NextFramePractice = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 60; Terminate();`.
  - Name: both PDBs; vtable 0x0128AF1C slot 12. PracticeSkills is slot 14, added by ApprenticeHero.
  - Shape: the guard-clause form gives the two duplicated `jmp Terminate` exits. The single-if form gave 52v60.
- **CGraphicFrame::InitialiseAsSurface 0x009FCB20 / InitialiseAsSurfaceSection 0x009FCB60** (byte twins, different callees)
  - What they do: `FrameData = new CGraphicFrameData; return FrameData->InitialiseAsSurface[Section](args...);`. The FableWin inline
    `??0CGraphicFrameData` sets ReferenceCount 1, clears the texture and IsInitialised, and the class is pack(1) at 27 bytes
    (`push 1Bh`). The forwarded call becomes a tail `jmp`.
  - Names: Ego_r symbols, in donor order with the CGraphicFrameData members 0x009FC950 / 0x009FCA40. The callee argument counts
    (`ret 14h` / `ret 1Ch`) and the callers' float pushes tell the two apart.
- **CGameCameraManager::SetRightStickLookAroundVector 0x00697B10**
  - What it does: `RightStickTargetLookAroundAngles = C2DVector(vector.X * 0.25f, vector.Y * 0.125f); CurrentMode->SetRightStickLookAround(vector);`.
  - Name: both PDBs; Ego_r donor order after ResetRightStickLookAroundVector 0x006976B0.
  - Shape: the FableWin two-float ctor temporary is built on the stack and copied as dwords. Separate float stores gave 48v68.
  - Callers: 0x0068D790 (landed ProcessInput) and an uncatalogued `__fastcall` helper at 0x0068D590.
- **CThingAICreature::OnKill 0x00830D80**
  - What it does, in order:
    1. If in melee, calls `GetCombatManager().DeregisterDestroyedCombatant(*this)`.
    2. Calls `CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetQuestInfo().RemoveElementsForThing(*this)`.
    3. If `PBrain` (+0x1A8) is not null, calls `PBrain->OnKill()`.
    4. Calls `CThingCreatureBase::OnKill()`.
  - Name: both PDBs; vtable 0x0127293C slot 12. CThing::OnKill is protected (MAE).
- **CVirtualKeyboard::~CVirtualKeyboard 0x00851720**
  - What it does: `PEntryBox->SetActive(false); PEntryBox->IgnoreAllEvents(); NUISystem::CManager::GetInstance()->RemoveObserver(PEntryBox);`.
    The ProfileName CWideString destructor is the tail jump. The class is not polymorphic.
  - Shape: the CObserver base sits at +4 of the edit box, so the upcast is the null-tested `p ? p+4 : 0`.
  - Name: both PDBs; Ego_r donor order matches the five landed neighbours.

## Parked for Fable 5.1 (0)

## Skipped (0)

## Label corrections
The full rows are in NAME_FIXES.tsv. Only the first two are chunk rows; the others are callees or callers found while proving
identities.
- 0x009FCB60: the catalog row carries its twin's name, InitialiseAsSurface. The correct name is
  `?InitialiseAsSurfaceSection@CGraphicFrame@@...MMJJ_N1@Z`.
- 0x008D68A0: the catalog module "CModeDrained" lacks the `NCreatureMode` namespace.
- 0x0088B6F0 `?OnKill@CAIBrain@@QAEXXZ`: the landed CThingAICreature_OnLevelUnload_00833ba0.cpp declares it as
  `CAIBrain::OnLevelUnload`, a symbol that exists in neither PDB. That gives one address a second spelling.
- 0x00594F78 `??_GCVirtualKeyboard@@QAEPAXI@Z` (FableWin): it is landed as `CLandscapeBackgroundPatch` vector deleting destructor, with
  callee `AutoTinyScalarDestructor_00851720`. That callee should be `??1CVirtualKeyboard@@QAE@XZ`.
- 0x0084CF40 `?SetRightStickLookAround@CBase@NCameraModes@@QAEXABVC2DVector@@@Z`: it is landed as `CEditControlCentre::OnEngineSurveyLMBHeld`,
  a C3DVector stand-in. Its only caller is 0x00697B10.
- 0x006925E0 `?GetPercentageComplete@CCreatureActionBase@@QBEMXZ`: it is landed as `CInputTypeMouseMovementEvent::GetMovementRatio`, and four
  landed callers call it GetProgress, GetProgressRatio or GetAnimationFraction. vland RELOC_NAME flagged it.
- No new chunk-707 spelling collides with a landed row. Every callee spelling used here matches the landed rows at the same address.

## Header debt
Every one of these was worked around with a local model, written with the header's offsets; see each src comment.
- **Flattened generated layouts** (vptr as data, no member functions):
  - engine/CCreatureActionBase.h, CCreatureAction_BalverineLungeAttack.h, CThingAICreature.h, CThingCreatureBase.h,
    CModeStandBase.h and CPlayer.h.
  - engine/CGameCameraManager.h: CurrentMode should be a CCountedPointer<NCameraModes::CBase> at +0x98, and the two RightStick
    members should be C2DVector at +0xc4 / +0xcc.
  - engine/CObserver.h and CManager.h: global-namespace layouts; they should be NUISystem::CObserver / NUISystem::CManager : CObservable.
  - engine/CWorld.h: needs `static long __fastcall GetFrame()`.
  - engine/CPlayerGui.h: needs PDrawQuestInfo as a CCountedPointer at +0x1c8 (Ego_r +0x1d0) and the inline GetQuestInfo.
  - engine/CAIBrain.h: needs `void OnKill()` 0x0088B6F0 (its offsets are already retail, e.g. OnKillCalled +0xa4).
  - engine/CTCPhysicsBase.h: needs the virtual slot list up to slot 84 `SetAsPushableByHero(bool)` (FableWin
    `?SetAsPushableByHero@CTCPhysicsBase@@UAEX_N@Z`).
  - vland flags SHARED_HEADER_AVAILABLE for CAIBrain / CPlayerGui (0x00830D80) and CTCPhysicsBase (0x008CC8A0). All three headers are
    flattened data layouts with no member functions, so the rows need the local models until those headers get a retail branch.
- **CThingAICreature.h**: the retail PBrain is at +0x1A8; the Ego_r header has it at +0x1D8.
- **No engine header**: CThing (CClass-derived, slot 12 OnKill protected), CCombatActionBase (0x114, PCreature +8),
  NUISystem::CComponent / CEditBox (slot 150 SetActive) / CObservable,
  CGraphicFrameData (pack(1), 27 bytes), CMainGameComponent (ConstantFPS / GetConstantFPS), CTBaseSingletonNoDefaultAlloc<T>.
