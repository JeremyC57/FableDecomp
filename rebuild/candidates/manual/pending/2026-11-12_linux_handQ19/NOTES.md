# Q19 hand chunk (Q18's 14 parked rows) -- 2026-11-12

Result: **5 RELOCATION_MATCH** (all newly ported onto shared headers), **3 DIFFER** (the ported near misses, unchanged),
**6 SKIPPED** (not ported). Grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ19/hdr`; all five matches were re-verified at the
end against the current repo (CTCVillage.h was re-merged after ISWORKTIME landed upstream).

## Matched (payload.json, sources in src/, tests in tests/)
- 0x0065C5BB CTCVillage::UpdateMirroredCrimeData -- `#pragma optimize("y",off)`+`("s",on)` (the CTCVillage TU). Levers: hoist
  `PlayersCrimes.end()` into a local (else `this` stays live and end is reloaded per pass); STLport set gives `::iterator` only.
  Needs the MIRROREDCRIMES opt-in (PlayersCrimes typed as `std::set<long>`, same 0xC bytes).
- 0x0079B6F0 CTCSummonSpell::ResetMinionPos -- CreateMinion 0x0079ACC0's TU define set (same-TU noinline GetSubDef /
  GetEffectFromString), effect lookup inside the minion-alive block, `offset` reused as the direction, NEWTON_SQUARE_PAREN.
  Keeps a local `CSpecialAbilitiesSummonSpellDef` model (no shared header for that def; descriptive, only the members read).
- 0x008C4FD0 CCreatureAction_ScorpionSummon::CreateBabies -- same-TU `__declspec(noinline)` GetDef<CCreatureGenerationFamilyDef>
  body (the in-place `dec [ecx+4]` release and shared frame slot), do-while creature loop, inline CThingAICreatureInit ctor.
  Local `CCreatureGenerationFamilyDef : CParentDefClassBase { std::vector<long> Creatures; }` (no shared header exists; Ego_r.pdb
  member name, retail +0x3C). Uses the new CCreatureAction_RiseUp.h.
- 0x008CDD40 CCreatureAction_ReleaseTimeSpell::FinishAction -- real class via the RELEASETIMESPELL opt-in; named
  `CCharString animation(...)`; the bullet-time -> haste tail is landed 0x006A02B0's.
- 0x008E16A0 NCreatureMode::CModeScreamerSuperAttack::FrameUpdate -- landed sibling Shield 0x008E20F0's define set +
  CINTELLIGENTPOINTER_DEREFERENCE + CVECTORMAP_INSERT_FORCEINLINE (new header NCreatureMode_CModeScreamerSuperAttack.h).

## DIFFER (differ/<addr>.cpp, residue on line 2)
- 0x008780D0 / 0x006FE7C0 GetAllThingsInRoughArea<...> (one template body): post-loop reload order only. c2prio probes of five
  spellings never change the split-piece order; reloads appear to follow ascending split-piece id.
- 0x00837220 CThingTrackNode::UpdateFacingDirection: three scheduling residues; every Newton spelling either leaves the
  `mov edx,17Ch` one tuple late (default, PAREN, GROUPED, UNION) or makes it one tuple early (SQUARE_PAREN, STEP_PAREN).

## SKIPPED (not ported; Q18 drafts left in differ/ with a Q19 note)
00A78B20 CNavQuadTree::SaveToFile, 00540320 NUISystem::CEditBox::KeyPressed (huge local CComponent virtual list, /Zc:wchar_t),
00649E8B NPlayerGui::CDrawFishingGame::Draw (local CTCFishing/CDrawQuestInfo model), 006C1250 CTCBuyableHouse::Update,
007BD845 NThingShot::CTCShotDefinitionDrainLife::InitMovement, 0096CD70 CAIStateGroup_EatAtTable::Update (heavy).

## For the coordinator
- Overlay: see hdr/README.md. Two NEW headers (CCreatureAction_RiseUp.h, NCreatureMode_CModeScreamerSuperAttack.h); the rest are
  `#ifdef FABLE_RETAIL_*` additions. NEWTON_STEP_PAREN is used by no matched row (may be dropped).
- NAME_FIXES: 0x0079C740 is CTCBulletTime::GetTimeMultiplier (landed as `_unattributed::GetFloatThrough_0x20_0x12c_0079c740`);
  0x004FB150 is CWorldMap::GetLoadedRegion (24 landed local models spell it GetCurrentRegionNumber).
- Made-up names: none in function names. Local descriptive models: CCreatureGenerationFamilyDef (PDB class name, one member),
  CSpecialAbilitiesSummonSpellDef in 0x0079B6F0 (PDB class name, only members read).
- Tests: g++ -std=c++03 PASS for all five; VC7.1 cl /Zs clean (C4530 warnings only, from <map>/<string> without /EHsc).
- Q18 drafts of the five matched rows moved out of differ/ to work/q18_drafts/.
