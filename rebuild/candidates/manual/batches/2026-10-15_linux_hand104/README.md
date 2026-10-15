# Hand chunk 104 (CTCThingOwner .. CTCVolumeContainmentTracker): 36 of 40 byte-exact

All 36 src/ rows grade MATCH/RELOCATION_MATCH (grades.txt) and their tests print PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). 3 rows parked in differ/, 1 skipped. Twelve rows carried propagated labels; the real
classes/names are in "Label corrections" and on line 1 of each source (with a `// Leaf:` selector).

## What matched (36)
- CTCThingOwner (3):
  - 0071b240 ClearDeadEntries -- erase every null/dead entry of the owned-object list. The retail list is the VC6-era
    one-pointer list (no size word), so erase = unlink + explicit in-place virtual destroy (`push 0 ; call [vt]`) + free;
    the IS_THING_DEAD nested-if idiom; head re-read at the loop bottom. Matched first try.
  - 0071b2d0 OnSetInLimbo / 0071b2a0 vfunc_16 (SetAsDrawable body) -- `for_each(begin(), end(), CSetThingInLimbo(b))`:
    the /Gr for_each is `__fastcall` with the hidden functor result in ecx (re-using the dead argument slot); the
    iterators need a user copy constructor (built in reserved slots), the 1-byte functor a constructor only (pushed as a dword).
- CTCTrapBase (6):
  - 00787d4f OnInitialActivate -- GetSubDef + trap type + optional owner call + InitTrap + flag bit; the
    `or byte ptr [eax+59h],2` read-modify-write only comes out under `optimize("s")`.
  - 00787d94 OnKill -- base OnKill then IS_THING_ALIVE(PObstruction) -> Kill(true).
  - 0078839a / 007898a3 SetThingAsImmune -- frame-pointer size TU (`y` off + `s`): `ImmuneThings.push_back(CIntelligentPointer(&thing))`
    with the bare temporary's constructor result pushed straight from eax.
  - 0078888a UpdateTrigger -- `/Os` (`cmp byte ptr`), `GetWorld()->GetWorldFrame()` (static through an object keeps the
    GetWorld call), nested ifs and a tail jump.
  - 0078918d constructor -- the wave-25 STLport vector model (_Vector_base initialisers + _STLP_alloc_proxy end pointer
    built from an allocator temporary) fixes the `mov ecx,edi` / `lea ecx` scheduling; Active stored last = body assignment.
- CTCTrespasser (1): 007badd0 GetDeedType -- three-case switch (`sub eax,0 ; je ; dec ; je ; dec ; je`), MATCH.
- CTCTrophy (3): 006dda20 OnCreate (GetSubDef, list clear, `Mountable = !FakeTrophy`); 006ddbf0 OnSerialise (slot 1:
  long default as a named local, bool literal default homed in the dead context slot); 006ddcd0 PlaySoundW (explicit
  HasTC test `&&` the inline out-parameter GetInterface template: one load of the bitset word tested twice).
- STLport deque (2, labelled CTCTrophy / CTCUserActionPointBase): 0059b039 `back()` (frame-pointer size TU, 16-byte
  iterator copied with four movsd, inline `operator--` = out-of-line `_M_decrement`), 0059b576 `_M_push_back_aux_v`
  (value copied to a local first, placement-new null test, `_M_set_node`).
- CTCTurncoat (1): 007c7840 OnInitialActivate (slot 6) -- `def * health + extra` (right operand loaded first), clamp
  with `test ah,5 ; jp` = ordered `<`. MATCH.
- CTCTurncoatSpell (1): 007c75e0 FinalCleanup -- IS_THING_ALIVE on the out-of-range effect (+0x84: retail = PDB - 4
  after Timer), fade it, tail-jump StopTurncoatingEffects.
- CTCUserActionPointBase (1): 007fec60 constructor -- STLport vector model again; two longs in the init list.
- CTCVillage labels (14, six of them other classes): 00658acb GetSchoolTimes / 00658c5b GetTavernTimes (`/Os`, `__forceinline` counted CDefPointer copy
  constructor/destructor/GetPDef: two add-ref'd by-value temporaries, the first one's release has no null test in
  both compilers); 00658a02 GetFirstUnAssignedPatrolTask (`/Os` jmp-into-test list loop, `&&` of two virtuals);
  00659104 IsVillagerPerformingVillageTask (`/Os` shared epilogue, out-of-line bitset test, crate-task loop);
  00659c32 StartSecuritySweep (`/Os`; list size() is the inline distance walk; the count is a named local taken before
  the urgency store); 00659ea9 RemoveThingFromAllTasks (`/Os` keeps every list address in eax); 0065ce56
  RemoveApologiseableCrimes (it/next iterator pair: `next = it` before the entry test, `++next` before the erasing call,
  `it = next` at the bottom; STLport `_M_increment` is `__cdecl`); 0065ce88 OnInitialActivate (slot 6,
  `if (Limbo) SetVillageLimbo(Limbo)`); plus 005c2e68, 0065f01d, 009cfb70, 00ae34d0, 009f7910/009f7940 which were
  other classes under CTCVillage labels (see corrections): CTCInventoryWeapons::RemoveAllConfiscatedItems (base call +
  two `and` zero stores), CTCDummyVillager::Evict (`Creature = (CThing*)0` implicit-conversion temporary + 64-bit UID
  clear, frame size TU), CBankFile::PostInitEntries (inline CVectorMap::Sort with the /Gr sort taking first/last in
  ecx/edx and the 1-byte comparator by value), the 0x00AE34D0 twin of that Sort, and two unoptimised list `begin()`
  bodies (`optimize("", off)`, first node in a named local).
- CTCVillageMember (2): 004daee9 constructor (`/Os`, `PVillage(0)` through the out-of-line converting constructor,
  `VillageUID(0)` as two `and` stores); 007ee240 IsAttachedToVillage (`return VillageUID != 0` gives the full-register
  `mov eax,1 / xor eax,eax` arms).
- CTCVolumeContainmentTracker (2): 00785110 constructor (theme-channel desc with an inline 3-field constructor, the rest
  in the init list); 0077c920 is really the CTCHighlightItem constructor (Running/StartRunning/LifeTimer stored after
  the emitter pointer's constructor = body assignments, in that order).

## Parked for Fable 5.1 (3)
- 006dee80 CTCTrophy::UpdateGainedWitnessSounds DIFFER(92v94): ours `mov [esp+8],fps ; call ; mov [esp+0Ch],frame ; fild fps ; fmul [def+44h] ; fiadd frame`, retail `mov [esp+0Ch],fps ; call ; mov [esp+8],frame ; fild frame ; fild fps ; fmul ; faddp` | tried: plain expression, def/fps/frame named locals, explicit (float) casts, operands swapped | next: the frame as a named `float` local (`float frame = (float)GetWorldFrame();`) so it is converted and kept on the FPU stack before the product
- 00659295 CTCVillage::InitialiseToggleTorchTypes DIFFER(79v77): ours `mov eax,[eax+4] ; cmp byte ptr [eax+1Ch],0 ; sete cl ; push ecx ; mov ecx,eax`, retail `mov ecx,[eax+4] ; cmp byte ptr [ecx+1Ch],0 ; sete al ; push eax` | tried: pointer-returning PeekTC + named light local, `__forceinline` PeekTC, inline CTCLight::ToggleActive on the returned pointer, out-parameter PeekTC | next: lane gap: register choice for the component (the 1-byte sete target), unless a `bool` local for `!Active` evaluated first changes it
- 007ee370 CTCVillageMember::OnCreate DIFFER(88v86): ours zeroes the def local before the argument pushes and holds the def object in edi (extra push/pop), retail zeroes it after the pushes and reloads it into the dead `this` register esi after `add esi,0Ch ; push esi` | tried: CDefPointer local with destructor (re-reads the escaped local), plain holder + pointer local + inline release, the same under `/Os` (worse: frame), an inline SubDef-and-release helper | next: lane gap: retail treats the address-escaped local as unchanged across GetSubDef (whole-program / LTCG alias knowledge) -- try a `__fastcall`/`const` GetSubDef declaration or a by-value def holder returned from GetDef

## Skipped
- 004a1c8e (CTCTrophy::OnShowTrophyInitialise label): not a function entry -- it starts with `lea ecx,[ebp-14h]` and ends
  in the `fs:[0]` restore/epilogue of an EH function; the catch funclet 0x004A1C41 returns the continuation 0x004A1C86
  just before it, so the row is the catch-continuation tail of the enclosing function (no callers, no references).

## Label corrections
- 0059b039 "CTCTrophy::OnShowTrophyInitialise" -> STLport deque<4-byte T>::back() (26 callers in 0x00595xxx..0x0059Axxx).
- 0059b576 "CTCUserActionPointBase::AllocateBuffer" -> the same deque's _M_push_back_aux_v(const T&); its callee
  0x0059B4D8 "CTCUserActionPointBase::GetNumberOfActionPointsInUse" is _M_reserve_map_at_back(size_type).
- 0059ac41 "lua_getglobals" -> _Deque_iterator_base::_M_decrement().
- 005c2e68 "CTCVillage::InitialiseSecuritySweeps" -> CTCInventoryWeapons::RemoveAllConfiscatedItems (vtable 0x01253FF4
  slot 74); 0x005BF969 ("Unwind@00fbdab0") is the base CTCInventoryBase::RemoveAllConfiscatedItems.
- 0065f01d "CTCVillage::InitialiseSecuritySweeps" -> CTCDummyVillager::Evict().
- 00658c5b "CTCVillage::GetSchoolTimes" -> CTCVillage::GetTavernTimes (def +0x94/+0xA0); 0x00658716 is CTCVillage::GetTimes.
- 00659c32 "CTCVillage::RecalculateSecuritySweep" -> CTCVillage::StartSecuritySweep(bool) (the landed caller
  0x00849250 declares it as RecalculateSecuritySweep(long)).
- 0065ce56 "CTCVillage::IterateCrimesAndRemoveApologisable" -> CTCVillage::RemoveApologiseableCrimes(long); 0x004292C0
  ("CMemoryAllocatorVariableSize::GetNoAllocatedAreas") is STLport _Rb_global::_M_increment.
- 0065ce88 / 007c7840 "vfunc_6" -> OnInitialActivate; 006ddbf0 "vfunc_1" -> CTCTrophy::OnSerialise; 007ee370 "vfunc_4" -> CTCVillageMember::OnCreate.
- 007898a3 "CTCTrapBase::SetThingAsImmune" -> SetThingAsImmune of the trap obstruction's component class (ImmuneThings +0x2C); kept the catalog class name in the source.
- 009cfb70 "CTCVillage::OnInitialActivate" -> CBankFile method (SymbolCRCIndices sort + PackRuntimeData), name inferred as PostInitEntries.
- 00ae34d0 "CTCVillage::OnInitialActivate" -> an unidentified class's CVectorMap::Sort + member call (descriptive names).
- 009f7910 / 009f7940 "CTCVillage::GetBestTaskForThing<CVillageTaskCarryCrate>" -> unoptimised retail list begin().
- 0077c920 "CTCVolumeContainmentTracker::CTCVolumeContainmentTracker" -> CTCHighlightItem::CTCHighlightItem (vtable 0x0126AF14).
- 00686800 "CEngineSubPrimitive2DClockSprite" = CTCBase::CTCBase(CThing*); 00686a70/00686a80 = CTCBase::GetWorld(); 0049d870 = static world-frame getter (all already known from landed sources).
