# Hand chunk 315 (CCreatureActionBase .. CEngineCamera): 12 of 40 byte-exact

## What matched (12)
All 12 are RELOCATION_MATCH under RTM cl 13.10.3077. Each behaviour test passes under g++ (`tall.sh`) and VC7.1 (`tall.py`).

- 00d2c810 CEndGameThunder::Main
- 00d79d50 CEndScythe::Main (catalogued as CEndBriarRose)
- 00d7a360 CEndBriarRose::Main
- 00e02e50 CDarkwoodAssassinSpawn::Main
- 00e4e0e0 bandit-toll placement helper "HandleBanditToll" (the catalog class is wrong; see Label corrections)
- 00e6ea30 CDF_ShineLight::Main
- 00e6f1c0 CDF_ThunderHelmet::Main
- 00e6f710 CDF_CombatMult::Main (catalogued as CDF_MarryLadyGrey)
- 00e71450 CDF_EatPies::Main (catalogued as CDF_MarryLadyGrey)
- 00e71980 CDF_GiveGift::Main
- 00e73410 CDF_MarryLadyGrey::Main
- 00ee7a20 CDemonDoorFace::Main

## Parked for Fable 5.1 (28)
Near misses (real reconstructions):
- 00846dd0 CCreatureAction_PerformExpression::FinishAction DIFFER(1685v1685): register name only. In the !ReplacedAction emote/sound block, &PCreature is in ebp in retail and ebx in ours (7 instructions). | tried: QFE, PCreature.GetPItem() vs the Creature() accessor, a CIntelligentPointer& alias, a __forceinline helper block, `== false`, a scoped alias (the last two moved nothing; the if-swap and dropping the frame local made it worse). | next: register priority (weighted-use rank). Give the value retail keeps in ebx more weighted real uses. Routed to the R&D register round.
- 00847520 CCreatureAction_PerformExpressionExtended::FinishAction DIFFER(1122v1122): spill choice. Retail spills the CTCInventory* and keeps the GamePlayerInterface in ebx/ebp; ours spills the interface. | tried: named gpi/player locals, a shared button/gpi local, a wider inventory scope, ctor arguments written inline. | next: register priority (R&D round).
- 00675e20 CCreatureDef::Transfer DIFFER(1912v1912): register rotation only. Retail uses eax for every scratch load in the four counted-pointer transfers. | tried: traits vs direct overloads, operator* vs .Data, case order, a helper, QFE. | next: lane gap: register priority.
- 00844bc0 CCreatureAction_GiveItemToThing::PerformAction DIFFER(1140v1144): register allocation. Retail spills the deed-log component (frame 0x18); ours uses frame 0x14. | tried: inline GetTC, named thing locals, a giveItem restructure, the deed-log declaration site. | next: weighted-use rank of the three member addresses.
- 00ea8130 CEmptyGrave::Main DIFFER(1185v1191): register allocation of the new-expression flag (ebx) and frame 0x5c vs 0x58. | tried: see differ header. | next: lane gap: register priority.
- 00e70460 CDF_BeEvil::Main DIFFER(1651v1665): VC7.1 cross-jumps both arms' after-wait terminate checks into one copy; retail keeps one per arm. | tried: separate exits, inline_depth placement. | next: lane gap: shared terminate block placement.
- 00e73930 CDF_SexMad::Main DIFFER(1755v1725): the lane keeps two extra pause(false) exit copies. | tried: exit forms. | next: lane gap: shared terminate block placement.
- 00846710 CCreatureAction_PerformExpression::Initialise DIFFER(1170v1192): frame-slot layout (retail has 0x10 bytes of locals, ours 0xc), the hidden-return push order of the SpriteOverrideText wide string, and scheduling in the sound block. | tried: scoped AddTC names, an unsigned HasTC mask, CDefPointer operator->, a def local for the share (all of these fixed something). | next: the named CCharString/CWideString temporaries for the sprite text; the &PCreature spill slot.
- 0064b1ac NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities DIFFER(1058v1045): retail keeps the component out-pointers and the presence-bits pointer in the frame and the hero in ebx; ours enregisters the weapons component. | tried: the landed /Os DrawAbilityList TU model, a present-bits alias (folded away). | next: weighted-use rank / the GetInterface variant with the `jg` pick (0x46/0x48 lookups use `cmp;jg;mov esi,eax`).

x87-heavy rows over 1 KB (decoded, skeleton graded once):
- 008c6db0 CCreatureAction_IceTrollIcicleAttack::PerformAction DIFFER(1v1090): 75 x87 ops plus ftol | tried: skeleton | next: lane gap: x87 scheduling
- 0087d590 CCreatureAction_InterruptableMidAttackAutoTurn::Initialise DIFFER(1v1215): 89 x87 ops | tried: skeleton | next: lane gap: x87
- 00b30b50 CEngineCamera::SetCameraState DIFFER(3v1499): 353 x87 ops, 2 calls | tried: skeleton | next: lane gap: x87
- 00649952 CDrawBettingHUD::Draw DIFFER(3v1337): 102 x87 ops, /Os | tried: skeleton | next: lane gap: x87
- 0064822a CDrawCombatMultiplier::Draw DIFFER(3v1526): 140 x87 ops | tried: skeleton | next: lane gap: x87
- 00647ab4 CDrawHealth::Draw DIFFER(3v1910): 191 x87 ops | tried: skeleton | next: lane gap: x87
- 00647575 CDrawProjectileTargetingCentre::Draw DIFFER(3v1343): 100 x87 ops plus ftol | tried: skeleton | next: lane gap: x87
- 0064e2cc CDrawSpiritsHealth::Draw DIFFER(3v1395): 133 x87 ops | tried: skeleton | next: lane gap: x87
- 0064ccb3 CDrawStamina::Draw DIFFER(3v1526): 165 x87 ops | tried: skeleton | next: lane gap: x87

Not reached past decode (large multi-feature rows; skeleton graded once, decode noted in each differ header):
- 00694890 CCreatureActionBase::UpdateActionAnimation DIFFER(1v1140): aligned-stack frame, x87 timing, anim-entry pointers | next: full attempt
- 008bc1a0 CCreatureAction_CastWillRaiseDeadSpell::PerformAction DIFFER(1v1083): random spawn ring, undead servant creation | next: full attempt
- 008d5aa0 CCreatureAction_FireMissileWeapon::PerformAction DIFFER(1v1126): multi-arrow shot definition, by-value counted pointers, a local vector | next: full attempt
- 00848020 CCreatureAction_UseWallMount::PerformAction DIFFER(1v1139): conditional def-pointer temporaries (flag bits), a bitset::test throw, inventory-room message | next: full attempt
- 0087cfe0 CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation DIFFER(3v1447): three local vectors with inline push_back, sort/unique/erase | next: full attempt
- 009ae7c0 CDefinitionManager::CompileDefinition DIFFER(3v1456): 81 calls, inline map/vector | next: full attempt
- 009afb90 CDefinitionManager::LoadBinaryDefinitions DIFFER(3v1344): six inline stream Reads (use the landed 0x00757990 template), map lookups, inline push_backs | next: full attempt
- 0064a9c8 CDrawGuildSeal::DisplayNextMessage DIFFER(1v1182): /Os message-queue pop and an x87 alpha tail | next: full attempt
- 0064b8b3 CDrawTargetInfo::Draw DIFFER(3v1426): /Os target panel, demon-door face name compares | next: full attempt
- 00b277a0 CEngine::LoadNewScene DIFFER(3v1446): aligned stack, 0x3f4 locals, rep movs copies | next: full attempt

## Skipped
None. Every row was graded at least once.

## Label corrections
- 0x00E4E0E0: autoname-t5 "CCreatureAction_TrollWhackGroundBase::HandleBanditToll" is wrong. `this` is an entity script (Game +4, Me +8), called from the entity Main at 0x00E4B5B0 (vtable entry 0x012E4D8C, beside CM_TollBandit / CM_TauntingBandit). It places the toll-bandit blockade.
- 0x00E6F710 is CDF_CombatMult::Main and 0x00E71450 is CDF_EatPies::Main; both are catalogued as CDF_MarryLadyGrey.
- 0x00D79D50 is CEndScythe::Main; it is catalogued as CEndBriarRose.
- 0x00415D70 is CDefString::operator CCharString.
- 0x0040F8A0 is the unsigned-long traits transfer, not float.
- 0x006925C0 (labelled DrawGetEnvironment) is a no-argument CCreatureActionBase member that returns a long. It feeds the PerformExpression constructors, which take 8 arguments, and the CEventExpressionPerformedTo constructor.
- 0x00843310 / 0x0084ADC0 are the constructor and destructor of the replacement expression action built by both PerformExpression FinishActions (most likely CCreatureAction_PerformExpressionExtended).
