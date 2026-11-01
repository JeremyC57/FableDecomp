# Chunk 680 -- authoring agent notes (43 rows ChatGPT pass 10 could not match)

Result: 18 rows RELOCATION_MATCH/MATCH in src/ + test/ (vland read-only: 18 WINS, all behaviour PASS); 2 rows match
at their TRUE extent in extent/ (defective catalog spans, manifest split needed; graded against MY OWN oracle
extent/oracle_extent.tsv, vland read-only on extent/payload_extent.json: 2 WINS); 23 rows parked in differ/
(one of them, 00c6b260, is an owner decision, one, 00863ca0, also needs a manifest split).

## Proven
Names below are the exact Ego_r/FableWin PDB spellings (function name sets) with a second source (reloc_bindings,
caller/vtable/argument shape, or the retail body) unless marked descriptive.
- PDB-proven row names: 0087bf40 NCameraModes::CRotateAround::OnInit, 009a4160 CStreamingFontBank::FindGlyph,
  00681790 CTCCreatureOpinionOfHero::TransformForcedOpinion, 006bd900 CMusicManager::PlayMusic (catalog label
  Music_LoadAndStartTrack wrong), 0072b330 CTCSpecialAbilities::SetupSummonSpell, 006d8980, 00eb3ba0, 008eebe0, 004be3e0,
  00a2ea20, 004feb70, 00854b90 CCreatureAction_SpiritAttack::FrameUpdate, 009dd960, 005c0121, 004c7e90, 00939dd0
  CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded (catalog labels CheckAndRemove /
  RemoveMyWarningMessage wrong), 009c3590/009c3660/009c5690 CFileInstaller::*, 00bebaa0, 00666d40, 00794e60, 00eb3f40,
  00b21a50, 00863ca0, 005ad305, 007d75a0, 006e3890, 00b8d420, 00911300, 005dc340 CAnimationSet::GetAnim (catalog
  Animation_FindNthValid wrong), 007622c0, 00698b50, 00773790, 006dffb0, 00540320, 00badb40, 00add690, 00d08b40.
- SDK-proven: 00c6b260 salFrameFloatToInt (GameCODA 2.10 SDK source + CODAApiDbg.lib SalFrame.obj symbols).
- Descriptive (no PDB/RTTI exists for the code): 00c73660 ?DetectWindowsVersion@@YAXXZ (void, no args: caller 0x00C73770
  pushes nothing; matches label chunk 586); 00c3d030 CCodaContext::GetResourceCounts (CODA SDK 2.10 members; the Fable 1.5
  fork's own declaration is not on disk); 00c0ffe0 PKCProtectionInstance::~PKCProtectionInstance (class from the
  source-file diagnostic literal; libpk members/globals descriptive, several spellings exist in the tree).
- Callee names fixed this chunk: see NAME_FIXES.tsv (13 rows) and labels_pending.tsv.
- 0x0129BA3C: the tree spells it ?EPSILON@@3MB (naming wave 13, 49 TUs); ChatGPT's raw Ego_r public dump puts
  ?DELTA_NOTIONAL_ZERO@@3MB at the slot directly before ?GD3DFormats@@3QBVCD3DFormatInfo@@B (landed at 0x0129BA40), the value
  is 1e-4f, and the Ego decompile (ghidra_out/decomp_navmesh2.c:4439) has the assert text `< DELTA_NOTIONAL_ZERO`. I could
  not dump the PDB data symbols here (GOTCHAS 2026-11-01: data symbols are not in the name sets), so src/00badb40.cpp keeps
  EPSILON with a comment. Owner/landing: confirm with a PDB data dump, then respell all 49 TUs if it is DELTA_NOTIONAL_ZERO.

## What matched
- 00681790 TransformForcedOpinion MATCH (1 probe): CDefPointer::IsNull test then operator-> re-read (landed twin 006817c0).
- 009a4160 FindGlyph: function-scope output size assigned in the branch, landed CStreamingFontBank shape.
- 0072b330 SetupSummonSpell: the landed out-parameter GetInterface(T**, long) template, negated test (sibling 0x72b0d0).
- 00a2ea20 HasHelperPoint: inline Find with if/return true/return false and a second end().
- 004c7e90 IsUnderScriptSupervision: same-TU CThing::GetScriptThing / HasScriptThing bodies (inlined, as in CThing.cpp).
- 005c0121 CTCInventoryBase::OnCreate (/O1 /Oy-): InventoryOpen=false written first in the reset group.
- 00939dd0 RemoveMessageEventsAdded: visible FindMostRecentMessage template + declared-only predicates.
- 009c3590 / 009c3660 / 009c5690 CFileInstaller: the PDB volatile members (ExitThread, PCompletedRead, Running,
  ThreadFinished) with `// Retail-volatile:` lines citing the discarded reads.
- 00c0ffe0 ~PKCProtectionInstance (/EHsc): named `now = GetTickCount()` local before the elapsed subtraction.
- 00c73660 DetectWindowsVersion (/GS): no locals; the version split straight into the globals.
- 007622c0 CShape::UpdateSplit: named insertion iterator `pos=b; if(indexA>indexB) pos=a;`, midpoint (b+a)*0.5f.
- 00698b50 PushUpManualCameraList: begin() bound to a const-ref temporary, index==first.
- 006dffb0 UpdateEmitterThingAttachment: default-constructed CRightHandedSet assigned in an if/else on the emitter.
- 00b8d420 RenderOcclusionBox: DrawBuffersIndexed called straight on NEngineInternal::GPShaderRenderManager with the
  buffer casts in the argument list (the prior's named manager local made VC reload [esi+1D4h]).
- 00badb40 CEngineInternalPrimitive2DClockSprite::Update: Colour2 a plain member copy; the by-value PDB getter
  GetInterpolationSet() bound to a `const EEngineInterpolationSet&` local (restarts the eax/edx copy alternation).
- 00d08b40 CCampHostageDoor::Main: `for (;;) { if (IsActiveThreadTerminating()) return; ... Game->NewScriptFrame(); }`
  (GOTCHAS b18 companion shape) -- the pre-guard early return now targets the final epilogue as retail.
- Extent rows (own oracle, manifest split): 0087bf40 OnInit true extent 295 bytes 0x0087BF40..0x0087C066 (catalog span 6);
  00854b90 FrameUpdate true extent 566 bytes 0x00854B90..0x00854DC5 (catalog span 21; ChatGPT's recovered body). Both
  RELOCATION_MATCH and their fixtures PASS under g++ and VC7.1. 00863ca0 true extent 358 (0x00863CA0..0x00863E05, catalog 53).

## Parked for Fable 5.1
- 00c6b260 salFrameFloatToInt DIFFER(391v315): OWNER DECISION, not a probe gap | tried: intrinsics (always realign the stack, never fold the load) | next: grade retail_asm -- C++ body with the two SDK __asm loops and `// Retail-asm: 0x00C6B2B0 cvtps2dq xmm0,[eax]`; proof is CODAApiDbg.lib SalFrame.obj labels $sse2FtoiLoop$21115 / $sseFtoiLoop$21127 (I am barred from __asm).
- 00c3d030 CCodaContext::GetResourceCounts (descriptive) DIFFER(98v94): out-pointer loaded at the top of each arm vs once before the m_pSal branch (5 lines) | tried: 19 shapes (SDK 2.10 members, arm order, early returns, locals) | next: the Fable 1.5 CODA fork declaration (name/ABI), by-value descriptor.
- 005dc340 CAnimationSet::GetAnim DIFFER(56v66): retail saves 4 regs, keeps dead this in edi, aligns loop | tried: 13 shapes | next: siblings GetNoAnims 0x005DC290/AnimExists 0x005DC2E0 share the prologue -- real CAnimationSet TU neighbours.
- 006bd900 CMusicManager::PlayMusic DIFFER(170v169): callee-saved permutation sound/this/key/entry | tried: 21 shapes (same-TU LowerBound lever 178->170) | next: real CMusicManager TU context; sound global 0x013B8394 identity.
- 004be3e0 CTCGraphicAppearance::IsResourcesLoaded DIFFER(175v173): begin loaded via eax+copy, dtor reload register | tried: 13 shapes incl. same-TU GetAllMeshes/IsResourcesLoadedForMesh | next: the real TU order of those helpers.
- 006d8980 CTCHeroExperience::UpgradeTrainableStat DIFFER(182v183): hoisted start in edi, copy+1 increment | tried: 12 | next: same-TU GetTrainableStatMaxLevel 0x006D6770 ordering.
- 00eb3ba0 CMurderTwistSheriffGuard::AttackHero DIFFER(186v187): seh_Guards[ID] address register choice | tried: 12 index/pointer forms | next: CV_MurderTwistFinalScript TU neighbours.
- 008eebe0 Mass::BackplaneFriction DIFFER(129v131): fxch st(1); fstp st(0) vs fstp st(1) | tried: 13 + ChatGPT 61 | next: x87 lane gap or real friction helper.
- 009dd960 CRenderManager2D::Draw2DLine DIFFER(227v229): RHW copied through x87 vs integer moves | tried: 12 | next: the real CVertex/RHW member type (integer copy).
- 004feb70 CWorldMap::HeightAreaChanged DIFFER(260v261): zero constant global in ebx in retail, eax here; loop-2 iterator ebp vs ebx | tried: 12 (STLport 4.6 text, IsNull/operator->) | next: real CWorldMap TU neighbours.
- 00bebaa0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix DIFFER(257v257): one commutative x87 pair (Fwd.Y vs Up.X) | tried: 20 incl. 256-variant alias/order sweep | next: the real C3DVector Cross/operator^ inline bodies, or owner call on a single-pair x87 gap.
- 00666d40 CThingCreatureBase::GetHeightZBelowAt DIFFER(346v348): frame slots top/finder swapped; x87 constant-first vs fld st(0) | tried: 13 | next: real anonymous-namespace finder + ForEachInRoughArea body.
- 00863ca0 CThingBodyReorienter::FillBodyReorientationPossibleIntersecting DIFFER(358v358) at own extent: box-corner registers and store order | tried: 14 | next: manifest split first, then real CThingSearchTools::GetAllThingsInRoughArea template body.
- 00eb3f40 CMurderTwistAssassin::Main DIFFER(378v381): dead 4-byte zero word below the filter (shared with 0x00EB55D0) | tried: 13 (if/else restores the bl copy) | next: find the shared TU construct.
- 00773790 CTCShop::GetAttitudeSellPriceMultiplier DIFFER(462v448): frame slots, zero register, exit merge (prior 430v448) | tried: 12 (GetDef declared-only fixes this/thing roles) | next: real CDefPointer release shape.
- 007d75a0 CTCTavernGameCardPairs::GetRandomCard DIFFER(465v465): ecx/edx rotation, store scheduling | tried: 12 | next: CTCTavernGameCardBase TU neighbours.
- 006e3890 CTCTalk::PlaySpeech DIFFER(535v529): text branch at end, join epilogue merge; identity-blocked global 0x013B8394 | tried: 12 | next: real speech-bank manager type + FindBank.
- 00794e60 CTCUnholyPower::CancelForCutscene DIFFER(552v552): arg vs ParentThing load order in inlined StopSound | tried: 12 | next: real CThing GetInterface header for the TU.
- 00b21a50 CSplinePosParamData::DatabaseSerialise DIFFER(565v565): CRC-temp store vs start load | tried: 12 | next: real CCardinalSpline + STLport resize body.
- 005ad305 CTCInventoryTrade::ApplyTattooCard DIFFER(620v620): morph/zero/cursor register cycle | tried: 12 (+1 SRC_FAIL) | next: real CTCHeroMorph::AddTattoo overload set, std::set<long>::insert.
- 00add690 C3DBoundingSphere::FastBuildFromPoints DIFFER(646v648): unroll remainder counter copy (mov esi,ebx) | tried: 12 | next: real C3DVector operator-/GetSquaredMagnitude bodies.
- 00911300 CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update DIFFER(658v666): best probe g (bodyPos default+assign) cuts the masked diff 125->73; left: xor cl,cl early vs xor al,al after the dead flags-temp store, bodyPos copy via lea, GetCreature register, merged fail block kept after the retry loop (retail keeps the LAST copy) | tried: 12 (type-test spellings, copy forms, RemoveMessage chain, for(;;) loop, mask order) | next: retail GetPOriginator<CThing> inline text (FableWin 0x02112220), CMessageEvent::GetPos accessor.
- 00540320 NUISystem::CEditBox::KeyPressed DIFFER(755v755): sub edi,5 vs add edi,-5 + three vptr scratch swaps | tried: 14 forms of the -5 (all canonicalise to sub) | next: a late constant (inline helper/named const through a function), vcall declarations.

## Skipped
- None skipped. 00c6b260 is parked only because the faithful form needs __asm (owner decision above).

## Label corrections
- labels_pending.tsv (format label/addr/old/new): 00939dd0, 006bd900, 004c7e50, 004c7e70, 00cd2770, 005c0c99, 005c2d32,
  009ad3b0, 005dc340 (?GetAnim@CAnimationSet@@...), 005bfcd9 (??0CCategory@NInventory@@... -- not CIntelligentPointer<CThing> ctor).
- 0x00863E70 = ??$GetAllThingsInRoughArea@VCThing@@...VCThingFilter_HasPhysicsMesh@@@CThingSearchTools@@... (catalog
  GetPBestThingInRoughArea<HasTrackName..> wrong; sole caller 0x863D42).
- 0x0143BB58 = ?CODA@@3VCCodaContext@@A (SDK 2.10 ApiSource.obj, ChatGPT claim, consistent with 00c3d030) vs landed
  g_audioSystem (00c335c0), g_listenerStateFactory (00c52980), CAudioSystem (00c3d660) -- review at landing.
- 0x00C25860: landed "C3DVector::CrossProduct" is probably the PDB ?Cross@C3DVector@@QBE?BV1@ABV1@@Z (no CrossProduct in
  the PDB) -- not proven, review.
- 0x0063A300 / 0x0063A390 both carry CWorldMap::GetGroundSizeZAt; one should be GetThemeSizeZAt (inconclusive).
- 0x0143CCC0 is bound as _g_defStringInit; in 00c73660 it is the "version probe done" flag (descriptive; review).
- 0x00F35B10: label files carry both CEntityScriptBase::RegisterFilter (chunks 241/318/320) and SetPredicate (280/296);
  reloc_bindings binds ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z there (PDB name set): drop RegisterFilter.
