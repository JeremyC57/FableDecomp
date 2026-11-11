# Chunk Q8 notes (40 parked near misses)

Grades: every one with `FABLE_HDR_OVERLAY=$A/hdr` (gr.sh exports it; gall.sh was run with it exported). The overlay is described in
hdr/README.md. 3 MATCH (src/ + test/), 37 differ/.

## Matched
- 0x009F5690 `??0CInputEventPackage@@QAE@AAVCAFile@@@Z` (CInputEventPackage::CInputEventPackage(CAFile&)). Catalog label "Save" is
  wrong (FableWin.pdb ctor; Save is the neighbour). Lever: redundant parentheses `(float)((GFGetTime() - age))`. No overlay needed.
  CInputEventPackage and CChecksum have no shared header; they are declared in the row with their PDB members (as landed
  CChecksumCacheFile rows do). Header debt: an engine/CInputEventPackage.h.
- 0x006A4FB0 `?GetFirstCollision@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z`. Rewritten from the local model onto
  CThing/CThingPhysical/CTCPhysicsBase/CPhysicsMeshObject/CMeshIntersectionData/C3DVector. Physics slot +0x174 is
  CTCPhysicsBase::GetPhysicsMeshInfo() const; the sphere is the component's Position (+0x0C) and Radius (+0x34). Needs overlay
  opt-ins FABLE_RETAIL_CTHINGPHYSICAL_FIRSTCOLLISION, FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION and
  FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED (engine/C3DVector.h).
- 0x0095D0E0 `?IsStillRunnable@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAE_NXZ`. Rewritten onto the shared AI headers
  (CAIStateGroupBase GETCREATURE / GOTOBED_WORLD, ThingAlive.h, CTCEntityEvents retail branch, NEntityEvents filters, CWorld::GetFrame,
  CMainGameComponent::GetConstantFPS). NEntityEvents::CEventFiredShot has no header: declared in the row as an empty CEventBase
  subclass (Ego_r ??0CEventFiredShot@NEntityEvents@@QAE@AAVCThing@@@Z). Needs overlay opt-in
  FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_STILLRUNNABLE. GetEvent<CEventFiredShot,CIsEventCreatedAfter> 0x0095D920
  stays declaration-only (retail calls it).

## Callee names (proven this pass; see NAME_FIXES.tsv)
- 0x00A442E0 is ?GFGetShortestDistanceSquaredBetweenPointAndLine@@YIMABVC3DVector@@00AAM@Z (Ego_r); the landed 0x006A7050 spells
  it GetSquaredDistanceToInfiniteLine (in no PDB).
- 0x00A120D0 takes CMeshIntersectionData& (Ego_r + FableWin); the landed 0x006A5460 spells it with a UCPhysicsHit.
- 0x00989830 takes `const CShaderLight&` (both PDBs; the shared header agrees); the landed 0x00B44F20 calls the PAV spelling.

## Unmatched rows worth another pass (all differ/ files carry the residue on line 2)
- 0x00989830 (98v99, shared header now): one CSE (`mov ecx,eax` for the second `lea [ebx-1]`).
- 0x00ADD690 (646v648, shared headers now): the unrolled remainder counter copy (`sub ebx,edi ; mov esi,ebx`).
- 0x006D8980 (180v183, shared headers now): the Abilities start pointer in ebx.
- 0x00918A40 (114v116, shared CTCShopKeeper.h now): `mov eax,[eax+4] ; pop esi ; mov ecx,eax`.
- 0x0093D270 (305v295): needs the owner's decision on CCombatManager's private ProximityForNavigation (getter temporary).
- The other 30 drafts were only regraded as they sit: they are local models of classes that now have shared headers (CThing,
  CTCBase, CAIStateGroupBase, CIntelligentPointer, ...). Their residues are mostly frame-slot packing around GetTC keys; the shared
  GetTC / CVectorMap bodies may remove those (as they did for 0x0095D0E0), but each needs the full rewrite first. 0x007D75A0 no longer
  compiles (its inlined copy of CIVCountedPointeeBase meets the current CDefPointer.h) and 0x007F68B0 still uses a stand-in search body.
- 0x00929020: the draft reads a CThingList at CTCVillage +0x70 and calls it VillageCages, but engine/CTCVillage.h puts VillageCages at
  +0x40: the +0x70 member needs its real name before a rewrite.

## Header debt
- engine/CPhysicsMesh.h keeps TotalPoints / BoundingBox / BoundingCircle opaque (0x00A6C640 needs them typed); no CCircle header.
- engine/CAIStateGroup_Gaze*.h, CAIStateGroup_BanditPlayDice.h, CAIStateGroup_OpinionReactionRoot.h are generated opaque structs.
- No engine/CInputEventPackage.h, CChecksum.h, CShaderLight.h, CTCExclusiveUse.h.
