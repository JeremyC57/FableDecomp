# Hand chunk 658 (std::vector<pair<CCharString,CRegisteredPSCInfo>> .. CTCFishingRod): 10 of 10 byte-exact

NA004 of the never-attempted pool (twin_landed): every row started from its twin's source, but each row's own identity
was settled from its caller chain / vtable first. All ten matched (RELOCATION_MATCH) on the first probe; the 12-probe rule
never applied. Tests PASS under g++ and the lane's VC7.1. Every manifest label except the two named Clones was wrong.

## What matched (10)
- **NParticleEngine registered-PSC map** (`std::vector<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>,
  std::allocator<...> >`): 0x00AECA80 `~vector` (STLport ~vector + inline ~_Vector_base) and 0x00AF36C0 the protected
  `_M_clear`; the twin 0x00687180's STLport model with the row's own element type and the two-parameter vector spelling.
- **Creature-action Clones** (K037 shape, `return new X(*this);` with the implicit copy constructor inline):
  0x0084C490 CCreatureAction_PlayConversationAnimation (base CCreatureActionBase, copy 0x0062EBC0, sizeof 0xB0),
  0x008F5740 CCreatureAction_DragonFlight, 0x00925D90 / 0x00925E40 the anonymous-namespace Kraken Low/High AttackLoop
  (base CCombatActionBase, copy 0x0062F3B0, sizeof 0x11C). Shape lesson: a `bool` first member copies through `mov al`,
  the following float/long through ecx (the twin's two longs give `mov eax` twice) -- the member TYPES from the PDB set it.
- **Destructors**: 0x0083D6D0 CThingPhysicalSwitch (user-declared: vtable store, ~CCharString, tail jump to
  ~CThingGameObject); three IMPLICIT destructors (no vtable store, no PDB ??1 in Ego_r): 0x0071A570
  NTCScriptedControl::CActionTalkToThing (three strings in reverse order, tail jump ~CActionTalkBase; emitted by the
  EmitImplicitDestructor helper as landed 0x0071A430), 0x007DFAF0 CTCWallMountEffects (intelligent pointer out of line,
  then the engine CDefPointer release), 0x007E24D0 CTCFishingRod (CDefPointer release); both TCs emitted by defining the
  ctor as landed 0x00771F30. engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE) gives the `dec [ecx+4]; jne` release verbatim.

## Proven
- 0x00AECA80 ??1?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$allocator@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@@2@@std@@QAE@XZ -- NParticleEngine::Initialise 0x00AEBE20 builds a 0x10-byte ??0CPSCManager 0x00AF2970 object into the CCountedPointer at 0x013D2E24 via Reset 0x00AECC30, whose deleter 0x00AECBF0 (DeleteData) calls this; CPSCManager = CVectorMap<CCharString,CRegisteredPSCInfo> (Ego_r.pdb); three `jmp` thunks 0x00AECB30/40/50 are the CArray / CVectorMap / CPSCManager levels (twin 0x00687180 has two: no owner class); FableWin.pdb has this exact ??1 spelling; element layout = the pair _M_insert_overflow 0x00AF37F0 copies.
- 0x00AF36C0 ?_M_clear@?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$allocator@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@@2@@std@@IAEXXZ -- sole caller 0x00AF38AD in the landed _M_insert_overflow 0x00AF37F0 (same vector; its relocation binding for 0x00AF36C0 already has this spelling); chain push_back 0x00AF39C0 <- Add 0x00AF2910 <- CPSCManager::RegisterPSCs 0x00AF8170; STLport 4.6 _M_clear is protected.
- 0x0084C490 ?Clone@CCreatureAction_PlayConversationAnimation@@UBEPAVCCreatureActionBase@@XZ -- vtable 0x012737CC slot 11 (sole reference 0x012737F8), RTTI CCreatureAction_PlayConversationAnimation : CCreatureActionBase, stores that vtable; Ego_r + FableWin name; new 0xB0 = header retail sizeof; members UseAnimationMovement +0xA8 / Duration +0xAC (Ego_r).
- 0x008F5740 ?Clone@CCreatureAction_DragonFlight@@UBEPAVCCreatureActionBase@@XZ -- vtable 0x01288544 slot 11 (sole reference 0x01288570), RTTI : CCombatActionBase, stores that vtable; Ego_r + FableWin name; new 0x11C = header retail sizeof; FaceTarget +0x114 / TurnRate +0x118.
- 0x00925D90 ?Clone@CCreatureAction_KrakenLowAttackLoop@?A0x29922355@@UBEPAVCCreatureActionBase@@XZ -- vtable 0x01290C64 slot 11 (sole reference 0x01290C90), RTTI .?AVCCreatureAction_KrakenLowAttackLoop@?A0x29922355@@ : CCombatActionBase; Ego_r/FableWin ?Clone@CCreatureAction_KrakenLowAttackLoop@?A0xe2035276@@UBEPAVCCreatureActionBase@@XZ (donor hash, respelled with the retail RTTI hash); Interrupt +0x114 / LoopNum +0x118 (Ego_r; the landed ctor 0x009255E0 stores the bool / long arguments there).
- 0x00925E40 ?Clone@CCreatureAction_KrakenHighAttackLoop@?A0x29922355@@UBEPAVCCreatureActionBase@@XZ -- vtable 0x0129107C slot 11 (sole reference 0x012910A8), RTTI .?AVCCreatureAction_KrakenHighAttackLoop@?A0x29922355@@; Ego_r/FableWin name (donor hash ?A0xe2035276); members as the Low loop (landed ctor 0x00925780).
- 0x0083D6D0 ??1CThingPhysicalSwitch@@UAE@XZ -- sole caller 0x00529CA3 in 0x00529CA0 = slot 0 of vtable 0x01245644 (RTTI CThingPhysicalSwitch), stores that vtable; Ego_r 0x00812920 / FableWin ??1CThingPhysicalSwitch@@UAE@XZ; TriggerableObjectScriptName +0xD4 (landed Save 0x0083D800 layout).
- 0x0071A570 ??1CActionTalkToThing@NTCScriptedControl@@UAE@XZ -- sole caller 0x0071A553 in 0x0071A550 = slot 0 of vtable 0x012659C8 (RTTI .?AVCActionTalkToThing@NTCScriptedControl@@ : CActionTalkBase); tail target = landed ??1CActionTalkBase 0x0071A430 (whose header comment already names this row); FableWin.pdb ??1CActionTalkToThing@NTCScriptedControl@@UAE@XZ; QuestionButton1/2/3 (Ego_r +156/160/164 -> retail +0x94/98/9C).
- 0x007DFAF0 ??1CTCWallMountEffects@@UAE@XZ -- sole caller 0x007DFAD3 in 0x007DFAD0 = slot 0 of vtable 0x0126FC84 (RTTI CTCWallMountEffects : CTCBase); FableWin.pdb ??1CTCWallMountEffects@@UAE@XZ, none in Ego_r (implicit); PDef +0xC, CreaturePerformingAction +0x14 (Ego_r layout).
- 0x007E24D0 ??1CTCFishingRod@@UAE@XZ -- sole caller 0x007E24B3 in 0x007E24B0 = slot 0 of vtable 0x01270104 (RTTI CTCFishingRod : CTCBase); FableWin.pdb ??1CTCFishingRod@@UAE@XZ, none in Ego_r (implicit); PDef +0xC (Ego_r layout).
- 0x00AECBF0 ?DeleteData@?$CCountedPointer@VCPSCManager@NParticleEngine@@@@SIXPAX@Z and 0x00AECC30 ?Reset@?$CCountedPointer@VCPSCManager@NParticleEngine@@@@QAEXPAVCPSCManager@NParticleEngine@@@Z (not chunk rows; proven on the way) -- Initialise 0x00AEBE20 -> Reset on 0x013D2E24 with the CPSCManager object; Reset stores 0x00AECBF0 as the deleter; Ego_r names.

Descriptive names: none for the rows. Descriptive helpers: `EmitImplicitDestructor` (0x0071A570, not a retail function,
as landed 0x0071A430); opaque pads for unneeded base members (CThingGameObject 0xD0, CActionTalkBase +0x0C..+0x8F,
CCreatureActionBase / CCombatActionBase bodies); `EWallMountEffectsConfirmationType` / `EPSCType` enumerators omitted.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections
- 0x00AECA80 "CCombatWheel::ResetRings" -> std::vector<std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo> >::~vector.
- 0x00AF36C0 "CCombatWheel::ResetRings" -> the same vector's _M_clear.
- 0x00925D90 / 0x00925E40 "vfunc_11" -> Clone.
- 0x0083D6D0 "CInputProcessManager::~CInputProcessManager" -> CThingPhysicalSwitch::~CThingPhysicalSwitch.
- 0x0071A570 "_Dest_val<std::allocator<CMacro>,CMacro>" -> NTCScriptedControl::CActionTalkToThing::~CActionTalkToThing.
- 0x007DFAF0 "CCreatureAction_PerformExpressionExtended::~..." -> CTCWallMountEffects::~CTCWallMountEffects.
- 0x007E24D0 "C3DAnimationBlendState::~C3DAnimationBlendState" -> CTCFishingRod::~CTCFishingRod.
- 0x00AECBF0 "CTCMapwho::OnDie" -> CCountedPointer<NParticleEngine::CPSCManager>::DeleteData; 0x00AECC30 -> its Reset;
  0x00AECB60 "CLandscapeBackgroundPatch::`vector_deleting_destructor'" calls ~vector 0x00AECA80 (its class is the PSC map
  family; the three jmp thunks 0x00AECB30/40/50 are ~CArray / ~CVectorMap / ~CPSCManager in some order -- unproven which).

## Header debt
- engine/CCreatureAction_PlayConversationAnimation.h, engine/CCreatureAction_DragonFlight.h, engine/CCreatureActionBase.h,
  engine/CTCBase.h, engine/CThingGameObject.h are flattened (vptr as a data member), so the polymorphic base chains with
  out-of-line copy constructors / destructors are local models over the header offsets (as the landed 0x008CFB70,
  0x00771F30, 0x0083D800). No header exists for CThingPhysicalSwitch, CTCWallMountEffects, CTCFishingRod,
  CActionTalkToThing, CPSCManager, CRegisteredPSCInfo or the Kraken classes.
