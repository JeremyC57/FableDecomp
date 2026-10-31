# Chunk 600 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A flag step that retail compiles as `or eax,4 ; jne store` with a fall-through call is a dead arm VC7.1 keeps:
  `unsigned long f = Flags | 4; if (f == 0) Stage(); else Flags = f;`, rare arm first (0x00C783F0).
- A same-TU stand-in for an out-of-line callee that takes a key by reference (here `PKCTRRedblackMap::Remove` calling
  the declared-only DeleteNode) lets VC7.1 see that the key's address does not escape and reuse the dead argument slot
  (0x00C1A370); `x <= 0` on an unsigned count gives retail's `ja`.
- A recursive FindNode inlined to retail's depth needs `#pragma inline_recursion(on)` + `inline_depth(N)`, and the
  helpers around it `__forceinline` (plain inline leaves GetResource out of line, 169v398) (0x00BCA510).
- A libvorbis static added to the landed vorbisfile.c TU must sit inside `extern "C" { }` (else it is C++-mangled) and
  the TU must end with a newline before appending (0x00C8C240).
- CEngine's component forwarders (`mov ecx,[g_component] ; jmp target`, 0x00B23Cxx) list the component methods in
  PDB virtual order: use them to name an unnamed component method (0x00B23CA0 -> 0x00B33390).
- Stat maps are `map<unsigned long, unsigned long>` keyed by a C3DMeshInfo address (stat 0 / 1) or a graphic bank index
  (stat 2), filled by CEngineInternalPrimitiveMeshBase::CollectStats 0x00BEC680 (0x00B33390).
- A `CCharString` returned by value then passed to push_back must be a named local: the unnamed temporary passes the
  returned pointer (eax) where retail re-leas the slot (385v396 -> 396v396, 0x00B33390).

Review at landing:
- 0x00B33390 used all 4 grades and is parked at 396v396 with frame-slot packing as the only residue; worth one more
  attempt by the lane (see NOTES.md next idea).
- 0x00B33390 uses `g_pResourceManager` (0x01436E98) from CEngine_SetVideoOptions 0x00B24690; the address carries ~20
  bound spellings (g_pEngine_01436e98, g_pEngineData, gResourceManager, ...): pick one catalog-wide.
- The landed CIEngine stubs 0x00B23430 / 0x00B23440 are free `__stdcall` functions with int parameters
  (`CIEngine_CollectPrimitiveStatsForArea`, `CIEngine_ProcessPrimitiveStats`); the PDB has them as CIEngine virtuals
  `?CollectPrimitiveStatsForArea@CIEngine@@UBEX...` / `?ProcessPrimitiveStats@CIEngine@@UBEX...` (empty `ret 10h`,
  0x00B23440 referenced from .rdata 0x012A0DAC = a vtable slot). Not in NAME_FIXES (no caller binds them); respell at landing.
- The landed 0x00BEC8B0 CEngineInternalPrimitiveMeshBase::CollectStats uses a local padded class and a dummy StatMap;
  the inner static is `?CollectStats@CEngineInternalPrimitiveMeshBase@@SIXABVC3DMeshInfo@@AAV?$map@KK...@Z` 0x00BEC680.
- 0x00B321B0 is landed as CollectPrimitiveStats with a pointer-to-struct map and `long` stat index; the PDB name is
  CollectPrimitiveStatsForArea (const, map by reference, EEnginePrimitiveStatIndex): NAME_FIXES.tsv.
- 00c783f0 / 00c1a370 method names are descriptive (libpk has no PDB); 00c783f0's stage callees are address-suffixed
  placeholders.
- NAME_FIXES.tsv has 7 rows (KillAllThreads, list clear, const Configure parameter, IsBusy, Sizeof, BuildNewBuffer,
  CollectPrimitiveStatsForArea): the landed callers need respelling before these rows land.
