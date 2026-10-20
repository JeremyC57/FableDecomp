# Hand chunk 230 (CTCCameraPointBuilding .. CTCExpression): 25 of 40 byte-exact

All 25 grade RELOCATION_MATCH under RTM cl 13.10.3077 (gall.sh), all 25 tests PASS under g++ (tall.sh) and VC (tall.py).

## What matched (25)
- **CTCCarriedActionUseGive** 007ed170 vfunc_28 -- base use hook, give the item to a targeted user; UseDrop pattern, named item local, map-before-key GetTC.
- **CTCClock** 00732850 OnInitialActivate -- reset last-updated to 0.5, start CLOCKHAND_TURN; AnimationComplex::OnCreate block + Cupboard-style GetTCIfPresent (first try).
- **CTCCoopSpirit** 0066fb80 UpdateLocalJostling -- push apart nearby player heroes; named spirit local for the subtraction.
- **CTCCoopSpirit** 006701a0 UpdateAttractionToMaster -- pull an idle spirit to its master; by-value inline position accessor + C3DVector user copy ctor gives the half-FPU temp.
- **CTCCoopSpirit** 006705f0 SetAppearanceParticle -- kill the old appearance particle, attach the new one; IntelligentPointer temp assign (first try).
- **CTCCreatureExpression** 00728ff0 UpdateBlinking -- 1-in-50 seed roll starts a blink; short-lived named bool sinks the ebx push (first try).
- **CTCCreatureFlyInCircles** 007dd430 InitFlightCircle -- store centre/radius/height, set up the circle; named RHSet local + named physics TC local.
- **CTCCreatureGroup** 008128f0 GetNearestMember / 00812a10 GetNearestActiveMember -- map walk keeping the nearest; static inline `_M_increment(node)`, best initialised to 0.
- **CTCCreatureGroupBoastingCrowd** 00784500 FrameUpdate -- kill crowd when the hero is gone; forceinline OffPodium predicate, named manager local in the loop.
- **CTCCreatureModeManager** 0073afc0 SetupInitialMode -- enter the def's initial modes; same-TU `__declspec(noinline)` GetSubDef stand-in lets the key share the out slot.
- **CTCCreatureOpinionOfHero** 0092e630 FindMostRecentDeed<CFilter_Bind2<IsStillHappening,TargetIs>> -- first matching deed; landed 0x0093A3C0 body shape + Bind2 functor, named target local.
- **CTCCreatureOpinionOfHero** 006610c7 FindMostRecentDeed<IsCrimeToRunAndFindAGuard> -- most recent matching deed; size TU (`optimize("y",off)` + `("s",on)`), forceinline GetInterface.
- **CTCDExplosion** 0080d750 FrameUpdate -- run/trigger the explosion, prune dead hits; `erase(remove_if(..., pred(temp)), end)` in one statement with the LTCG-convention remove_if (0x0080E2F0) modelled `__fastcall`.
- **CTCDParticleEmitter** 006e13e0 OnCreate -- dependent emitter setup; named thing local before the named identity RHSet.
- **CTCDParticleEmitter** 006e14e0 CreateParticleWaitingFor -- create the waiting emitter; named thing/params locals, inline bit setters, inline counted clear.
- **CTCDPhysicalObstruction** 004e160c ~CTCDPhysicalObstruction -- size TU, implicit dtor (no vtable reset), null-safe GetRefCount ternary. Leaf `??1CTCDPhysicalObstruction`.
- **CTCDegradableBase** 008ecac0 OnParentHealthChanged -- degrade on hit; IsFullyDegraded inline at first use only. Leaf `OnParentHealthChanged`.
- **CTCDivineWrath** 0078f6e0 FinishMove -- end the spell, fade environment back; named times/world locals, out-param GetInterface for the emitter.
- **CTCDoor** 007308f0 UpdateAnimation -- stop updating once the animation finished; zero-preload HasTC/GetTC, float frame-time words avoid a zero register.
- **CTCDoor** 00730ae0 IsLockPickable -- def allows + closed + locked; nested HasTC/GetTC, owner refetched into a named local.
- **CTCEffectOnDie** 007e0bf0 FrameUpdate -- life countdown and fade; local-then-member decrement, cached loop bounds.
- **CTCExpression** 007ef070 OnSerialise -- persist the def by name; AtmosPlayer OnSerialise pattern, inline CDefString::GetString.
- **CThingCreatureBase** 00666090 ctor (catalogued as CTCDParticleEmitter) -- alloc-proxy STLport vector model anchors the zero stores. Leaf `??0CThingCreatureBase`.
- **CTCPhysicalShieldSpell** 0079e990 dtor (catalogued as CTCElectrocutedEffect) -- forceinline FadeOrKillEffect helper. Leaf `??1CTCPhysicalShieldSpell`.

## Parked for Fable 5.1 (13)
- 00567400 std::basic_string<char>::append(first,last,forward_iterator_tag) DIFFER(281v280): only the int3 VC emits after the final noreturn _M_throw_length_error call | tried: exact STLport body (otherwise byte-identical) | next: lane gap: grader trims retail padding but not ours (same as 00a5b260); land by hand with Leaf `append`
- 0076c8a0 CTCEnemy::CTCEnemy DIFFER(254v254): ebp<->edi swap between the thing-def and enemy-def locals | tried: holder/holder, raw/holder-in-block, raw/raw direct if | next: lane gap: register allocation
- 0077d230 CTCCreatureHive::Open DIFFER(243v243): inlined GetInterface key homed in [esp+0Ch] (dead out word) v retail [esp+14h] (dead name word) | tried: stand-in GetSubDef, named filter | next: named filter WITHOUT the stand-in (out escaping)
- 00750110 CTCChest::DisplayLockedMessage DIFFER(299v298): ParentThing / g_pMainGameComponent loaded into ecx v retail eax | tried: plain, named thing-def local (fixes prologue, scrambles arms) | next: lane gap: register allocation
- 0075d9a0 CTCCameraPointBuilding::vfunc_17 DIFFER(255v256): esi push sunk below the HasTC branch v retail at entry | tried: plain HasTC, if/else, else-if chain, out-param GetInterface (best) | next: lane gap: callee-saved push placement
- 00734210 CTCCombat::SetBlockPercentageChance DIFFER(256v261): retail reloads header/root ([ecx+70h],[esi+4]) for the second tree walk, ours CSEs them and pushes ebp/edi at entry | tried: plain/alloc-proxy header, user iterator copy ctor | next: separate inline helper per walk with its own map reference
- 00739360 CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockHeavyWeaponAttackDef> DIFFER(259v262): equal_range key temp stored early + edx compare v retail immediate compare + late store | tried: stand-in GetSubDef, separate upper/lower statements, static _M_increment | next: by-value key parameter to an out-of-line lower_bound model
- 0071ab20 CTCEmoteIcon::DrawEmoteTypeText DIFFER(244v244): eax/ecx/edx rotation at focal-point call, text-primitive pushes and renderer vcall | tried: inline font arg (reorders colour stores), named locals | next: lane gap: register allocation (vptr-in-eax family)
- 006a2540 CTCCarrying::OnCreate DIFFER(308v312): ebp/edi swap between the sub-def and the CIntelligentPointer vtable constant | tried: holder and raw-pointer defs, stand-in GetSubDef (required), __fastcall sort 0x006A3F20 | next: lane gap: register allocation
- 007ceb00 CTCElectrocutedEffect::CTCElectrocutedEffect DIFFER(299v299): discarded __copy hidden-return temp homed in [esp+10h] v retail [esp+24h] | tried: __fastcall __copy model (0x007CE1F0), tag user ctor, alloc-proxy TintList | next: scope the __copy call in a block after the index local dies
- 00c14f90 CDiceGameHost_00c14f90::~CDiceGameHost_00c14f90 DIFFER(248v248): `lea ebx,[esi+40h]` scheduled before the first handle vtable store v retail between the 2nd and 3rd | tried: inline COwnedPointer::Delete pin (fixed zero stores) | next: lane gap: scheduling; class identity unresolved (no RTTI)
- 00730f90 CTCDoor::UpdateOpenStatusTriggerTypePerson DIFFER(293v292): C2DBox/filter build: one extra frame word, fxch v `fld st0 ; fadd st3`, late 1.0 z-range store | tried: by-value C2DVector with user copy ctor (fixed x/y loads) | next: build the box from min/max locals in retail order
- 0080c590 CTCDExplosion::UpdateExplosiveAdditionalImpulse DIFFER(342v305): functor layouts, transform result slot and ring-radius selects | tried: __fastcall models of the three LTCG callees (1 grade) | next: decode 0x0080EBE0/0x0080E530/0x0080E5D0 prologues to pin the functor shapes

## Skipped
- 00567d60 -- row holds two functions: an STLport basic_string<char>::append copy (0x567D60) and a 13-byte thunk after int3 padding.
- 007c977c -- row starts mid-function; the real entry is 0x007C9770 (`mov al,[esp+8] ; sub esp,10h ; push ebx ; xor ebx,ebx ; cmp al,bl`).

## Label corrections
- 00666090 is CThingCreatureBase::CThingCreatureBase(long) (vtable 0x0125AC0C), not CTCDParticleEmitter.
- 0079e990 is CTCPhysicalShieldSpell::~CTCPhysicalShieldSpell (vtable 0x0126C53C), not CTCElectrocutedEffect.
- 00567400 / 00567d60 are STLport basic_string<char>::append<const char*> (range/forward-iterator), not CTCElectrocutedEffect.
- 00c14f90 is not CTCDiceGame: an unidentified no-RTTI container class dtor (descriptive name used).
- 004e160c is reached from the ??_G at 0x004E15F0, which the manifest labels CLandscapeBackgroundPatch; the body is the CTCDPhysicalObstruction dtor -- check that ??_G label.
- Propagated callee labels seen wrong in Ghidra: 0x0070D920 is CAnimateThing::SetMainAnim (1-arg), not operator_delete; 0x0070D8E0 is RemoveCombinationAnim, not CSpellContainerList::AddChild; 0x00A14480 is C3DVector::GetLength, not GetScaled. Confirmed: 0x00686D60 GetGameSubsystem, 0x00686A80 GetCamera (world+0x24), 0x00686D30 GetWorldMap, 0x00686D40 GetPlayerManager, 0x005B33F4 CAnimInit 7-arg ctor, 0x0072ABD0 FadeOrKillEffect, 0x0066ADD0 StopSample, 0x004109A0 CPersistContext::Transfer(CCharString).
