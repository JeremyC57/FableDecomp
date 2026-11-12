# agentFAM05 notes (2026-11-11): family naming follow-up to FAM04

Result: 136 of the 165 FAMILY.tsv rows respelled to the agentFAM03-proven leaf, each graded MATCH (68) or RELOCATION_MATCH (68)
with the leaf symbol present (SYM), 157 grades in all (at most 3 on any row; budget 10). Behaviour tests: 136 / 136 PASS under
VC7.1 /Od and g++ -std=c++11 (`work/tests.tsv`). 29 rows left (LEFT.tsv). Headers: 33 overlay copies, `#ifdef` additions only,
`hdr/README.md`; `work/hdrcheck.py` strips every block back to the current repo header (33 / 33 "same"). No git, no repo writes.

## Rows by shape (leaf counts from STATUS.tsv)
- Type getters: GetInterfaceType 15, GetType 9, GetLifeTime 6, GetCameraPointType 5, GetLeafType 2, IsFlourish 1, plus small
  constant / member getters (GetWhenToActivate, GetStatToModifyOnHit, GetMaxNumItems, GetDecalLifeSecs, GetTotalBytesFree,
  GetCardOrder 3, the two targeting getters, GetActionName, GetWinPhrase): 50
- Persist bodies: OnSerialise 14 + OnSerialseLeaf 2 (CPersistContext::Transfer<T>(name, member, default)), Resize 5: 21
- Lifecycle overrides (OnKill 8, OnInitialActivate 6, FrameUpdate 6, BulletTimeFrameUpdate 3, OnRemove, OnAdd, OnTriggerActivate,
  OnFrameUpdate, UpdateTrap, NewGame 2, NewRound, CardsMoving, CardsFinishedMoving 2, OnErase 3, OnCollision, ProcessInput,
  DrawBetAndMoney, OnChangeCategory): 41
- Remaining one-offs (HasFrameUpdate 2, IsComponentSerialisable 3, IsSerialiseCreatable, Delete 2, Copy, ClearPrimitivesIfNecessary,
  IsInventoryScreen, AnimsCanBeIgnored, GetInterpolatedMeshAnim, GetMeshAnims, GetInventoryShoppingValidity, EnterTimelineEnd,
  FreeMemory, DatabaseClone, GetClosestPointToPos2D, the particle callbacks GetUpdateParam / CollectUpdateParameters /
  UpdateAddParticle / PreparePrimitives / GetApproximateSplineLength): 24

## Evidence and judgement calls
- Every leaf is FAM03's proof, re-checked against the slot, the Ego_r / FableWin name sets and the `ret N` (gen.py writes the
  Identity line). Callee names: two sources each (PDB name set + landed body or reloc_bindings); wrong landed spellings go to
  NAME_FIXES.tsv (29 targets: 16 hand-checked, 13 auto).
- CTCGatherPointBase::OnSerialise is public in both PDBs (UAEX) but protected in the repo header: the opt-in reopens `public:`
  around it. CTCTargeted::OnSerialise and CTCBoastingArea / CTCCarriedActionUseRead OnSerialise are protected (MAEX) as the PDBs say.
- Tavern cards: card_sorting_types.h is right (GameType +0x300, CardOrder +0x304, checked with a probe); the earlier "4 bytes high"
  suspicion was wrong. GetCardOrder's `dec edx ; jne` comes only from a switch on GameType with a result local.
- CPersistContext_TransferableVector<T>::Resize calls STLport vector<T>::resize kept out of line
  (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE); retail folds identical resize bodies, so one callee address carries several
  element-type spellings (NAME_FIXES notes this).
- CTCCameraPointDefinitionBase: FAM03's _VSLOTS and my _VSLOTS3 blocks declare its virtuals out of slot order (harmless for leaf
  bodies). FrameUpdate 0x0075C5F0 calls slots 29 / 34, so it uses the new _VTABLE opt-in, which declares them in TLC slot order
  (slot 29 IsLeafCriteriaMet, 34 UpdateActiveCameraPosition, from the vtable aligned with the Xbox 360 Test PDB order).
- NParticleEngine::CPSCRenderMesh::DatabaseClone calls slot 18: the local CPSCBase declares slots 2..18 in TLC order from the same
  alignment; CPSCRenderMesh::Alloc is __fastcall static (SI) in both PDBs (one landed binder spells it SA).
- Local definitions (no shared header exists) on the RTTI base with Ego_r member names at retail offsets: CTCBoastingArea,
  CTCRandomAppearanceMorph (+ CTCAppearanceMorphBase), CTCWallMountEffects, CTCCarriedActionUseRead (+ CTCCarriedActionUseBase),
  CTCActionUseBed, CTCCameraPointBuilding / FixedPoint, CTCJackShield, CTCObstacle (+ CObstacleNavigatorExternalInterface,
  CSteeringAgent: only its ctor and the 0x68 allocation size, `unsigned char Body[0x68]`), CTCTavernGameCardPairs / Blackjack,
  CPersistContext_TransferableVector<T>, NHitLocations::CPhysicalPrimitiveMesh, NParticleEngine::CPSCBase / CPSCRenderMesh, and
  the earlier batches' local classes (levitation, tavern coin base, shot definitions ...). Enum scopes not in any name set
  (EHeroWithinRadiusState, EWallMountEffectsConfirmationType, EBlackjackWinStates) are declared nested and appear in no symbol.
- Descriptive (made-up) names, used once each: the static local s_MaxFrames in 0x007BBA70, the DrawBetAndMoney parameter names,
  the `transfer` name of OnSerialise's bool parameter (the PDBs keep no parameter names).
- Tests: 8 rows got their own tests (`rows/<addr>_test.cpp`, printing `<addr>_TEST PASS`): 0x00569B10, 0x00905420, 0x00B05AB0
  (the catalog tests linked the old symbols; 0x00905420 / 0x00B05AB0 link the row object under VC7.1 with a portable twin under
  g++, 0x00569B10 is self-contained) and 0x005E9820, 0x005E9A70, 0x00744F70, 0x0077B0B0, 0x007E35D0 (their catalog tests are
  __fastcall / __thiscall shims that never linked the row and do not build under g++; the new ones are self-contained twins).
  Every other row keeps its catalog test and PassPattern.
