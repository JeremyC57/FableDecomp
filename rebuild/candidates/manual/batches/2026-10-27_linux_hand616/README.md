# Chunk 616 (thirteenth parked-row chunk from the sweep ranking, 10 same-length rows 500-564 bytes): 2 of 10 landed

Landed (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0): 0x00C11D00 CSoundEventManager::CSoundEventManager (libpk,
descriptive class name established by the landed ~CSoundEventManager 0x00C13470 and vtable 0x012B2748; three CVec3 members built in
the init list give the 3x3 frame of float stores; the stale caller bindings ??0CSteeringSolver / ??1C3DMesh2_00c09380 were respelled
and both callers 0x00C094A0 / 0x00C09380 rechecked RELOCATION_MATCH + PASS) and 0x009718F0
CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay (compound sums in both inlines -- `squared = Y*Y; squared += X*X;` and
Normalise as z, x, y -- set the registers and spill slots; engine header + engine/CBaseObject.h). Parked 8 (pending/.../differ/):
00910df0 `xor eax,0Fh ; test al,0Fh` type test retail emits only at inline sites (ours `not eax`, 38 grades, LTCG), 0077c960 one x87
operand order (leaf now protected per the PDB -- the old grade was of another function), 0070d580 23 -> 19 lines with a real
_Vector_base, 007d5070 one store order, 007f5ae0 register choice for best / cached pointer / map, 00871a80 scratch-register rotation
(a bool temporary fixes it but merges the two release epilogues, 520v547), 00589a30 esi/edi reload order after repe cmpsb, 004a34f0
identical instructions with frame slots swapped. 9 NAME_FIXES + 6 review rows queued (prefix c616: CTCTavernGame GetDef / PlaySoundW
/ SetStateAfterOK / SetState, GetCardDef CDefPointer spelling, GetPItem CBaseObject*, SetPNearestInterestingThing protected; folded
getters 0x00686D90 and 0x0049D870 need one canonical name each).
