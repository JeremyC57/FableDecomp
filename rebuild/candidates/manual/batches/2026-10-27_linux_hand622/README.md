# Chunk 622 -- 2 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 622` (vland --land 2/2, multi-check 0):
CAIStateGroup_ServeFood::IsRunnable 0x00923070 (RELOCATION_MATCH 1184; lever: the GetServingPoint out-pointer declared inside
each call's block puts it on retail's frame word; the real CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound body as a same-TU
noinline member replaces the stand-in) and CThing::IsEntitySerialisable 0x004C7870 (MATCH 15; catalog said vfunc_22; proven from
vtable slot 22 and the landed CThingBuilding override; `bool result = false; if (driver) result = driver->IsComponentSerialisable();`
gives retail's preload-false shape). Eight rows stay DIFFER in `pending/2026-10-27_linux_hand622/differ/` (008de7a0, 006c2170
1345v1329, 00927f30 44->38 lines, 00590235 callee-saved permutation, 007b3120 328->300 lines, 0046313e and 0045cfad post-call
byte-copy scheduling, 007af940). 4 NAME_FIXES (GetSex 0x00661FF0, GetCreatureType 0x00662000, CTCTavernTable::FreeSeat 0x007B8AA0,
canonical LowerBound 0x0040F020 with 32 bound spellings) + 4 review rows queued (prefix c622). The agent ran one read-only
`git status` against the rule; nothing was written. Labels: ghidra_out/labels_chunk622_2026-10-27.tsv.
