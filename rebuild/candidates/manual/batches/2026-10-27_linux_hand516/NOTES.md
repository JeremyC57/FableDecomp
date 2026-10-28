# Hand chunk 516 (CGameScriptInterface .. CGameScriptThing): 39 of 40 byte-exact

All 39 are RELOCATION_MATCH (0 full MATCH, 0 retail_asm). Tests PASS under g++ and the lane VC7.1 cl.exe. vland dry run: WINS 39,
no RELOC_CONFLICT / RELOC_DUPLICATE / RELOC_IDENTITY; every symbol the sources define or call that already sits in
rebuild/oracles/reloc_bindings.tsv is bound to the same single address.

## What matched (39)
- CGameScriptInterface component rows (35, all on the FIRST grade): the 2026-10-27_linux_hand955 L083/L084 model verbatim --
  CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound 0x0040F020 with its Ego_r template spelling,
  CThing::GetInterface(T** out, type) as a __forceinline bool/out-pointer template, the component used through the out pointer.
  Guard forms: V = `if (thing.IsValid()) { if (thing.GetThing()->GetInterface(&tc, k)) ...; }`; VC = V plus `t->ThingType == 1 &&`
  before the lookup; N = `CThing* t = thing.GetThing(); if (t && t->GetInterface(...))`; ND = N plus `!t->Dead`; H = hero:
  `PlayerManager->GetMainPlayer().GetPControlledCreature()` then `hero && !hero->Dead && GetInterface`. The mask test shapes
  (`test ch,1`, `test ch,ch ; jns`, `test cl,cl ; jns`, `test ecx,ecx ; jns`, `test byte/dword`) all fall out of the one
  bitset expression; the `mov eax,[eax+4] ; push ; mov ecx,eax` component copy (the old hand44 "register-choice gap") comes
  from the out-pointer form under RTM 13.10.3077. Tail calls with a constant (`mov [esp+4],K ; jmp`) and the getters'
  shared `xor al,al` / `xor eax,eax` tail come for free. SetUseableByHero / SetOwnedByHero carry a real `if (bed)` null test.
  - 00899000 GiveHeroExperience (slot 128, form H, interface 0x68 CTCHeroExperience)
  - 0089b050 EntitySetWillBeUsingNarrator (slot 362, form VC, interface 0xF CTCTalk)
  - 0089b3f0 SetHeroAsWearing (slot 190, form H, interface 0x12 CTCInventoryClothing)
  - 0089c190 EntitySetInFaction (slot 601, form V, interface 0x49 CTCEnemy)
  - 0089c420 EntitySetPersonalityOverride (slot 619, form N, interface 0x4F CTCCreatureOpinionOfHero)
  - 0089c470 EntitySetPersonalityOverride (slot 618, form N, interface 0x4F CTCCreatureOpinionOfHero)
  - 0089c510 OpinionSourceSetAsAttentionGrabbing (slot 625, form N, interface 0x50 CTCOpinionSource)
  - 0089c850 RemoveOpinionDeedStillSearchingForWitnesses (slot 630, form N, interface 0x51 CTCHeroOpinionDeedLog)
  - 0089d910 EntityForceToLookAtNothing (slot 500, form V, interface 0x8 CTCTargetingBase)
  - 0089dbb0 EntitySetAsAbleToWalkThroughSolidObjects (slot 514, form V, interface 0x7 CTCCreatureNavigation)
  - 0089dcd0 EntitySetAllowBossPhaseChanges (slot 521, form V, interface 0xAD CTCBoss)
  - 0089dd90 EntitySetBossPhase (slot 523, form V, interface 0xAD CTCBoss)
  - 0089de50 EntityLeaveCombatStance (slot 528, form V, interface 0x30 CTCCombat)
  - 0089deb0 EntitySetAsDisplayingEmoteIcon (slot 530, form V, interface 0x45 CTCEmoteIcon)
  - 0089e130 EntitySetMaxNumberOfAttackers (slot 463, form ND, interface 0x36 CTCStealth)
  - 0089ee00 SetTeleporterAsActive (slot 563, form V, interface 0xDD CTCTeleporter)
  - 0089ee60 IsTeleporterActive (slot 564, form V, interface 0xDD CTCTeleporter)
  - 0089ef30 SetMoralityChangingAsEnabled (slot 573, form H, interface 0x4 CTCHeroStats)
  - 0089f1e0 IsDiggingSpotEnabled (slot 576, form V, interface 0xEF CTCDiggingSpot)
  - 0089f240 IsDiggingSpotHidden (slot 577, form V, interface 0xEF CTCDiggingSpot)
  - 0089f2a0 SetDiggingSpotAsHidden (slot 578, form V, interface 0xEF CTCDiggingSpot)
  - 008a01a0 EntityGetStockItemPrice (slot 486, form V, interface 0x54 CTCStockItem)
  - 008a0200 EntitySetAppearanceMorphSeed (slot 479, form V, interface 0x3 CTCRandomAppearanceMorph)
  - 008a0540 EntityClearRangedTarget (slot 468, form V, interface 0x8 CTCTargetingBase)
  - 008a0600 EntitySetTargetingType (slot 470, form V, interface 0x9 CTCTargeted)
  - 008a06c0 EntitySetLightAsOn (slot 533, form V, interface 0x27 CTCLight)
  - 008a07c0 EntityFadeIn (slot 535, form V, interface 0xF9 CTCFadeOutAndIn)
  - 008a0950 EntityEndScreamerSuperAttackThing (slot 543, form V, interface 0x31 CTCCreatureModeManager)
  - 008a0bc0 EntitySetAppearanceSeed (slot 482, form N, interface 0x3 CTCRandomAppearanceMorph)
  - 008a0c10 EntityGetAppearanceSeed (slot 483, form N, interface 0x3 CTCRandomAppearanceMorph)
  - 008a0f70 SetReadableObjectText (slot 713, form ND, interface 0xD CTCActionUseReadable)
  - 008a1830 SetUseableByHero (slot 741, form V, interface 0xD CTCActionUseBed)
  - 008a18f0 SetOwnedByHero (slot 742, form V, interface 0xD CTCActionUseBed)
  - 008a1950 SetIsThingTurncoatable (slot 744, form V, interface 0x9F CTCTurncoat)
  - 008a1ab0 RemoveCreatureScriptedMode (slot 750, form V, interface 0x31 CTCCreatureModeManager)
- CGameScriptInterface::SetIsGossipForPlayer x2 (0x008A96F0 const-ref slot 724, 0x008A97D0 by-value slot 723): the body is Ego_r's
  static __fastcall CTCAIScratchpad::SetIsGossipForPlayer(CCharString, bool) inlined (`IsGossipForPlayer[name] = isGossip`);
  its by-value parameter is the copy retail builds (in the dead `name` slot for the const-ref overload, in the `push ecx` slot
  for the by-value one, which then also destroys its own argument). Map/CCharString model from landed 0x008A9810. First grade.
- CCombatSequenceBase::CCombatSequenceBase(CThingCreatureBase&, CThingCreatureBase&) 0x008B0AB0 (2nd grade): init list stores
  the two references, the body is vector::clear() (STLport erase(begin(), end()) -> __copy_ptrs -> out-of-line __copy 0x008B25C0,
  _Destroy -> out-of-line __destroy_aux 0x008B3010) then the four state resets. Both helpers need same-TU bodies under
  auto_inline(off)+noinline (tag homed in the dead arg slot); the last residue (`push edi` scheduled before the three zero
  stores) went away only with STLport's real `_Vector_base(const _Alloc&)` + `_STLP_alloc_proxy` end-of-storage member.
- CGameScriptThing::GetFocalPos() const 0x008D03E0 (3rd grade): nested `if (PThing) { if (!PThing->Dead) { CThing* thing = PThing;
  return thing->PTCGraphicAppearance->GetFocalPos(1.0f); } } return Pos;` with C3DVector's inline user copy constructor. The `&&`
  form tests `!Dead` as a value (`mov al ; not al ; test al,1`); without the named local the third GetPItem runs after the pushes;
  without the copy ctor the fallback copies through `add edi,28h`. The hand45 "register choice" park was these source shapes.

## Parked for Fable 5.1 (1)
- 008c6160 CCreatureAction_RockTrollFallToDeath::Initialise DIFFER(87v87): `mov edi,ecx ; lea esi,[edi+8]` vs retail `mov esi,ecx ; lea edi,[esi+8]` (this / &PCreature register swap, every later use follows) | tried: named creature local (order right, registers swapped), reference-to-PCreature local, temporary CCharString argument (83 bytes), no local + static_cast of operator-> (registers right, but the second GetPItem runs after `push 0 ; push 1`); compile-only also tried accessor-on-action, typed-pointer-local and pointer-to-PCreature locals (all swap) | next: a form that both keeps `this` the longer-lived value and evaluates GetPItem before the pushes -- e.g. a CCreatureActionBase inline returning CThingAICreature* used only in the call, or the landed ScorpionSummon 0x008C4A80 ordering (that row matched with the same tail); else lane gap: register allocation

## Skipped
- none (no row is a fragment or a multi-function span).

## Names
### Proven
Every name: CGameScriptInterface rows by RTTI vtable 0x01260F0C slot + ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) +
the Ego_r.pdb mangled name (and the callee's PDB class/method); the overload pairs settle by slot (MSVC lays overloaded virtuals
out in reverse declaration order: Ego_r donor order is CCharString then long, vtable 618 long / 619 CCharString; by-value 723 /
const-ref 724) and each body's callee (0x00681E60 long, 0x006820E0 CCharString) / argument handling (copy-ctor of a reference vs of
a by-value argument plus its destruction).
00899000 CGameScriptInterface::GiveHeroExperience -- ?GiveHeroExperience@CGameScriptInterface@@UBEXJ@Z
0089b050 CGameScriptInterface::EntitySetWillBeUsingNarrator -- ?EntitySetWillBeUsingNarrator@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
0089b3f0 CGameScriptInterface::SetHeroAsWearing -- ?SetHeroAsWearing@CGameScriptInterface@@UBEXABVCCharString@@@Z
0089c190 CGameScriptInterface::EntitySetInFaction -- ?EntitySetInFaction@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
0089c420 CGameScriptInterface::EntitySetPersonalityOverride -- ?EntitySetPersonalityOverride@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
0089c470 CGameScriptInterface::EntitySetPersonalityOverride -- ?EntitySetPersonalityOverride@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
0089c510 CGameScriptInterface::OpinionSourceSetAsAttentionGrabbing -- ?OpinionSourceSetAsAttentionGrabbing@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
0089c850 CGameScriptInterface::RemoveOpinionDeedStillSearchingForWitnesses -- ?RemoveOpinionDeedStillSearchingForWitnesses@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
0089d910 CGameScriptInterface::EntityForceToLookAtNothing -- ?EntityForceToLookAtNothing@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
0089dbb0 CGameScriptInterface::EntitySetAsAbleToWalkThroughSolidObjects -- ?EntitySetAsAbleToWalkThroughSolidObjects@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
0089dcd0 CGameScriptInterface::EntitySetAllowBossPhaseChanges -- ?EntitySetAllowBossPhaseChanges@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
0089dd90 CGameScriptInterface::EntitySetBossPhase -- ?EntitySetBossPhase@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
0089de50 CGameScriptInterface::EntityLeaveCombatStance -- ?EntityLeaveCombatStance@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
0089deb0 CGameScriptInterface::EntitySetAsDisplayingEmoteIcon -- ?EntitySetAsDisplayingEmoteIcon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
0089e130 CGameScriptInterface::EntitySetMaxNumberOfAttackers -- ?EntitySetMaxNumberOfAttackers@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
0089ee00 CGameScriptInterface::SetTeleporterAsActive -- ?SetTeleporterAsActive@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
0089ee60 CGameScriptInterface::IsTeleporterActive -- ?IsTeleporterActive@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
0089ef30 CGameScriptInterface::SetMoralityChangingAsEnabled -- ?SetMoralityChangingAsEnabled@CGameScriptInterface@@UBEX_N@Z
0089f1e0 CGameScriptInterface::IsDiggingSpotEnabled -- ?IsDiggingSpotEnabled@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
0089f240 CGameScriptInterface::IsDiggingSpotHidden -- ?IsDiggingSpotHidden@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
0089f2a0 CGameScriptInterface::SetDiggingSpotAsHidden -- ?SetDiggingSpotAsHidden@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
008a01a0 CGameScriptInterface::EntityGetStockItemPrice -- ?EntityGetStockItemPrice@CGameScriptInterface@@UBEJABVCScriptThing@@@Z
008a0200 CGameScriptInterface::EntitySetAppearanceMorphSeed -- ?EntitySetAppearanceMorphSeed@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
008a0540 CGameScriptInterface::EntityClearRangedTarget -- ?EntityClearRangedTarget@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
008a0600 CGameScriptInterface::EntitySetTargetingType -- ?EntitySetTargetingType@CGameScriptInterface@@UBEXABVCScriptThing@@W4ETargetingType@@@Z
008a06c0 CGameScriptInterface::EntitySetLightAsOn -- ?EntitySetLightAsOn@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
008a07c0 CGameScriptInterface::EntityFadeIn -- ?EntityFadeIn@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z
008a0950 CGameScriptInterface::EntityEndScreamerSuperAttackThing -- ?EntityEndScreamerSuperAttackThing@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
008a0bc0 CGameScriptInterface::EntitySetAppearanceSeed -- ?EntitySetAppearanceSeed@CGameScriptInterface@@UBEXABVCScriptThing@@K@Z
008a0c10 CGameScriptInterface::EntityGetAppearanceSeed -- ?EntityGetAppearanceSeed@CGameScriptInterface@@UBEXABVCScriptThing@@AAK@Z
008a0f70 CGameScriptInterface::SetReadableObjectText -- ?SetReadableObjectText@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCWideString@@@Z
008a1830 CGameScriptInterface::SetUseableByHero -- ?SetUseableByHero@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
008a18f0 CGameScriptInterface::SetOwnedByHero -- ?SetOwnedByHero@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
008a1950 CGameScriptInterface::SetIsThingTurncoatable -- ?SetIsThingTurncoatable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
008a1ab0 CGameScriptInterface::RemoveCreatureScriptedMode -- ?RemoveCreatureScriptedMode@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
008a96f0 CGameScriptInterface::SetIsGossipForPlayer -- ?SetIsGossipForPlayer@CGameScriptInterface@@UBEXABVCCharString@@_N@Z (slot 724)
008a97d0 CGameScriptInterface::SetIsGossipForPlayer -- ?SetIsGossipForPlayer@CGameScriptInterface@@UAEXVCCharString@@_N@Z (slot 723)
008b0ab0 CCombatSequenceBase::CCombatSequenceBase -- ??0CCombatSequenceBase@@QAE@AAVCThingCreatureBase@@0@Z (the only ctor in Ego_r.pdb; stores vtable 0x01278FEC = RTTI CCombatSequenceBase; already bound to 0x008B0AB0)
008c6160 CCreatureAction_RockTrollFallToDeath::Initialise -- ?Initialise@CCreatureAction_RockTrollFallToDeath@@UAEXXZ (vtable 0x01262B04 slot 12, the Initialise slot of the landed siblings; string FALL_TO_DEATH) [parked]
008d03e0 CGameScriptThing::GetFocalPos -- ?GetFocalPos@CGameScriptThing@@UBE?AVC3DVector@@XZ (vtable 0x012852CC slot 5; Ego_r.pdb)
### Probable
(row names: none; callee names introduced on probable evidence)
004c1060 NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) const -- Ego_r.pdb spelling, already one of two reloc_bindings names; the other (GetHelperPointOrCentre) is in neither PDB; GetCameraFocalPos has the same signature, so the pick rests on the caller being GetFocalPos
008b25c0 std::__copy<CCombatAction*, CCombatAction*, int>(..., const random_access_iterator_tag&, int*) -- STLport erase/__copy_ptrs shape + fastcall 5-argument call; landed as _global::Move_CountedPointer_008b25c0 (new binding from this chunk)
### Descriptive
(row names: none) Member names: CThing::ThingType (+0x10, Ego_r unnamed padding); CTCOpinionSource::ReactNonstopFlag (+0x77, the Ego_r +0x7B member after the 4-byte vector shift, for "AttentionGrabbing"; the identity of the flag is layout-only).

## Callee names
All from Ego_r.pdb and matching their existing bindings: GetMainPlayer (non-const QAE) 0x00449970, CPlayer::GetPControlledCreature() const 0x00487DC0,
CVectorMap<ETCInterfaceType,...>::LowerBound 0x0040F020, CTCHeroExperience::AddExperience 0x006D77C0, CTCTalk::SetAsUsingNarrator 0x006E2CF0,
CTCInventoryClothing::SetAsWearing 0x005B413D, CTCEnemy::SetFaction 0x0076C810, CTCCreatureOpinionOfHero::SetPersonalityOverride 0x006820E0 / 0x00681E60,
CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses 0x006C5920, CTCCreatureNavigation::SetAsAbleToMoveThroughSolidObjects 0x006CB9D0,
CTCBoss::SetNextBossPhase 0x007AEDA0, CTCTeleporter::SetAsActive 0x007DFEB0, CTCRandomAppearanceMorph::SetAppearanceSeed(unsigned long) 0x00728460,
CTCTargetingBase::SetPScriptOverrideRangedTarget(CThing*) 0x008ABB50, CTCTargeted::SetTargetingFlags(unsigned long) 0x007EA270, CTCLight::SetActive 0x006412B0,
CTCFadeOutAndIn::FadeIn(float,bool) 0x006C3BD0, CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, CWideString::operator= 0x0099B7D0,
CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0, map<CCharString,bool>::operator[] 0x008ADF10, CTCAIScratchpad::IsGossipForPlayer 0x013BAE38,
std::__destroy_aux<CCombatAction*> 0x008B3010, CBaseIntelligentPointer::GetPItem 0x00A01B50.
Interface kinds were taken from each TC class's GetInterfaceType (vtable slot 21, `mov eax,imm ; ret`): 0x03 CTCAppearanceMorphBase (CTCRandomAppearanceMorph),
0x07 CTCCreatureNavigation, 0x0F CTCTalk, 0x30 CTCCombat, 0x31 CTCCreatureModeManager, 0x36 CTCStealth, 0x45 CTCEmoteIcon, 0x49 CTCEnemy, 0x4F
CTCCreatureOpinionOfHero, 0x50 CTCOpinionSource, 0x51 CTCHeroOpinionDeedLog, 0x68 CTCHeroExperience, 0x9F CTCTurncoat, 0xAD CTCBoss, 0xDD CTCTeleporter,
0xEF CTCDiggingSpot, 0xF9 CTCFadeOutAndIn, 0x27 CTCLight; the rest by callee or layout: 0x04 CTCHeroStats (MoralityChangingEnabled), 0x08 CTCTargetingBase,
0x09 CTCTargeted, 0x0D action-use (CTCActionUseReadable TextTag +0x18, CTCActionUseBed UseableByHero/OwnedByHero +0x20/+0x21), 0x12 CTCInventoryClothing,
0x54 CTCStockItem (Price +0x10). Every store offset agrees with Ego_r.pdb after the STLport container shifts.

## Label corrections
- 0x00681E60 "SetPersonalityOverride_BuildOpinionTable" -> CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool)
- 0x008ABB50 "AssignCreatureToTask" -> CTCTargetingBase::SetPScriptOverrideRangedTarget(CThing*)
- 0x007EA270 "AddExclusiveObserver" -> CTCTargeted::SetTargetingFlags(unsigned long) (body `mov [ecx+0Ch],arg`; landed name already right)
- 0x00728460 "AddToInterface" -> CTCRandomAppearanceMorph::SetAppearanceSeed(unsigned long) (stores Seed +0x18, rebuilds)
- 0x00487DC0 "CPlayer::GetHeroCreature" -> CPlayer::GetPControlledCreature() const
- 0x004C1060 "CTCGraphicAppearance::GetHelperPointOr..." -> NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) const (probable)
- 0x008B25C0 "_Move<CCountedPointer...>" -> std::__copy<CCombatAction*, CCombatAction*, int> (probable); 0x008B3010 "_Destroy_range<...CEntityScriptBindingBase...>" -> std::__destroy_aux<CCombatAction*>
- 0x00833A10 "CThingAnimatable::SetSuspendAnimation" -> CThingAICreature::SuspendAI(bool, void*) (landed name)
- 0x0099EAE0 "_Cons_val<...>" -> CCharString::~CCharString; 0x008574D0 "SetPDef" -> CCombatActionBase::SetCombatAnimation(const CCharString&)

## Header debt
- engine/CGameScriptInterface.h carries the vptr as a data member (`__vftable`), so it cannot host the virtual members: local
  CGameScriptInterface models (PlayerManager +0x14 as in the header).
- engine/CScriptThing.h, CThing, CPlayer, CPlayerManager, CGameScriptThing and the TC headers (CTCCombat, CTCCreatureOpinionOfHero,
  CTCHeroExperience, CTCHeroStats, CTCInventoryClothing, CTCStockItem, CTCTargeted, CTCTurncoat) are flat Ego_r/Dinkumware
  layouts without vtables; modelled locally with retail offsets (vland SHARED_HEADER_AVAILABLE warnings only).

## Review at landing
- No RELOC_CONFLICT / RELOC_DUPLICATE. 008b0ab0's TU also defines the bodies of std::__copy<CCombatAction*,...> and
  std::__destroy_aux<CCombatAction*> (same-TU helpers for the tag homing); the Leaf selects the constructor. __destroy_aux is
  already bound to 0x008B3010; __copy becomes the PDB-style binding for 0x008B25C0 (landed under a descriptive _global name).
- Not in this chunk: landed 0x00899060 SetHeroAbleToGainExperience still declares 0x00487DC0 as CPlayer::GetPPlayerCharacter;
  the settled name is GetPControlledCreature.
