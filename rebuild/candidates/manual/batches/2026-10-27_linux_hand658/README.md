# Chunk 658 -- 10 of 10 landed (2026-11-01, NA004 of the never-attempted pool: landed-twin rows)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA004). Landed via `coordinator/land_chunk.sh 658` (vland --land 10/10
RELOCATION_MATCH + PASS, multi-check 0; every row on its first probe): 0x00AECA80 ~vector<pair<CCharString,CRegisteredPSCInfo>> and
0x00AF36C0 its _M_clear (CPSCManager's CVectorMap, via the CCountedPointer deleter chain), the slot-11 Clones 0x0084C490
CCreatureAction_PlayConversationAnimation, 0x008F5740 CCreatureAction_DragonFlight, 0x00925D90 / 0x00925E40 the anonymous-namespace
Kraken Low/High attack loops (retail hash ?A0x29922355), 0x0083D6D0 ~CThingPhysicalSwitch, 0x0071A570 ~CActionTalkToThing,
0x007DFAF0 ~CTCWallMountEffects, 0x007E24D0 ~CTCFishingRod (engine/CDefPointer.h). No row parked.
6 NAME_FIXES queued (c658). Labels: ghidra_out/labels_chunk658_2026-10-27.tsv (15). Later pass: Kraken ctor members Interrupt /
LoopNum (0x009255E0 / 0x00925780 use m_flag / m_value), 0x00AF2910 likely the CVectorMap Insert (unproven), the twin 0x006F8280
family's non-const `CBase_0062f3b0* Clone()` (PDB: UBE returning CCreatureActionBase*). Header debt: CCreatureActionBase, the two
named creature actions, CTCBase and CThingGameObject headers flatten the vptr (local polymorphic models over the header offsets).
Lessons in GOTCHAS (1 line) and LESSONS.md.
