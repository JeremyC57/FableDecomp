# Chunk 831 notes (Linux lane, wave 25)

Six rows: 2 RELOCATION_MATCH (src/), 4 parked (differ/). No `/d2` flag, no `inline_depth`, no asm/naked/_emit/volatile anywhere.
Every hand-back file is in this directory. Overlay headers are both opt-in branches (hdr/README.md); the default paths are untouched.

## 0x00689DF0 CInputProcessInventory::ProcessInput -- RELOCATION_MATCH (src/00689df0.cpp)
- Name came from the catalog's PDB name and is confirmed by vtable 0x012376F8 slot 4 (.?AVCInputProcessInventory@@).
- Shape: one long if-chain over the event type and game action, with a 9-entry jump table (0x0068ACA8) and its byte index.
  It needs `/GS`, because the CGameEvent's 32-byte Data array gives the cookie frame, as in the landed siblings 0x0068B7C0 / 0x0068BC70.
- Key levers:
  - shared engine/CThing.h with FABLE_RETAIL_CTHING + _GETTC_EXPANDED;
  - a same-TU `__declspec(noinline)` IS_THING_ALIVE<T> header-template body, so VC7.1 keeps the hero in esi;
  - `processed.SetAsPerformedEvent()` placed after the alive `if`.
- Callee spellings: IsEventButtonReleased 0x00687B30 / IsEventButtonHeld 0x00687BD0 keep the landed spellings. Chunk 829 evidence says
  the two are swapped (queued land0711e); this row does not re-decide it.
- vland warnings, all non-fatal:
  - DELTA_NOTIONAL_ZERO at 0x0129BA3C is still labelled EPSILON in the DB. The wave-18 spelling is right; it is in NAME_FIXES.tsv.
  - SHARED_HEADER_AVAILABLE for C2DVector, CInputEvent, CProcessedInput, NUISystem::CManager, CMainGameComponent, CTCInventoryMap,
    CTCInventory, CPlayer and CInputProcessGameBase. This is header debt, listed below.

## 0x00E0F8A0 NScript::CQ_WaspBossScript::CWaspHelper::Main -- RELOCATION_MATCH (src/00e0f8a0.cpp)
- Name proven from three sources:
  - vtable 0x012E0284, stored by the landed Alloc 0x00E0F7F6, slot 1;
  - the Ego_r.pdb name set (`?Main@CWaspHelper@CQ_WaspBossScript@NScript@@EAEXXZ`, private virtual);
  - the Aeon Lua port of the wasp-helper thread.
- Catalog label `_global::CScriptGameResourceObjectScriptedThingBase_HandleWaspQueenHelper` was a placeholder.
- Shape: started from the landed CWaspVictim::Main 0x00E11670 (same TU family).
- Levers:
  - CTimer `operator=` / GetTime `__forceinline` (the brief's established CTimer exception);
  - `dist(Me) < dist(hero)` as one expression. Retail evaluates the left operand's distance first; a hero-first compare reorders the calls.
- Overlay opt-in: FABLE_RETAIL_CQ_WASPBOSSSCRIPT_WASPHELPER in engine/NScript_CQ_WaspBossScript.h, which forward-declares
  `class CWaspHelper;`. The row defines the class body after the include. Layout is from Ego_r: 0x20 bytes, ParentClass +0x14,
  PMasterData +0x18, PleadedToHero +0x1c, LeadToRegion +0x1d, WavedOver +0x1e.

## 0x00D6D080 NScript::CQ_HangingTreeEvilScript::Introduction -- DIFFER(1500v1516), 33 md, 10 attempts
- Name proven from three sources:
  - the Ego_r / FableWin name set;
  - the body plays CS_HANGINGTREE_EVIL_INTRO and sets the PDB member IntroCutscenePlayed;
  - the only caller is DoMission 0x00D6BFA0, calling right after SpawnParty.
- Old label `_global::CQ_HangingTreeEvil_Init` was a gap-scan placeholder.
- Overlay opt-in: FABLE_RETAIL_CQ_HANGINGTREEEVILSCRIPT_INTRODUCTION declares `void Introduction();`.
- Residue: a per-site inline split.
  - Retail inlines STLport `vector<STB>::resize(n)`. For its default-value temporary it calls the STB ctor 0x007E72A0 and dtor 0x007E74D0
    out of line, and then calls `resize(n, x)` 0x00CD3CCB. The hero handle's ctor/dtor in the same function are expanded.
  - VC7.1 inlines resize(n) only when ~STB is out of line, and that also makes the hero's ctor/dtor calls.
  - Each combination of OUTOFLINE_CTOR / DTOR / row-local inline bodies is logged in STATUS.tsv.
- Callees: CGameScriptInterfaceBase slots 7, 8, 70, 75, 309, 370, 371 and 379; RunCutsceneMacro 0x00CBFB7D; map operator[] 0x00CD3D2E;
  _M_erase 0x00CD344A. All are bound spellings.

## 0x008DEC80 NCreatureMode::CModeJackOfBladesFlash::FrameUpdate -- DIFFER(1647v1665), 108 md, 23 attempts
- Name proven: slot 3 of vtable 0x012857A8. Slot 3 is FrameUpdate in every creature-mode vtable (CModeBase declaration order), plus the
  PDB name. Old label: vfunc_3.
- Shape:
  1. Count the timer down; Flash() while it is below g_JackFlashFlashFrames.
  2. At JACK_GLOW_START_FRAME:
     - kill the old emitter;
     - fade the environment to ENVIRONMENT_JACK_OF_BLADES_GLOW over g_JackFlashGlowFadeFrames/30 s;
     - GetSubDef<CSpecialEffectsDef>, then the FLASH_SWORD_GLOW emitter at the WEAPON_FOCUS_01 dummy;
     - the MARKER_SPOT_LIGHT_JACK_OF_BLADES marker.
  3. At 0: fade (0.75 s), kill both, ExtinguishFlames, play ST_STRIKE_LIGHT_OUTOF, RemoveMode(MODE_JACK_OF_BLADES_FLASH).
  4. Every frame: aim the spot light at the camera (ResetRHSet, slot 32), set its cone angle (acos/2pi), and drop the mode when the
     boss is ready for a phase change.
- Levers that moved bytes:
  - action temporary of sizeof 0x118 (frame 0x15c);
  - `CWorld& world = GetWorld();` locals;
  - shared CDefPointer / CDefClassBase / CSpecialEffectsDef;
  - a same-TU noinline GetSubDef<CSpecialEffectsDef> body (the 0x00667450 idiom);
  - the CThing.h migration;
  - CAIGameCameraBase slot 11 `const C3DVector PeekCurrentPos() const` with CWorld::PeekGameCamera / GetEnvironment. With these, x87 is
    identical.
- Residue:
  - VC7.1 calls `CIntelligentPointer<CThing>::operator=` (??4, 0x004A9760) out of line where retail expands it. This is the documented
    inline-budget blocker.
  - Frame-slot assignment: retail keeps type / temp / GetTC key at frame 8.
- Callees (PDB):
  - Flash and ExtinguishFlames are private (AAE);
  - SetCurrentAction(const CCreatureActionBase&) 0x006644F0, the reference overload;
  - CTCBoss::IsReadyForPhaseChange 0x007AEE90 (QBE);
  - CTCSpotLight::SetAngle 0x00731BE0;
  - CEnvironment::TransitionToThemeAllChannels 0x006B38D0 (QAEXJJM). The landed binding there is the placeholder `?Apply_006b38d0@Inner@@`.
- Globals:
  - 0x013BAFDC is the anonymous-namespace JACK_GLOW_START_FRAME. The landed binding is `?JACK_GLOW_START_FRAME@?A0x49475d5d@@3JA`.
    The hash is per TU, so a new TU cannot emit the same symbol. The differ source uses a plain extern. **Coordinator decision needed**
    on how to bind an anonymous-namespace global from a second TU.
  - 0x01388A7C and 0x01388A84 have no PDB name, so they are **descriptive**: g_JackFlashFlashFrames / g_JackFlashGlowFadeFrames.
    0x01388A7C is bound elsewhere as the placeholder `?g_01388a7c@@3JA`.

## 0x008DBC80 NCreatureMode::CModeDragonFlying::FrameUpdate -- DIFFER(2323v2319), 272 md / 38 structural, 10 attempts
- Name proven: slot 3 of RTTI vtable 0x0126868C (slots 2, 5 and 6 are landed CModeDragonFlyingBase / CModeDragonFlying members), plus the
  PDB name.
- Shape:
  1. progress = (loops + anim fraction) / loops_to_do, first-frame remap, clamped to 1.
  2. Spline position (CCardinalSpline::GetSplinePosition const&), then SetPosition (physics slot 30).
  3. Turn = signed angle difference ×3, clamped ±0.125; bank the up vector unless in the last loop with an end action.
  4. Interpolate the RH set 0.25 toward it (slots 72 / 75).
  5. Leg-anim alpha blend (anim type 0x22).
  6. Look ahead along the spline, or 50 units along the facing.
  7. After the last loop: level the orientation, RemoveMode(MODE_DRAGON_FLYING), then SetCurrentAction(PEndAction).
- Levers that moved bytes:
  - retail engine/CTCPhysicsBase.h. MSVC orders overloaded virtuals adjacent in reverse declaration order:
    70 GetFacingVector() const, 72 GetRHSet() const, 75 SetRHSet;
  - GFGetSignedAngleDifference (PDB __fastcall);
  - GFArcTan + GET_ROUNDED_ANGLE `__forceinline` (FableWin `?GET_ROUNDED_ANGLE@@YIMM@Z`; precedent is the landed 0x00EC5DE0 spelling);
  - one TCI_LOOK presence test;
  - `level.Forward = C3DVector(X, Y, 0).Normalise()` on a temporary.
- **GET_ROUNDED_ANGLE note:** this is a row-local `__forceinline` helper. The brief allows it only through the 0x00EC5DE0 precedent. The
  owner ruling bans row-local forceinline for inlining residues, so the coordinator should move it to a shared header if the row ever lands.
- Callee spellings were fixed at hand-back (grade unchanged, 272 md). All are PDB spellings:
  - CTCAnimationComplex::GetAnimation (QAEAAV, returns a reference), GetAnimIndex const, GetAnimDuration const;
  - CCardinalSpline::GetSplinePosition const with a reference out-parameter;
  - CTCLook::LookAtPos(const C3DVector&, long).
- Residue:
  - Frame-slot order. Retail: forward 0x10, progress 0x1c, up 0x28, direction 0x34, creature 0x40. Ours: progress 0x10 ... forward 0x40.
    Declaration order changes nothing.
  - x87 order in the direction Normalise: retail does X*X first, ours Z*Z first.

## 0x00873E10 NCameraModes::CPCMouseLook::Update -- DIFFER(2384v2386), 33 md / 23 structural, 11 attempts
- Name proven: slot 1 of vtable 0x01277784 is CBase::Update per engine/NCameraModes_CBase.h, plus the PDB name.
- Shape:
  1. Save the state.
  2. Smooth the mouse over Def->NumMouseSamples, using std::list<C2DVector> MouseInputs (FABLE_RETAIL_STLP_LIST).
  3. Sensitivity from the user profile (CUserProfileManager::Get), with inversion.
  4. Turn about AxisZ.
  5. If a target is locked (the IsGameActionEventInQueue / GetCurrentMode checks), swing behind it.
  6. Recentre inside ±0.375 turn.
  7. Auto-pitch blend and pitch clamp.
  8. Rotate the axis about side = AxisZ × axis, then go_to = axis*DollyLength + thing_pos (AddAttractorGoTo), and LookAtPos = head.
- Levers:
  - CThing.h + PEEKPLAYER (const PeekTC gives LowerBound 0x004365B0);
  - the interface pointer fetched into a local after `bool locking = false`;
  - `if (!locking)` twice;
  - `fabs(angle) < 0.375f` (`test ah,5 ; jp` = less-than);
  - a named go_to local, which put every slot into line with retail.
- Residue:
  - x87 product order of the first dot product: retail does Z, Y, X.
  - The scaled-axis multiply: retail keeps DollyLength on the x87 stack until the end.
  - The samples spill is scheduled before the average zero stores.
- **Unproven / descriptive:**
  - The enumerator names PLAYER_MODE_46 (EPlayerMode 0x2e) and the EGameAction / EInputButtonEventType values are value-named only.
    The enum type names are PDB.
  - 0x004C10E0 is declared `GetHeadPosition` (the landed binding's name, which is wrong; see LABELS.tsv). The real name is one of the PDB
    getters GetStaticFocalPos / GetStaticCameraFocalPos, and one more source is needed to pick.
  - GetObservedThing is declared returning `const CThing*`. The PDB returns `const CThingGameObject*`, and engine/CThingGameObject.h
    exists. Switch to it when the row is resumed: the emitted symbol is currently a wrong spelling, though the bytes are the same.
- AddAttractorGoTo was made protected (PDB IAE) at hand-back; the grade is unchanged.

## Binding conflicts found (all in NAME_FIXES.tsv)
- AngleDifference vs GFGetSignedAngleDifference (0x00A441E0).
- Five spellings of GetObservedThing (0x0084F1B0).
- GetGamePlayerInterface (0x00416359).
- CUserProfileManager::Get (0x0040D2A0).
- QAE/JJJ IsGameActionEventInQueue (0x00446B10).
- Six GetCurrentMode spellings (0x00633BE0).
- Public AddAttractorGoTo (0x0084E1A0).
- Animation accessors 0x0070B460 / B490 / B4A0.
- GetSplinePosition (0x00A567A0).
- Pointer LookAtPos (0x00753E90).
- CIntelligentPointer ctor / operator= flat-class spellings (0x004A96F0 / 0x004A9760).
- g_NormaliseEpsilon (0x0129BA3C).
- The g_01388a7c placeholder.

## Header debt
- 0x00689DF0: local models of C2DVector, CInputEvent, CProcessedInput, NUISystem::CManager (vtable to slot 38), CMainGameComponent,
  CTCInventoryMap, CTCInventory, CPlayer and CInputProcessGameBase, while shared headers exist (vland SHARED_HEADER_AVAILABLE).
  The landed siblings 0x0068B7C0 / 0x0068BC70 use the same local models. Migrate the CInputProcess* family together, with an opt-in where
  the shared layout differs (CManager needs its slot-38 SetRightStickForce, CTCInventoryMap PanMode +0x58 / ZoomedIn +0x248).
- The parked creature/camera rows still carry local models: CRightHandedSet, CAnimSet / CAnimateThing / CTCAnimationComplex,
  CCardinalSpline, the CPCMouseLook / CBase layout and CCameraModeDef. Replace them with the shared headers (engine/NCameraModes_CBase.h,
  CThingGameObject.h, NGraphicAppearance_CTCGraphicAppearance.h, ...) when the rows are resumed.
