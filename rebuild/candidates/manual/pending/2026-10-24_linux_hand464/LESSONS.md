# Chunk 464 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `resize(n)` temporary with a real destructor (vector member, CIVCountedPointer, virtual dtor): give the
  out-of-line callee (erase + `_M_fill_insert`, or the two-argument resize) a same-TU `__declspec(noinline)` stand-in
  under `auto_inline(off)` that never writes through the value reference -> VC7.1 drops the temporary's destructor test
  after the call, as retail (0x0086EA80, 0x006DD4A0, 0x00A43FF0, 0x00A3D570). Declaration-only callees re-test it
  (+13..+27 bytes); `__default_constructed((_Tp*)0)` and the QFE-4035 compiler change nothing (0x008883C0 probes).
- resize(n) temporary store order follows the element constructor: a user `: P(0), N(0)` constructor vs a POD
  value-initialised `_Tp()` schedule the zero stores differently (0x00639810 vs 0x0066E6D0); members assigned in the
  constructor body store after the init-list members (0x00B6CC50 WaterType, 0x008883C0 BaseWeightLocked).
- Chained assignment `Edges[0] = Edges[1] = Edges[2] = -1;` stores right to left (0x00AD7350).
- A pair<long, float> / CDefString(-1) temporary gives immediate stores after the size compare (0x00786410, 0x0081DF50,
  0x0086DCB0).
- One-argument resize rows: the `// Leaf:` must be the full one-argument symbol -- the inline two-argument resize is
  emitted as its own COMDAT and a prefix leaf may grade it (0x00639810).
- CIVCountedPointer release in a destructor: `T* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete();
  Object = 0; }` -- reading the member again after the decrement costs 7 bytes (0x00A43A00, 0x00AF1D90).
- `__unguarded_insertion_sort_aux`: keep `__unguarded_linear_insert` declaration-only (a call) unless retail inlines it,
  then define it in the TU (0x009B2AC0); pair<CCharString, C3DVector> needs a memberwise C3DVector copy constructor
  (0x0077EA10), while implicit copies of C3DMeshVolume / CUniqueVertex give retail's `lea edi,[esi+10h] ; lea
  ecx,[edi-0Ch]` rebasing (0x00A710F0, 0x00AD62E0).
- STLport string constructors: `_String_base(a)` zero stores, then the pointer dispatch's empty tag (user constructor)
  homed in the dead `s` argument slot and pushed first, `s` reloaded after the push (0x0099CA40, 0x00568CE0); the same
  constructor inlines into CWideStringData(text, length) with `NoRefs = 1` after the call (0x0099CC40).
- Member constructor templates instantiate explicitly in VC7.1: `template vector<long>::vector(int, int, const
  allocator<long>&);` gives `??$?0H@?$vector@J...` (0x00453659; frame-pointer size TU = `optimize("y", off)` + `("s", on)`).
- STLport list constructor = `_M_node(a, 0)` (a zero store), malloc(sizeof node), the two self links, then the head
  store; empty CSB/CSBInput/CSBConstraint constructors leave only the final vptr store (0x00A21F50).
- `__uninitialized_fill_n` with an unsigned size: `test edx,edx ; jbe` + `dec ; jne`, placement-new null test
  (0x00AD62E0).
- A map lookup without an end test is `(*map.find(k)).second.second` with `_M_find` out of line (0x0051F040).
- Unresolved: a `bool()` resize temporary in a `push ecx` frame word lands at byte 3 under VC7.1 RTM where retail
  0x00CDD0A0 uses byte 0 (four source forms identical); other bool resize callers (0x00D34973, 0x00E54A22) use byte 3.

Review at landing:
- 0x008883C0 still models CEnvironmentThemePriorityStackEntry without its CArray destructor (grade budget spent): apply
  the stand-in recipe above (a read-only same-TU body for resize(n, x) 0x0088A390) and give the entry its real
  destructor.
- The stand-in bodies in 0x0086EA80, 0x006DD4A0, 0x00A43FF0, 0x00A3D570 are minimal read-only bodies, not retail's
  (never graded); replace them with the real landed bodies if those keep the value reference unwritten.
- Local CIVCountedPointeeBase / CIVCountedPointer models (0x00A43A00, 0x00AF1D90, 0x00A43FF0, 0x00A3D570) sit beside
  engine/CIVCountedPointeeBase.h, which carries the vptr as a data member and cannot be a polymorphic base.
- 0x0051F040 models CThingManager with `_Before[0xCC]` (no engine header exists; retail NPCNameMap at +0xCC, Ego_r +0xF4).
- 0x00568CE0 uses namespace `std` for the network module's STLport basic_string<char> to match the landed 0x00567D00
  family; confirm `std` vs `_STL` once (it changes only the mangled names).
- 0x00786410's element pair<long, float> is inferred (shared erase COMDAT 0x006B5AA0, neighbour rows), not PDB-named.
- Already landed: 0x0051F010 is CThingManager::DoesNPCNameEntryExistForThing(unsigned __int64), not
  `CKeyedSet_0051f010::Contains(unsigned long, unsigned long)`; 0x00781CE0 models 0x007817B0's element as an 8-byte
  SMeshElement (it is the 0x2C NFire::CFirePoint); 0x004CFCF0 calls its caller 0x004D0448 landed (the caller is this
  chunk's 0x004D0420); 0x009FF8B0 NotifyBankSize is a stub-like toy source; 0x0086E4B0 is a toy-named
  (`Vector_0086e4b0`) body of vector<CVectorMap<unsigned long, CTextureMorph> >::_M_fill_insert.
