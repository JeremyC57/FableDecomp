# Hand chunk 101 (CTCSmashableBase .. CTCSwitchableNavigation): 33 of 40 byte-exact

All 33 src/ rows grade MATCH/RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). Seven rows are parked in differ/.

## What matched (33)
- CTCSmashableBase
  - 007eebb0 IsAvailableToHit: `GetWorld()->GetWorldFrame() - LastWorldFrameHit > 1 ? true : false`; the frame getter is
    STATIC, called through the out-of-line GetWorld() object expression (so no `mov ecx,eax`); `?:` gives `setg al`.
  - 007eecf0 OnInitialActivate (vtable slot 6 of CTCSmashableBarrier): `GetDef()->Smashable` through a by-value
    CDefPointer temporary (null-tested add-ref); the release must be a member of the DEF called from the pointer's
    destructor, or the `dec` is scheduled above the flag load.
- CTCCameraPointGeneralCase (catalogue: CTCSmokeGenerator)
  - 004d3b41 constructor: /Os, pass-through base argument; `CameraHeightOffset = CameraCageRadius = 2.0f` (chained, so
    `fld ; fst ; fstp` from the 2.0f pool) then the 5.0f immediate and three flag bytes in the body.
- CTCSmokeGenerator
  - 00722540 constructor: GenerationDistance init-list (before the vtable), the vector as a member object with an inline
    zeroing constructor, PositionsCreated(false).
  - 007224e0 OnKill (slot 14): loop over the 0x14-byte CGenerationPos records with `last` re-read; three GetPItem calls
    (`p && !(p->flags & 1)` then `Kill(true)`).
- CTCSolidToThing / CTCTargetingAI
  - 00770fa0 AddThingNotSolidTo and 007f61b0 CTCTargetingAI::AddThingToNotAttack: a stack `CIntelligentPointer(thing)` built
    by an inline converting constructor (base ctor, own vtable, SetPItem), pushed by reference, destroyed through the base
    destructor (the generated derived destructor drops its vtable reset).
- CTCSoundPlayer
  - 0066ac80 constructor: `PAttachedEmitter(0)` through the converting constructor (base ctor, vtable, SetPItem(0)), two
    default CCharStrings, then the short and long zeros.
  - 0066bbf0 FrameUpdate (slot 9): raw TC-word test + tail-jumped void call.
  - 0066aeb0 StopAllAttachedSounds, 0066afa0 SetSpecificSamplePitch, 0066aff0 SetSpecificSampleVolume (catalogue:
    CTCSummonSpell::ResumeForCutscene x3): global sound-system test, an inline "live emitter" predicate (two GetPItem calls)
    and a fresh GetPItem for the call.
- CTCSpecialAbilities
  - 0072a950 OnKill (slot 14): `RemoveTC(0x92)` guarded by the raw bitset word.
  - 0072ab40 OnSerialise (slot 1, `ret 8`): Transfer of a STATIC long member with a zero frame default.
  - 0072ac90 ToggleUsingAbility(long): the landed out-parameter GetInterface template, which must be `__forceinline` here
    (plain inline it is emitted out of line).
  - 0072c370 FrameUpdate (slot 9): GetInterface(0x29) then a flag byte test before CancelAllAbilities.
- _unattributed
  - 00c35c30 ForwardIfStateMask217 (catalogue: CTCSpecialAbilities::OnKill): a free `__stdcall` of the 0x00C3 module.
- CTCStealableItemLocation
  - 007b2800 OnSerialise (slot 1): two Transfer<float> calls, each default a zero float local (immediate dword stores).
- CTCStealth (names confirmed in ghidra_out/egor_signatures.tsv)
  - 00747090 ClearDangerStatus: the two `GetWorld()->member->Call(arg)` chains need the GetWorld result in a NAMED local so
    the call precedes the argument push.
  - 007470e0 SetAsRunningAwayFromCombat: `(long)(PDef ? PDef->Hangtime * g_fps : 1.0f)` (per-arm `__ftol2`).
  - 007471a0 IsAbleToFlourishDangerThings: global switch, then a loop with a named thing local.
  - 00747560 StartSneaking: GetInterface(0x31) + AddMode(7), flag store after the inner if.
- CLocalDetailLayer::CObject (catalogue: CTCStealth::OnCreate)
  - 00bfbbc0 default constructor: bounding-sphere zeroes through one register, then the CEnginePrimitiveHandle whose data
    pointer is a CIVCountedPointer MEMBER OBJECT (its zero follows the handle vtable store).
- CParticleHandle (catalogue: CTCStealth::OnCreate)
  - 00c25cb0 destructor: three zero stores, unconditional DecRef, null the pointer.
- CTCStockItem
  - 004dc737 constructor: /Os; the handle's and the counted pointer's constructors must be `__forceinline` under /Os.
  - 008abbf0 SetPrice: built on engine/CTCStockItem.h; the landed CEnginePrimitiveHandle::Release model (0x0071AF30).
- CTCSummonSpell
  - 0079a660 CancelForCutscene / 0079a6a0 ResumeForCutscene: flag store first, Minion at the unaligned +0x11 (pack(1)).
- CTCSummonableCreature
  - 0079c240 constructor: def pointer member object (immediate zero), bool initialiser, intelligent pointer default ctor,
    then `ParentThing->PDef->GetSubDef(&PDef)` in the body.
- CTCSwitchableNavigation (names confirmed in egor_signatures.tsv)
  - 00787550 OnCreate (slot 4): the map source fetched into a named local first.
  - 00787580 SetAsBlockingNavigation(bool), 007875d0 OnSetInLimbo(bool) (`!inLimbo` -> `sete`), 00787690 InitialActivate
    (slot 5): navigation manager named local; `UniqueID & 0xFFFFFFFFFF` gives the `and ecx,0FFh` on the high word.

## Parked for Fable 5.1 (7)
- 0066b040 CTCSoundPlayer::OnInitialActivate DIFFER(69v61): built stores the def local's zero before `push edx` and re-reads the local after `dec [eax+4]` (`mov ecx,[esp] ; mov eax,[ecx+4]`); retail keeps it in ecx (`dec [ecx+4] ; jne ; mov edx,[ecx]`) | tried: inline release in the destructor, Release() member on the def, local pointer copy in the destructor (both copy forms give the wave-18 `mov esi,[eax+4] ; add eax,4 ; dec esi` form) | next: the landed CTCSummonSpell ??_G release (`def = m_pDef; if (def) { if (--def->m_refCount == 0) def->Destroy(); m_pDef = 0; }`, null store inside the arm), or GetSubDef taking the def pointer by reference
- 0072a970 CTCSpecialAbilities::SetupBulletTime DIFFER(63v63): `mov ecx,[esi+18h]` after the CCharString constructor and before `push 0 ; push 1 ; lea eax` in retail; the lane loads it after the pushes (`lea ecx ; push ecx ; mov ecx,[esi+18h]`) | tried: plain member, inline accessor, local after the name, optimize("s") (right order but frame pointer + byte test) | next: lane gap: argument/`this` scheduling (RTM vs QFE 4035)
- 0072a9b0 CTCSpecialAbilities::SetupForcePush DIFFER(68v68): same AddTC `this`-load order as 0072a970 | tried: optimize("g",off), optimize("sy") | next: lane gap: argument/`this` scheduling
- 0072aa00 CTCSpecialAbilities::SetupHealLife DIFFER(68v68): same AddTC `this`-load order | tried: plain, inline by-value name helper | next: lane gap: argument/`this` scheduling
- 0072aa50 CTCSpecialAbilities::SetupGhostSwordSpell DIFFER(72v72): same AddTC `this`-load order (the rest, incl. the component in esi across the string destructor, matches) | tried: plain scoped-name form | next: lane gap: argument/`this` scheduling
- 0072aaa0 CTCSpecialAbilities::SetupMultiArrow DIFFER(67v67): same AddTC `this`-load order on ParentThing (+4) | tried: plain form | next: lane gap: argument/`this` scheduling
- 0072c680 CTCSpecialAbilities::SetupLightningSpell DIFFER(83v83): same AddTC `this`-load order (CalculateWeaponState + the +0x0c setter match) | tried: plain scoped-name form | next: lane gap: argument/`this` scheduling

## Skipped
- none (no row holds two functions).

## Label corrections
- 004d3b41 "CTCSmokeGenerator::CTCSmokeGenerator" -> CTCCameraPointGeneralCase::CTCCameraPointGeneralCase(CThing*) (vtable 0x0123B194).
- 007f61b0 "CTCSolidToThing::AddThingNotSolidTo" -> CTCTargetingAI::AddThingToNotAttack (list +0x48, guard = landed IsThingAttackable).
- 0066aeb0 / 0066afa0 / 0066aff0 "CTCSummonSpell::ResumeForCutscene" -> CTCSoundPlayer::StopAllAttachedSounds / SetSpecificSamplePitch / SetSpecificSampleVolume (CTCSoundPlayer::PAttachedEmitter +0x0c; Ego_r method signatures).
- 0072a9b0 / 0072aa00 / 0072aaa0 "SetupForceMove" (x3) -> SetupForcePush ("CTCForcePushPower"), SetupHealLife ("CTCHealLife"), SetupMultiArrow ("CTCMultiArrow"), named from the TC each adds (unverified in the PDB).
- 00c35c30 "CTCSpecialAbilities::OnKill" -> free `__stdcall` helper (_unattributed::ForwardIfStateMask217).
- 00bfbbc0 "CTCStealth::OnCreate" -> CLocalDetailLayer::CObject::CObject() (76-byte PDB CObject: bounding sphere +0x34, primitive handle +0x44).
- 00c25cb0 "CTCStealth::OnCreate" -> a 16-byte record's destructor (CParticleHandle, named from its callee).
- 00787580 "CTCSwitchableNavigation::InitialActivate" -> SetAsBlockingNavigation(bool) (not in the vtable; Ego_r signature).
- Callee labels: 0x00686A70/0x00686A80 "CViewBase::GetDisplayEngine" are CTCBase::GetWorld (game+0x24); 0x00686D30 returns game+0x28;
  0x004C73D0 "CVertexBufferWin32::DoSizeof" is CThing::GetPosition; 0x006BDE50 "CGameEvent::AddUBYTE" is CMusicManager::EnableDangerMusic;
  0x00836790/0x008367A0 "CScriptThing::operator==" are the emitter's SetSpecificSamplePitch/Volume; 0x0079DCB0 "C3DMeshInfo::HasPhysicsMesh"
  is the shield component's active test; 0x0079F680 "AddExclusiveObserver" is a +0x0c setter; 0x0049D870 "GFGetPreMainMemoryUsed" is the
  static world-frame getter.
