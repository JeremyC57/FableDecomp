## 0x00972CC0


## 0x00972CC0 CAIStateGroup_JackOfBladesStage1::MeleeCombat -- RELOCATION_MATCH (grade ~16)
- What made it match: port onto shared headers (CAIStateGroup_JackOfBladesStage1/Base, CAIStateGroupBase, CAIBrain, CWorld GetCombatManager,
  CCombatManager, CIntelligentPointer, ThingAlive) put `this` in edi (local models left an esi/edi swap); landed sibling 0x0093DDE0's
  CBrainOutput model (init-list ctor, declared empty dtor, SetAsLookAtThing with an explicit temp CIntelligentPointer<const CThing>) gave
  the memory `or`s and ebp vptr; reading the target pointer (`PTargetThing.Get()`) BEFORE `me.GetPos()` fixed the last order/register residue.
  IsStillRunnable (0x00971B00, protected `?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@IAE_NXZ` in both PDBs; declared protected by the
  shared Base header) real body inline in the TU = landed row's nested-if spelling verbatim (that row's public `QAE` Leaf is wrong, NAME_FIXES.tsv).
- Headers: overlay opt-ins FABLE_RETAIL_JACK_STAGE1_HASHITSCAUSEDRUSH (Stage1.h) and FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY
  (CCombatManager.h); see hdr/README.md. No flags. CBrainOutput/CNavigationInput: shared engine/campfire_types.h since repair 3 (opt-ins FABLE_RETAIL_CBRAINOUTPUT_ONLY + _INLINE); PAction = shared CCountedPointer<CCreatureActionBase>.
- Draft name fixes confirmed (dossier): IsInMeleeAsAttacker, GetWorld, SetOutput, SuspendSuspendableProcess, GetPos, FrontMiddleLimit, PThingCombatManager.

### Repair (2026-11-12, verifier findings) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), test PASS
1. Local CCountedPointer_CCreatureActionBase / SCountBlock removed; PAction is the shared engine/CCountedPointer.h
   `CCountedPointer<CCreatureActionBase>` (Data/Info, CCPPointerInfo) + engine/CCreatureActionBase.h; no opt-in needed. The old
   "no shared header" note for the count block was wrong.
2. IsStillRunnable now spelled exactly as landed rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesBase_IsStillRunnable_00971b00.cpp
   (nested ifs, PTargetThing.Get()->IsDead()); the earlier IS_THING_ALIVE one-expression spelling (from the parked 0x00975BB0 Update
   draft) gives the same bytes; the source comment says both.
3. The `creature` / `world` steering locals were not needed: `GetWorld().GetCombatManager().SetAttackerAsReadyToAttack(GetCreature(), true)`
   grades the same (right-to-left argument evaluation loads [[edi+4]+0x18] into esi at 0x00972CDE..0x00972CE1 before `call 0x8fcfc0`
   at 0x00972CE6). Removed; the source comment cites those bytes.

### Repair 3 (2026-11-12, verifier findings) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), test PASS (vt.py)
1. `output.Types |= 0x20` -> `output.SetAsPumpCombatActions()` (?SetAsPumpCombatActions@CBrainOutput@@QAEXXZ, FableWin.pdb only: thunk 0x0179B3F0 /
   FableWin 0x028923D0). The local CBrainOutput/CNavigationInput models are gone: both classes DO have a shared definition, in
   rebuild/include/engine/campfire_types.h (the earlier "no shared header" note was wrong). Overlay copy adds two opt-ins:
   FABLE_RETAIL_CBRAINOUTPUT_ONLY (include only the CNavigationInput/CBrainOutput slice; the full bundle's CCombatActionBase branch
   selected by FABLE_RETAIL_JACK_STAGE1_UPDATE does not compile with the campfire classes) and FABLE_RETAIL_CBRAINOUTPUT_INLINE (inline
   bodies of the CNavigationInput/CBrainOutput ctors, the declared empty ~CBrainOutput, SetAsPumpCombatActions, SetAsLookAtThing).
   The row also needs FABLE_RETAIL_CCREATUREACTIONBASE (the header's inline SetAsPerformAction calls Clone()). Bytes unchanged.
2. IsStillRunnable is protected (IAE) in both PDBs; the row already uses the shared Base header's protected declaration. NAME_FIXES.tsv
   entry for 0x00971B00 (landed Leaf spells QAE); DOSSIER.md section 5 and the NOTES line corrected.
3. CNavigationInput::MovementType is EMovementType (shared campfire_types.h member, PDB +0x10), zero-initialised as AI_MOVEMENT_NULL from
   engine/EMovementType.h (the shared enum the repo actually uses; fable_ai_action_enums.h defines the same enum and would clash). Bytes unchanged.
Grades: a1-a7 SRC_FAIL (full campfire bundle prerequisites), b1 SRC_FAIL (Clone), b2/final RELOCATION_MATCH, strict RELOCATION_MATCH. Backup of the
pre-repair state: repair3_backup/.

