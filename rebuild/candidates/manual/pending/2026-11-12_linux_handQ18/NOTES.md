# Q18 notes

Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` (tools/gg.sh and tools/pq.sh export it; gall.sh is run with it too).
Header changes: hdr/README.md, "Q18 changes" table.

## Matched (6, RELOCATION_MATCH, tests PASS under g++ -std=c++03 and VC7.1 /Od /W3)

- **0x008BF500 CCreatureAction_GenericFireProjectileWeapon::FinishAction**: Q17's port; a named `CThingCreatureBase* target`
  for `GetCombatantInfo(*PCreature).AttackerInfo.Attacked` takes one temporary out of the eax/ecx/edx rotation, so the key-address
  registers come out as retail's eax/ecx (c2prio --rotation).
- **0x00555A90 NUISystem::CScrollableList::UpdateVisibleList**: Q17's port; `Children[i].Get()` (FableWin
  ?Get@?$CCountedPointer@VCComponent@NUISystem@@@@QBEPAVCComponent@NUISystem@@XZ) in place of `.Data`.
- **0x00DE3CE0 NScript::CQ_RansomVictimScript::CNPCComment::Main**: ported onto engine/CQ_RansomVictimScript.h with the new
  NPCCOMMENT opt-in. The loop is `for(;;)` with the terminating return first; `while (!IsActiveThreadTerminating())` gave
  9 hunks. The mixed inline/out-of-line ~CScriptThing uses scoped `inline_depth(0)` regions on the in-loop returns, as in the landed
  CCampHostageDoor 0x00D08B40 (line-2 Inline-budget note).
- **0x008886E0 NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::BlendInLevel** (`/Gr`): sibling 0x008884D0's STLport
  opt-ins plus the new CBLENDEDENVIRONMENTTHEME_ASSIGN declaration (`result = base` calls 0x006B5DF0; without it VC7.1 inlines a
  `rep movsd`). Thresholds 0.9990234375f (0x01278090) and 0.0009765625f (0x01278080) were read from Fable.exe.
- **0x009644F0 CAIStateGroup_StealStealableItem::OnReplace**: a new retail class opt-in in CAIStateGroup_StealStealableItem.h (Ego_r
  member names at the retail offsets), CThing::SetAsDeathPersistent (FableWin; `or byte [edi+93h],2`). The body is sibling
  0x00965640's plus the CTCAIScratchpad `KillSelfAndStealableItemAfterReachingDestination` tail (SetAsDeathPersistent, Kill(false)).
- **0x00ED94D0 NScript::CV_TalentlessBardScript::WatchForResetWanderCentre**: a new header engine/NScript_CV_TalentlessBardScript.h
  (base CLevelScriptBase<CQ_SunnyvaleMasterData>; Ego_r members at PDB - 0x14; vector<bool> 12 bytes, so RandomSpeechTimer is at
  +0xe0 as in the landed Init 0x00ED83C0; sizeof 0xe4). Levers:
  - IsAlive as `PImp.Get() && PImp->IsAlive()` gives retail's `test al,al`; the two-return form gave `cmp al,bl`, Q17's only
    residue.
  - The CIsThingAlive ctor is defined inline (retail stores vtable 0x012C32F0 in place). FABLE_RETAIL_CPREDICATEBASE_NOVTABLE.
  - Sibling WatchForAttack's exit spelling: `do { if (...) return; } while (0);` at the three exits retail expands the dtor at
    (prologue, loop condition, wait-loop return); plain `if` at the four out-of-line ones.
  - CTimer::GetTime / operator= are FABLE_FORCEINLINE through pDSTGame, as in the landed 0x00D647F0.

## Parked, ported (3)

- **0x008780D0**: the identity is corrected (NAME_FIXES). It is the GetAllThingsInRoughArea instance for CThingBuilding into a
  std::list, called only by GetThingsOfType<CThingBuilding> 0x00878390. Built on the shared CThingSearchTools.h ORIGINAL_ROUGHAREA
  body plus two new opt-ins, THISEND (the loop ends at the member `AreaMaps.end()`) and CTHINGMAP_PEEKCELL_INLINE.
  - Result: 680v680 with one residue, the reload order after the node loop. Retail reloads edx = cells.BRX, edi = map,
    esi = cells.BRY; ours reloads edi, esi, edx (0x008782CD..0x008782D5).
  - c2prio candidates: #21 BRX, #22 BRY, #13 map.
  - Tried without effect: the C2DBoxI ctor form for `cells`.
  - Tried and worse: PeekCell per x (646v680). Both trial opt-ins were removed from the overlay.
  - Next: a `const CThingMap*` instead of the reference, or declaring the map after `local`/`cells`. This needs a header variant
    of the ORIGINAL_ROUGHAREA body.
- **0x006FE7C0**: the same template body for CThingGameObject with CThingFilter_MinAllowedRadius (new header; operator() 0x006A7410).
  Same single residue at 0x006FE9BD. A fix for 0x008780D0 should carry over to this row.
- **0x00837220 CThingTrackNode::UpdateFacingDirection**: ported to C3DVector (ISNONZERO_ZYX, SQUAREDMAGNITUDE_ZYX, new
  ROTATE_ANGLES declaration), CRightHandedSet CTOR_INLINE, CTCPhysicsBase and CThingTrackNode (new UPDATEFACING declaration), with
  a `physics` local. The physics local took the result from ratio 0.864 to 0.980.
  - Residues (716v716):
    - The third Normalise's `mov edx,17Ch` lands one tuple late (0x008373EC).
    - The cross product's third component loads side.X before direction.Y (0x00837419). CROSS_XY_GROUP and the swapped spelling
      compile the same.
    - The PhysicsTC vtable load is hoisted (retail has it just before the call, at 0x008374DD).
  - Next: try the scheduling-region levers, GFFASTONEOVERSQRT_NEWTON_PAREN / _GROUPED.
  - The callee 0x006A4D00 is spelled CThing::GetPos here (FableWin ?GetPos@CThing@@QBEABVC3DVector@@XZ). reloc_bindings also
    has ?PeekPos@CThingPhysical@@ at that address. Which one this row calls is unproven; the bytes are the same either way.

## Parked, NOT ported (11): still the prior local models (differ/ carries them with the Q18 probe result)

Not reached this wave: 00a78b20, 00540320, 00649e8b, 0065c5bb, 006c1250, 007bd845, 008cdd40, 008e16a0, 0096cd70, 0079b6f0, 008c4fd0.
Each still needs its port onto the shared headers before any residue work. Port plan for 0x008cdd40:
- CCreatureActionBase FULL_MEMBERS + accessors (GetWorld 0x00692940); CCombatActionBase MELEE + FINISHACTION.
- A new retail opt-in class CCreatureAction_ReleaseTimeSpell : CCombatActionBase with `HasteKey` at +0x114.
- Declarations:
  - the WithHasteAction ctor;
  - CTCPhysicsBase::GetAirResistance 0x0079C740;
  - CTCHaste::SetActionSpeedMultiplier;
  - CWorld::GetGameCamera()->GetCameraManager (CAIGameCameraBase slot 9);
  - ReleaseScriptedControl 0x00698950;
  - SetCurrentAction(const CCreatureActionBase&).

## Made-up names

- `ENPCComment` enumerators `NPC_COMMENT_VICTIM` / `NPC_COMMENT_GUARD` (engine/CQ_RansomVictimScript.h, NPCCOMMENT opt-in). The
  enum's name is Ego_r.pdb's (`W4ENPCComment@@` in MakeNPCComment's mangled name). The enumerator names are in no source we hold,
  so they are descriptive (made up). They reuse the landed 0x00DE3F90 row's names, so there is one spelling everywhere.
- No other made-up names. Every other function, member and class name is a PDB spelling.

## Header debt

- engine/NScript_CV_TalentlessBardScript.h is new. The landed bard rows (WatchForAttack 0x00ED9790, Init 0x00ED83C0, ...) still
  carry local models and can move onto it.
- The landed callers 0x008889C0 and 0x00555EB0 / 0x00556200 spell their callees wrongly (NAME_FIXES.tsv).
