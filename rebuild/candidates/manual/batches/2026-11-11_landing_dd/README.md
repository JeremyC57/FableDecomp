# 2026-11-11 landing dd -- chunk Q17 (register-swap reworks, Opus 5.5): 7 rows

vland --land: 7 wins (RELOCATION_MATCH), tests PASS. bindings_multi_check is at its NullCoord baseline.
- **0x009711D0 CAIStateGroup_CreatureUseCoverBase::MoveIntoCover.** Porting to the shared headers removed the esi/edi swap.
  - New headers: CCreatureAction_DuckAndCover.h and CAIStateGroup_HoverMovementBase.h.
  - An inline GFGetWorld2DDistance with a named `sqr_dist` local.
- **0x00966030 CarryCrateAndFollow::OnReplace and 0x00965640 RecoverStealableItem::OnReplace.** GETSUBDEF_NOINLINE drops the
  null test on the CDefPointer release. A named `slot_thing` local, compared first, gives retail's compare order.
- **0x0093C030 CAIStateGroup_SeekScriptTarget::Update.** Ported onto the shared headers, with a do/while action loop.
- **0x00795090 CTCUnholyPower::FinalCleanup.** Byte twin of the landed 0x0078FAF0 CTCDivineWrath::FinalCleanup: the twin's spelling
  and its `/d2inlT62` flag.
- **0x0095B820 CAIStateGroup_HoverHornetQueen::Spawn.** ROUGHAREABODY. A named `def_index` and the filter temporary built inside the
  call shrink the frame from 0x13c to 0x138.
- **0x00A87700 CSBCAgents::Process.**
  - New CSBConstraint.h and CKinematic.h.
  - `Output.SetVector(...)` fixes the x87 tail.
  - The global 0x013D2D56 is `g_SBCAgentsDisabled`. That name is DESCRIPTIVE (made up): no PDB, string, FSE or EgoCore source has
    one, and it is the only spelling bound to that address.

**Headers**

Q17's overlay started as a copy of Q16's (now landed). The headers Q17 changed were 3-way merged with base 62f45b3a8. All were
clean, and every default is the same. Two guards only gained opt-in macros:
- CCombatManager.h: LEADER_INITIALISE || DIALOGUE.
- CTCSpecialAbilities.h: DIVINE_FINALCLEANUP is skipped when CTCUNHOLYPOWER_CANCELFORCUTSCENE is defined.

New headers: CCreatureAction_DuckAndCover.h, CAIStateGroup_HoverMovementBase.h, CSBConstraint.h and CKinematic.h.

**Names**
- Queued as land1011dd: Q17's seven NAME_FIXES. Among them:
  - 0x0097F3A0 AIAction_ProjectileAttackAndStrafeToPos.
  - Const and access fixes for IsFreeToFinishAction and the two IsStillRunnable rows.
  - The DuckAndCover and QueenHornetShootBabies constructor parameters.
  - GetSubDef<CCarryableDef>.
- Also queued: 0x007FEF40's self row is still the made-up `FreeActionPoint`. Its binders already bind the PDB
  `SetActionPointAsFree`.
- Labels: ghidra_out/labels_landing_2026-11-11dd.tsv (25 names).
