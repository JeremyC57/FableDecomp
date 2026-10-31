# Hand chunk 609 (CCreatureAction_UnsheatheItemFromInventoryWithHasteAction .. CDegradableDef): 2 of 10 byte-exact

All 10 rows were parked same-length misses from the 2026-10-30/31 retry sweep. Each best attempt was copied, its grade
reproduced (all 10 reproduced exactly), then one change per grade.

## What matched (2)
- CSubtitleRenderer::DrawTextEmbeddedHighlights 0x0048F40A (RELOCATION_MATCH, 258 bytes). Lever: C2DVector::operator+
  builds a named `result` and returns it through the PDB's user copy constructor (`??0C2DVector@@QAE@ABV0@@Z`; VC7.1 has
  no NRVO). X is summed first in place on the GFVXToAX value, then Y, and the copy into the GFACoordToVCoord temporary
  is retail's `fld st(1) ; fstp X ; fst Y ; fstp st(0) ; fstp st(0)` with the argument leas between the fadds.
  `return C2DVector(X + o.X, Y + o.Y)` sums Y first (right-to-left constructor arguments) -- the residue three sweeps
  could not move. Respelled to the PDB: private `(const CEnginePrimitivePrepareInfo&, const C2DBoxF&)`, the callees
  GFVXToAX / GFVYToAY (not GFAXToVX / GFAYToVY), static CPlayerGui::PeekGuiDef, private Draw2DText(const CFontBank&).
  `this` = engine/CSubtitleRenderer.h (its offsets are now retail), CPlayerGui = engine/CPlayerGui.h.
- CTCInventoryExperience::BuyAbility 0x005785B3 (RELOCATION_MATCH, 258 bytes). Lever: every record access goes through the
  node (`it->second.SaveBefore`, `it->second.SaveAfter`, `++it->second.Levels`) instead of a `SExpSpendUndo& undo`
  reference: the node stays live in esi (`inc [esi+24h]`), &it->second is CSE'd into ebx and VC7.1 colours
  this edi / map ebx / node esi as retail; the reference released the node early and permuted all three registers (the
  "pure esi/edi/ebx permutation" the sweep parked). Respelled to the PDB: private GetTCHeroExperience /
  GetTCInventoryAbilities, const GetAbilityLevel / GetAbilityStatType, SetAbilityLevelTo (see review), `this` =
  engine/CTCInventoryExperience.h (UndoableAbilityBuys +0x14C viewed through the landed twin BuyStat's map model,
  DestroyEverything +0x21C), CTCHeroExperience / CTCInventoryAbilities from their headers (methods added by derivation).

## Parked for Fable 5.1 (8)
- 006a02b0 CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction DIFFER(240v240): `sub esp,10h` v `14h` (pair at +0 shares the key/string word; retail key/string +0, pair +4, insert result +0Ch) | tried: inline CTCHaste::SetActionSpeedMultiplier (PDB; its out-of-line copy is 0x005812AE), std::make_pair conversion, named pair + named result, conditional get-or-add, key as by-value inline parameter (244), get-or-add helper (frame 0x18), helper as separate statement, StatGetClassTypeName() string | next: lane gap: frame packing of a full-expression temporary (no scope form found that keeps key/string shared and the pair apart)
- 007c6460 CTCGhostSword::FrameUpdate DIFFER(240v240): i/8i ebp/ebx v ebx/ebp, j/minion edi/ebx v ebx/edi | tried: operator->, pre-decrement, IS_THING_ALIVE macro (232), one shared `long i` for both loops | next: lane gap: register colouring with equal use counts
- 005c4671 CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon DIFFER(248v248): this esi v edi, active pointer edi v esi in the two weapon tests | tried: PDB template IS_THING_ALIVE<CIntelligentPointer<CThing> >, const GetFreeSlot (both inert; kept in the differ for the names) | next: lane gap: register colouring
- 00c77c80 CReverbProcessor::UpdateLineAndTapLengths DIFFER(259v259) (retail_asm grade): tap loop store pointer/temporary esi/edx v edx/esi | tried: moving tap + delay pointers with down-counters (fixes edx, then delay esi v eax), declaration order, unnamed length, per-tap out pointer, moving tap pointer (254) | next: lane gap: register colouring
- 00957db0 CAIStateGroup_MinionBehaviourCrowdBase::Update DIFFER(266v266): this/component esi/edi | tried: out-param GetTC returning the bit-test byte (68 lines, worse), component declared before the flag | next: lane gap: register colouring (model still carries filler virtual slots)
- 00a42170 CPlane::Initialise DIFFER(269v269): cross-product temporary copy loads one x87 instruction early | tried: operator^ through a named result (adds an operand swap) | next: lane gap: store/load scheduling around fstp
- 00c6b9d0 CStateVariableFilter::FilterWithGain DIFFER(269v269) (retail_asm grade): `fld [esi+18h] ; fmul [esi+4]` v retail `fld [esi+4] ; fmul [esi+18h]` | tried: compound damped local, `high -= product`, nested state struct, compound state updates, swapped product orders -- all inert | next: lane gap: x87 commutative operand choice
- 0067e290 CDegradableDef::Transfer DIFFER(273v273): scalar-Transfer temporaries rotate eax/edx/ecx (retail eax each time) + sort operand loads | tried: traits local, non-static Out, case 3 before case 2, by-value Out, forceinline Transfer | next: retail's landed CActionUseDef::Transfer 0x006773F0 rotates exactly like ours, so the difference is outside the Transfer template (maybe the vector Transfer / sort tail); lane gap otherwise

## Names (proven vs descriptive)
- Proven (PDB name set + a second source): DrawTextEmbeddedHighlights, Draw2DText, GFVXToAX, GFVYToAY (PDB order = retail
  order 0x009E1DA0/1DD0/1E10/1E40, bodies multiply by the render-target size), GFACoordToVCoord, CPlayerGui::PeekGuiDef
  (static: callers set no ecx), BuyAbility, SaveExperienceValuesForCountDown, GetTCHeroExperience, GetTCInventoryAbilities,
  GetAbilityLevel, GetAbilityStatType, SpendExperience(EHeroExperienceStatCategory, long), SExpSpendUndo ctor / copy ctor /
  dtor / SaveBefore / SaveAfter, DestroyEverything (header), CTCHaste::SetActionSpeedMultiplier (0x005812AE) and
  UpdateActionSpeedMultiplier (0x00800020), CThing::AddTC, IS_THING_ALIVE<CIntelligentPointer<CThing> >,
  NInventory::CTCInventoryBase::CreateInventoryObject.
- Derived: CPlayerGuiDef retail +0x5BC = Ego_r +0x5F8 TextBoxTextGameActionColour (the 0x3C Dinkumware-vector shift the
  landed 0x0048F230 proves for ControllerGraphicOffset, no container in between).
- Probable (review): 0x005EE01A SetAbilityLevelTo (ForceAbilityLevelTo has the same signature; the plain-store body fits
  the setter).
- Descriptive: the map models of BuyAbility (CAbilityUndoMap::find 0x0057914C / insert 0x0057A1AA, as the landed BuyStat's
  CUndoMap), CRetailVector<T> / GetEmbeddedHighlights() view of the header's opaque vector bytes, CUnprovenTC_0079c740
  (interface 0x92 getter in 0x006A02B0's differ), CUnprovenTC_005d8e50 (interface 0x1A in 0x005C4671's differ).

## Label corrections
- 0x005812AE: catalog CEngineStaticMapFileBlockManager::GetBlockData -> CTCHaste::SetActionSpeedMultiplier(void*, float).
- 0x00800020: catalog CAnimation_RecalculateScale -> CTCHaste::UpdateActionSpeedMultiplier.
- 0x005797A4: catalog CHeroExperienceDef::CHeroExperienceDef -> SExpSpendUndo::SExpSpendUndo().
- 0x00579E1B: catalog _unattributed::SPairLongCSecond_00579c35 -> std::pair<const EHeroAbility, SExpSpendUndo> converting ctor.
- 0x005EE01A: catalog CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation -> CTCInventoryAbilities::SetAbilityLevelTo.
- 0x009E1E10 / 0x009E1E40 are GFVXToAX / GFVYToAY (several bindings say GFAXToVX / GFAYToVY).

## Header debt
- engine/CSubtitleRenderer.h and engine/CTCInventoryExperience.h carry their STL members as opaque byte arrays; the
  matched rows view them through a local vector / map model (`GetEmbeddedHighlights()`, `*(CAbilityUndoMap*)
  UndoableAbilityBuys`). A typed STLport member in the generated headers would remove the casts.
- No engine header for CTCHaste, CTCGhostSword, CTCInventoryWeapons, CPlane, CDegradableDef, C2DVector, C3DVector (local
  models in the differ files).

## Skipped
- none.
