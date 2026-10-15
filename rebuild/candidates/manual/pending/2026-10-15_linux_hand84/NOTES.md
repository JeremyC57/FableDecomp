# Hand chunk 84 (CTCCarrying .. CTCCreatureGroup): 36 of 40 byte-exact

All 36 sources in src/ grade MATCH (1) or RELOCATION_MATCH (35) in grades.txt; all 36 behaviour tests print
`<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). No `// Flags:` line was needed;
two sources carry an in-source `#pragma optimize` (00453de0, 006543af). Many chunk labels are propagated
across byte-identical bodies: the real class is given first below (see "Label corrections").

## What matched (36)
- **CTCChest** (0x0126957C)
  - 00750010 Persist(CPersistContext*, bool): CTCContainer's persist hook first, then the "ChestOpen" transfer. The
    `false` default is a one-byte temporary bound to a const reference, homed in the dead argument slot.
  - 00750730 vfunc_4: under `if (ChestOpen)` clears the interface-9 component's +0x10 flag. The landed out-parameter
    GetInterface (0x007504E0 shape) must be `__forceinline` -- under the outer guard VC calls it out of line.
  - 007508d0 ctor: CTCContainerRewardHero base, COpenCloseHelper member object with an inline ctor (its stores follow the
    vtable store), CIntelligentPointer at the unaligned +0x31 (`#pragma pack(1)` on the pointer class), cutscene call in the body.
- **CTCDRegionExit** 00778fa0 ctor: plain initialisers (the three `true` bytes share cl, scheduled before the vtable
  store), 1.0f radius immediate, pack(1) intelligent pointer at +0x2a.
- **CTCClock**
  - 00732730 GetHandAnimationUpdateAmount: `time > last` is `fcom ; test ah,41h`; the def-type tests are an `==` chain
    on the type read once (a `switch` gives `dec eax ; je`); the fall-through `fstp st0` then the 0.0f pool word.
  - 007327b0 vfunc_12: `PDef = other->PDef` through the inlined CDefPointer::operator= (landed 0x0083E0D0 family). MATCH.
- **CTCCombat** (engine/CTCCombat.h used for every row)
  - 007332a0 SetPossibleQueuedAction: `if (test) { call; return true; } return false;` with the parent re-read.
  - 00733410 GetCombatReducedDamage: one `&&` chain (virtual slot 77, IsPerformingAnyAction), action read once for the
    two compares, `return 0.0f` is the 0x0122DEDC pool load; the unused second argument only shows in `ret 8`.
  - 00733750 GetBlockAbilityIterator / 00734880 GetPrioritizedBlockAbilityIterator: a zero-initialised one-word iterator
    local in the `push ecx` slot, `if (Find(&it, wc)) { *out = it; return true; } return false;`.
  - 00733910 GetComboMaxStage: the CGetMaxComboStage functor has a USER constructor, so the `__fastcall` for_each
    (0x00738F30, built /Gr) returns it through the hidden pointer and takes it by value as two pushed dwords.
  - 00734050 SetBlocked: `type == 2 && GetInterface(48, &c)` (forced inline), `(long)(g_FramesPerSecond * 2.0f)` =
    `fild ; fadd st0,st0 ; __ftol2`; deregistration unconditional.
  - 00734570 SetNextComboChainPos: the combo out pointer homed in the consumed argument slot; if/else on the member for
    `pos == -777 ? 1 : pos + 1`; one shared reset tail.
- **CCombatManager** 006e9960 GetNumberOfAttackers: success path as the `if` body; the 64-bit key copied high word first
  into the 8-byte frame and passed by const reference.
- **CTCContainer** 0074fde0 `??_G` (leaf `??_GCTCContainer`): no own vtable store, so CTCBase (out-of-line dtor) is the
  FIRST MEMBER of a non-polymorphic class deleted through a helper (GOTCHAS 24m); STLport `_Destroy` + free inline.
- **CTCContainerRewardHero**
  - 0074e240 Persist (label OnSerialise): flag held in ebx, `true` default temporary homed in the flag's own slot, base hook last.
  - 0074e500 HasReward: `long gold = 0;` before the inline GetInterface(204) (`xor eax,eax` first); `size() > 0 || gold > 0`
    with a `long` 1/0 result.
- **CTCCoopSpirit**
  - 0066ffd0 GetFocalPos: C3DVector with a user copy ctor returns by hidden pointer; a NAMED local built field by field.
  - 00670050 ctor; 006707a0 OnHit / 006707e0 OnStrike: the particle name a named local in the `push ecx` slot, the def's
    table position pushed before the hidden result pointer (twins differ only in the def member +0x28/+0x2c).
- **CTCCrateStack** 007b6660 GetNumCrates (`!= 0` -> `neg ; sbb ; neg`), 007b6690 GetSpaceLeft (`first ? 1 : 2` ->
  `neg ; sbb ; add eax,2`), 007b6750 GetNextCrate (zero-initialised result local assigned per arm -> both arms tail-jump).
- **CTCCreatureFlyOnSpline** 007dcf90 ctor: member objects with inline ctors, all stores after the vtable store.
- **CTCTextureDecal** 0070a520 ctor and **CMovieSubtitles** 009bdba0 ctor: STLport list sentinel malloc'd and self-linked
  inside the list's inline ctor, then stored; malloc's `add esp,4` delayed past the member stores.
- **CTCHitOnCollision** 007c81b0 ctor: zero bytes share al, scheduled before the `1` byte store.
- **NPlayerGui::CMultiStatBar** 006543af ctor: `#pragma optimize("y", off)` + `("s", on)` (ebp frame); the list's
  allocator temporary homed in the high byte of the argument slot (`lea eax,[ebp+0Bh]`); CRGBColour stored R,G,B,A.
- **CCombatAbilityBlockingData** 00453de0 ctor: `#pragma optimize("s", on)` (`push dword ptr [esp+8]`, immediate byte stores).
- **CTCCreatureGenerator** (local model: engine/CTCCreatureGenerator.h is 0x10..0x1c bytes high, see LESSONS)
  - 00749e40 CloneFromCreator: implicit memberwise `GenParams = other->GenParams` inlined (CCharString operator= out of line).
  - 00749ea0 OnCreate: `seconds * rate` = `mov ; imul [rate]`, `-(a * rate)` = `imul ; neg`, LevelPersistent bitfield set.
  - 0074a190 HasHitCreationLimit: three sequential `if (lim != -1 && n >= lim) return true;`, STLport list distance loop
    compared unsigned.
- **CVectorMap<long, CIntelligentPointer<CThing> >** 006a2b50 FindValue: top-tested pointer loop, `last` re-read from
  `this` after each call.
- **CTCCreatureGeneratorCreator** 007c9720 ctor: zeroes share ebx, pack(1) intelligent pointer at +0x11.
- **CTCCreatureGroup** 00812f20 IsMember: two SEQUENTIAL early `return false` tests put the shared `xor al,al` exit after
  the second test; `find(id) != end()` becomes `xor edx,edx ; setne dl ; mov al,dl`.

## Parked for Fable 5.1 (3)
- 00be1e20 CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager DIFFER(77v77): retail forwards the four zero dwords of the frame temporary all through edx (`mov edx,eax` then four `mov [ecx+n],edx`); the lane alternates edx/eax and schedules `lea ecx,[esi+14h]` before `mov edx,eax` | tried: member copy-constructed from a temporary (elided, 51 bytes), user memberwise operator= (elided), implicit assignment in the body (77v77) | next: lane gap: copy-register allocation
- 005e3c60 vector<pair<unsigned long, CAnimationEntry> >::resize DIFFER(78v68): the default pair temporary's counted-pointer release -- retail `mov ecx,[esp+10h] ; test ecx,ecx ; je ; dec dword ptr [ecx+4] ; jne`, the lane `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx` | tried: `--Object->m_count` in the dtor, a local pointer copy, Release() on the pointee, Release() on a CRefCounted second base | next: lane gap: counted-pointer release register form (4 attempts used)
- 00c6ac90 CAudioFilterStage::Process DIFFER(39v38): retail loads the gain into ecx (`mov ecx,[esp+10h]`) for 0x00C6AAE0, which reads ecx as a parameter and returns with a plain `ret` while the caller cleans four stack arguments (`add esp,10h`) | tried: `__cdecl` callee | next: lane gap: LTCG custom calling convention

## Skipped
- 00750960 CTCChest::vfunc_10 (32-byte row): two functions -- `mov eax,[ecx] ; jmp [eax+24h]` (5 bytes), int3 padding,
  then `mov al,1 ; ret` at 0x00750970.

## Label corrections
- 00c6ac90 "CTCCarrying::SetAllCarriedThingsAsBypassFilter" is an audio filter stage (MMX IIR, CAudioFilterStage::Process).
- 00778fa0 "CTCChest::CTCChest" is CTCDRegionExit::CTCDRegionExit (vtable, rtti-vtables.tsv).
- 005e3c60 "CTCCombat::AddMeleeFlourishAbility" is vector<pair<unsigned long, CAnimationEntry> >::resize(n).
- 006e9960 "CTCCombat::SetPossibleQueuedAction" is CCombatManager::GetNumberOfAttackers.
- 00734880 "CTCCombat::GetBlockAbilityIterator" (shared with 00733750) is GetPrioritizedBlockAbilityIterator (calls 0x00734340).
- 0074fde0 "CTCContainer::vfunc_0" is the `scalar deleting destructor` (??_GCTCContainer).
- 0074e240 "CTCContainerRewardHero::OnSerialise" and 00750010 "CTCChest::vfunc_1" are the Persist overrides.
- 006707e0 "CTCCoopSpirit::OnHit" (shared with 006707a0) is OnStrike (reads OnStrikeParticle).
- 006543af "CTCCreatureFlyOnSpline" ctor is NPlayerGui::CMultiStatBar::CMultiStatBar; 0070a520 is CTCTextureDecal; 007c81b0 is
  CTCHitOnCollision; 00be1e20 is CEnginePrimitive2DViewportManager (only 007dcf90 is really CTCCreatureFlyOnSpline).
- 006a2b50 "CTCCreatureGenerator::RemoveCreatureTriggerer" is CVectorMap<long, CIntelligentPointer<CThing> >::FindValue.
- 00453de0 "CTCCreatureGeneratorCreator" ctor is CCombatAbilityBlockingData; 009bdba0 is CMovieSubtitles (only 007c9720 is
  really CTCCreatureGeneratorCreator).
- Callee labels found propagated: 0x00686800 "CEngineSubPrimitive2DClockSprite" = CTCBase::CTCBase(CThing*); 0x00686830
  "~CChunkedFileChunk" = CTCBase::~CTCBase; 0x004C73D0 "CVertexBufferWin32::DoSizeof" = CThing::GetPosition; 0x00661F20
  "CTCUserActionPointBase::IsActionPointFree" = CThingCreatureBase has-queued-actions; 0x00664490 "PhysicsSegment_AssignSegment"
  = CThingCreatureBase queue action; 0x0074E6B0 "CTCActivationTrigger" = CTCContainerRewardHero ctor; 0x00812790
  "Faction_FindMemberIndex" = map<u64, CGroupMemberData> find; 0x00453572 "CDisplayManager::CopyBackBufferToTexture" = set<EWeaponClass>::set().
