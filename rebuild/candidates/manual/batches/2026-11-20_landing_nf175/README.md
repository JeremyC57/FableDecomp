# Landing nf175 (Account A, 2026-11-20): creature action / AI / hero names (11 rows)

Agent NF175 (round 10) handed back early (context) with 41 entries: 7 done, 2 rejected, 8 blocked, 1 deferred (acctB lock), 23 open.
Its files are in `naming/2026-11-20_agentNF175/`. Of the open entries, the five cut-scene cast-map entries went to the dedicated
agent NF178, the eleven GameCODA voice entries are deferred under the vendor rule (below), and seven go back to the queue.

- 11 rows. Self renames (6): CScriptedMapBrush::GetThingsMovement 0x0082E4E0, the nested NScript::CQ_OrchardFarmRaidScript::
  CCrateTeamMember::GoOnPatrol / IsThingCarryingCrate 0x00DCEC70 / 0x00DCED10 (each with an `// Inferred:` line for the
  IsActiveThreadTerminating tail), the CCreatureAction_ThrowShotObject overrides IsToFaceTarget / GetIsToFaceTargetCone
  0x00956720 / 0x00956730, and CCreatureActionBase::Update 0x006929A0. Every landed referencer already binds the new spellings.
  Binders moved: 0x008AF1E0 onto the shared CombatDialogueManager.h; 0x00956380, 0x009621F0, 0x00984A20, 0x00961790 onto the PDB AI
  action spellings.
- Headers (add-only, member-specific opt-ins, `orig` equals HEAD, pin_asserts pinned 0): CScriptedMapBrush.h (_THINGSTOMOVE),
  CCombatActionBase.h (_MELEE_OWN_VIRTUALS; a scratch compile put the two overrides at +0x124 / +0x138), CCreatureAction_ThrowShotObject.h
  (_FACETARGET), CCreatureActionBase.h (_UPDATE). The three rows that emit the ThrowShotObject vftable must adopt the first two together
  (queued).
- No casts of stand-ins to real types, no new local models.
- Recheck (`logs/recheck.log`): 1 MATCH + 10 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 1196 good, 0 bad (CCreatureActionBase.h is widely included); owner recheck 5 of 5 good. tablecheck: six addresses shrink,
  none grows.
- Link (`logs/link_landing.log`): 1 LNK2005 (??1CBrainOutput), the same as HEAD (`logs/link_control_head.log`).
- Coordinator calls:
  - GameCODA voice family (q12021-q12032, and the SDK-proven names 0x00C59710 CSalBaseVoice::Release, 0x00C3F3D0 ~CSalBaseVoice,
    0x00C4CF40 ~CCodaObject, 0x00C5A700 CSalSwBaseVoice::CalcMultiFuncEnvelope): vendor code reconstructed by hand gets LIGHT renaming
    only (owner, 2026-11-12), so no dedicated naming agent; the entries are deferred and the names are recorded for when a pass touches
    the family. q4780 (CCodaVoice::Stop) and q5120 (UpdatePitchEnvelopeStages) are rejected: the SDK names supersede them.
  - Cut-scene cast-map family (5 entries, about 40 binders): handed to the dedicated agent NF178.
