# Chunk 747 -- bare-dword `fistp` retry (65 rows)

**11 matched, 17 parked, 37 deferred.** All 11 matched rows grade RELOCATION_MATCH in g.sh and in the read-only vland
(`vland_result.txt`: WINS 11, every behaviour test PASS). Every matched row is plain C++; its only asm is inside an existing
shared helper (engine/GFFloatToLongNear.h, GFFloatToLongFloor.h, GFFloatToLongCeil.h, GFFastFloatToLong.h), each row carries a
`// Retail-asm: <its own fistp address(es)>, fistp dword ... -- engine/<helper>.h` line. No `__asm`/`naked`/`_emit`/`volatile` in
any row source or test. **No new helper or macro was needed** (the existing function forms match wherever the macro form does).

Pass 1 graded every authored row once or twice; pass 2 spent 10+ grades on the closest parks (0x00B41A80 23, 0x00B05C60 13,
0x00F36540 13, 0x0057B26F 10). Rows whose residue after pass 1 is large and not fistp-related (first drafts of 400-2400-byte
bodies, skeletons, invented stand-ins, rows with no fistp of their own) are `deferred` with a one-line residue + next idea, per the
brief's budget rule -- they are not 10-attempt parks.

## Matched (src/ + test/)
| Row | Name | Attempts | Helper | Note |
|---|---|---|---|---|
| 0x004385F0 | CPlayerGui::UpdateCrime | 11 | GFFloatToLongNear | RELOCATION_MATCH try n: Ego_r names (GetPNearestTCVillage/CTCVillage, SetFine, inline GetCrime + SetNormalisedTimeLeft), PDB UpdateStat(..., CStatBase&) const, steps (not the discarded rounding) is UpdateStat's third arg |
| 0x006378E0 | NThingMap::CThingMapManager::GetThingMapwhoLevel | 6 | GFFloatToLongFloor | RELOCATION_MATCH; __assume(0) after the level loop |
| 0x009FCA40 | CGraphicFrameData::InitialiseAsSurfaceSection | 16 | GFFastFloatToLong | RELOCATION_MATCH try o: the PDB signature (long width/height, tlx/tly) and the shared CCountedPointer.h; long height moved the inlined helper's value slot to the texture slot |
| 0x00A76E00 | CNavQuadTree::GetPreferabilityCostForNode | 6 | GFFloatToLongFloor x2, GFFloatToLongNear x2 | RELOCATION_MATCH; C2DBoxF + C2DVector rel locals |
| 0x00A88D10 | CQuaternion::SetFromAxisAngle | 1 | GFFloatToLongNear | RELOCATION_MATCH; inlined GFFastSin/GFFastCos |
| 0x00AA3C50 | C3DKeyframe::BlendWith | 11 | GFFloatToLongNear (GFFastArcCos/GFFastSin) | RELOCATION_MATCH try h/m: identified as FableWin ?BlendWith@C3DKeyframe@@QAEXABV1@M@Z (not CRightHandedSet::Interpolate); inline Dot before the sign local; engine/C4DVector.h opt-in gains operator-/operator+/__forceinline Interpolate (hdr) |
| 0x00B05E10 | NParticleEngine::CPSCSingleSprite::SmoothFadeValue | 1 | GFFloatToLongNear | RELOCATION_MATCH; body of landed GFSmoothFadeValue 0x00AF9000 |
| 0x00B16220 | NParticleEngine::CPSCOrbit::InterpolateTo | 4 | GFFloatToLongNear | RELOCATION_MATCH try b: PDB signature (const CPSCBase&), nested COrbitSettings, the landed CPSCLight GFInterpolate overloads (KKKM rounds through GFFloatToLongNear) |
| 0x00B18820 | NParticleEngine::CPSCLight::Update | 6 | GFFloatToLongNear | RELOCATION_MATCH |
| 0x00B1AD90 | NParticleEngine::CPSCLight::SmoothFadeValue | 1 | GFFloatToLongNear | RELOCATION_MATCH; body of landed GFSmoothFadeValue 0x00AF9000 |
| 0x00B84580 | CEngineSubPrimitiveRenderer::AddSubPrimitive | 2 | GFFloatToLongNear x2 | RELOCATION_MATCH; real STLport vector<CSortElement> push_back |

## Parked (differ/<addr>.cpp + differ/<addr>.test.cpp; line 2 carries the grade and residue)
| Row | Name | Attempts | Helper | Note |
|---|---|---|---|---|
| 0x00439BB0 | CPlayerGui::UpdateMoney | 2 | GFFloatToLongNear | best b 592v592 dl=23 (frame-masked 26): frame slots shifted by 4 and the uninitialised-bar tail reloads PMoney per store in retail; GOLD_PAYMENT name as a named CCharString local fixed the temp order |
| 0x0057B26F | CTCHeroStats::ForceStaminaTo | 10 | GFFloatToLongNear | best n dl=2 (156v156): the inline GetInterface key is packed into [ebp-8] (the helper result slot) where retail uses the dead fraction slot [ebp+8]; tried literal/named GFLimit bounds, fraction *= MaxStamina inside the call, listener at function scope, block scope, a stamina local, the macro form (all dl=2) |
| 0x005EE77C | CTCInventoryAbilities::GetExperienceCostToUpgradeAbility | 2 | GFFloatToLongNear | best b dl=14: GetInterface select keeps last in edi and moves the hit into it (jg; mov edi,eax), morality int copied through the key slot before fild, cost preloaded in eax |
| 0x007E2DF0 | CTCWobble::OnInitialActivate | 5 | GFFloatToLongCeil | best b 252v257 dl=27: retail copies GetRHSet into ParentRHSet through edi/edx pointer regs (lea ecx,[esi+0Ch]) and re-reads the copy via ecx; not fistp-related |
| 0x0082A020 | CMiniMapDisplay::GetDrawPosition | 1 | GFFloatToLongFloor x2 | try a (rewritten) 332v343, frame-masked dl=31: x87 schedule of the zoomed box (retail keeps MinY on the stack and reuses height) and the final sums (fild of both ints first); frame layout |
| 0x00A48F10 | CLipSyncSampleData::GetFrameInterpolate | 1 | GFFloatToLongFloor | try a dl=128 (815v811): helper in place of the GFFloor stand-in; the phoneme blend loop is first-draft |
| 0x00A4C1F0 | C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&,ulong,float) const | 1 | GFFloatToLongNear (GFFastArcCos/GFFastSin) | try a dl=210: retail frame 0x34 vs ours 0x18 (a quaternion-sized local, cosom spilled to the dead frame slot); the PDB InlineGetKeyframeInterpolate + CQuaternion::SetSlerp inline shape not yet modelled |
| 0x00A567A0 | CCardinalSpline::GetSplinePosition | 2 | GFFloatToLongFloor | try b dl=247 (683v766): the Ego_r signature is void GetSplinePosition(C3DVector& out, float t) const (QBEXAAVC3DVector@@M@Z) but the draft returns C3DVector by value, so the leaf is absent and the largest function was graded; next: rewrite to the out-reference form, then grade |
| 0x00A770B0 | CNavQuadTree::GetNodeAt | 5 | GFFloatToLongFloor | try d dl=24: engine/CNavQuadTree.h retail opt-in + hdr friend (CANavQuadTreeNode grants CNavQuadTree its protected slot 11); a named const cellSize gives retail's guarded dynamic init of the 1/32 static; residue: the area box (retail copies MapPos as ints and computes BR = size + MapPos; the contains test loads pos and fcomps the box words) and the BlockedNode store order (retail Y,X computed then X stored first -- likely the inline static SetBlockedNode(x, y, 0, layer) of Ego_r ?SetBlockedNode@CNavQuadTree@@SIXMMJJ@Z, args evaluated right to left) |
| 0x00AAD340 | CWaveFile::ConvertSamplingRate | 1 | GFFloatToLongFloor x4 | try a dl=124 (606v584): the Oct draft with the helper; the resample loop shape (pointer/counter registers, the empty-vector destination select) not yet reworked; PDB ?ConvertSamplingRate@CWaveFile@@QAEXK@Z |
| 0x00B05C60 | NParticleEngine::CPSCSingleSprite::GetComponentCost | 13 | GFFloatToLongNear (RoundToDecimals) | try b dl=2: everything matches except the CrossedSprites bit test -- retail `mov al,[esi+93h] ; test al,20h`, ours `test byte ptr [esi+93h],20h` (ternary/if/accessor/unsigned-char-bitfield/(bool)/long-local forms all give test byte ptr) |
| 0x00B41A80 | CEngineMap::PeekWaterHeight | 23 | GFFloatToLongNear (QuantiseLandscapeHeight) | try p dl=4: Ego_r const signatures, inline CEngineWorldMap::GetMap + QuantiseLandscapeHeight, world local, ly before lx; residue: retail spills the zero-extended MapWidth (movzx) for the x clamp -- a long width local is rematerialised from this, an unsigned short / short local spills but loads `mov ax ; movzx eax,ax`; plus one ebp reload placement |
| 0x00B51AD0 | CEngineWeatherRenderer::BuildBoxPositionMatrix | 4 | GFFloatToLongFloor x6 | best b (snap part matches incl. inline GetBoxCoord) 965v969 dl=53: the 9x9 fill -- retail keeps the outer counter in [ebp-4] with a 9-trip down-counter and ebx/esi/edi live (pops after the loop), one imul operand order in the Z snap; the inner loop is j*BoxSizeY + CentreY fully unrolled |
| 0x00B93B30 | CEngineSceneMap::AddPrimitive | 2 | GFFloatToLongFloor | try b dl=72: C2DVector rel = position - TL (shared C2DVector/C2DBoxF retail opt-ins) fixed the area normalisation; residue is frame-slot packing -- retail keeps pos in the dead radius slot, the first floor result in the position slot and builds the shifted cell pos as a 3-byte temporary at [ebp+0Dh] whose first word is copied back |
| 0x00B947E0 | CEngineSceneMap::Update | 5 | GFFloatToLongNear | best c dl=16 (251v246): retail keeps the span in esi (pushed at entry) and increments it in place for the inlined HighestSetBit; ours rounds into eax and copies to a lazily pushed esi |
| 0x00F36010 | NFractal::CPerlinNoise::Get2DNoise | 1 | GFFloatToLongFloor x2 | try a (rewritten with Ego_r LookupIndex / LookupGradient / Interp) 473v442 frame-masked dl=71: gradient copies (retail copies only X/Y through integer registers) and the dot-product x87 schedule; the old draft's GetGaussian2DNoise label belongs to 0x00F36540 |
| 0x00F36540 | NFractal::CPerlinNoise::GetGaussian2DNoise | 13 | GFFloatToLongFloor x2 | best f dl=4 (321v321): only the epilogue differs -- retail pops edi/esi before the two x87 cleanup fstp st(2)/st(0); tried result local, Interp operand order, sy/b/g01 orders, optimize(y off) (same), optimize(s on) (worse) |

Next ideas: 0x00B05C60 -- find the source form VC7.1 turns into `mov al,[m] ; test al,imm` for a bool bit-field (a byte copy in a
union / an `unsigned char` flags word read through a reference?); 0x00B41A80 -- a `long` width that cannot be rematerialised (read
through a non-inline accessor pattern or a `const` member copy); 0x00F36540 -- epilogue order (pops before the x87 kills) may need
the dead values to die in a different block; 0x0057B26F -- frame-slot packing order of the helper result vs the GetInterface key;
0x00A770B0 -- write the blocked-node fill as the Ego_r inline static `SetBlockedNode(x, y, 0, layer)` and the area test as a
C2DBoxF contains with pos loaded first; 0x00B93B30 -- the shifted cell position as a `CEngineSceneGridPos` temporary built by a
3-argument constructor; 0x00A4C1F0 (and its three dependants) -- model Ego_r `InlineGetKeyframeInterpolate` with an inline
`CQuaternion::SetSlerp` into a quaternion temporary (retail frame 0x34).

## Deferred
| Row | Name | Attempts | Helper | Note |
|---|---|---|---|---|
| 0x0041AFA0 | CSprite::Draw | 0 | 4 sites | 1455-byte first draft (1041v1455): the inlined sprite-bank texture refresh is missing |
| 0x0057C4F1 | CTCHeroStats::ApplyBonusItemEffects | 0 | GFFloatToLongCeil (fld1 variant) | retail materialises the helper's 1.0f with `fld1 ; fstp [ebp-4]` (0x00646762 too) where engine/GFFloatToLongCeil.h stores `mov [..],3F800000h`: needs a helper variant (new shared macro/helper + dual-path test) before the row can match; the body is a 394-instruction first draft |
| 0x005C7452 | ConstructWeaponDescriptionDamage | 0 | - | not authored (no earlier draft body; 698 bytes, 2 bare-fistp sites): needs a full first draft |
| 0x00646762 | NPlayerGui::CStatBar::DrawStatBarOnLeft | 1 | GFFloatToLongFloor | try a 731v740 dl=121: brief Oct-22 draft (graphic / text / change-marker flow far from retail); retail materialises the helper's 1.0f with fld1/fstp; rewrite from the landed CCrimeStatBar sibling 0x00646010 next |
| 0x0064D404 | NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo | 0 | - (no fistp in the row) | retail has no fistp at all (fild of a virtual's int result only); the residue is the unauthored sprite/text tail (calls 0x00437870, slot 0x60, 0x00644FC5, 0x00643EB2) -- a plain C++ row for a non-fistp chunk |
| 0x0066CD50 | CRegionMinimap::BuildRegionMinimapTGA | 1 | GFFloatToLongNear | try a 433v423 frame-masked dl=124: loop counters / register map and an invented CCellVector stand-in for the STLport cell vector release; needs the real vector model |
| 0x006B2960 | CGameNavigatorManagerExternalInterface::GetNavigationInformation | 1 | GFFloatToLongNear x2, GFFloatToLongFloor x2 | try a 237v250 dl=70: the manager interface is unidentified (slot 13 called once with no arguments and once with four), param-slot reuse for the rounded copies; needs the navigator manager interface model first |
| 0x006CC800 | CTCCreatureNavigation::FrameUpdate | 0 | GFFloatToLongCeil (the fadd 0.5 / fsub 1.0 / dec block at 0x006CC90B) | not authored this pass: needs the CTCCreatureNavigation.h opt-in (FrameUpdate, typed Agents vector), CThingSearchTools::ForEachInRoughArea<CThing,CThingFilter_IsNot,(anon)::FrameUpdateThing> 0x006CE800 (an anonymous-namespace functor: the ?A0xd4ba0483 symbol is per-TU), CThing::GetCurrentSpeed 0x00661CD0, CThingPhysical::PeekPos 0x006A4D00, CWorld::GetFrame 0x0049D870; the draft's names were invented |
| 0x006E5A00 | CScriptConversation::FrameUpdate | 0 | 2 sites | skeleton draft (120v1772): not authored |
| 0x007727E0 | CTCShop::GetCurrentBasicPrice | 0 | GFFloatToLongCeil | EH-framed def-pointer body with invented def accessors (GetShopDef/GetValueDef stand-ins); needs the real CDefPointer/GetSubDef model |
| 0x0077A430 | CTCDExperienceOrb::FrameUpdate | 0 | GFFloatToLongCeil/Near (1 site) | 1115-byte semantic first draft (rough-area search template, merge flow); residue not fistp-related |
| 0x0078D250 | CTCBerserk::Update | 0 | 2 sites | 1729-byte first draft (release/knock-back/ramp flow); residue not fistp-related |
| 0x0079D360 | CTCBulletTime::CTCBulletTime | 0 | 1 site | 1115-byte constructor first draft (members, def load, sounds, CAST effect); residue not fistp-related |
| 0x00914650 | CAIStateGroup_BirdStartled::IsRunnable | 1 | GFFloatToLongCeil | try a 350v340 dl=63: Oct-21 draft with invented interface / search-tool stand-ins (frame 0x38 v 0x30, register map); needs the landed CThing GetTC / GetPBestThingInRoughArea models |
| 0x00986210 | CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable | 0 | GFFloatToLongCeil (planned) | Oct-21 draft with invented filter-bundle stand-ins (CFilter4) for the GetPFirstThingInRoughArea template call; needs the landed CThingSearchTools filter model first |
| 0x00A48BA0 | (C3DAnimation WriteToFile, draft name) | 0 | 1 site | 873v875 old grade, but the lane's <map> is not retail's STLport map (header malloc, inline _M_erase): container model first |
| 0x00A4D770 | C3DAnimation2::CalcLocalTransformationsRelative | 0 | GFFloatToLongNear | inlines the slerp of 0x00A4C1F0 into a dead keyframe (only the asm blocks survive): follows that row |
| 0x00A52760 | C3DAnimationSequenceData::GetKeyframeInterpolate (other TU copy) | 0 | GFFloatToLongNear | byte-near twin of 0x00A4C1F0 (pending dl=210): follows that row |
| 0x00A53860 | C3DAnimation2::CalcLocalBoneTransformation | 0 | GFFloatToLongNear | inlines the 0x00A4C1F0 body: follows that row |
| 0x00A781A0 | UpdateLines | 0 | - | not authored (no earlier draft body; 1244 bytes, 4 bare-fistp sites): needs a full first draft |
| 0x00B012E0 | CPSCRenderMesh | 0 | - | not authored (no earlier draft body; 1296 bytes, 22 bare-fistp sites): needs a full first draft |
| 0x00B075D0 | AddToBoundingSphere | 0 | - | not authored (no earlier draft body; 977 bytes, 1 bare-fistp sites): needs a full first draft |
| 0x00B103F0 | CPSCEmitterGeneric | 0 | - | not authored (no earlier draft body; 1463 bytes, 28 bare-fistp sites): needs a full first draft |
| 0x00B16410 | Update | 0 | - | not authored (no earlier draft body; 1671 bytes, 8 bare-fistp sites): needs a full first draft |
| 0x00B1D550 | DecalCollides | 0 | - | not authored (no earlier draft body; 576 bytes, 2 bare-fistp sites): needs a full first draft |
| 0x00B1D790 | PoolDecal | 0 | - | not authored (no earlier draft body; 876 bytes, 2 bare-fistp sites): needs a full first draft |
| 0x00B1DC70 | AddToBoundingSphere | 0 | - | not authored (no earlier draft body; 860 bytes, 4 bare-fistp sites): needs a full first draft |
| 0x00B1DFD0 | CPSCDecalRenderer | 0 | - | not authored (no earlier draft body; 931 bytes, 14 bare-fistp sites): needs a full first draft |
| 0x00B328C0 | (draft: heavily invented) | 0 | - | Oct draft built on invented stand-in types; not re-authored this pass |
| 0x00B53F60 | CEngineWeatherRenderer::FrameStart | 0 | - (no fistp in the row) | old residue: retail keeps this in ecx across the same-TU call UpdateBoxPositions 0x00B52990; the draft uses a derived _Methods stand-in and invented globals. Next: same-TU noinline copy of the landed UpdateBoxPositions body (VC7.1 then knows ecx survives), real class spellings |
| 0x00B94BF0 | CEngineScene::GetMapAt | 0 | GFFloatToLongFloor x2 (planned) | draft uses invented stand-ins for the nested STLport map<long, map<long, CCountedPointer<CEngineSceneMap>>> (0x00B988E0 / 0x00B97C10) and a pointer return where Ego_r says reference; needs the real STLport 4.6 map model first |
| 0x00B99300 | (SetVSConstants renderer, no fistp) | 0 | - (no fistp in the row) | register/schedule residue only (bool/count args loaded at entry, two SetVSConstants tails kept apart, temporaries' frame order); not a fistp row |
| 0x00BA1140 | CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive | 0 | 2 sites (GFFastFloatToLong family) | 2344-byte body: LOD loop rotation, frame 0x294 v 0x298 and callee-saved drift besides the two FtoL sites |
| 0x00BADE40 | CEnginePrimitiveManagerSprites::RenderSprite | 0 | GFFloatToLongNear (FtoL) | 793-byte first-pass body with a local camera/render-state model; render-state capture block not modelled |
| 0x00BD71B0 | CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive | 0 | 1 site (GFFastFloatToLong) | 2116-byte body: radius load order, frame 0x290 v 0x294 and callee-saved drift besides the FtoL site |
| 0x00C41FB0 | CCombatant::ComputeDerivedCombatStats (autoname) | 0 | 4 sites | 81-block first pass with an autoname label; needs identity first |
| 0x00F361D0 | NFractal::CPerlinNoise::Get3DNoise | 0 | GFFloatToLongFloor x3 | the Perlin family model (0x00F36540 pending dl=4, 0x00F36010 dl=71) first |

## C++ attempts before helper (owner rule 2026-11-04)

Method: `tools/probe.py <addr> <src>` replaces every helper call site of the row by each spelling below, compiles with VC7.1 RTM
13.10.3077 (`cc2.sh`, the row's flags) and counts in the leaf: `__ftol2` calls, bare `fistp dword`, `fistp qword`, `cvtss2si`,
`cvttss2si`, other calls (raw rows: `ATTEMPTS.tsv`). Spellings -- Near / FastFloatToLong sites: `(long)x`, `(int)x`, `(short)x`,
a long temporary, `(long)(x+0.5f)`, `(long)(double)x`, `floor` with `#pragma intrinsic(floor)` /Oi, a same-TU `__declspec(noinline)`
static C++ callee, `/QIfist` on the cast and on the temporary, `_mm_cvtss_si32(_mm_set_ss(x))`, `_mm_cvttss_si32(...)` (VC7.1 RTM
`<xmmintrin.h>`), and the MACRO form (an `__asm` block hoisted to the statement naming the caller's operand directly, or a named
float temporary when the operand is a member/expression; graded with g.sh). Floor / Ceil sites: `floor`/`ceil` intrinsic, `floorf`/
`ceilf`, `(long)x`, the C++ block of the helper (`(long)(x-0.5f)` + compare + bump), the same with /QIfist, double floor/ceil, a
noinline static callee, /QIfist floor/ceil, `_mm_cvtss_si32`, MACRO.

Result, every site: plain C++ never emits a bare dword `fistp` (always `__ftol2`; floor/ceil call the CRT then `__ftol2`; the
noinline callee is a call); `/QIfist` emits `fistp qword` into an 8-byte slot; the SSE intrinsics emit `cvtss2si` / `cvttss2si`;
VC7.1 RTM has no x87-fistp intrinsic. At 0x004386B4 the rounded value is dead in retail (only the asm keeps it): every C++
spelling deletes the conversion outright. Landed grep: no landed source reproduces a bare `fistp dword` without a helper or asm
(every landed file containing `fistp` either includes a GFFloatToLong* helper / retail_asm block or only mentions it in a comment).
Macro vs function: over the 12 helper groups of the matched rows the macro form matched in 9, differed in 2 and was not
applicable in 1 (0x00B16220: the operand is inside an inline return), while the function form matches in all 12; where they differ (0x009FCA40 macro 200v218, 0x00A76E00 Near macro 294v298) only the function form (operand copied through a
frame slot, as retail) matches, so every row keeps the existing `__forceinline` function -- no new macro.

| Row | Helper | Retail site(s) (bare `fistp dword`) | Plain C++ spellings -> VC7.1 RTM emits | /QIfist | Intrinsics (`_mm_cvtss_si32` / `_mm_cvttss_si32`) | Macro form (asm names the operand) | Function form (`__forceinline` helper) |
|---|---|---|---|---|---|---|---|
| 0x004385F0 | GFFloatToLongNear | 0x004386B4 `fistp DWORD PTR [esp+0xc]` | `long_cast`: __ftol2 x1; `int_cast`: __ftol2 x1; `short_cast`: __ftol2 x1; `temp_local`: __ftol2 x1; `add_half`: __ftol2 x1; `double_cast`: __ftol2 x1; `floor_intrinsic`: __ftol2 x1; `noinline_static`: __ftol2 x1 | `QIfist_long_cast`: fistp qword x1; `QIfist_temp_local`: fistp qword x1 | __ftol2 x1 / __ftol2 x1 | named-temp operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x0057B26F | GFFloatToLongNear | 0x0057B29C `fistp DWORD PTR [ebp-0x8]` | `long_cast`: __ftol2 x2; `int_cast`: __ftol2 x2; `short_cast`: __ftol2 x2; `temp_local`: __ftol2 x2; `add_half`: __ftol2 x2; `double_cast`: __ftol2 x2; `floor_intrinsic`: __ftol2 x2; `noinline_static`: __ftol2 x1 | `QIfist_long_cast`: fistp qword x2; `QIfist_temp_local`: fistp qword x2 | __ftol2 x1, cvtss2si x1 / __ftol2 x1, cvttss2si x1 | direct operand, graded DIFFER(156v156) | parked: non-rounding residue (STATUS.tsv) |
| 0x005EE77C | GFFloatToLongNear | 0x005EE86C `fistp DWORD PTR [ebp+0xc]` | `long_cast`: __ftol2 x1; `int_cast`: __ftol2 x1; `short_cast`: __ftol2 x1; `temp_local`: __ftol2 x1; `add_half`: __ftol2 x1; `double_cast`: __ftol2 x1; `floor_intrinsic`: __ftol2 x1; `noinline_static`: no dword fistp (calls=4) | `QIfist_long_cast`: fistp qword x1; `QIfist_temp_local`: fistp qword x1 | cvtss2si x1 / cvttss2si x1 | direct operand, graded DIFFER(259v267) | parked: non-rounding residue (STATUS.tsv) |
| 0x006378E0 | GFFloatToLongFloor | 0x00637903 `fistp DWORD PTR [ebp+0x8]` | `floor_intrinsic`: __ftol2 x1; `floorf`: __ftol2 x1; `long_cast`: __ftol2 x1; `cpp_block`: __ftol2 x1; `double_floor`: __ftol2 x1; `noinline_static`: no dword fistp (calls=1) | `cpp_block_QIfist`: fistp qword x1; `QIfist_floor`: fistp qword x1 | cvtss2si x1 | direct operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x007E2DF0 | GFFloatToLongCeil | 0x007E2ECE `fistp DWORD PTR [ebp-0x4]` | `long_cast`: __ftol2 x1; `cpp_block`: __ftol2 x1; `noinline_static`: no dword fistp (calls=4); `ceil_intrinsic`: __ftol2 x1; `ceilf`: __ftol2 x1; `double_ceil`: __ftol2 x1 | `cpp_block_QIfist`: fistp qword x1; `QIfist_ceil`: fistp qword x1 | cvtss2si x1 | direct operand, graded DIFFER(252v257) | parked: non-rounding residue (STATUS.tsv) |
| 0x009FCA40 | GFFastFloatToLong | 0x009FCAD4 `fistp DWORD PTR [edx]`; 0x009FCAFD `fistp DWORD PTR [edx]` | `long_cast`: __ftol2 x2; `int_cast`: __ftol2 x2; `short_cast`: __ftol2 x2; `temp_local`: __ftol2 x2; `add_half`: __ftol2 x2; `double_cast`: __ftol2 x2; `floor_intrinsic`: __ftol2 x2; `noinline_static`: no dword fistp (calls=4) | `QIfist_long_cast`: fistp qword x2; `QIfist_temp_local`: fistp qword x2 | cvtss2si x2 / cvttss2si x2 | named-temp operand, graded DIFFER(200v218) | RELOCATION_MATCH (src/) |
| 0x00A76E00 | GFFloatToLongFloor | 0x00A76E6C `fistp DWORD PTR [ebp-0x4]`; 0x00A76EA3 `fistp DWORD PTR [ebp+0x8]`; 0x00A76EC3 `fistp DWORD PTR [ebp-0x10]`; 0x00A76EF9 `fistp DWORD PTR [ebp-0x10]` | `floor_intrinsic`: __ftol2 x2; `floorf`: __ftol2 x2; `long_cast`: __ftol2 x2; `cpp_block`: __ftol2 x2; `double_floor`: __ftol2 x2; `noinline_static`: no dword fistp (calls=2) | `cpp_block_QIfist`: fistp qword x2; `QIfist_floor`: fistp qword x2 | cvtss2si x2 | named-temp operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x00A76E00 | GFFloatToLongNear | 0x00A76E6C `fistp DWORD PTR [ebp-0x4]`; 0x00A76EA3 `fistp DWORD PTR [ebp+0x8]`; 0x00A76EC3 `fistp DWORD PTR [ebp-0x10]`; 0x00A76EF9 `fistp DWORD PTR [ebp-0x10]` | `long_cast`: __ftol2 x1; `int_cast`: __ftol2 x1; `short_cast`: __ftol2 x1; `temp_local`: __ftol2 x1; `add_half`: __ftol2 x1; `double_cast`: __ftol2 x1; `floor_intrinsic`: __ftol2 x2; `noinline_static`: no dword fistp (calls=1) | `QIfist_long_cast`: fistp qword x1; `QIfist_temp_local`: fistp qword x1 | cvtss2si x1 / cvttss2si x1 | named-temp operand, graded DIFFER(294v298) | RELOCATION_MATCH (src/) |
| 0x00A88D10 | GFFloatToLongNear | 0x00A88D39 `fistp DWORD PTR [esp]`; 0x00A88D9C `fistp DWORD PTR [esp+0x10]` | `long_cast`: __ftol2 x2; `int_cast`: __ftol2 x2; `short_cast`: __ftol2 x2; `temp_local`: __ftol2 x2; `add_half`: __ftol2 x2; `double_cast`: __ftol2 x2; `floor_intrinsic`: __ftol2 x2; `noinline_static`: no dword fistp (calls=2) | `QIfist_long_cast`: fistp qword x2; `QIfist_temp_local`: fistp qword x2 | cvtss2si x2 / cvttss2si x2 | named-temp operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x00AA3C50 | GFFloatToLongNear | 0x00AA3CDE `fistp DWORD PTR [esp+0x4]`; 0x00AA3D49 `fistp DWORD PTR [esp+0x4]`; 0x00AA3DB2 `fistp DWORD PTR [esp]` | `long_cast`: no dword fistp (calls=0); `int_cast`: no dword fistp (calls=0); `short_cast`: no dword fistp (calls=0); `temp_local`: no dword fistp (calls=0); `add_half`: no dword fistp (calls=0); `double_cast`: no dword fistp (calls=0); `floor_intrinsic`: no dword fistp (calls=0); `noinline_static`: no dword fistp (calls=0) | `QIfist_long_cast`: no dword fistp (calls=0); `QIfist_temp_local`: no dword fistp (calls=0) | no dword fistp (calls=0) / no dword fistp (calls=0) | named-temp operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x00B05E10 | GFFloatToLongNear | 0x00B05E35 `fistp DWORD PTR [esp]` | `long_cast`: __ftol2 x1; `int_cast`: __ftol2 x1; `short_cast`: __ftol2 x1; `temp_local`: __ftol2 x1; `add_half`: __ftol2 x1; `double_cast`: __ftol2 x1; `floor_intrinsic`: __ftol2 x1; `noinline_static`: no dword fistp (calls=1) | `QIfist_long_cast`: fistp qword x1; `QIfist_temp_local`: fistp qword x1 | cvtss2si x1 / cvttss2si x1 | direct operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x00B16220 | GFFloatToLongNear | 0x00B16269 `fistp DWORD PTR [esp+0xc]` | `long_cast`: __ftol2 x1; `int_cast`: __ftol2 x1; `short_cast`: __ftol2 x1; `temp_local`: __ftol2 x1; `add_half`: __ftol2 x1; `double_cast`: __ftol2 x1; `floor_intrinsic`: __ftol2 x1; `noinline_static`: no dword fistp (calls=1) | `QIfist_long_cast`: fistp qword x1; `QIfist_temp_local`: fistp qword x1 | cvtss2si x1 / cvttss2si x1 | n/a: operand is an expression inside a call/return (only a named temporary = the function form) | RELOCATION_MATCH (src/) |
| 0x00B18820 | GFFloatToLongNear | 0x00B18852 `fistp DWORD PTR [esp+0x8]` | `long_cast`: __ftol2 x2; `int_cast`: __ftol2 x2; `short_cast`: __ftol2 x2; `temp_local`: __ftol2 x2; `add_half`: __ftol2 x2; `double_cast`: __ftol2 x2; `floor_intrinsic`: __ftol2 x2; `noinline_static`: __ftol2 x1 | `QIfist_long_cast`: fistp qword x2; `QIfist_temp_local`: fistp qword x2 | __ftol2 x1, cvtss2si x1 / __ftol2 x1, cvttss2si x1 | named-temp operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x00B1AD90 | GFFloatToLongNear | 0x00B1ADB5 `fistp DWORD PTR [esp]` | `long_cast`: __ftol2 x1; `int_cast`: __ftol2 x1; `short_cast`: __ftol2 x1; `temp_local`: __ftol2 x1; `add_half`: __ftol2 x1; `double_cast`: __ftol2 x1; `floor_intrinsic`: __ftol2 x1; `noinline_static`: no dword fistp (calls=1) | `QIfist_long_cast`: fistp qword x1; `QIfist_temp_local`: fistp qword x1 | cvtss2si x1 / cvttss2si x1 | direct operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |
| 0x00B84580 | GFFloatToLongNear | 0x00B845A6 `fistp DWORD PTR [esp+0x28]`; 0x00B84648 `fistp DWORD PTR [esp+0x28]` | `long_cast`: __ftol2 x2; `int_cast`: __ftol2 x2; `short_cast`: __ftol2 x2; `temp_local`: __ftol2 x2; `add_half`: __ftol2 x2; `double_cast`: __ftol2 x2; `floor_intrinsic`: __ftol2 x2; `noinline_static`: no dword fistp (calls=4) | `QIfist_long_cast`: fistp qword x2; `QIfist_temp_local`: fistp qword x2 | cvtss2si x2 / cvttss2si x2 | direct operand, graded RELOCATION_MATCH | RELOCATION_MATCH (src/) |

## Header overlay (hdr/engine/, all behind FABLE_RETAIL_<CLASS>; `hdrcheck2.py $D/hdr`: default=same for all three)
- `C4DVector.h` (FABLE_RETAIL_C4DVECTOR): FableWin `operator-` / `operator+` (const, return const C4DVector) and
  `__forceinline const C4DVector Interpolate(const C4DVector&, float) const` = `(v - *this) * t + *this` (row 0x00AA3C50; plain
  inline is emitted out of line there).
- `CANavQuadTreeNode.h` (FABLE_RETAIL_CANAVQUADTREENODE): `friend class CNavQuadTree;` -- retail CNavQuadTree::GetNodeAt calls the
  protected slot 11 GetLeafNodeAt (parked row 0x00A770B0; no codegen change).
- `CNavQuadTree.h` (FABLE_RETAIL_CNAVQUADTREE): declares `GetPreferabilityCostForNode(const CANavQuadTreeNode*) const` (0x00A76E00,
  which now uses the shared retail model instead of a local packed class).
- SIBLINGS.tsv: the 38 landed sources that include these headers through a retail opt-in (additions only: members/friend/a
  declaration). vland was run read-only with `--recheck-landed` after the positional args; it printed no sibling
  lines in that mode, so the siblings are listed for the coordinator's landing recheck.

Header debt (vland WARN SHARED_HEADER_AVAILABLE, local models kept because the shared header is the generated flattened/opaque
layout or lacks the members): CThingMapManager (0x006378E0), CEngineSubPrimitiveRenderer (0x00B84580), CThing / CTCVillage /
CPlayer / CPlayerGui (0x004385F0, same model as the landed UpdateMorality 0x00439F20).

## Name fixes (NAME_FIXES.tsv)
UpdateStat 0x00437AE0 is `ABE...AAVCStatBase@NPlayerGui@@` (landed UpdateMorality binds a `QAE ... PAVCStatBar` spelling);
0x00487B70 is `?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ` (landed callers say GetPlayerCharacter); 0x00643FF6 SetFine;
0x0065A28F GetNormalisedTimeLeftOnFines; 0x00AA3C50 is C3DKeyframe::BlendWith (not CRightHandedSet::Interpolate); 0x00A4C1F0's
PDB spelling takes `C3DKeyframe&, unsigned long` (landed 0x00A4DEA0 declares `PAV, long`); 0x00A567A0 returns through a
`C3DVector&`; plus the pass-1 fixes (GetThingMapwhoLevel IBE, GFLimit template, GetOneOverTickRate, AddSubPrimitive,
ResizeLevels, BuildBoxPositionMatrix AAE, Get2DNoise). Globals with competing spellings seen: 0x013B86A0
CMainGameComponent::PMainGameComponent; 0x0137DAD8 (three spellings); 0x01436EA0 (g_pEngineCamera vs g_pCamera_01436ea0).

## GOTCHAS one-liners
- A float local whose initialiser divides by a named `const float` (`const float cellSize = 32.0f; static float inv = 1.0f / cellSize;`) gives VC7.1's guarded dynamic init with the folded constant (0x00A770B0); `1.0f / 32.0f` is static-initialised.
- `long` vs `unsigned short` parameter/local types move which dead argument slot an inlined helper's by-value copy is packed into (0x009FCA40: `long height` put the GFFastFloatToLong copy in the texture slot like retail).
- Declaring the slerp cosine before the `sign` weight local makes VC7.1 store and reload it (`fstp ; fld`) as retail; declared after, it keeps it with `fst` (0x00AA3C50).
- A by-value C4DVector `Interpolate` written as `(v - *this) * t + *this` reproduces retail's three frame temporaries, but only as `__forceinline` (plain inline is emitted out of line).
- `GetCrime().SetX(call())` (inline reference accessor) loads the bar into a callee-saved register before the call where `PCrime->SetX(call())` loads it after (0x004385F0); the Ego_r `UpdateStat(..., CStatBase&) const` signature also fixed the register rotation.
- A rounding whose result is dead survives in retail only because it is asm (0x004386B4, 0x00A4D770): any C++ spelling deletes it.
- A same-TU noinline body of the out-of-line callee (`ScaleParticleDimensions`) removes the extra frame word (0x00B05C60), as for the landed CPSCRenderSprite twin.
- Rows 0x0057C4F1 / 0x00646762 materialise the Ceil helper's 1.0f with `fld1 ; fstp`, which engine/GFFloatToLongCeil.h (`mov [m],3F800000h`) does not give: those need a helper variant.

## Housekeeping
- Stray files outside $D from an unset `$S` in an earlier compile (the safety check refused `rm`): `/t_00a76e00`, `/t_00b18820`,
  `/t_00b84580` -- please delete. (Later stray test binaries in `/w22/tt_*` were removed again; `/w22/agent724` is not mine.)
