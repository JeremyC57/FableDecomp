# Hand chunk 278 (CAIStateGroup_GoToBed .. CAIStateGroup_MazeTutorialBattle): 11 of 40 byte-exact

## What matched

- 0092d7c0 CAIStateGroup_GoToBed::Update RELOCATION_MATCH(604v604): bool arrived declared before failed
- 00948060 CAIStateGroup_GoToPub::InitCreaturePosition RELOCATION_MATCH(571v571): uninit out-param scratchpad + named item operator=
- 0094a840 CAIStateGroup_GoToPub::PayMoney RELOCATION_MATCH(637v637): named barman/creature/zero-pos locals ahead of a named world/manager local per message call
- 0091e050 CAIStateGroup_HoverAttack::Update RELOCATION_MATCH(534v534): creature named before a named CWorld* local orders the GetPWorld call
- 0095b020 CAIStateGroup_HoverHornetQueen::GoToSpawnPoint RELOCATION_MATCH(599v599): /GS; search rectangle as a function-scope local written member by member keeps its own frame words and the x-60-first schedule; dead CWorld seed-getter call
- 0091d510 CAIStateGroup_HoverMovementBase::AIAction_MoveToPosProximityAndFacePos RELOCATION_MATCH(724v724): sibling of landed 0x0097E890 model; length-squared written Y*Y + Z*Z + X*X for retail Z,Y,X load order
- 009621f0 CAIStateGroup_IceTrollIcicleAttack::Update RELOCATION_MATCH(733v733): bool-has HasTC locals; named stealth GetTC; boast result compared to 1 keeps the edi copy
- 009624d0 CAIStateGroup_IceTrollPoundGroundAttack::Update RELOCATION_MATCH(738v738): frame target copy (copy ctor); full 3D difference then Z=0 fixes dot operand order
- 00951b20 CAIStateGroup_JackDragon::FlyOutOfGround RELOCATION_MATCH(562v562): named bool for the or-terminate condition puts the Terminate arm first; dead GetPosition call; packed JackDragon
- 0044bc10 CAIStateGroup_JackOfBladesBase::(catalog) = CPlayer::CPlayer RELOCATION_MATCH(533v533): CPlayer::CPlayer (label wrong); list sentinel via local node
- 00946850 CAIStateGroup_LightHomeCandle::Update RELOCATION_MATCH(653v653): HasTC result in the arrival bool; frame C3DVector copy ctor; named null pointer for operator=

## Parked for Fable 5.1 (29)

- 0092cb30 CAIStateGroup_GoToBed::IsHeroTrespassingPerceived DIFFER(610v610): component ptr eax/ecx + byte cl/al tie-break in TC-flag tests | tried: 5 tries | next: advisor pass on register tie-break
- 00949580 CAIStateGroup_GoToPub::WalktoSeatAndSit DIFFER(805v808): HasTC bit-24 byte-load narrowing and look-at vector slot scattering (+4 frame) | tried: 6 tries | next: HasTC narrowing variants
- 0094a3b0 CAIStateGroup_GoToPub::ChooseAction DIFFER(813v785): shared Drink/SitIdle tail blocks not merged; 0xcf key frame slot (+4 frame) | tried: see differ/ grade line | next: tail-merge of the Drink/SitIdle exits; key slot
- 00977e00 CAIStateGroup_Guildmaster::Update DIFFER(729v732): def pointer zero-after-push + ebp cache not reproduced; key/boss frame words swapped (frame +0x10) | tried: see differ/ grade line | next: GetSubDef stand-in + zero-after-push form
- 0091ddd0 CAIStateGroup_HoverAllAroundTest::Update DIFFER(509v626): x87 rotation-matrix schedule and 8-aligned frame not modelled | tried: 1 try | next: model rotation matrix x87 order
- 0091e270 CAIStateGroup_HoverAttack::IsEnemyRunningAway DIFFER(499v513): distSq not spilled before range call; Normalise r kept in st; this edi vs ebx | tried: see differ/ grade line | next: apply the const float& operator*= FastInvSqrt form for the r spill
- 0095a490 CAIStateGroup_HoverHornetQueen::IsRouteToPosBlockedByTarget DIFFER(558v560): second inline Normalise: retail fstp r/fxch/fmul [r] vs ours fst r/fmul st2 (x87 stack) | tried: see differ/ grade line | next: apply the const float& operator*= FastInvSqrt form (fixed the same spill in 0095bcf0)
- 0095ad20 CAIStateGroup_HoverHornetQueen::GotoSafestCirclePoint DIFFER(754v754): fast Normalise matched via const float& operator*=; ecx/edx register choice in 2nd/3rd normalise left | tried: 8 tries | next: register choice; advisor queued
- 0095b9e0 CAIStateGroup_HoverHornetQueen::DiveBomb DIFFER(782v782): one int/x87 interleave of the diveTo.Z frame copy left | tried: 13 tries | next: one interleave; advisor queued
- 0095bcf0 CAIStateGroup_HoverHornetQueen::Attack DIFFER(948v948): one mov ecx,edi scheduled 4 x87 ops later than retail | tried: 15 tries | next: one scheduling slot; advisor queued
- 0095c1f0 CAIStateGroup_HoverHornetQueen::Update DIFFER(572v595): code+jump table RELOCATION_MATCH at 0x23C; row over-captures two stubs (needs split + jumptable lander) | tried: see differ/ grade line | next: split catalog row, land with jump-table lander
- 00959f80 CAIStateGroup_HoverKeepInArea::Update DIFFER(716v752): first pass: rotation-matrix x87 schedule (HoverWander family) not modelled | tried: 1 try | next: model rotation x87 order (HoverWander family)
- 00959bf0 CAIStateGroup_HoverWander::Update DIFFER(626v739): first pass: rotation-matrix x87 schedule and unsigned fild fix-ups not modelled | tried: 1 try | next: model rotation x87 order
- 00959800 CAIStateGroup_HoverWanderAroundThing::IsRunnable DIFFER(625v595): frame order of the two vectors and the rough area; retail reuses the live vector slot for the pointer temp | tried: 6 tries | next: type-class frame packing
- 00950cf0 CAIStateGroup_JackDragon::UpdateStrafe DIFFER(896v886): first approximation: marker register pair, heading fst/fstp, +4 frame | tried: 2 tries | next: second pass
- 00951070 CAIStateGroup_JackDragon::IntoGround DIFFER(944v953): success-tail placement after the terminate return, +12 frame, physics-body vcall register | tried: 4 tries | next: tail placement via per-arm exits
- 00951d60 CAIStateGroup_JackDragon::IntoHover DIFFER(601v588): zero register (ebx) and def-in-ebp allocation not reproduced; frame +0x10 | tried: 4 tries | next: GetSubDef noinline stand-in (fixed the def/ebx shape in 0095bcf0)
- 009735d0 CAIStateGroup_JackOfBladesBase::RushToPos DIFFER(670v690): x87 spill pattern of the inline normalise + distance (squared terms in frame words) | tried: 3 tries | next: const float& operator*= normalise form
- 00973dc0 CAIStateGroup_JackOfBladesBase::SummonPillars DIFFER(834v833): Pillars[i] add vs lea, squared-length add order, SetPosition vcall register | tried: 6 tries | next: lea form of Pillars[i], length order, vcall accessor
- 00972990 CAIStateGroup_JackOfBladesStage1::OnActivate DIFFER(792v803): frame-slot packing of keys/filters/areas; code shape otherwise aligned | tried: 3 tries | next: type-class frame packing
- 00972cc0 CAIStateGroup_JackOfBladesStage1::MeleeCombat DIFFER(601v612): vptr constant held in ebp and +4 frame not reproduced; this/zero register swap | tried: 3 tries | next: vptr constant local
- 00974770 CAIStateGroup_JackOfBladesStage1::PowerUpSword DIFFER(817v807): frame-slot packing of the sword-block locals (+8 bytes); body otherwise aligned | tried: see differ/ grade line | next: type-class frame packing (advisor finding) on the sword-block locals
- 00915350 CAIStateGroup_KidHurtSelfRunToMother::Update DIFFER(867v867): frame packing of the present/arrived byte vs the interface key word; one mov ecx,esi scheduling | tried: 11 tries; advisor requested | next: advisor AdvB3 queued
- 00941690 CAIStateGroup_KidPlayHopscotch::Update DIFFER(550v609): retail keeps a dead HasTC byte store + lower_bound loop (result unused); ours folds it away | tried: 2 tries | next: keep dead HasTC/lower_bound via out-of-line stand-in
- 00941900 CAIStateGroup_KidPlayHopscotch::IsRunnable DIFFER(529v529): scheduling only: int copy of cross-product temp interleaved with fld/fmul (2 spots) | tried: see differ/ grade line | next: lane gap candidate (int/x87 interleave)
- 00941fc0 CAIStateGroup_KidReactToBlockage::IsRunnable DIFFER(578v578): one register choice left (add edi,eax vs add eax,edi on the box corner) after advisor AdvB3 | tried: 8 own tries + 64 advisor grades | next: lane gap candidate
- 009552c0 CAIStateGroup_MazeShieldedChase::Update DIFFER(862v861): hand-released def pointer frame word reused for the proximity float; move-result jump into the shared check | tried: 4 tries | next: def lifetime split
- 00954f40 CAIStateGroup_MazeTeleport::Update DIFFER(604v604): /GS; code identical, frame slot order of position/found/filter/name only | tried: VC7.1 packs by type class; 12 own + 16 advisor tries | next: lane gap candidate (frame order)
- 00955c90 CAIStateGroup_MazeTutorialBattle::Update DIFFER(916v970): counted-def release inlining, by-value intelligent pointer built in the argument slot, defender-zone switch shape | tried: 3 tries | next: second pass on def release + zone switch

## Skipped

None: every row was attempted at least once.

## Label corrections

- 0x0044BC10 catalogued as CAIStateGroup_JackOfBladesBase ctor: it is CPlayer::CPlayer (landed with Leaf ??0CPlayer).
- 0x0049D810 labelled CVertexBufferWin32::DoSizeof: it is the CWorld random-seed getter (returns unsigned long&).
- 0x0062E900 labelled ~CTCLightningOrb: it is the combat/creature action destructor (~CCombatActionBase) used by every action temporary here.
- 0x008FCFB0 labelled `_Cons_val`: it is CAIStateGroupBase::GetDefinitionManager.
- 0x007355F0 labelled CCreatureAction_InitializeForCombat: it is a CTCCreatureCombat method taking a counted def.
- 0x00A14040 labelled Math_CartesianToSpherical: it is C3DVector::RotateAboutZ.
- 0x00718A00 labelled CCountedPointer<CFileInstaller::CFileInfo const> ctor: propagated name; it is the mode counted-pointer ctor.
- 0x00693EF0 labelled ~CPhysicsMeshInfo: it is ~CCreatureActionBase.
- 0x008C5410 labelled CCreatureAction_BriarRoseShotFarInto ctor: DiveBomb calls it as a 2-arg (creature, target) hornet-queen dive action ctor; the label is propagated and unverified.
- 0x008C33E0 / 0x0095C5C0 both labelled GetSubDef<CAICreatureWillPowerIndicatorDef>: the Jack (+0x4c pillar interval) and hornet-queen (+0x3c look time) callers read different def layouts, so at least one template argument is propagated.
