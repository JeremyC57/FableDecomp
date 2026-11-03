# Chunk 718 — CGameScriptInterface + CPlayer (agent718)

Grade all rows with `FABLE_HDR_OVERLAY=$S/w22/agent718/hdr`. The header changes must land with the rows.

## Matched (9)
| addr | symbol | grade | shape |
|---|---|---|---|
| 008914f0 | CGameScriptInterface::SetQuestCardGoldReward | RELOC | `mov ecx,[CQuestManager singleton]; jmp` |
| 00891500 | CGameScriptInterface::SetQuestCardRenownReward | RELOC | same |
| 00892f20 | CGameScriptInterface::PrepareQuestsWhenFinalQuestIsActivated | RELOC | same |
| 00892f30 | CGameScriptInterface::PrepareQuestsWhenFinalQuestIsCompleted | RELOC | same; tail target 0x004AF270 |
| 0088e970 | CGameScriptInterface::StopProgressDisplay | RELOC | `NProgressDisplay::StopProgress(true,false)`, a __fastcall tail jump |
| 0088e0f0 | CGameScriptInterface::SetDebugCameraType | MATCH | `World->GetGameCamera()->SetDebugCameraType(type)`, vtable slot 21 (+0x54) |
| 00487dc0 | CPlayer::GetPControlledCreature | RELOC | `add ecx,2Ch; jmp CIntelligentPointer::Get` |
| 00487dd0 | CPlayer::PeekPControlledCreature | RELOC | same, const Peek |
| 00487b70 | CPlayer::GetPPlayerCharacterThing | RELOC | `add ecx,34h; jmp` (the oracle had GetPlayerCharacter) |

## Header overlays (hdr/engine; hdrcheck2: the default path is the same for all three)
- `CAIGameCameraBase.h` gets a new `FABLE_RETAIL_CAIGAMECAMERABASE` branch: the retail vtable through slot 24.
  - Slots: Update, GetRenderCamera, …, SetDebugCameraType = 21, …, virtual dtor.
  - Overloaded virtuals are laid out in reverse declaration order: GetCameraManager is declared const first, then non-const.
- `CWorld.h` (`FABLE_RETAIL_CWORLD` branch) adds inline `GetGameCamera()` and `PeekGameCamera() const` over `PCurrentGameCamera`.
- `CPlayer.h` (`FABLE_RETAIL_PLAYER`):
  - Under `FABLE_RETAIL_PLAYER && FABLE_RETAIL_CINTELLIGENTPOINTER`, ControlledCreature (+0x2C) and PlayerCharacter (+0x34) are `CIntelligentPointer<CThingPlayerCreature>`.
  - Adds the declarations of GetPPlayerCharacterThing and PeekPControlledCreature.

## Not attempted (budget)
| addr | function | size |
|---|---|---|
| 0x0088EDC0 | CameraDoConversation | 1564 |
| 0x0089E190 | CameraUseCameraPoint | 803 |
| 0x008A93A0 | EntityWillTeleportToArea | 620 |
| 0x00487570 | CPlayer::DrawFreeCamDebug | 1305 |

Head start for 0x008A93A0. The callees resolved from the labels are:
- NCreatureHiding::GetValidPositions 0x0074A5D0
- std::remove_if<C3DVector*,CDistancePredicate> 0x008AC060
- vector<C3DVector> erase/copy 0x0074CA20
- CVectorMap::LowerBound 0x0040F020 (the TC lookup, interface 7)
- CWorld::GetWorldSeed 0x0049D810, then the LCG step `*seed = *seed*0x24A1+0x24DF` and 0x00497890 (the random draw)
- CThing::GetPosition 0x004C73D0
- CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620
- an action ctor 0x008BCF20 (unlabelled)
- CThingCreatureBase::SetCurrentAction 0x006644F0
- CCombatActionBase dtor 0x0062E900

The frame is 0x158 and has no EH prologue, so the TU has no unwind state at this point. The function returns `ret 18h`.

## Gotchas
- VC7.1 lays overloaded virtuals out in reverse declaration order. The CAIGameCameraBase const/non-const GetCameraManager pair proves it against the 0x0088E0F0 slot.

## Round 2 (coordinator follow-up, 2026-11-03)
- 0x008A93A0 EntityWillTeleportToArea is parked at DIFFER(618v620) after 10 attempts. The source is `differ/008a93a0.cpp`.
  - Everything matches but one register swap: retail keeps `this` in ebx and spills the thing to [esp+20h]; ours does the reverse.
  - The landed sibling 0x008C1600 GetValidRushPosition was the model.
  - What moved it:
    - `C3DVector targetPos = pos;` at the top, reassigned to the chosen position. Retail copies pos into the action's target slot early.
    - A named `CDistancePredicate` local.
    - `if (positions.size() == 0) return false;` followed by a `while (positions.size() != 0)` loop with `GFRandom(positions.size(), seed)`. This keeps retail's zero test inside GFRandom.
  - What moved nothing: variable order, a CThingCreatureBase-typed thing, nested ifs, a cached `CWorld*`.
  - Header: `hdr/engine/CThing.h` gets `GetTypeFlags()` in the FABLE_RETAIL_CTHING branch (FableWin ?GetTypeFlags@CThing@@QBEKXZ). hdrcheck2: default path same.
  - Callee names are in NAME_FIXES.tsv. The TeleportOut ctor 0x008BCF20 is the (CThingCreatureBase&, C3DVector&, bool) overload.
- 0x0089E190, 0x0088EDC0 and 0x00487570 were analysed (see STATUS.tsv) but not drafted. The camera rows need models for the camera-manager and camera-point TC; DrawFreeCamDebug needs the CRenderManagerCore text API.
