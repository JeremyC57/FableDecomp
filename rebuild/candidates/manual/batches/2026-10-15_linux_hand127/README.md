# Hand chunk 127 (CCrateTeamMember .. CCreatureAction_CastStrengthMultiStrike): 36 of 40 byte-exact

All 36 matched sources grade RELOCATION_MATCH (grades.txt), every test prints `<addr>_TEST PASS` under g++ and under the
lane's VC7.1 cl.exe (tall.sh / tall.py). One row needs `// Flags: /EHsc` (00a44740). Purity grep (`__asm`/`naked`/`_emit`) clean.

## What matched (36)
- **CCreatureActionBase-derived constructors (7 + 1)** -- 00718650 AimMissileWeapon, 00718710 ReloadMissileWeapon, 007eb2d0
  AddRealObjectToInventory, 00843840 FillBucket, 008438c0 ShowLove, 00842370 AlertAllNearbyToThing, 00865940 BeStunned:
  `CCreatureActionBase(owner, a, b, GetRandomLoadedAnimation(owner, "NAME"), CSoundPair(), c, d)` where 0x006924B0 is a
  `__fastcall(owner, const CCharString&)` thunk onto GetRandomLoadedAnimFromName(name, true). SHAPE: the animation name is an
  IMPLICIT conversion temporary (CCharString ctor with a defaulted length) -- built first, `lea`-re-addressed, destroyed after
  the base call; the CSoundPair is the zeroed inline-ctor temporary. A trailing CIntelligentPointer member is
  `CIntelligentPointer() : CBaseIntelligentPointer() {}` (own vtable, no SetPItem) or `CIntelligentPointer(p) { SetPItem(p); }`.
  00843f50 ActivateQuest(owner, thing, name, flag) uses the 4-arg base 0x00693C70 and a `#pragma pack(1)` layout
  (string +0xa8, bool +0xac, pointer +0xad).
- **CCombatActionBase-derived constructors (10)** -- 008c9410 BanditKingAttack, 008c94d0 BanditKingStuck, 008e45b0
  ScorpionTailWithdraw, 008e4640 ScorpionTailPlunge, 008c39a0 BriarRoseShotFarOutof, 008bbe40 BuildHealLifeSpell, 008bbeb0
  FinishHealLifeSpell, 008bb670 CastPhysicalShieldSpell, 008e6a70 CastStrengthMultiStrike, 008ce340 CastBattleCharge.
  SHAPE: `GetSoundPair(owner, "SWORD_SWING")` is `__fastcall` with a hidden 8-byte result (CSoundPair needs a user ctor);
  a body `SetAnimationName("LIT")` is again an implicit temporary (vtable store scheduled between its pushes and its ctor
  call); `healSelf ? "A" : "B"` selects the literal pointer into eax and builds ONE temporary; init-list members sink below
  the vtable store. CIntelligentPointer must be modelled 8 bytes (vptr + item) or the next member lands 4 bytes low.
  CastBattleCharge's `Dir` copy is a C3DVector with a memberwise user copy constructor (three dword moves straight into
  the member; a POD copy forms the member address once with `lea`).
- **Copy constructors (5)** -- 0084c900 ActivateQuest, 0078bd60 HeroLoadRangedWeapon, 008c9860 BanditKingStuck, 008cf020
  CastBattleCharge, 008cef50 CastAssassinRush: `Base(other)` out of line, own vtable, then memberwise init-list copies;
  CIntelligentPointer copies are the landed `CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }`; a CCountedPointer
  copy is Data/Info + `if (Info) ++Info->m_count`.
- **Initialise / PerformAction / FinishAction overrides (9)** -- 00844b40, 008cc9c0, 008cca60, 008ccb00, 008c8960, 008ed850,
  008eda10, 008e6400 and 00849140: the landed `GetInterface(T** out, type)` template (bitset probe + LowerBound) reproduces
  every lookup; a component pointer used as `this` of a call must come from an INLINE ACCESSOR (`GetPhysics()`,
  `GetGraphicAppearance()`) so it loads straight into ecx (a member read gives `mov eax,[..] ; mov ecx,eax`); a
  `__fastcall(thing, def)` whose def operand retail reads BEFORE the GetPItem call wants a named `long def` local;
  an unchecked `GetInterface(&p, 49)` leaves `p` read back from the frame slot on the not-present path (BeDazed/BeStunned);
  the message post is the 00842f70 shape (two creature locals, the first dead, the manager named) plus a hand-written
  `if (p) { if (--p->m_count == 0) p->Destroy(); }` release of the CDefPointer word.
- **CCreatureActionBase methods (2)** -- 00692b30 UpdateActionMoveToPos: `SetMoveToPos(AnimMovementStep +
  PCreature.GetPItem()->GetPosition())` with SetMoveToPos inlined (`GetWorld()->GetFrame()` static) and an inline
  `operator+` building the sum in a frame temporary; the `else` is a void virtual tail jump. 00692db0 EditUpdate(float):
  the unused float argument, UpdateActionRunningTime inlined, `(long)(RunningTimeSeconds * g_framesPerSecond)` (`fild` of the
  long global first), TotalTime read once.
- **00a44740 CSuspendableProcessMicrothread ctor** -- an unoptimised TU (`#pragma optimize("", off)`) with a `throw` of a
  one-byte exception object (`/EHsc`): `PMicrothread = 0; PMicrothread = CreateMicroThread(entry, this); if (!PMicrothread)
  throw CFibreException("Failed to create fibre"); g = g + 1;`.
- **00dcec70 CCrateTeamMember::GoOnPatrol** -- void; `if (MemberState != 0) { IsOutOfAction(); return; }` becomes the `jmp`
  tail call; the nested `script->SetPatrolTarget(&m_thing, script->FindPatrolPosition(3.0f, true))` shares one vtable load
  (member reloaded after the inner call), the separate third call reloads the vtable; counters
  `--MyTeam->StateCounter[MemberState]; MemberState = 1; ++MyTeam->StateCounter[1]`. Retail layout has the script
  interface at +4 and a CScriptThing at +8 (the PDB's first 20 bytes are "padding").

## Parked for Fable 5.1 (4)
- 00692aa0 CCreatureActionBase::UpdateActionMovement DIFFER(132v132): retail `call [eax+0ECh] ; add esi,68h ; mov ecx,edi ; push eax ; push esi ; call [ebx+88h]`, ours `call ; push eax ; add esi,68h ; push esi ; mov ecx,edi ; call` | tried: nested `GetPhysics()->MoveToPos(&NextMovementPos, GetMovementWorldFrame())`, named `pPhysics`+`frame` locals (loses ebx), inline non-virtual wrapper (loses ebx) | next: lane gap: scheduling of the pushes after the argument call (compiler build)
- 008b9dd0 CCreatureAction_CastPhysicalShieldSpell::Initialise DIFFER(125v125): retail `mov edi,ecx ; call Initialise ; lea esi,[edi+8]` (this=edi, &PCreature=esi), ours `mov esi,ecx ; ... ; lea edi,[esi+8]`; every other instruction identical | tried: scoped named AddTC name (0x008C4250 form), implicit temporary (0x008C08B0 form), an explicit `CBaseIntelligentPointer&` local after the base call | next: lane gap: esi/edi tie-break for `this` vs the pointer-member address (the matched 008cca60 sibling got this=edi with the same source style)
- 00692ec0 CCreatureActionBase::HasPrecedingAction DIFFER(95v97): retail's early `return false` is its own `xor al,al ; pop esi ; pop ecx ; ret` block at the end, ours jumps to the shared epilogue returning the just-tested al; the `GetInterface(&q,70) && !q->Has()` body matches byte for byte (`mov eax,1` / `xor eax,eax` = an int-typed `&&`, GetInterface returning long) | tried: `return !q->Has()` (setcc), ternary (setcc), long-GetInterface `&&` (closest), slot 36 returning unsigned char | next: find the form whose early false is not folded into the tested register -- maybe the first test is on a `long`-returning slot compared differently, or `return false` through a bool local
- 007c3070 CCreatureActionBase::GetActionSpeedParameter DIFFER(112v112): retail `mov esi,[esp+8] ; je ; lea ecx,[esp+0Ch] ; push ecx ; lea edx,[esp+14h] ; push edx ; mov ecx,esi ; call` and `mov eax,[esi] ; call [eax+4]`, ours `mov ecx,[esp+8] ; mov esi,ecx ; je ; lea edx ; push edx ; lea eax ; push eax ; call` and `mov edx,[esi] ; call [edx+4]` | tried: hand release + `return value` (load at the end), inline CDefPointer dtor + operator-> (full inlined release), hand release + `result = value` after the call, register `result` + addressed `value` copied inside the found branch (closest: frame, zeroing and the edi result now match) | next: lane gap: register choice around the def-pointer local; maybe the lookup is a `__thiscall` on a different object expression (`def.m_p->` vs the local)

## Skipped
- none (no row spans two functions).

## Label corrections
- 00718710: manifest CCreatureAction_AimMissileWeapon -> CCreatureAction_ReloadMissileWeapon (vtable 0x012656A4, "PROJECTILE_WEAPON_LOAD").
- 00843840: manifest CCreatureAction_AddRealObjectToInventory -> CCreatureAction_FillBucket (vtable 0x01274E2C, "STANDARD_SCARED").
- 008438c0: manifest CCreatureAction_AddRealObjectToInventory -> CCreatureAction_ShowLove (vtable 0x01274F4C, "STANDARD_EXCITED").
- 0078bd60: manifest CCreatureAction_BattleChargeImpact -> CCreatureAction_HeroLoadRangedWeapon copy constructor (vtable 0x0126B9D4).
- 008e45b0 / 008e4640: manifest CCreatureAction_BriarRoseShotFarInto -> CCreatureAction_ScorpionTailWithdraw (0x01286334) / CCreatureAction_ScorpionTailPlunge (0x01286494).
- 008bbeb0: manifest CCreatureAction_BuildHealLifeSpell -> CCreatureAction_FinishHealLifeSpell (vtable 0x0127CD34, code 145).
- 00a44740: manifest CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge -> CSuspendableProcessMicrothread::CSuspendableProcessMicrothread(bool, long, long) (vtable 0x0129D440; base 0x00A445D0 is the landed CASuspendableProcess ctor).
- Callee labels that are propagated noise: 0x006924B0 ("CMainGameComponent::BeginInputLoading") is the GetRandomLoadedAnimFromName(name, true) thunk; 0x00F35B30 ("CCreatureAction_TrollWhackGroundBase::Initialise") is a CCrateTeamMember bool query; 0x006633A0 ("_Dest_val<..CFaction>") is a CThingCreatureBase release of its counted pointer at +0x100 (ClearCurrentAction here); 0x0073A840 ("CNavQuadTreeNode::RemoveNeighbours") is CTCCreatureModeManager::RestartCurrentMode; 0x006A1270 ("CThing_Array_GetFirstPhysicalSwitch") is a bool query on the interface-70 component; 0x007CAB30 ("TCHeroReceiveItems_AddItemToInventory") takes (CThing*, bool) on the interface-203 component; 0x00692910 is `PCreature.GetPItem()->` the creature's definition-manager getter (0x004C7990); 0x004C73D0 is CThing::GetPosition (same body as 0x006A4D00).
