# acctC hand-back c30a (agentC30, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x007B57C0 | CTCCarriedActionUseActivateQuest::OnActionUse(CThing&) (catalog: vfunc_28) | 776 | RELOCATION_MATCH (`/GS`, cookie in the bytes) | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.

Review items: the `bool done` flag guarding the shared reset block was chosen by grading (the `&&` form leaves a frame-slot
residue; VC7.1 removes the flag); `IsPerformingRegionLoadReally` body inferred from the bytes (name from FableWin);
0x00629060 is labelled `~CAggregate_00629060` but is reached as `~CCreatureAction_ActivateQuest` (folded destructor).
Catalog label fixes (NAME_FIXES.tsv): 0x007BC38A is NThingShot::CTCShotDefinitionThrowPredictive::InitMovement; 0x00C271F0
is the libpk audio CPhysicalPlayer::Play (descriptive; no PDB).

`open/`: 0x007E15C0 OnAppearanceDraw DIFFER(969v963) and 0x00735FD0 AttemptToStartParticle DIFFER(993v973), both allocation residues.
