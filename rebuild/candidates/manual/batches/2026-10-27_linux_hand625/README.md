# Chunk 625 -- 3 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 625` (vland --land 3/3 RELOCATION_MATCH + PASS, multi-check 0):
CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition 0x0093CAB0 and CTCBuyableHouse::CreateMainSignPost 0x006C0F00
(one whole CIntelligentPointer assignment per if/else arm, tail-merged by VC7.1 -- fixes push order and the esi/edi swap; the
switch-arm lesson extended to if/else), CTCInventoryMap::GetGuildSealRecallRegion 0x005FA990 (recall position by const reference,
inline GetSquaredMagnitude -> Z, Y, X loads with X squared first; the CWorldMap shared-header warning is deliberate, the header
flattens the vptr). Seven rows stay DIFFER in `pending/2026-10-27_linux_hand625/differ/` (009bee40 14 lines, 0051d870 4,
00c0a790 2, 00c720c0 2 -- qword vs dword zero --, 005c8101 32, 00bddf30 4, 00ec3270 2 -- jump threading). 12 NAME_FIXES
(CTCBase::PeekWorld 0x00686A80 / GetWorld 0x00686A70, const 0x004FC190, private CreateSignPost 0x006C0970, 0x009F2F60, 0x009FA280,
the CBaseIntelligentPointer callees 0x00A01B50/0x00A01B90/0x00A01C10 with dozens of bound spellings, three sign-post def-name
globals) + 4 review rows queued (prefix c625). Tooling: concurrent q.py grades of one address need QTAG (agent's bin/gp.sh).
Labels: ghidra_out/labels_chunk625_2026-10-27.tsv.
