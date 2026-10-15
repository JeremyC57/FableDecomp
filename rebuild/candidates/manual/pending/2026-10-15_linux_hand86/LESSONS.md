# Chunk 86 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A CTCD* constructor whose vtable store sits right after `xor eax,eax`, with the zero stores AFTER it, initialises
  member OBJECTS that have inline constructors: a 12-byte retail vector `CRetailVector() : first(0), last(0), end(0)` or a
  CDefPointer. Init-list scalars would be scheduled before the vtable store. (0x004E079C, 0x004E0AAB; confirms the
  wave-24i rule.)
- A bitfield bit on an object reached through `GetPItem()`, stored with `[eax+44h]` addressing twice, goes through an
  inline setter ON the pointee class. A direct `p->bit = x` makes the lane form `add eax,44h` once. (0x006E0590)
- An inline CCountedPointer release that re-reads the block after the decrement and again after the deleter is
  `--PBlock->Count; if (PBlock->Count == 0) { PBlock->Deleter(PBlock->Data); delete PBlock; } PData = 0; PBlock = 0;`.
  The static `__fastcall` deleter is called through `[eax+4]` with `ecx=[eax+8]`, and the POD block is deleted
  without a null test (`add esp,4`). A destructor with no vtable store is modelled with no virtuals and graded with
  `// Leaf: ??1<Class>`. (0x006E1E60)
- A CPersistContext::Transfer<bool>(name, member, default) with a literal default homes the one-byte temporary in the
  high byte of the `push ecx` slot (`lea eax,[esp+0Fh]`, store late). After `fullState` has been tested, the
  temporary goes into that dead argument slot instead (`lea eax,[esp+10h]`). `this` is advanced in place
  (`add ecx,0Ch`) to form &member. (0x00778EF0, 0x007E2530, 0x00731170)
- `test eax,eax` on the OLD value, then `lea edx,[eax-1]` store, then `jge`, is `if (LifeTimer-- < 0)`: a post-decrement compared
  with zero. (0x007F1890)
- `GetWorld()->GetWorldFrame()` where GetWorldFrame is STATIC still calls GetWorld (its result is unused, with no
  `mov ecx,eax`). A bool result preloaded in bl before the calls is a local initialised to false and set inside the
  if. (0x008EC7E0)
- An inline accessor that returns a CDefPointer BY VALUE gives an add-ref with a null test (`inc dword ptr [eax+4]`),
  then the compare parked in `sete bl` across the release (load/dec/store of the count, then a virtual destroy).
  (0x008EC900)
- When an inlined "available" test and a member store open both arms of an if/else, the store is hoisted between the
  `cmp` and its `jle`. Writing `bool available = IsAvailableToHit(); Set(frame); if (available)` keeps the result in
  bl instead and does not match. (0x008ECC80)
- A void virtual forwarder whose callee pops fewer bytes than the caller (`ret 0Ch` vs the callee's 8) stays
  `call ; ret`. Two byte-identical slot bodies (OnHitByShot / OnHitByMelee) are told apart by the egor function
  order. (0x008EC810, 0x008EC830)
- A chained call `Singleton::Get().GetDef(index, member)` pushes the arguments before the static `Get()` call, and
  `add ecx,0Ch` forms &member in place. (0x007C3040)
- A ctor that makes a virtual call on the PARENT thing (`call [edx+70h]`) keeps it in the body. The parent pointer is
  re-read from +4 after the base ctor, and its vtable is fetched before this class's vtable store. (0x00771200)
- Signatures: egor_signatures.tsv often has reference parameters where the vtable label gives nothing
  (`ABVCTCBase@@`, `ABVCEnginePrimitivePrepareInfo@@`, `ABVCThing@@`). The codegen is the same as with a pointer, so
  use the egor form. (0x007305A0, 0x00771230, 0x007E3490)

Review at landing:
- No row needed more than 4 grades. Each of the 4 parked rows used 3 or fewer.
- Local base models next to existing engine headers:
  - 006e0590, 006e05f0, 006e0fc0 and 006e11a0 model NParticleEngine::CParticleEmitter locally:
    engine/CParticleEmitter.h has the PDB offsets, but the retail emitter sits 0xc lower (AlwaysUpdate is bit 4 of
    +0x44, not +0x50).
  - 008ec7e0 and 008ecc80 model CWorld locally (only the frame getter is used).
  - Every CTC* source models CTCBase locally (vptr, ParentThing +4, flags +8/+9) instead of including
    engine/CTCBase.h. The lander may want to switch those to the header.
- Retail-vs-PDB layout shifts used here:
  - std::vector is 12 bytes in retail (16 in the PDB), which moves every later member down by 4
    (CTCGatherPointBase, CTCShotDefinitionDrainLife, CTCDPhysicalObstruction, CTCDivineWrath, CDegradableDef).
  - CThing members from the component map on sit 0x10 lower than the PDB (PTCGraphicAppearance +0x64, PhysicsTC
    +0x60, the dead bit at +0x91).
- Unaligned members modelled under pack(1): CTCDiscovery::Discoverer +0x0d (0x007509A0, 0x007509D0), and
  CTCDivineWrath / CTCUnholyPower PTarget +0x6d (0x0078E7C0, 0x00793CC0).
- 0x0078E7C0 and 0x00793CC0 are byte-identical. They land together, from the same source with a different class
  name.
- Catalog rows that hold several functions: 0x00991620 (second body at 0x00991640) and 0x007312F0 (second body at
  0x00731300). Both need re-splitting in the catalog.
