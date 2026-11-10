# Chunk 1016 notes (w22 agent1016, 2026-11-10)

**Overlay:** every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (tools/gr.sh does this). `gall.sh` does not export it,
so run it as `FABLE_HDR_OVERLAY=$A/hdr bash $A/gall.sh`. Every opt-in is documented in `hdr/README.md`. The overlay copies of
CThingAICreature.h and CThingCreatureBase.h were rebased onto the current repo headers before hand-back. The repo had since
gained the COMBATTYPE / COMBATRADIUS / DRAINED opt-ins, and the overlay now adds only insertions.

## Matched (6): src/ + test/
| Row | Identity (two sources) | Lever |
|---|---|---|
| 0x00B67E30 | `?RegisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJPAVCLandscapeBackgroundPatch@@@Z` (Ego_r + FableWin) | Shared header. A local entry built by field stores, then push_back; the call binds the `_M_insert_overflow` false_type instance (= reloc_bindings 0x00B6C4C0). |
| 0x006D8790 | `?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z` (private; Ego_r 0x0058A540, FableWin) | The `map<EDamageAttribute,float>::find` member template (D1) with its body visible. A declaration only gives 125v124. **Split:** the row is 124 bytes; 0x006D8810 (17 bytes) is `AddSuccessfulWillStrike(float)` (Ego_r 0x0058A5D0). |
| 0x00B84470 | `??0CEngineSubPrimitiveRenderer@@QAE@XZ` | The retail CEngineComponent ctor (FABLE_RETAIL_CENGINECOMPONENT), plus the FableWin private `GetList(ESubPrimitiveType)` accessor. Writing through it keeps the source store order; direct array stores were scheduled differently (254 / 270 v 270). |
| 0x0085FA00 | `?IsToEvadeAllHits@NCreatureMode@@YI_NABVCThing@@@Z` | Early returns, and the FableWin inline `CThingPhysical::IsToEvadeAllHits() const`. |
| 0x00663990 | `?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z` (static member; the catalog said free function) | **(1)** The shared `GetSubDef<T>` template body in the TU (auto_inline off): VC7.1 then knows the def pointer's address does not escape, so it holds the def in esi and releases it with a bare `dec [esi+4]`, with no reload and no null test. **(2)** Without FABLE_RETAIL_STLP_BITSET_TEST_DECL the bitset test is inline. **(3)** An implicit `""` -> CCharString conversion: the temporary is constructed before the other arguments are pushed. An explicit `CCharString("")` is built in argument order and hoists the player-number load (377v381). |
| 0x006A16A0 | `?RemoveThingInCarrySlot@CTCCarrying@@QAEPAVCThing@@J_N@Z` (returns the removed thing; the bool is unused in retail) | The new CTCCarrying MEMBERS layout. Then `ThingsInPrimarySlots.remove(0)` / `.remove(thing)`, `CarrySlots.Find(slot)`, the CIntelligentPointer ASSIGN form `carried = 0`, and a GetTC'd carryable `SetAsCarriedBy(0)` (left uninitialised when absent, as retail). For MODE_CARRYING_GENERIC_BOX (34): `GetRHSet(1.0f)`, a Rotate about (Up.Y, -Up.X, 0) by `-(acos(Up.Dot(Z)) * 1/2pi)`, Up = Z, Forward.Z = 0, MakePerpendicular, SetRHSet. Then, unless Dying, the CTCSolidToThing (GetTC or AddTC) gets `AddThingNotSolidTo(&parent)`. Implicit `"CTCSolidToThing"` conversion (same lever as 0x00663990). |

## Unmatched (7): differ/ (line 2 = measured residue)
- **0x005EABC0 PositionMesh** 495v511 (6 grades).
  - Running `+=` statements give retail's evaluation order (Right*X, Forward*Y, Up*Z, position). A one-expression sum evaluates Up*Z first (85/168), and nested `(U + (F + R)) + P` is worse (96).
  - Residue 1: retail keeps the running Y/Z sums in st(1)/st(2) and spills the product temporaries.
  - Residue 2: retail hoists `mov edx,[edi]; lea eax,[esp+10h]; push eax` above `test bl,bl / je` for both setter pairs.
  - Next: try a helper returning the offset vector by value (a named temporary), or a C3DVector operator+ returning by value with a parenthesised body; try a ternary member-function-pointer-free form for the setter pairs.
- **0x009F18F0 CGameEventDataPackage::GetData** 166v166: a register permutation (this/start/size). c2prio: this prio 75 > start 61. Needs a change in how `this` is used early (e.g. a member read before `start` is computed).
- **0x00934E50 FindMostRecentMessage<IsOf, ThingSearch<IsNot>, ThingSearch<In2DRange>>** 208v208.
  - Only residue: the fail-path reload order (this before it).
  - c2prio: the split pass over ids [2,3,5] gives `this` the recycled piece #1 and the iterator #13/#16. Reference-local and while-loop variants are inert.
  - **Identity correction:** the bytes call 0x006A76A0 (CThingFilter_IsNot op(), a pointer compare) and 0x006A76B0 (In2DRange op()). The row is not the catalog's In2DRange/IsValidGossipee instance.
- **0x0074A4E0 DoNavCheck** 232v234 (7 grades).
  - Residue 1: the creature-reference reload (`mov ebx,[esp+20h]` after GetPos, `add ebx,44h`).
  - Residue 2: the constant-0 split piece in ebx.
  - 6 shape variants were inert.
- **0x00A55F90 CMatrix3x4::SetFromVectorAndAngle** 259v259: the pair-product load order ([Y]/[Z] first in retail) and the translation-zero stores 2 instructions early.
- **0x006668A0 GetPossibleCombatAnimations** 673v673: the prior was re-graded (local models, header debt: anim-filter classes). There are 2 residues: the vptr store order, and the DEFAULT fallback store slot vs `~CCharString`.
- **0x00BCF4F0 CEngineInternalPrimitive2DAnimatedMesh::Initialise** 363v374: the prior was re-graded. Retail re-reads Scale after each store. The class header is a flattened pad model (debt).

## Parked unattempted (1)
**0x006CBAC0 CTCCreatureNavigation::GetNextNavPosOnRoute** (Ego_r `?GetNextNavPosOnRoute@CTCCreatureNavigation@@QAE?AVCCreatureNavOutput@1@ABVCNavigationInput@@_N1111@Z`; `ret 1Ch` = hidden 16-byte CCreatureNavOutput + input + 5 bools). It was analysed but not graded because of header debt.

Structure:
1. If GotNavigationTarget (+0x56): `NavigationTarget(+0x1C).0x006CCF70(input)`. On false, clear GotNavigationTarget; then CalculatingNewRoute (+0x54) = true unless it was already navigating to the same target.
2. If `0x0085F3E0(parent)` (an IsAbleToMove-style fastcall): `speed = switch(input.MovementType +0x10, 0..10)` over the parent CThing's virtual slots 79 (+0x13C, two cases *0x0122F59C), 81 (+0x144; the random-jitter case uses GetRandom 0x00686DA0 / GFFloatRandom 0x004978A0 / GFROR13 0x00497890 and *0x0125A318), 83..86 (+0x14C..+0x158), or input.Speed (+0x14); else 0. Store it to Speed (+0x34).
3. `bool b = GetThingManager().+0x54`. `const C3DVector& p = PeekWorld()->vslot 8 (+0x20)(...)`. distance = |parent.GetPos() - p|.
4. PNavigator (+0x0C) slots 3/4/5 (MoveToPos / MoveToPosProximity / MoveToNavLineOfSightProximity). The choice is made by the first bool argument and by `input.Proximity (+0xC) > 0`. The from-position comes from `0x006A4D00(parent)` (PeekPos).
5. A null result clears GotNavigationTarget. FrameLastNavigated (+0x50) = -1. Then either CalculatingNewRoute is cleared once |out|^2 < (0x0129BA3C)^2, or, when the result is 2, FrameLastNavigated = CWorld::GetFrame() (0x0049D870, via 0x00686A70).

Needed headers:
- the CThing virtual slots 29..86 (the parent's movement-speed getters);
- the CNavigationInput layout (0x18 bytes: Pos +0, Proximity +0xC, MovementType +0x10, Speed +0x14);
- CCreatureNavOutput;
- the names of 0x006CCF70 and 0x0085F3E0 (no landed binding is trustworthy: 0x0085F3E0 shows `?IsAbleToMove@@YI_NPAVCThing@@@Z` and `?Entity_CanAttack@@..`).

## Callee names bound (all checked against the PDB name lists)
- CThingObject::Create 0x00703210
- CTCInventoryItem::AddToInventory 0x005D8D50
- CTCInventoryWeapons::SetThingAsActiveWeapon 0x005CB2B0
- CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E
- CTCCarrying:
  - RemoveThingInCarrySlot 0x006A16A0
  - GetThingInCarrySlot 0x006A11F0
  - PutThingInCarrySlot 0x006A1F80
  - IsThingCarryingWeapon 0x006A1540
- `list<CIntelligentPointer<CThing>>::remove` 0x0065DE8A
- CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound 0x006A29B0
- CTCCarryable::SetAsCarriedBy 0x00705D40
- CTCCreatureModeManager:
  - RemoveMode 0x0073ABA0
  - GetCurrentMode 0x0073A6E0
- CRightHandedSet:
  - Rotate 0x00A22F80
  - MakePerpendicular 0x00A22E10
- CThing:
  - AddTC 0x004C9D60
  - DrawIsDrawable slot 27 0x004C7790
  - SetAsDrawable slot 28 0x004C8E50
  - SetAsFrameUpdated 0x004C7C40
  - SetAsRenderedThisFrame 0x004C7590
- CTCSolidToThing::AddThingNotSolidTo 0x00770FA0
- CTCPhysicsBase::SetAsMoving 0x006B0130
- `_CIacos` (the acos intrinsic)

Wrong landed spellings for these are in NAME_FIXES.tsv.

## Header debt
- CThing virtual slots 29..86 (for 0x006CBAC0).
- engine/CThingCreatureBase.h has no anim-filter members (0x006668A0 still uses local models).
- engine/CEngineInternalPrimitive2DAnimatedMesh is a flattened pad model (0x00BCF4F0).
- Slot 26 of CThing is declared as DrawUpdate by inference only (the empty one-argument void body); confirm it from a vtable dump before landing the DRAWABLE opt-in.
- The friend declaration CTCCarryable -> CTCCarrying is inferred from the private access.

## Made-up / inherited names
- `SubPrimitiveRendererScratchTable` (0x014378C8, 0x1000 dwords cleared by the 0x00B84470 ctor only) is **descriptive**. No PDB, string or sibling name was found for it.
- `PFrameMemoryPool_0143bac4` (0x0143BAC4) follows the spelling of the landed sibling 0x00B84390 (one name per address). Its real name is still in the naming queue.
- `CTCTavernGame::PositionMesh` parameter names and `CThingCreatureBase::CreateWeaponForThingFromDef(.., long defIndex)` are descriptive. The PDB name sets keep no parameter names.
