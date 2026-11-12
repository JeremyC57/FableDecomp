# agentQ17 notes (w22)

Every grade used `FABLE_HDR_OVERLAY=$A/hdr` (gg.sh / pq.sh export it). Header changes: `hdr/README.md` (Q17 table).
Result: 7 RELOCATION_MATCH (src/ + test/, all tests PASS under g++ and VC7.1 /Od via vt.py), 13 parked in differ/
(2 ported this wave, 11 still the prior local-model drafts). 9 official grades in total (ATTEMPTS.log).

## Matched rows: identity and lever
- 0x009711D0 `CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector&)` (Ego_r IAE). Porting onto the shared
  headers alone removed the prior esi/edi this swap. New retail class header (CREATUREUSECOVERBASE), DuckAndCover header,
  AIAction_ProjectileAttackAndStrafeToPos decl, inline GFGetWorld2DDistance (named `sqr_dist` local so fsqrt follows the pops).
  `#include "rebuild_abi.h"` first (FABLE_FASTCALL).
- 0x00966030 `CAIStateGroup_CarryCrateAndFollow::OnReplace`. Port + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE with
  engine/CDefClassBase_GetSubDef.h (the visible noinline body tells VC7.1 the CDefPointer is not captured, so the release loses its
  null test) + `CThing* slot_thing = carrying->GetThingInCarrySlot(passive_slot); if (slot_thing == PPreviouslyCarriedItem.Get())`
  (named local, compared first, gives retail's operand order). campfire_types.h needs CCOMBATACTIONBASE_SOUNDCTOR + SETCOMBATANIMATION.
- 0x00965640 `CAIStateGroup_RecoverStealableItem::OnReplace`. Same spelling as its sibling (active slot before passive slot,
  `!ReachedDestination` block, `NeedsCleanup = false`).
- 0x0093C030 `CAIStateGroup_SeekScriptTarget::Update`. Port onto shared headers (CVECTORMAP_INLINE_LOWERBOUND, GFRandom,
  GetPDialogueDef()->EnemySighted, `(NStealth::EDangerStatus)2`); `do {...} while (ret == AI_ACTION_IN_PROGRESS)` loop.
- 0x00795090 `CTCUnholyPower::FinalCleanup` (identity: the draft's owner was right; byte twin of landed 0x0078FAF0
  CTCDivineWrath::FinalCleanup). Copy of the twin's spelling with its `/d2inlT62` flag and Inline-budget note.
- 0x0095B820 `CAIStateGroup_HoverHornetQueen::Spawn`. Port + FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY; frame 0x13c -> 0x138
  by a named `unsigned long def_index` local and the CThingFilter_HasDefGlobalIndex temporary built inside the call.
  Needs CTHING_STATEACCESS for IsLevelPersistent.
- 0x00A87700 `CSBCAgents::Process(const CSteeringAgent*)`. New CSB / CSBConstraint / CKinematic headers; `Output.SetVector(side *
  along + push)` instead of assigning a CKinematic temporary fixed the x87 tail; the iterator/this swap vanished with the port.
  Global 0x013D2D56 named `g_SBCAgentsDisabled` (DESCRIPTIVE: no original name anywhere).

## Parked rows
- 0x008BF500 `CCreatureAction_GenericFireProjectileWeapon::FinishAction` (slot 4; slot 3 0x008BF280 is PerformAction, the
  draft's "Abort" was wrong). Ported (FULL_MEMBERS, MELEE + FINISHACTION, CCombatantInfo Attackers map). Residue: the LowerBound
  key-address scratch register in both GetTC lookups (retail eax/ecx, ours ecx/edx; 2x2 instructions). Next: c2prio `--rotation`.
- 0x00555A90 `NUISystem::CScrollableList::UpdateVisibleList`. Ported onto engine/NUISystem_CScrollableList.h (new opt-in
  FABLE_RETAIL_CSCROLLABLELIST_VISIBLELIST) with STLport std::min (retail's by-address select proves it) and `Children[i].Data`.
  Residue 12 hunks / 42 lines of register colouring: retail loads `Children._M_start` into a register before the down-arrow
  size test (ours `sub reg,[mem]`), so its vtable loads stay per-branch where ours hoists one; loop-2 by-value pos copy stored
  Y-first in retail; child pointer eax/edx picks in loops 1/3. Tried compare flips, named count, break/while loop forms,
  ctor-form vectors (worse).
- 0x00805600 `CTCTargetingPlayer::FrameUpdate` -- class side is ready (engine/CTCTargetingPlayer.h UPDATE_LIST: slots +0x84
  GetPTargetedThing, +0x88 SetPTargetedThing, +0x9c UpdateNearestInterestingThing; base call is CTCBase::FrameUpdate 0x006868A0;
  0x00804020 = UpdateTargetList, 0x00803D60 = UpdateProjectileTargeting, 0x00803540 = UpdateTargetedPositionRandomWalk,
  0x00803280 = UpdateTargetThreatening; 0x00445B80 is bound as CGamePlayerInterface::IsToggleZTarget, 0x00487510 as
  CPlayer::GetJoystickDeviceNumber, 0x00416359 as CMainGameComponent::GetGamePlayerInterface). Blocked on two callee names:
  0x0085EE10 (result unused at the tail; bound as CThing::IsControlledCreature, catalogued as IsCreatureButNotHero /
  CTCScriptedControl::IsActive) and the player-GUI text setter 0x004379D0 (FableWin candidate CPlayerGui::SetNPCNameDisplay,
  one source only). Prove both, then port.
- 0x00878430 / 0x009871C0 (CThingSearchTools templates): need a CThingFilter_HasTrackName header + CThingTrackNode type flags
  (for the shared BESTROUGH_BODY), and a shared GetPFirstThingInRoughArea body (none exists; all landed GetPFirst instances are
  local models).
- 0x00DE3CE0, 0x00649E8B, 0x0065C5BB, 0x006C1250, 0x007BD845, 0x008CDD40, 0x008E16A0, 0x0096CD70: not reached this wave; the
  differ/ files are the prior local-model drafts (bare `__forceinline` respelled FABLE_FORCEINLINE + fable_compiler.h; each still
  compiles to its prior DIFFER size). Port first.

## Header debt
- All Q17 header changes are overlay-only (hdr/README.md); the matched rows need them landed with them.
- Landed rows whose spellings are wrong (NAME_FIXES.tsv): the DuckAndCover and QueenHornetShootBabies ctor rows take pointers
  (PDB: references), the SeekScriptTarget / HoverHornetQueen IsStillRunnable rows have the wrong access/const, and
  IsFreeToFinishAction / AIAction_ProjectileAttackAndStrafeToPos bindings are wrong.
