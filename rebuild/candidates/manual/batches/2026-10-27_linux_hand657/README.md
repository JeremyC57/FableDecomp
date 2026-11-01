# Chunk 657 -- 10 of 10 landed (2026-11-01, NA003 of the never-attempted pool: landed-twin rows)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA003). Landed via `coordinator/land_chunk.sh 657` (vland --land 10/10
RELOCATION_MATCH + PASS, multi-check 0; every row on its first probe): the five 33-byte "_Dest_val" twins are compiler-generated
destructors -- 0x00628EE0 ~CCreatureAction_GiveItemToThing, 0x007ED750 ~CCreatureAction_ShowItemToThing, 0x006DD690
~CCreatureAction_ShowTrophy (emission-order split from 0x006DD6C0 ShowToSheatheTrophy), 0x008C9580 ~CCreatureAction_BanditKingStuck,
0x0066A530 ~CCombatAction_GenericStrikeResponseKnockdown; 0x007710A0 ~CTCDegradableBase (engine/CDefPointer.h), 0x00529AD0
~CThingHolySite, 0x007599C0 CTCPhysicsRigidBody::SetFacingAnglesFromRHSet (slot 74), 0x007C75A0 CTCTurncoatSpell::OnKill and
0x007C7560 FinalCleanup (the private member inlined into OnKill / OnRemove). No row parked.
19 NAME_FIXES queued (c657). Labels: ghidra_out/labels_chunk657_2026-10-27.tsv (20). Coordinator decisions (owner delegation
2026-11-01): emission-order identity for the ShowTrophy pair accepted; the ungraded `TUHarness_<Class>` emission function (six rows)
accepted as the chunk 656 helpers were. Open for a later pass: 0x0057A545's identity, 0x006DF860, the 0x006DF890 ctor without a
catalog row. Header debt in NOTES.md (CCreatureActionBase 0xA8 v 0xB4, CCombatActionBase 0x114, CThingGameObject 0xD0; CTCBase /
CTCPhysicsBase / CTCPhysicsRigidBody / CTCTurncoatSpell flattened). Lessons in GOTCHAS (1 line) and LESSONS.md.
