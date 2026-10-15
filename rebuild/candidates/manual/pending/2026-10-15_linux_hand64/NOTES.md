# Hand chunk 64 (CParticleEmitterDatabase .. CPatchTesselationEdgeStrip): 38 of 40 byte-exact

All 38 sources in `src/` grade MATCH (10) or RELOCATION_MATCH (28) with no pragma or flag. Their tests in `test/`
print PASS under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). `payload.json` and `oracle.tsv`
were written by `finish.py`. The two parked rows are in `differ/`.

## What matched (38)

- **CParticleSystem: component-map walks** (00af3ee0 GetApproximateTotalSplineLength, 00af3fb0
  GetBoundingSphereWorldSpace, 00af4010 Initialise, 00af4070 EnterTimelineEnd, 00af40b0 PreparePrimitives, 00af4320
  ClearAllPrimitives). Each one forwards a virtual call to every enabled component of the 24-byte
  `pair<EPSCType, CComponentInstance>` vector map at +0x0c. The calls are slots 31, 19, 28, 25, 13 and 29, and they
  get `this` plus `&UpdateParameters` (+0x24). Shape: retail walks two pointers, the entry (esi) and the component
  state (`lea edi,[esi+8]`). Every single-iterator form folds into one register, whether it uses a pointer, an
  iterator object or a reference to the instance. The form that matches is an explicit
  `SComponentState* state = &it->second` stepped by `sizeof(SComponentEntry)`, in a do/while behind an entry test,
  with `last = m_componentsLast` re-read before the increment. The end is loaded into eax at entry when it comes
  from a local (`last`). It is compared from memory when the entry test reads the member. Other details:
  `found |= f()` reproduces `or bl,al`, and `Initialised &= ok` on a `bool:1` reproduces `or al,0FEh ; and cl,al`.
  The bounding-sphere call order is (sphere, time, this, &params). The PDB's params argument is unused in both
  Initialise and GetBoundingSphereWorldSpace.
- **CParticleSystem: lists and transforms.**
  - 00af58a0 ClearAllParticles and 00af58d0 ClearAllDecals: Clear*(it) runs over the STLport list at +0x1c / +0x20.
    Retail keeps the walk in esi and refreshes an address-taken iterator in the frame after each step; it was built
    with whole-program optimisation, so it knows the callee leaves the iterator alone. `for (...; ++it) Clear(it)`
    re-reads the iterator from the frame after the call, and a temporary copy stores it before the call. Walking
    the node and writing `it._M_node = node` after each step matches.
  - 00af5330 DeleteParticle and 00af5690 DeleteDecal: Clear, `delete *it`, the inlined STLport `erase` (free),
    then `--NoActive*`, and the iterator returned through the hidden pointer. CDecal has a non-virtual destructor
    and a static `__fastcall operator delete`.
  - 00af8f80 ScaleParticleDimensions: the macro `MAX(MAX(x,y),z)`. Retail evaluates the inner compare twice, so a
    function or reference max does not fit.
  - 00b05840 GetUnscaledWorldTransform: the identity is two CHAINED assignments
    (`m1 = m2 = ... = 0.0f; m0 = m4 = m8 = 1.0f;`). A chain stores right to left, which is retail's reversed
    store order with one register.
  - 00b05980 GetParticleWorldTransform: `m *= s` on the diagonal.
  - Retail layout used throughout: vector map 16 bytes, lists 4 bytes, UpdateParameters 0x24, flags +0x50
    (StayWithEmitter bit 6, ScaleParticles bit 7), Scale +0x60.
- **CParticleEmitterList / CParticleEmitterManager.**
  - 00aeaf20 / 00aeaf80 ClearLowestPriorityNonVisibleEmitterParticles / ...Decals: an STLport `reverse_iterator`
    over Active. `*rit` reads `current->prev`, and rend is begin() re-read from the sentinel on each pass.
    Initialised is bit 8 of the emitter's ushort flags at +0x40.
  - 00aeb000 GetParticleStats: STLport `size()` as a distance loop. Sleeping (ushort bit 10) is read through an
    inline `bool IsSleeping() { return Sleeping; }`, which gives `xor edx,edx ; mov dx ; shr ; test dl,1`. A direct
    bitfield test gives `test byte ptr [ecx+41h],4`.
- **CParticleEmitterDatabase.**
  - 00ae8bf0 constructor: the members' inline constructors share one zero register, and `CurrentHandle = 1` is a
    body assignment.
  - 00ae8980 OpenBank: `EmitterBank.Reset(new CParticleDataBank)`. The bank's vptr must sit on the CBankFile root,
    because declaring the virtuals in the derived class adds 4 bytes and an `add ecx,4`.
  - 00ae85b0 GetEmitterTemplateHandleFromName: the `ToUpper()` temporary goes in the dead argument slot, and
    `found ? handle : 0` gives `neg/sbb/and`.
- **CParticleSimulation.**
  - 00adfc60 and 00a4cd70 RelocateData: `p = p - old + new` under null tests.
  - 00adf2a0 AllocateArrays: the PDB layout is exact at retail offsets. The handle has to be read into a local
    before `MemoryPool->GetPointerFromHandle` (the inline member read is pushed before the pool load). The
    accelerations pointer is a local stored AFTER the alphas are derived from it, which reproduces `add ecx,eax`.
- **CPatchTesselationEdgeStrip-labelled destructors** (00459abe, 00459b73, 004ddf6f, 004e0d7e, 004e424b), all built
  with `/Os` (`pop ecx`).
  - Freed blocks are `__forceinline` array-record destructors, or body frees, run last to first. The base
    destructor is a tail `jmp`.
  - In 004ddf6f the intelligent pointer is destroyed before the frees, so all three are members.
  - The 0x01230BA0 bodies are `__declspec(novtable)` defs over CDefPointeeBase's empty inline destructor, the
    landed 0x0044F81A model.
- **Others.**
  - 004c7d50 CThing::OnDie: `AddingTCsPermitted = false; for_each(first, last, CCallOnDie()); = true;`. The functor
    is VALUE-initialised, with no user constructor, which gives the `mov byte ptr [esp+4],0` store.
  - 004af560 CQuestManager::IsQuestCardActive: list walk; the CDefString temporary goes in the name's dead slot.
  - 007f8be0 CTCAnimatingObject::ClearQueuedAnims: STLport `clear()` as `tmp = cur; cur = cur->next; free(tmp)`.
  - 0049bdc0 CParticleEnvironmentInteraction::GetCameraPos: the class really is `#pragma pack(1)` (PDB CameraPos
    +0x15), and the result is an out reference.
  - 00743780 CModeDragonFlyingBase `??_G`: novtable derived class, non-virtual destructor and a `delete` helper.
  - 00cacb80 DirectShow `CUnknown::CUnknown`: AM_NOVTABLE, the empty CBaseObject base, and a dllimport
    InterlockedIncrement.
  - 00a39cc0 CWADFile::Open: a `ConvertToCharString()` TEMPORARY passed to an inline `GetNameCRC(const CCharString&)`
    with two null-case-first accessors. A named local reloads from the frame, merges the two null tests and
    destroys after the FilePos store.
  - 00b81e20 CEnginePrimitiveSimulationInfo::ClearSimulations: VC6 `erase(begin(), end())`, with last read before
    first (agent 54's model).
  - 00c1bed0: CProfileLock plus a `__stdcall` notify with no EH frame.
  - 0056d300 / 0056d720: STLport tree erase (0x0042A34E helper, three-string value).

## Parked for Fable 5.1 (2)

- 006aee40 CTCRegionFollower::GetFollowers DIFFER(92v90): retail homes the push_back item AND the SScalarTag at the dead `out` argument slot (`mov [esp+10h],eax ; lea eax,[esp+18h] ; lea edx,[esp+1Ch]` = one address, no `push ecx` frame), the lane gives the item a fresh `push ecx` slot and the tag the arg slot | tried: retail push_back(const T&) with an rvalue `Get()` item, push_back(T) by value | next: lane gap: empty tag temporary overlaid on a live argument slot (GOTCHAS 24l)
- 006d5850 CTCFollowed::GetFollowers DIFFER(92v90): byte-identical shape to 006aee40 (CullNullFollowers 0x006D5260): the item and the tag share the dead argument slot in retail, the lane adds a `push ecx` slot for the item | tried: the 006aee40 forms | next: lane gap: empty tag temporary overlaid on a live argument slot (GOTCHAS 24l)

## Skipped

- None. No row spans two functions; every body ends at its `ret`/`jmp` before the padding.

## Label corrections

- 0x004C7D50 "CParticleSystem::Kill" (a BSim guess) is **CThing::OnDie**: CThing vtable 0x01239ABC slot 11, a for_each that calls TC slot 13 (OnDie).
- 0x004AF560 "CParticleSystem::ClearAllDecals" is **CQuestManager::IsQuestCardActive**-like (`this` = quest manager singleton 0x013B89FC, list ActiveQuestCards +0x5c; caller GiveHeroQuestCardDirectly).
- 0x006AEE40 / 0x006D5850 "CParticleSystem::ClearAllDecals" are **CTCRegionFollower / CTCFollowed::GetFollowers(vector<CThing*>*)**: CullNullFollowers, then a push_back of each Followers entry.
- 0x007F8BE0 "CParticleSystem::ClearAllDecals" is **CTCAnimatingObject's QueuedAnims.clear()** (only caller: PlayAnimation 0x007F90B0).
- 0x00AF58A0 "ClearAllDecals" is **CParticleSystem::ClearAllParticles** (Particles +0x1c, ClearParticle). 0x00AED140 "CParticleEmitter::ClearAllDecals" is ClearAllParticles and 0x00AED1B0 "HasAllocatedDecals" is HasAllocatedParticles. The landed 0x00AF3F30 "GetNoActiveDecals" reads NoActiveParticles (+0x48), so it is GetNoActiveParticles.
- 0x00AEAF20 "ClearLowestPriorityNonVisibleEmitterDecals" is **...EmitterParticles** (it calls the particle pair above; caller AllocateParticle).
- 0x00AED010 "CTCPhysicsStandard::CanCollideWithLandscape" is **CParticleEmitter::IsVisible** (bit 3 of +0x44).
- 0x00A4CD70 "CParticleSimulation::RelocateData" (2 arguments) relocates a record reached from C3DAnimation2 +0x1c (caller 0x00A4E960), not CParticleSimulation.
- 0x00A39CC0 "CParticleMorphs::InsertEntry" is **CWADFile::Open** (vtable 0x0129CF8C slot 1 through the `jmp` at 0x00A39DC0).
- 0x00CACB80 "CParticleMorphs::CEntry_Initialize" is the DirectShow **CUnknown::CUnknown(pName, pUnk)** (InterlockedIncrement of CBaseObject::m_cObjects 0x0143CCF4).
- 0x00743780 "CParticleMorphs_CEntry::~CParticleMorphs_CEntry" is **NCreatureMode::CModeDragonFlyingBase's scalar deleting destructor** (vtable 0x01268658 slot 0).
- 0x0056D300 / 0x0056D720 "CParticleMorphs::Erase/EraseEntry" are STLport tree erases of a map whose value holds a CCharString (+0x20) and two CWideStrings (+0x24/+0x28). Neither has a direct caller.
- 0x00B81E20 "CParticleProgram::ClearGroups" is **CEnginePrimitiveSimulationInfo::ClearSimulations**-like (run by ~CEngineInternalPrimitiveMeshBase on SimulationInfo +0x64).
- 0x00C1BED0 "CParticleProgram::AddConstraintRepeat" never reads `this`; the caller 0x00C10B30 passes the object at its +0x7bc. Named by address.
- 0x00459ABE.. / 0x004DDF6F / 0x004E0D7E / 0x004E424B "~CPatchTesselationEdgeStrip" are a propagated label over many byte-identical strip/def destructors. The bases they tail-jump to are mislabelled as well: 0x00430300 "CTCInventoryItem::OnDie" and 0x00686830 "~CChunkedFileChunk" are plain out-of-line base destructors.
- Callee labels: 0x009E5200 "CParticleUpdateParameters" is CSymbolMap::CSymbolMap, 0x00AE93E0 is CCountedPointer<CParticleDataBank>::Reset, 0x009E5170 is CSymbolMap::Lookup, and 0x004CBA30 "_Copy_backward<CPixelFormat*>" is the `/Gr` for_each<…, CCallOnDie>.
