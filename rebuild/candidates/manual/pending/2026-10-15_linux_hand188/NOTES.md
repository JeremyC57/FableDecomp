# Hand chunk 188 (CCreatureAction_BanditKingThrowWeapons .. CCreatureAction_DragonStrafeInto): 31 of 40 byte-exact

## What matched (31)
- CCreatureAction_BanditKingThrowWeapons ctor 008c7fd0: literal passed to `const CCharString&` must be an implicit conversion (default length) so it builds before the other base args; Target assigned in the body from a converting temporary.
- CCreatureAction_BattleChargeImpact ctor 008ce4b0 / CCreatureAction_BattleChargeMiss ctor 008ce570: by-value CIntelligentPointer params copy-constructed into the members (inline copy ctor), callee destroys the params.
- CCreatureAction_BlockBase::GetNextPos 008552f0: `(float)exp(t*t*-100.0f)` with `#pragma intrinsic(exp)` gives the inline fldl2e/f2xm1/fscale; vector ops as inline C3DVector operators.
- CCreatureAction_BlockBase::Initialise 00855430: GetSoundPair result as the PlaySound first argument (string temporary still first); the play-parameter block assigned in its ctor BODY with Handle before Priority.
- CCreatureAction_BlockCounter::Initialise 00855920: HasTC+FindEntry (`mov ecx,[eax+4]`) for the owner, GetInterface out-param for the target as the last term of the chain.
- CCreatureAction_BlowHorn ctor 00842ec0: two implicit literals (sound name built first: right-to-left args).
- CCreatureAction_BodgeSideStrike::Initialise 008bf0f0: if/else of two calls tail-merges to `push 1/push 0` (ternary gives mov/xor); body vcall through an inline creature forwarder.
- CCreatureAction_BriarRoseIntoInvulnerable::Initialise 008c1e80 (extra try): owner argument through an inline `operator CThing*()` so it is fetched before the plain enemy GetPItem.
- CCreatureAction_CastAssassinRush ctors 008ce040 / 008ce100 / 008ce1b0: by-value CCountedPointer param (inline copy ctor add-ref, inline dtor release + delete), packed layout of the landed copy ctor.
- CCreatureAction_CastStrengthMultiStrike::FinishAction 008e6ae0: target/owner/creature as three named locals before the 0x118-byte temporary.
- CCreatureAction_CastTimeSpell::Initialise 008cd730, CastWillGhostSwordSpell::Initialise 008b9970, CastWillHealLifeSpell::PerformAction 008b97e0, CastWillSummonSpell::Initialise 008b9b00: AddTC by implicit literal, named copy of the +0x2c target, inline BY-VALUE operator= for the component's target.
- CCreatureAction_CastWillEnflameSpell ctors 008bb450 / 008bb500, CastWillHealLifeSpell ctors 008bbcc0 / 008bbd80, CastWillTurncoatSpell ctors 008bb8f0 / 008bb9a0: base target assigned in the body from a converting temporary; HealLife literal is a null-first ternary (`target == 0 ? SELF : HEAL`).
- CCreatureAction_DowseFire ctor 00843790: two init-list intelligent pointers from raw pointers.
- CCreatureAction_DragonBurninateInto::Initialise 008f4320, DragonFireBeamInto::Initialise 008f41d0, DragonRearUpInto::Initialise 008f39a0: pointer arguments through the inline `operator CThing*()` (evaluated before the plain member pushes).
- CCreatureAction_DragonBurninateInto ctor 009537b0 / copy ctor 00953df0: memberwise C3DVector copy ctor, out-of-line vector copy ctor 0x006F0810.
- NParticleEngine::CParticleEmitter::~CParticleEmitter 00af10e0: `__declspec(novtable)` emitter + `__forceinline` CEnginePrimitiveHandle dtor (landed grid-cell model).
- CCreatureAction_DragonHoverInto::Initialise 008f3fd0: new position as a default-constructed C3DVector assigned member by member.

## Parked for Fable 5.1 (9)
- 008bb6f0 CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell DIFFER(215v212): inline CCharString::operator==("") null-block strcmp gives `sete al ; test al,al` where retail branches on the repe cmpsb flags | tried: if/else either order, __forceinline operator== | next: lane gap: inline bool-return branch threading
- 008bb130 CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell DIFFER(200v197): same `sete al` materialisation | tried: inline operator!=, ternary operator==, early return | next: lane gap: inline bool-return branch threading
- 008bb200 CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell DIFFER(195v192): same | tried: #pragma optimize("s"), non-null-first operator== | next: lane gap: inline bool-return branch threading
- 008cdc40 CCreatureAction_CastSkillMultiArrow::Initialise DIFFER(183v168): ours rotates the add-until-present loop (duplicated GetPItem + bit test at the bottom), retail keeps the test at the top with jmp back-edges | tried: for(;;)+return, while((owner=...)), goto | next: tail-recursive inline helper
- 008e6690 CCreatureAction_CastStrengthMultiStrikeProper::Initialise DIFFER(230v226): retail caches the def object in ebx after GetDef and reuses its slot | tried: function-scope CDefPointer + block name | next: lane gap: CDefPointer kept in a register across calls
- 008e6bb0 CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper DIFFER(222v215): def cached in edi, `dec [edi+4] ; jne` release without re-read | tried: as 008e6690 + temporary GetString arg | next: lane gap: CDefPointer kept in a register
- 008e6c90 CCreatureAction_CastStrengthMultiStrikeProper::CCreatureAction_CastStrengthMultiStrikeProper DIFFER(217v210): same | tried: same | next: lane gap: CDefPointer kept in a register
- 008f3020 CCreatureAction_DragonFlight::PerformAction DIFFER(219v212): ability def cached in ebx after GetSubDef (8-byte frame) | tried: function-scope CDefPointer + inline owner conversion | next: lane gap: CDefPointer kept in a register
- 008f4480 CCreatureAction_DragonStrafeInto::Initialise DIFFER(233v233): ebx/ebp swap only (&PCreature vs mode manager) | tried: block float locals, named new-result local | next: lane gap: register allocation

## Skipped
- none (no row was already landed; no two-function rows).

## Label corrections
- 008ce570: catalog "CCreatureAction_BattleChargeImpact" ctor -> CCreatureAction_BattleChargeMiss ctor (vtable 0x012847C4, "ST_BATTLE_CHARGE_MISS").
- 00af10e0: catalog "~CCreatureAction_DragonFireBlastBase" -> NParticleEngine::CParticleEmitter::~CParticleEmitter (only caller the deleting dtor 0x00AF10C0 = vtable 0x0129F348 slot 0). Graphics/particle layer: owner note applies.
- 0x007C5440 / 0x007C3EA0 ("CObservable::AddExclusiveObserver"): CTCGhostSword / CTCHealLife SetAICast(long) setters (+0x3c / +0x18).
- 0x004C9840 ("CBaseObject::OnPropertyChanged") is CThing::RemoveTC(long); 0x009AD390 ("COptimisedPrimitive::ActualToUniqueIndex") is GetDefIndexFromName; 0x0062E900 ("~CTCLightningOrb") is a shared dtor body used for CCreatureAction_CastStrengthMultiStrike; 0x008574D0 ("CTCInGameMenu::SetPDef") is CCombatActionBase::SetAnimationName; 0x006924B0 ("BeginInputLoading") is the animation lookup; 0x00733E10 is CTCCombat::SetMeleeFlourishValid.
