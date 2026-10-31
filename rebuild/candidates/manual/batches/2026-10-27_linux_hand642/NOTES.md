# Hand chunk 642 (CEngineMap .. CActionFollowCreature): 1 of 10 byte-exact

Parked-row retry chunk: all ten rows came in as same-length misses with a best attempt. Each row started from its
best attempt (grade reproduced first), then one lever per grade. Probe files are in `probe/` (`<row>_<n>.cpp`).

## What matched (1)
- CEngineMap::PeekInterpolatedWaterDepth 0x00B41C30 (RELOCATION_MATCH, 377 bytes). The function averages the water
  depth (over 0.001) of the square of cells within radius of (x, y). When (x, y) is dry, it re-centres the square on the
  first wet cell (x outer, y inner), and returns 0 when there is none. Shape lesson: the four bounds are written mins
  first (`minX, minY, maxX, maxY`), both in the setup and in the re-centring block. The inherited x-pair order
  (`minX, maxX, minY, maxY`) left the prologue's x + radius in ecx and the re-centring block in eax/ecx/edx. Retail
  reads y first, puts x + radius in ebp and uses ecx/edx. The y-first and max-first orders were worse. Callees are the
  Ego_r CEngineMap::PeekHasWaterFast 0x00B40260 and PeekWaterDepth 0x00B40BD0 (the obj symbols match the PDB spelling).

## Parked for Fable 5.1 (9)
- 005fb4b0 CTCInventoryMap::TeleportHeroToBoastingArea DIFFER(379v379): the filter temporary and the marker swap frame slots (retail: filter +0x10, marker +0xC shared with the AddFloat temporaries); so the marker load reads [esp+14h] for [esp+10h] | tried: PDB respells (const QBE, PeekThingSearchTools chain, GetPos, g_pMainGameComponent, HandleMoveHeroToRegionGameEvent(const&)) =; marker in an inner block with an early return (angle float moves into the marker slot); named filter at if-scope and in the block (filter above the marker as in retail, but the floats take a fresh slot, frame +4); bool result local =; inline pointer-returning GetPFirstLeafThing<F,T>(filter) (filter above the marker, +1 slot and a null test, 387) | next: a scope shape that nests the marker AND lets the AddFloat temporaries reuse its slot (retail has 4 slots: bools +8, floats/marker +0xC, filter +0x10, event +0x14)
- 00acd960 COptimisedPrimitive::Optimise (catalog "SimplifyByVertexPairs") DIFFER(383v383): esi/edi swapped for the copied counted pointer (retail: Data and the erased node in edi, Info in esi) | tried: PDB respell (Optimise, IsIllegal, DetectFoldOver(const&), PerformContraction(&), UpdateCost(const CCountedPointer&)) =; the real same-TU UpdateCost body (erase + insert of (Cost, contraction)) =; split release `--Count; if (Count == 0)` =; earlier: copy-ctor orders, raw Data local, local info in the release, if/else vs continue/break, shared CCountedPointer.h (worse) | next: lane gap: register colouring tie (park)
- 007534b0 CTCLook::IsPosInViewRange DIFFER(384v384): the copy into `from` is ld x, ld y, st x, ld z, st y, st z in retail; ours uses ld/st pairs | tried: FIXED the inherited control flow (a null or dead creature returns false; the old nested if fell through), which needed the single inline predicate `IsAbleToLook(creature, animation)`. Split returns, an `||` chain and else-return all sink the false block to the end (396). __forceinline WrapAngle. Copy assignment `from = cond ? a : b` =. Earlier: implicit / body-assign / forceinline copy ctor, POD, const& (all =) | next: find the copy form that gives the two-register interleave (a by-value inline parameter, or GetAnglesFromBodyToPos)
- 0065b142 CTCVillage::EnableVillagerDefTypes DIFFER(386v386): retail loads this->ParentThing (mov eax,[ebp-4]; mov ecx,[eax+4]) right after fld 50.0, before the pushes; ours loads it after push eax | tried: inline CTCBase::GetParentThing() (worse 383: hoisted into edi); PDB respells (GetNearbyWanderPosition const with a C3DVector&, CreateVillager(CTCVillage&), GetPos, const GetDefGlobalIndexFromName) =; earlier: parent locals, nav/centre/radius locals, default args, same-TU GetPosition / GetNearbyWanderPosition / bitset bodies | next: lane gap: argument-load scheduling
- 0086b9d0 CRandomAppearanceMorph::GetRandomSkeletalMorph DIFFER(389v389): retail keeps this in ebp (spilled at entry, restored at the loop bottom), the map in ebx and end spilled to [esp+18h]; ours keeps this in ebx (restored at a rotated loop top), the map in ebp and end in ebx | tried: PDB respell (const protected IBE, std::vector<long>& GetKeyList, const bounds, AddSkeletalMorphToMorphSet, GFROR13 returning long) =; Ego_r GFRandom(range, &seed) inline =, byte-identical, kept; a POD const_iterator object (worse 438); earlier: named map reference, split inner init | next: lane gap: register colouring / spill choice
- 009122a0 CBezierCurve::GetPointOnCurve DIFFER(358v390): in the 3 Point2 t (1-t)^2 term retail loads the (1-t) CSE before the Point2*3 products and spills (1-t)^2 into the dead t slot | tried: PDB names Point1..4 and operator shapes (const-returning operator*/operator+, user copy ctor) =; named (1-t) local (362); earlier: operator*(const float&) (390v390, scalars hoisted), ungrouped products (414), NRVO / ctor operators, by-ref factor | next: lane gap: x87 CSE placement
- 00911a90 CAIStateGroup_WanderAroundInitialPos::Update DIFFER(391v391): creature and interface-map pointer swap esi/edi in the first half (retail: creature in esi) | tried: Ego_r two-level GetCreature() reference =; real CVectorMap<ETCInterfaceType, CTCBase*> types (as the landed IsRunnable) =; a same-TU LowerBound body =; earlier: locals before the creature, a direct map, last cached, one-level GetCreature, a radius local | next: lane gap: register colouring tie
- 005cb128 CTCInventoryWeapons::RemoveItemFromInventory DIFFER(392v392): this and the weapon-pointer address swap esi/edi | tried: stripped the inherited pack pragma (byte images + reference accessors, same bytes); inline GetParentThing =; a named self pointer =; earlier: same-TU LowerBound, if/assign GetInterface, forceinline per-weapon helper | next: lane gap: register colouring tie
- 00713d50 NTCScriptedControl::CActionFollowCreature::SetAsSneaking DIFFER(395v395): scratch-register rotation in the sneaking-on arm (retail chains the creature through eax and keeps the bitset word in ecx) | tried: Ego_r CTCScriptedControl::GetParentCreature() =; bool HasTC (worse); body written straight into the private SetAsSneaking =; earlier: GetParentThing, the bitset<274>::test model, map-before-key locals | next: lane gap: scratch register choice

## Skipped
None.

## Label corrections
- 0x00ACD960 "COptimisedPrimitive::SimplifyByVertexPairs" is really `QuadricOptimiserInternals::COptimisedPrimitive::Optimise(float)`.
  No PDB name set has SimplifyByVertexPairs. The proof chain runs: OptimiseMesh 0x00AA41E0 calls 0x00ACDAE0
  (COptimisedMesh::Optimise, `_gapscan::sub_00acdae0`) with q*q, and that calls this function for every primitive.
- 0x00ACD500 "COptimisedMesh::CollapseSegments" is COptimisedMesh::Initialise(const COptimisationParameters&): it calls
  COptimisedPrimitive::Initialise 0x00ACA830 per primitive and takes the parameter block (landed 0x00AA4210).
  CollapseSegments of the mesh is 0x00AC8680 (OptimiseMesh calls it with q*2).
- 0x00AC8360 "QuadricOptimiser_VertexPairContraction_Initialize" is probably CVertexPairContraction::IsIllegal. It is the
  only no-argument bool member in Ego_r/Ego_d. This is one source only, so it is marked probable.
- 0x00661540 / 0x00A441E0 are GFGetAngleXYTo / GFGetSignedAngleDifference (NAME_FIXES.tsv).
- 0x0070B470 (the interface-0x5A accessor `mov eax,[ecx+0Ch]`, bound as CTCTurnSource::GetState) is unresolved. The
  007534b0 differ keeps the descriptive CTCAnimation::GetAnimateThing.

## Names and evidence
- PDB spellings used in the differ files: CTCBase::PeekPlayerManager / PeekWorld / GetWorld /
  PeekThingSearchTools, CWorld::PeekThingSearchTools, CPlayer::PeekPControlledCreature, CThing::GetPos,
  CAIStateGroupBase::GetCreature, CAIBrain::GetCreature, CTCScriptedControl::GetParentCreature, GFRandom, GFROR13,
  AddSkeletalMorphToMorphSet, CVectorMap<long,CSkeletalMorph>::GetKeyList / UpperBound / LowerBound.
- Descriptive names that are still unproven: CTCAnimation::GetAnimateThing 0x0070B470, the CAnimateThing state fields,
  CTCTargetingBase::ClearTargets 0x007F6160, CTCInventoryWeapons::RefreshWeaponSheathing 0x005C552C (could be
  PutWeaponsInSheathes), and the 005cb128 accessor names ActiveMelee()/ActiveRanged().

## Header debt
- engine/COptimisedPrimitive.h is in the global namespace (the class is QuadricOptimiserInternals::) and flattens the
  multimap, so 00acd960 uses a local model.
- engine/CTCVillage.h, CTCBase.h and CAIStateGroupBase.h flatten the vptr, so the 0065b142 / 00911a90 differs keep local
  models.
- CTCInventoryWeapons packs PActiveRangedWeapon at +0x141, 1-aligned. Without a pack pragma, the 005cb128 differ models
  both intelligent pointers as byte images behind reference accessors.
