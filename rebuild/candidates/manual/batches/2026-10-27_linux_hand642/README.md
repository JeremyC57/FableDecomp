# Chunk 642 -- 1 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 642` (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0; new
bindings PeekHasWaterFast 0x00B40260 and PeekWaterDepth 0x00B40BD0, PDB spellings): CEngineMap::PeekInterpolatedWaterDepth 0x00B41C30 --
the four bounds written mins first (minX, minY, maxX, maxY) in both the setup and the re-centring block set the prologue registers (the
inherited x-pair order gave the same length with the wrong registers). Nine rows stay DIFFER in `pending/2026-10-27_linux_hand642/differ/`
(005fb4b0 TeleportHeroToBoastingArea filter/marker slot order -- scope moves the slots, no shape yet does both --, 007534b0
CTCLook::IsPosInViewRange copy order into `from` after the inherited best attempt's wrong control flow was fixed with one inline predicate,
0065b142 EnableVillagerDefTypes ParentThing load before the pushes (GetParentThing() accessor hoists it for the whole function, worse),
009122a0 CBezierCurve::GetPointOnCurve 358v390 x87 order with (1-t)^2 spilled into the dead t slot, 00acd960 QuadricOptimiserInternals::
COptimisedPrimitive::Optimise (catalog SimplifyByVertexPairs wrong) esi/edi tie, 0086b9d0 GetRandomSkeletalMorph ebp/ebx tie + spilled
loop end, 00911a90 WanderAroundInitialPos::Update esi/edi tie, 005cb128 RemoveItemFromInventory esi/edi tie -- inherited #pragma pack
stripped --, 00713d50 SetAsSneaking scratch rotation). Lessons: the source order of paired bounds assignments sets the prologue registers
(try every order before parking); a same-length residue whose jump targets differ is wrong control flow, check it before tuning
scheduling; one inline predicate keeps a shared `return false` inline after an || chain; temporaries share frame slots by scope;
Ego_r accessors are usually byte-neutral so respell anyway, except CTCBase::GetParentThing() which hoists ParentThing; a pack(1) member
can be modelled as a byte image behind a reference accessor. 17 NAME_FIXES + 4 review rows queued (prefix c642): the COptimised*
relabels (0x00ACD960 Optimise, 0x00ACD500 Initialise), GFGetAngleXYTo, GFGetSignedAngleDifference, HandleMoveHeroToRegionGameEvent
(const CGameEvent&), g_pMainGameComponent 0x013B86A0 (landed 0x005FAD40 spells it g_pEngine), the three CTCInventoryMap Teleport* consts
(landed 0x005FAD40 / 0x005FBD90 are QAE), PeekPControlledCreature (19 bindings), GetMainPlayer const&, GetMainPlayerNumber,
GetNearbyWanderPosition, CreateVillager(CTCVillage&), GFROR13 YIJK, GetPItem CBaseObject*, AddSkeletalMorphToMorphSet.
Labels: ghidra_out/labels_chunk642_2026-10-27.tsv.
