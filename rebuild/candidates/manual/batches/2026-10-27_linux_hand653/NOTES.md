# Hand chunk 653 (CAudioBuffer .. CGameScriptInterface): 3 of 10 byte-exact

All ten rows were parked priors. Every row got at least 12 logged probes (`probe/<addr>.log`) except the three that matched sooner.
Every matched source was also checked beyond the relocation mask: each `__real@` constant relocation against the retail
.rdata value (`cc.py`, 22 constants, 0 mismatches), every call target and global against the retail reloc targets
(`$S/w22/calls.py`), and the string literals against the retail strings.

## What matched (3)
- **CAudioBuffer::Create (0x00C4DC40)**: creates the OpenAL buffer for a PCM description. More than two channels fails
  (-14). Otherwise it clears the AL error, suspends the context (unless it already was), generates one buffer and processes
  the context again, then uploads the data in the AL format picked from sample width and channel count. An upload error
  deletes the buffer and fails. Shape: the landed CAudioContext model with every entry-point wrapper (GetError
  0x00C4D960, Suspend 0x00C4D790, Process 0x00C4D7D0, GenBuffers 0x00C4D870, DeleteBuffers 0x00C4D8C0, BufferData
  0x00C4D910) as an inline header member. The resolver is the ternary
  `UseALGetProcAddress ? GetProcAddress(name) : ::GetProcAddress(Module, name)`. VC7.1's per-function inline budget then
  inlines the early wrappers and calls the later ones, and keeps the GetProcAddress import in ebp. Retail's case mapping:
  sample format 1 gives MONO16/STEREO16 and format 2 gives MONO8/STEREO8. `Flags |= 1` comes before `Desc = desc`.
- **CGameScriptInterface::CreateCreatureNearby (0x0089F300)**: creates the creature. If it is alive and has a navigation
  component, the creature is moved to a nearby wander position within 0.8 + random(radius - 0.8). Shape: the retail
  polymorphic engine/CScriptThing.h (`FABLE_RETAIL_NSCRIPT`) with the inline ctor/dtor/copy/GetPThing/IsAlive, plus
  engine/CGameScriptInterface.h. The key lever: a same-TU noinline body of the const
  `CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...>>::LowerBound` (0x004365B0). VC7.1 then knows the lookup
  writes no memory and keeps the creature's PImp.Data in edi across it, as retail does.
- **NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions (0x00712340)**: works out the conversation camera's
  from/at things and positions for a camera tag:
  - talking to self: in front of the head;
  - speaker or listener: their focal point;
  - otherwise: the midpoint, pushed sideways by the camera view (WIDE 1.75, MEDIUM 1, CLOSE 0.75), at mid head height;
  - demon door: pulls the camera in front of the speaker.

  The prior had the three scales on the wrong cases (0 unscaled, 1 x0.75, 2 x1.75). The grader's relocation mask hid this
  because only the `__real@` addresses differed. Reading retail .rdata (0x012657FC = 1.75 on view 0, 0x01238174 = 0.75 on
  view 2) fixed the semantics, and the bytes then matched. Re-authored on retail names:
  - the Ego_r.pdb CCameraTag members, with installed-game enumerators;
  - the retail CScriptThing header (GetPThing slot 11);
  - CTCPhysicsBase::GetFacingVector (slot 70, 0x006B1410), NGraphicAppearance::CTCGraphicAppearance::GetFocalPos,
    CThing::GetPos and C3DVector::GetMagnitudeAndNormalise;
  - a private const member with a global CSubtitledMessage.

  The object symbol equals retail's mangled name.

## Parked for Fable 5.1 (7)
- 005ebfa0 CTCTavernGame::CameraUseCameraPoint DIFFER(321v329): the end-of-function `_M_start` free keeps the allocator::deallocate null test (`test eax,eax; je`), which retail drops while still keeping the vector in memory | tried: real STLport 4.6 vector, retail names (PeekWorld/GetWorld/GetAllThings<CThing>/GetPointType/GetStartPos/GetLookDirection), same-TU stand-ins (scalar-replace the vector into ebp, frame 0x18), real GetAllThings body, single-exit form, block-scoped manager, same-TU LowerBound (19 probes) | next: the real CThingSearchTools::GetAllThings template body with its PeekAllThings/filter callees as retail compiled them in this TU; else lane gap: escape analysis
- 00c5a9d0 CCodaVoice::SetModulation DIFFER(382v382): VolumeDown is `fld d; fmul -1; fdiv s` where retail has `fld -1; fdiv s; fmul d` (no /Op; /Op gives 390v382 with a rounding store after the millibel fild) | tried: 20 spellings of the six divisions (per-division (float)steps, reciprocal temporaries, -1 local, negations, double temporaries, /Op variants) | next: the -1.0 division in a same-TU noinline float helper; else lane gap: x87 reassociation
- 00896b60 CGameScriptInterface::KickOffQuestStartScreen DIFFER(437v445): retail lays the shared epilogue after the quest-card path and gives the tutorial exit its own copy; VC7.1 shares one epilogue | tried: if/else orders, early returns, goto, nested ifs, engine/CPlayerManager.h + CGameScriptInterface.h re-author, open-coded and forceinline (probe only) GiveHeroTutorial (16 probes) | next: GiveHeroTutorial's real inline body from a CTutorialManager header (a same-TU definition is never auto-inlined); else lane gap: tail duplication
- 00666310 CThingCreatureBase::OnCreate DIFFER(915v908): `mov al,[esi+0x15c]; and al,0x7d; mov [esi+0x15c],al` where retail has `and byte ptr [esi+0x15c],0x7d` | tried: bit order, chained, =0, unsigned char bitfields, raw byte &=, statement moves, a named local, /Ob1, QFE (14 probes; p07 fixed the ShotAccuracyPercentage store order) | next: real PDB bool bitfields for 0x15c..0x15e in engine/CThingCreatureBase.h (header debt); else lane gap: RMW byte and
- 00984f60 CAIStateGroup_OpinionReactionRoot::FleeUpdate DIFFER(856v863): navigation pointer enregistered (`mov ebp,[eax+4]` + else reload) where retail stores it to [esp+0x14] (the key slot) and caches the brain in edi across the faction lookup | tried: same-TU LowerBound bodies (const/non-const/both), plain-inline lookups, operator== map compares, T*& lookup, local placement, /Ob1 /Op /GS (16 probes) | next: re-author on the faithful CVectorMap specialization + engine/CAIStateGroup_OpinionReactionRoot.h; else lane gap: register allocation
- 00e37e40 CM_AssassinTrigger::Main DIFFER(1104v1103): guard bool `mov 0; jne; mov 1` v retail `jne; mov 1; jmp; mov 0`, and ~CTimer inlined at the two inner-loop returns where retail inlines it at the loop exit and the first inner-loop return | tried: 6 guard spellings, region code inside the loop (p08, retail's flow), for(;;)/do-while, break, in-loop timer test, QFE (14 probes) | next: the guard as an inline helper returning bool; the dtor-site choice is shared with 0x00E4AA00; else lane gap: dtor inline-site choice
- 00e4aa00 CBanditLeader::Main DIFFER(1362v1356): handle dtor inlined at the talk StartScriptingEntity-loop return (call 0x007E74D0 at the loop exit) where retail inlines it at the loop exit and calls it at every return | tried: if/else and goto block orders (byte-identical output), user dtor, scoped block, for(;;), while(true), do-while, plain-inline CTimer, /Ob1, QFE (12 probes) | next: lane gap: dtor inline-site choice (see 0x00E37E40)

## Semantic fixes to priors (not only bytes)
- 0x00712340: camera-view scales were swapped between cases 0 and 2 and case 1 was scaled. Retail: WIDE x1.75, MEDIUM x1,
  CLOSE x0.75.
- 0x00E37E40: the second wait loop was `while (!IsCameraPosOnScreen(pos) && timer)`. Retail loops `while
  (IsCameraPosOnScreen(pos) && timer.GetTime() != 0)`. The region/spawn code runs inside the loop when the marker is off
  screen; the normal (terminating) loop exit returns. The differ copy has this flow, but the prior's line-1 description
  still reads "leave the screen and for the off-screen timer to run out".

## Names (evidence)
- Proven (PDB name sets, two sources):
  - CWorld::GetWorldSeed 0x0049D810;
  - CTCBase::GetWorld 0x00686A70 / PeekWorld 0x00686A80;
  - CThingSearchTools::GetAllThings<CThing,...,CThingFilter_HasScriptName> 0x005ED860;
  - CTCCameraPointScripted::GetStartPos 0x0075CD30 / GetLookDirection 0x0075CCB0;
  - CThing::GetPos 0x004C73D0;
  - NGraphicAppearance::CTCGraphicAppearance::GetFocalPos 0x004C1060 (wave-1 decision);
  - C3DVector::GetMagnitudeAndNormalise 0x00A14480;
  - CTCPhysicsBase::GetFacingVector 0x006B1410 (vtable 0x0125D2C4 slot 70 read from Fable.exe);
  - CCameraTag members and CActionTalkBase::PTargetThing / ParentIsDemonDoor (Ego_r.pdb);
  - CTCScriptedControl::CActionBase::PTCScript and CTCBase::ParentThing (Ego_r.pdb);
  - mangled GetCameraSetupPositions (retail symbol tables).
- Descriptive (landed spelling, no PDB): CCodaVoice::SetModulation 0x00C5A9D0, SModulationParams, SecondsToMixBlocks
  0x00C59B00, the CCodaVoice m_ members.
- Descriptive (no name found): `g_CameraHeadOffset` 0x01380C44. Installed-game enumerator values carry the landed
  CONVERSATION_* prefixes (0x00719A70 batch).
- NAME_FIXES.tsv: 9 rows (see the file).

## Label corrections
- 0x00C5A9D0 catalog `C3DMeshFileXBoundingBoxChunk::ReadFromFile` / `??0C3DMeshFileClothChunk@@QAE@XZ` -> CCodaVoice::SetModulation.
- 0x0049D810 `DoSizeof` / GetPRandomSeed / GetBank -> CWorld::GetWorldSeed.
- 0x004C73D0 `DoSizeof` -> CThing::GetPos.
- 0x00A14480 `GetScaled` -> C3DVector::GetMagnitudeAndNormalise.
- 0x00C59B00 `Graphics_ConvertTimeToPixels` -> SecondsToMixBlocks.
- 0x004C1060 `GetHelperPointOr` -> NGraphicAppearance::CTCGraphicAppearance::GetFocalPos.
- 0x0075CD30 catalog `GetEndPos` -> CTCCameraPointScripted::GetStartPos.
- 0x0075CCB0 binding `TransformVector@CEnvironmentSpellEffectInterface` -> CTCCameraPointScripted::GetLookDirection.

## Header debt
- engine/CThingCreatureBase.h flattens the 0x15c..0x15e bool bitfields:
  - 0x15c: CurrentlyFrameUpdating, Blinking, MeleeAttacker, MeleeAttacked, LeaveDeadBodyOverride,
    LeaveExperienceOrbsOverride, FollowHeroThroughTeleporters, HeadPosCached;
  - 0x15d: UseMovementInActions, UpdateMovementVectorThisFrame, AbortingCurrentAction, DemonDoor, Oracle,
    HadFacingAngleYZSetByBodyOrientation, UnderInfluenceOfEpicSpell, HiddenOnMiniMap;
  - 0x15e: IsShowingDebugInfo.

  The 0x00666310 differ copy still uses the prior's local model with descriptive member names. The Ego_r offsets are
  0x10..0x1c larger than retail's in this range (PDef PDB +0xfc / retail +0xe0; ShotAccuracyPercentage PDB +0x100 /
  retail +0xe4), so remap before renaming.
- CActionTalkBase: retail reads ParentIsDemonDoor at +0x8d; Ego_r.pdb places it at +0x91. The TLC layout is 4 bytes shorter
  somewhere between PTargetThing (+0x0c) and it. 0x00712340 uses a local model with that offset.
- 0x00984F60 still uses the prior's local CAIStateGroup_OpinionReactionRoot model (`#pragma pack(1)` for ThingReactingTo at
  +0x6b) next to engine/CAIStateGroup_OpinionReactionRoot.h. It also uses an invented CInterfaceMap instead of the CVectorMap
  specialization.
- 0x00E37E40 and 0x00E4AA00 keep the priors' local CScriptThing / CGameScriptInterface padding models (VPAD vtables). Re-author
  them on engine/CScriptThing.h + engine/CGameScriptInterface.h before landing.

## Skipped
- none

## Header debt (verifier SHARED_HEADER_AVAILABLE warnings, kept on purpose)
- 0x00712340 and 0x0089F300 model CTCPhysicsBase locally. engine/CTCPhysicsBase.h is a flattened data layout with no
  virtuals, and these rows call its slot 70 (GetFacingVector) and slot 31 (ResetPosition) virtually. 0x00712340 now
  takes CTCBase from engine/CTCBase.h.
- 0x0089F300 models CWorld locally, because engine/CWorld.h declares no member functions and the row calls the const
  member GetWorldSeed 0x0049D810.
