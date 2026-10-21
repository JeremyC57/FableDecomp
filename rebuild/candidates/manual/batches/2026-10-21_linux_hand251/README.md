# Hand chunk 251 (CTCContainerRewardHero .. CTCHeroOnlineScoreboard): 33 of 40 byte-exact

All matched rows grade RELOCATION_MATCH under RTM cl 13.10.3077 (lane default); tests PASS under g++ and VC7.1.
Special flags: `// Flags: /EHsc` on 00c14e40 (EH-frame ctor); `// Flags: /GS` on 0077fb00 and 00779080 (security cookie 0x0139C8A8 checked by __security_check_cookie 0x00BFE9F9).

## What matched (33)
- 0074e6e0 CTCContainerRewardHero::ReleaseItems (374v374): C3DVector member-wise copy (X,Y,Z+0.1); index declared first; for_each modelled __fastcall with hidden-pointer functor return
- 006702e0 CTCCoopSpirit::UpdateAppliedMovement (400v400): user copy ctor + inline Zero() (X=Y=Z=0); fast Normalise sum written (x*x + y*y) + z*z so only the squares CSE with GetLength
- 00729200 CTCCreatureExpression::Update (344v344): named bool removal test; CAnimFrameInfo(1,1) temporary assigned in the loop; inline CWorld::IsInMovieSequence wrapper
- 007dce40 CTCCreatureFlyOnSpline::DrawPreparePrimitivesForRendering (327v327): if(size) { colour; do-while(++i != size) } with i declared first; id named local; handle as *(begin()+i)
- 0074baa0 CTCCreatureGenerator::UpdateCreatureTrigger (384v384): C3DVector user copy ctor (dead Z copy dropped, 12-byte slots); separate spaceTrigger/timeTrigger bools both true, timeTrigger cleared by if
- 00812ca0 CTCCreatureGroup::GetNearestOtherActiveMember (384v384): inlined STLport _M_increment map walk; best distance in the dead arg slot; squared distance vs squared best kept as retail
- 007849c0 CTCCreatureGroupBoastingCrowd::OnHeroAttackCrowdMember (352v352): delay and string each in a block (two shared stack slots); manager locals; deed log as T* const& (const& idiom)
- 006cc6b0 CTCCreatureNavigation::OnCreate (321v321): per-case creature/manager locals; by-value push_back + overflow stand-in; const vector& types binding keeps the def release null test
- 006e0730 CTCDParticleEmitter::OnSerialise (332v332): database getter result in a named local before the argument pushes; EmptyCCharString no-NRVO default
- 00779080 CTCDRegionExit::MoveHeroThroughExit (359v359): /GS; CGameEvent model; heading via an inline CThing::GetAngleXY() wrapper (direct vcall reshuffles registers)
- 00779430 CTCDRegionExit::OnSerialise (351v351): first try on the landed 0x77B0D0 UID-remap model; engine/CTCBase.h base
- 007f1930 CTCDRumble::OnInitialActivate (391v391): vfunc_6 is OnInitialActivate; joystick device as a named value local read before the CInitRumble is built (const& binding keeps an address and misses)
- 00c14e40 CTCDiceGame::CTCDiceGame (336v336): /EHsc; actually the CTCDiceGame ctor (label Initialize wrong); empty polymorphic base, parent as first derived member
- 0072f170 CTCDummyVillager::SetupFromData (384v384): same-TU noinline stand-in bodies for GetDef/GetSubDef keep the creature def in ebx; UIDs as two words (int64 pads the vptr)
- 0072f9e0 CTCDummyVillager::CanBeRecreated (400v400): GetPItem() null test then a named GetPItem() local for HasTC+GetTC; Max(Day, today) by reference, identical in both arms (tail-merge)
- 006d5260 CTCFollowed::CullNullFollowers (340v340): first try: local intelligent-pointer copy + IS_THING_ALIVE; inline list erase destroys through the element pointer (virtual dtor) + free
- 006d56e0 CTCFollowed::QuitAIFollower (360v360): intelligent-pointer temporaries for the two list removes; unchecked out-param GetInterface; IsAlive(GetPItem()) single-fetch helper
- 004bde90 CTCGraphicAppearance::GetNoHelperPoints (356v356): real name GetNoHelperPoints (PDB order); landed 0x4BDA30 model
- 004be000 CTCGraphicAppearance::GetNoDummyObjects (356v356): GetNoDummyObjects; twin of 0x4BDE90
- 004be170 CTCGraphicAppearance::HasHelperPoint (336v336): HasDummyObject 0x4BE2C0 model + null-tested mesh info
- 007066a0 CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea (352v352): stand-in GetDef/GetSubDef bodies; both def pointers at the loop top (failed GetDef skips the sub release); push_back reads the old tail once
- 00706f40 CTCHeroAttachableAppearanceModifiers::OnSerialise (400v400): vfunc_1 is OnSerialise; inline IsSaving() keeps the save path first; name built into a named local before the element assignment
- 0077f7e0 CTCHeroCentreDoorMarker::UpdateParticleTeleporter (395v395): twin of 0x77F970 (+0xd7, MARKTELEPORTER); renamed UpdateParticleTeleporter
- 0077f970 CTCHeroCentreDoorMarker::UpdateParticleExperience (395v395): IS_THING_ALIVE arms; implicit CCharString conversions
- 0077fb00 CTCHeroCentreDoorMarker::FrameUpdate (321v321): /GS; CGameEvent model from 0x77D4B0; sum < R*R
- 006d7170 CTCHeroExperience::CTCHeroExperience (344v344): STLport _Vector_base(n)+fill_n (no placement new), _List_base local node, by-value def getter
- 006d8170 CTCHeroExperience::UpdateAbilitiesForStat (383v383): const out-param GetInterface (eax) + LowerBound stand-ins (both overloads)
- 006d8580 CTCHeroExperience::FrameUpdate (330v330): vfunc_9 = FrameUpdate; ClampToZero(float&) reload; const overload order
- 0071d7c0 CTCHeroMorph::AddTattoo (372v372): AddTattoo(const CThing&): forceinline clear/push_back(temp copy); GetSubDef + _M_insert_overflow stand-ins
- 0071da70 CTCHeroMorph::AddTattoo (340v340): AddTattoo(long): GetDef/GetSubDef/push_back/clear/erase stand-ins
- 0071dbd0 CTCHeroMorph::AddCompositeTexture (384v384): first try: colours in dead arg slots, inline push_back + Dirty, CCompositeBlendDesc copy-ctor same-TU body, free() for local vector
- 0055ef00 CTCHeroOnlineScoreboard::DisplayErrorMessage (385v385): actually DisplayErrorMessage(const CWideString&) (label OnOkClicked wrong); OK button as a reference (no null test on CObserver conversion); inline CCountedPointer dtor
- 00561450 CTCHeroOnlineScoreboard::UploadPlayerScores (363v363): force-inlined helper takes the by-value score string by const& (returned pointer read, temp outlives helper locals); keys as named locals; allocator user ctor; strlen intrinsic

## Parked for Fable 5.1 (7)
- 007de030 CTCCreatureFlyInCircles::FrameUpdate DIFFER(382v382): scheduling only: two stores before SetRHSet swapped (Up.X fstp vs Forward.Y copy); ~40 probes over cross/set/copy-ctor spellings; const& idiom tried (up/side/forward bindings) | tried: ~40 cross/set/copy-ctor spellings, const& up/side/forward bindings | next: x87 store scheduling; probably a compiler-build artefact, compare with the 007dccb0 twin if one ever lands
- 007dccb0 CTCCreatureFlyOnSpline::FrameUpdate DIFFER(390v390): scheduling only: CRightHandedSet build tail (same gap as 007de030); 288-combination search + QFE tried | tried: 288-combination search over copy-ctor/set-ctor/cross spellings, QFE | next: same as 007de030
- 0073acc0 CTCCreatureModeManager::AddMode DIFFER(331v331): scheduling only: early return false xor al,al before the pops in retail, among the pops in ours (find/return shapes, dtor spellings, QFE all tried); const& idiom tried (head/type bindings) | tried: find/return shapes, dtor spellings, const& head/type bindings, QFE | next: tail-merge of the false return; try a goto-style shared exit or a different list find helper
- 006cc800 CTCCreatureNavigation::FrameUpdate DIFFER(358v399): inline-asm fistp rounding helper (0.5/1.0 frame locals, ebp frame): asm_bake family, not reproducible in pure C++ | tried: pure C++ RoundToInt as (long)(f + 0.5f) through __ftol2 (the rest of the body lines up; the 41-byte gap is the inline-asm helper) | next: asm_bake only (an inline-asm fistp helper); keep as an oracle, or land via the de-bake lane if it ever admits a known helper
- 0071ad20 CTCEmoteIcon::DrawEmoteTypeIcon DIFFER(365v365): register choice only: temporaries rotated (edx/eax/ecx vs ecx/edx/eax) from the draw-distance copy to the sprite args; getters x16, range helper, const&, QFE tried | tried: inline getters x16, expression order, range helper, const&/scoped locals, QFE | next: register rotation; try reordering the draw-distance copy into its own helper
- 006d4e60 CTCFollowed::IsCreatureValidFollower DIFFER(363v363): register choice only: component-key address temps eax/ecx/edx vs ecx/eax/ecx, RenownLevel via eax vs edx; const& idiom tried (renown/stats bindings) | tried: const& bindings of RenownLevel/stats, named locals, key-temp spellings | next: compiler-build register allocation; try a QFE/other build of VC7.1 or accept
- 007b2fd0 CTCGuard::WitnessedCrime DIFFER(333v329): flag shape only: severe in bl needs the assignment form (adds an else arm); the bare if-set puts it in memory; ~30 probes (scoping, types, helpers, == true) | tried: ~30 probes: if-set vs assignment, scoping, bool types, helpers, == true | next: find a spelling where the flag lives in bl with no else arm (maybe a ternary on a later read)

## Skipped
- none (no row covers two functions).

## Label corrections
- 0x0055EF00 labelled CTCHeroOnlineScoreboard::OnOkClicked is CTCHeroOnlineScoreboard::DisplayErrorMessage(const CWideString&) (PDB method list; one by-ref arg, `ret 4`; the real OnOkClicked is 0x0055EE20).
- 0x00C14E40 labelled CTCDiceGame::Initialize is a constructor (vptr 0x012B29F8 store, EH frame); class attribution unverified (PDB CTCDiceGame is 24 bytes).
- 0x00706F40 CTCHeroAttachableAppearanceModifiers::vfunc_1 is OnSerialise.
- 0x007F1930 CTCDRumble::vfunc_6 is OnInitialActivate.
- 0x006D8580 CTCHeroExperience::vfunc_9 is FrameUpdate.
- 0x004BDE90 labelled GetNoDummyObjects is GetNoHelperPoints.
- 0x0077F7E0 labelled UpdateParticleExperience is UpdateParticleTeleporter.
- Callees: 0x0049B8D0 is a shared COMDAT Transfer (here vector<CCharString>; manifest TransferSignedChar); 0x006BF890 is CTCBuyableHouse::IsInitiallyEmpty (byte twin labelled CanBeHitByMeleeStrike); 0x0071D7C0/0x0071DA70 are AddTattoo overloads; 0x0065DE48/0x0065DE8A are list::remove; 0x00686AE0 is GetThingManager; 0x006869D0 is GetDefinitions; 0x004378C0 is CPlayerGui::Refresh; 0x006D5050 is RemoveEmoteIcon; 0x0070D900 is RemoveAllAnimsWithTypeFlags; 0x0070C170 is GetCombinationAnimFromTypeAndIndex; 0x0074E140 is GeneratePopoutVectorFromVars; 0x0074E560 is CreateNewObject; 0x0074FB20 is a vector erase (not CopyAssignSegment); 0x0074FBB0 is a for_each with a functor; 0x00415FD3 is DivideByGameSpeed.
- Scoreboard callees (PDB method list): 0x0055F480 CreateStringFromMyScores, 0x0055F710 ResetAndOpenHTTPAndPrepareRequest (manifest LHNetworkLib_RequestLoginWithCredentials), 0x00560BB0 GetDecryptedDocument (manifest LHNetworkLib_DecryptResponseDocument), 0x00566EA0 operator==(const std::string&, const char*), 0x00569940 ~vector<std::string>.
- Review: landed OnOkClicked 0x0055EE20 names scoreboard +0x60 PScoreboardScreen, while the landed ctor 0x00562EC0 / ResetScoreboard 0x00562E70 call it PConnectingMessage (0055ef00 follows the ctor); the 1.0f pool at 0x01237F00; 0x00686AE0 +0x34 is a navigator manager.
