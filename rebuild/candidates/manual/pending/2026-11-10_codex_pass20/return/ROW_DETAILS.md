# Pass20 row results

See START_HERE_CLAUDE.md for extent/status interpretation. Sizes are object/function bytes; attempts are the recorded authoring count, not compiler invocations. Full chronology is in each PROBES.md; final evidence hashes and symbol are in FINAL_GRADE.json.

| Address | Identity | Result | Bytes | Attempts | Residue |
|---|---|---|---|---:|---|
| 00405220 | TransferIMEColour | MATCH | 290/290 | 2 | none |
| 004b2280 | CQuestManager::SetQuestAsFailed | MATCH | 527/527 | 1 | none |
| 004b2890 | CQuestManager::PostBoastTransfer | PARKED | 569/569 | 13 | 569/569,189/189 instructions;4hunks: quest/list arguments use ecx/eax instead of edx/ecx, interface key address uses edx instead of eax. All20external operands correct;141byte GetAcceptedBoastsForScript and72byte list clear helpers exact. |
| 0057dc22 | CTCHeroStats::GetCrimeReport | MATCH | 498/498 | 12 | none |
| 005f64dd | CTCInventoryAbilities::CycleAssignedAbilities | MATCH | 508/508 | 7 | none |
| 0062bef0 | CWorld::EAUseProjectileWeapon | MATCH | 483/483 | 2 | none |
| 00641ae0 | CTCLight::DrawFlare | MATCH | 331/331 | 1 | none |
| 0064619f | NPlayerGui::CCrimeStatBar::DrawStatBox | MATCH | 509/509 | 14 | none |
| 0065be6c | CTCVillage::UpdateHornTasks | MATCH | 336/336 | 15 | none |
| 0067e290 | CDegradableDef::Transfer | PARKED | 273/273 | 10 | 10 strict instruction hunks; enum EAX/EDX swap begins+5b, then bool temporary rotation and sort operand order; same273bytes/102instructions |
| 006d18a0 | CTCWife::Marry | MATCH | 1330/1330 | 3 | none within original1330-byte code; assigned1344 includes14 int3 alignment bytes |
| 0071ad20 | CTCEmoteIcon::DrawEmoteTypeIcon | MATCH | 365/365 | 2 | none |
| 00744fb0 | CTCPerceiveThing::UpdateHearing | MATCH | 355/355 | 7 | none |
| 007471f0 | CTCStealth::DrawPreparePrimitivesForRendering | MATCH | 308/308 | 14 | none |
| 00752f30 | CTCLook::UpdateHeadCreatureAnim | MATCH | 226/226 | 1 | none |
| 007677e0 | CTCObjectAugmentations::AttachAugmentationParticles | MATCH | 1467/1467 | 17 | none within1467-byte code; assigned1472 includes five int3 padding bytes |
| 0076efa0 | CTCWeaponTrail::UpdateWeaponTrails | MATCH | 1312/1312 | 12 | none within corrected function; assigned1888 includes24unrelated following helper entries/padding |
| 00785620 | CTCVolumeContainmentTracker::Update | MATCH | 1301/1301 | 6 | none within1301-byte code; assigned1312 includes11 int3 padding bytes |
| 0078a390 | CTCHeroRangedAttacks::FireRangedWeapon | MATCH | 563/563 | 4 | none |
| 0078faf0 | CTCDivineWrath::FinalCleanup | MATCH | 586/586 | 7 | none |
| 00794e60 | CTCUnholyPower::CancelForCutscene | MATCH | 552/552 | 1 | none |
| 007f5ae0 | CTCTargetingBase::UpdateNearestInterestingThing | MATCH | 544/544 | 6 | none |
| 007f6200 | CTCTargetingAI::UpdatePBestEnemyCreatureForAttack | PARKED | 1458/1458 | 14 | parent/owner spill14/18 inverted; comparator store/call scheduling, filter temporaries and later parent caching;36 strict hunks |
| 00813010 | CTCCreatureGroup::AddNewMember | MATCH | 238/238 | 1 | none |
| 00815420 | CTCHitLocations::GetBoundingSphere | MATCH | 498/498 | 7 | none |
| 00846bc0 | CCreatureAction_PerformExpression::FrameUpdate | MATCH | 527/527 | 3 | none |
| 0085cde0 | NThingMap::CThingMap::Validate | MATCH | 214/214 | 1 | none |
| 008bf800 | CCreatureAction_GenericLeaderCommand::Initialise | MATCH | 570/570 | 2 | none |
| 008daa80 | NCreatureMode::CModeThunderLightningStorm::CreateSkyBeam | MATCH | 551/551 | 1 | none |
| 00910df0 | CAIStateGroup_MinionGuardHeardYellForHelp::Update | MATCH | 500/500 | 1 | none |
| 00919240 | CAIStateGroup_LookAtInterestingThings::FindInterestingThing | PARKED | 508/508 | 17 | entry only: Creature load before filter179 initialization/vector zero stores; all subsequent instructions/operands and17branches exact |
| 00924b90 | anonymous::CCreatureAction_KrakenRoarLoop::FinishAction | MATCH | 219/219 | 2 | none; anonymous namespace TU hash compiler-specific and normalized only for symbol selection |
| 00934e50 | CMessageEventManager::FindMostRecentMessage | MATCH | 208/208 | 2 | none |
| 00956e70 | CAIStateGroup_BriarRose::Update | MATCH | 363/363 | 1 | none |
| 00a55f90 | CMatrix3x4::SetFromVectorAndAngle | PARKED | 259/259 | 10 | xy/xz commutative loads reversed; terminal three zero stores have different interleaving with final x87 subtract/store/pop |
| 00aa39a0 | C3DKeyframe::ToMatrix | MATCH | 332/332 | 1 | none |
| 00ab5030 | CMouseDX::ResetNonExclusiveMouse | MATCH | 342/342 | 1 | none |
| 00bcf4f0 | CEngineInternalPrimitive2DAnimatedMesh::Initialise | PARKED | 374/374 | 22 | one adjacent independent load/store pair swapped at tail: retail loads StartPosition.Z before EndPosition.Y store; ours stores Y before loading Z.101/101instructions,99.01%strict,2hunks. |
| 00c3d470 | CCodaContext::Process | MATCH | 408/408 | 2 | none |
| 00c3db80 | CCodaContext::Create3DSource | MATCH | 335/335 | 1 | none |
