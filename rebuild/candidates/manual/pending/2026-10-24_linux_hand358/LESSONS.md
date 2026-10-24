# Chunk 358 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Propagated `_Move` / `_Move_backward` labels sit on SGI `__copy` / `__copy_backward` (`ret 0Ch`), STLport
  `__uninitialized_copy` (`ret 8`, placement null test), `remove_copy_if` (`ret 0Ch`, by-value predicate, no tag) and
  `__destroy_aux` (`ret 4`, no destination); read the `ret` and the stride, never the label (chunk 358, 34 of 40 rows).
- STLport `__destroy_aux` over records holding a vector reproduces first try with an inline
  `~CRetailVector() { DestroyRange(_M_start, _M_finish); if (_M_start) free(_M_start); }` member and the element's
  implicit destructor: retail loads `_M_finish` (ebx) before `_M_start` (esi), members die in reverse order
  (0047c800, 006f44e0, 00722130, 00cdc000).
- An `__uninitialized_copy` element {CCharString, w, w, CCharString} with a second src+0xC/dst+0xC pointer pair is
  pair<CCharString, {w, w, CCharString}> -- the flat struct schedules the middle words differently (00af2ba0; it is
  pair<CCharString, NParticleEngine::CRegisteredPSCInfo>).
- An uncopied trailing word in an element whose key is `unsigned __int64` is alignment padding:
  pair<u64, CPersistInfo{u64 UID; CCharString}> is 24 bytes with +0x14 never touched (00cb9c80, 00cb94d0).
- An uncopied BYTE followed by a copied byte at the end of a 12-byte member run is a CVectorMap (vector, empty Compare,
  bool Dirty): the implicit operator= skips the empty comparator (00a85740).
- A `double` member in an implicit element operator= copies through the FPU (`fld qword ; fstp qword`) while every
  other member uses integer moves (00889650).
- Retail CEngineTextureDecal is 20 bytes (vector 12, PDB 24), so pair<pair<long,long>, CEngineTextureDecal> divides
  by 28 (00bee4e0, 00bee7b0).
- Intrusive `CIVCountedPointer` copy-assignment: `if (Object != other.Object) { Release(); Object = other.Object; if
  (Object) ++Object->m_refCount; }` with `Release()` storing 0 inside its non-null branch reproduces `mov ecx,[dst] ;
  cmp ecx,[src] ; je` + `dec [ecx+4] ; jne ; call [vptr+4]` + `mov [dst],0` (005e0e70, plain MATCH).
- `remove_copy_if` with predicate `!(fabs(p.z - h) < tol)` gives `fld [p+8] ; fsub [h] ; fabs ; fcomp [tol] ;
  test ah,5 ; jp skip` (0096ba30).
- The `__copy` count stored in a dead argument slot (`mov [esp+18h],eax ; ... dec`) is ordinary register pressure in
  the plain `for (n = last - first; n > 0; --n)` loop when result is held in ebp: no special source form (6 counted
  pointer rows, 006fbc50, 008076d0).
- POD element names use the decimal byte size (`SElement12/20/24`), as the landed `SElement12` / `SElement28` rows do.

Review at landing:
- No row needed more than 3 grades (00af2ba0: flat, nested, then real CRegisteredPSCInfo names); nothing parked or skipped.
- All models are local (no engine header covers CCharString, CSymbolInfo, CRegisteredPSCInfo, CTargetPoint,
  CAnimationEntry, CEngineTextureDecal or the CPersistInfo pair types; CParticleEmitter.h exists but is only a template
  argument in a label, never `this`).
- 0x0071ED80 and 0x0075B5B0 are byte twins of the landed 0x00ADAB20; 0x006B9100 / 0x00A802A0 and 0x0085D870 /
  0x00A31480 / 0x008197B0 are twins of each other (the lane does not fold them: each graded separately).
- Callee labels to fix in the manifest: 0x0099EAE0 is ~CCharString (labelled `_Cons_val<...EHeroMorphType...>`);
  0x006F3C20 is a vector<28-byte record>::operator= (labelled `CCombatWheel::CopyCombatWheel`).
