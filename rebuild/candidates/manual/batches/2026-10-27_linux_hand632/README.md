# Chunk 632 -- 1 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 632` (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0):
CAIStateGroup_WorkInWarehouse::Cleanup 0x00943B40 -- the base CAIStateGroup_MoveGoodsBase::Cleanup 0x009162F0 runs on every path
(both retail early-outs jump to it); the old "ebx/ebp colouring" residue was a wrong jump target. PDB spellings throughout
(PTask : CIntelligentPointer<CVillageTaskCarryCrate>, CTCVillageMember::GetPVillage, CVectorMap LowerBound, RemoveThingFromTask);
engine/CAIBrain.h and CTCVillage.h via the `#define X XLayout` include. Nine rows stay DIFFER in `pending/2026-10-27_linux_hand632/differ/`
(0090bc60 GetFreeBench one register, 00ac7eb0 x*x*W reassociation -- named n2 = n*n keeps the order, next step untried --, 006b0b30
one float operand order, 00c590e0 CMixBuffer::WriteChannel esi/edi, 00e7f330 NScript::CV_FishingCompetitionScript::Init two stores
early, 009b9e00 loop-end operand order, 006a7fb0 coordinate-copy slot, 005e1210 esi/edi -- the real-template version is 225 bytes
because the shared CIVCountedPointeeBase.h Release() cannot give `dec [ecx+4] ; jne` --, 008e7f80 const + count_if(not1(CIsImposterHit()))
register swap). 10 NAME_FIXES (vtables 0x0128B0B0 / 0x0125A8A4, RemoveThingFromTask 0x00659EFC, GetPVillage 0x007EE540 with 8 wrong
spellings, GetPWorldSeed 0x008FCFD0, GFROR13 returns long, CIsImposterHit::operator() 0x008E8A10, GetNextPrizeWeight 0x00E7F7D0,
CalcInQuotes access 0x009B9570, WriteChannel 0x00C590E0) + 3 review rows queued (prefix c632; anonymous-namespace binding policy
is an owner call). Labels: ghidra_out/labels_chunk632_2026-10-27.tsv.
