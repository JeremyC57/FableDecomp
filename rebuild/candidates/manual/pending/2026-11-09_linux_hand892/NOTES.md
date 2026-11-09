# Hand chunk 892 (NScript::CV_TravellingHeroesScript::CTH_Thunder .. CEnginePrimitiveRenderUtil): 24 of 27 byte-exact

All 24 graded with gall.sh (FABLE_HDR_OVERLAY=hdr): 22 RELOCATION_MATCH and 2 MATCH. Their tests PASS under g++ (tall.sh) and the
lane VC7.1 (tall.py). check_portable_spellings: 0 bare __forceinline. 3 rows are parked. Rows were re-based on the shared headers
where those exist; the local models that remain are listed under "Review at landing" in LESSONS.md.
Every `// Leaf:` is the full mangled symbol (each checked against its graded object), except the two anonymous-namespace STL rows
(00807250, 00808aa0), whose leaves end in `?A0x` by the GOTCHAS convention. 0x008FC430's UpdateScan is now declared protected (Ego_r /
FableWin `IAE`). Header changes are behind FABLE_RETAIL_* opt-ins in hdr/ (index: hdr/README.md); there is one NEW
declaration-only header, engine/CTCSmashableBase.h.

## What matched (24)
- **Scheduler PAREN tuples (the 81-tuple region cap and single-slot ties).**
  - std::__unguarded_partition 00807250 (first-person targeting sort): `float a = (Get(x));` keeps a no-code PAREN tuple on the
    first compare value, so the pivot `lea` issues before the `fstp`.
  - NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix 004bc800: `(a * b) * c` empties retail's int slot inside the
    x87 chain.
  - NCameraModes::CPhysicsForceModelBase::UpdateScan<const CWorldMap> 008fc430: parenthesised float factors move the
    81-tuple region cut.
  - CEngineInternalPrimitive2DViewport::Update 00be1c10: parenthesised C4DVector constructor arguments (+2 PAREN), on new
    typed primitive headers.
  - CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe 00963880 and CAIStateGroup_MeleeCombatNearby::
    IsEnemyCreatureNearCombatPosition 0096e830: GFFastOneOverSqrt's Newton step parenthesised (hdr NEWTON_PAREN). 00963880
    also writes toMe's components out, and 0096e830 uses the Ego_r private GetCombatManager() accessor.
- **No-capture / exposure (same-TU bodies).**
  - NCreatureMode::GetStrafingAnimationName 008e8ff0: the 0x008E8E50 overload defined first.
  - NUISystem::CComponent::ChangeTargetPositionDelta 0052eaf0: `MoveTo(d + TargetPosition)`.
  - std::__pop_heap 00beeda0 (decal pairs): the shared STLport heap templates.
  - CTCWeaponTrail::GetLastWeaponTrailPoints 0076e4d0: STLport uninitialised copy/fill bodies out of line with the default
    convention.
  - CTCBoastingCrowdControl::Startup 00783850: the landed CThingCompare_Nearest::operator() body.
  - LHNetworkLib::LSocket::Receive 008f99a0: the LSocket.cpp chain down to the read-only leaves.
  - CTCInventoryTrade::RefreshBuyPossibilityPC 005a887d: transitive CDeletion / list bodies, visible and noinline.
  - NParticleEngine::...::CSplinePosParamData::DatabaseSerialise 00b21a50: visible `_M_fill_insert`, `resize` out of line,
    the CCRC::Calc body, and the early-return GetAsCharArray.
  - CAIStateGroup_MinionWorkInInn::GetFreeBench 0090bc60: an out-pointer GetTC.
- **Inline-COMDAT callee bodies.** std::__linear_insert 00808aa0: the comparator's scoring bodies are INLINE definitions
  (hdr CVectorCompare_BestForTargeting.h BODIES). c2 then knows the comparator is not captured, but it does not use the callee
  register usage that plain out-of-line bodies give it (218 v 229 bytes).
- **Forwarded locals and an out-of-line filter.** CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint 008a8010 is built
  on the shared NSCRIPT / CWorld / CThingSearchTools ROUGHAREABODY / CThing GETTC_EXPANDED headers and the new
  CTCSmashableBase.h, using `long x; long y; C2DBoxI area(C2DCoordI(x, y), (long)radius);`.
  - The single-use x / y are forwarded into the temporary. That keeps the X, Y, radius truncation order and gives x / y higher
    candidate ids than the radius, so `x + r` / `y + r` are computed in place.
  - FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE (retail 0x006A80A0) makes the filter temporary escape, so its 4Eh store issues
    before the search-tools load.
- **Other single levers.**
  - CEngineLightingManager::CEngineLightingManager 00b482a0: the real STLport vector constructor shape.
  - CV_TravellingHeroesScript::CTH_Thunder::Main 00ee2490: a parent reference local.
  - MixGainRampUp 00c6e790 / MixGainRampDown 00c6e820: the accumulator is indexed through a copy of the counter.
  - CTCInventoryClothing::GetItemDescriptionArmourRating 005b6767: the game pointer is read first (see LESSONS, review).
  - CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur 00b5e0f0 and CEnginePrimitiveRenderUtil::
    SetupOutlineSmoothAlphaStencilMaskStates 00bf01e0: rewritten on the shared render-state headers.

## Parked for Fable 5.1 (3)
- 00b6fa40 CEngineWaterRenderer::RenderSeaMidground DIFFER(2019v2019): bookmark `test eax,esi ; or esi,eax ; mov [edx+8],esi` (retail) v `test esi,eax ; or eax,esi ; mov [edx+8],eax` at UV setters 2-5 (+0x2F84..0x00B6FD48), and the AlphaBlendEnable(false) mask load order (0x00B7014A) | tried: 52 grades: shared-header rewrite, constant / stage-loop / local spellings, one-level, plain-inline, if/else and GetD3DTexture attach, a mask local, swapped source operands, copy-init capture, unused locals k=1,2,60, TU dummies; diagnostics: the key moves with the IL TUPLE count before the site (a same-symbol `dsum +=` chain moves it, unused symbols do not); it is function-local and periodic (~280 dead-load units); deleting one attach gives A A A B B B, two give A A A A B B | next: lane gap: c2's commutative canonicalisation key (find it in c2.dll; the same gap as 0x00B70230 / 0x00B761D0 / 0x00B751F0, CRenderStateManager.h)
- 00f05a10 NScript::CQ_AmbushTradersScript::RefreshSpyAllegiences DIFFER(1312v1312): the nested `Game->GetHero()` inner vtable is in edx/eax (ours) v eax (retail `push eax ; mov eax,[ecx] ; call [eax+118h]`, 0x00F05CF1..0x00F05D2F) | tried: 51 grades: shared-header rewrite, 31 argument / object / cast / local / front() / index-type spellings, inline_depth(0) at the terminating returns (kept, comment cites 0x00F05F23) | next: find what sets c2's +0x30 hint link on the `&ConvoyMembersVector[i]` lea temp (gdb watch, micro wk/micro/m2a.cpp), then the source shape that ties retail's begin() value
- 005fd5e0 CTCInventoryMap::ConstructRegionDescriptionQuests DIFFER(975v982): block placement only (the same 323 instructions; 7 bytes of jump encodings). Retail keeps the cross-jumped FindChild / SetText / AddChild tail after the core UI leaf (0x005FD761) and the non-core subtree after `ret 0Ch` (0x005FD82D..0x005FD9B1); ours keeps the tail after the last leaf and the subtree in source order | tried: 50 grades (36 here incl. one real /GL+/LTCG link, 14 in chunk 194) + ~60 reduced-model probes: `!IsCore()` / `== false` / `!= false` nesting (986), a fully nested chain, pc-first chains (1018), pc arm order, bool locals, `continue`, `text = 0`, scopes, declaration placement, operand order, explicit temporaries, while / for(;;) / cached-end / const_iterator loops, tail per leaf / per arm, goto, /Oy-; models: goto into the loop, inline helpers with early returns, switch, do/while(0) and for(;;) break wrappers, flags, QFE 4035, LTCG -- each gives the source-order or the then-arm-first layout | next: lane gap: find what marks retail's non-core subtree cold (a c2.dll trace of the block-ordering pass on wk/5fd/m0.cpp), then the shared-header rewrite

## Skipped
- None.

## Label corrections
- 0x0041E5F2 is NUISystem::CManager::GetInstance (landed 0x006001E0 and the NAME_RECOVERY pass); the 005FD5E0 model's
  "CFrontEndManager::GetInstance" was wrong (the parked source now declares and calls
  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ; byte-identical).
- Catalog labels that name the wrong function (gall.sh prints them):
  - 00807250 "std_introsort_Partition_TargetPoint" = std::__unguarded_partition<pair<C3DVector,long>*, ...>.
  - 00808aa0 "std_insertion_sort_Generic" = std::__linear_insert<...>.
  - 0076e4d0 "WeaponTrail_CollectPoints" = CTCWeaponTrail::GetLastWeaponTrailPoints.
  - 00beeda0 "_Cons_val<...pair<CCharString,C3DVector>...>" = std::__pop_heap<pair<pair<long,long>,CEngineTextureDecal>*, ...>.
  - 005a887d "PC_PopulateBuyList" = CTCInventoryTrade::RefreshBuyPossibilityPC.
  - 0052eaf0 "CComponent::ChangePositionDelta" = NUISystem::CComponent::ChangeTargetPositionDelta.
  - 008f99a0 "LSocket_ReceiveWithTimeout" = LHNetworkLib::LSocket::Receive(vector<char>&).
  - 008fc430 "ComputeEvasionParticleTrajectory" = NCameraModes::CPhysicsForceModelBase::UpdateScan<const CWorldMap>.
  - 0090bc60 "GetBench" = CAIStateGroup_MinionWorkInInn::GetFreeBench.
  - 00c6e790 / 00c6e820 "DSP_Accumulate(Unsigned)ModulatedWaveform" = MixGainRampUp / MixGainRampDown.
- Landed 0x00963810 "CIsImposterHit::operator()" is ?FindChargeEvent@CAIStateGroup_VersusScorpionKing@@AAE_NXZ, and landed
  0x008E8A10 "FindChargeEvent" is the anonymous-namespace CIsImposterHit::operator() (the two names are swapped; NAME_FIXES.tsv).
