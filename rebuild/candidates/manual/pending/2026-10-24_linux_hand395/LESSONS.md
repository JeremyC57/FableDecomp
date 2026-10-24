# Chunk 395 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport sort over pair<CCharString, C3DVector>: compile `CBasicString<char>::operator<`'s real body ABOVE the
  algorithms under `#pragma auto_inline(off)`, keep `CCharString::operator<` inline, and give C3DVector a memberwise
  user copy ctor. This matched all six of __unguarded_linear_insert, __unguarded_partition, __linear_insert,
  __make_heap, __partial_sort and __introsort_loop (0x0077E6A0..0x0077F370).
- A one-pointer STLport iterator class is returned through a hidden pointer. When retail gets the node back in eax from
  an out-of-line `find`, declare `find` as returning the node pointer (0x00783240).
- The other way round, a POD `pair<iterator,bool>` or iterator is returned in edx:eax / eax. Give both their STLport
  constructors to get the hidden-pointer return (0x0079E580).
- Retail's stale-register read after an inline GetInterface is the UNCHECKED form: a lookup with no presence test
  (0x00783680).
- An STLport map member ctor inlines fully: `_M_header(0)`, malloc, count 0, then colour / parent / left / right through
  re-reads of the header word. A body `clear()` follows it (0x007837C0, 0x007A3BC0).
- A by-value class argument built in its own slot (`push ecx ; mov eax,esp`) needs a user copy ctor plus
  `optimize("s", on)` in the trap TU (0x00787C35, CRGBColour).
- `x = a / b; if (!c) return x; if (d) return x / e; return x;` keeps the early exit on the x87 stack and `fst`-spills
  for the others. An `&&` merge of two exits loses the spill (0x00787C62).
- `v += d; if (v < 0) v = 0; else if (v > 1) v = 1;` gives retail's single `fst` compare-and-store (0x00789A10).
- VC reassociates `s * s * scale` to `(s*scale)*s`; write `scale * (s * s)` to get retail's x87 order (0x00789CE0).
- In the hero-combat TU, a by-value CDefPointer's release has no null test (`if (--Object->RefCount == 0)
  Object->Destroy();`) (0x00789D90, 0x00789CE0, 0x0078C000).
- A file-scope function pointer keeps an anonymous-namespace `__fastcall` helper emitted for grading (0x00789D90,
  0x00787C62).
- An `and esp,-8` frame inside a map walk comes from `__declspec(align(8))` on the per-entry CIntelligentPointer copy
  (0x0078F820 / 0x00794D90).
- In `vector<Record>::push_back`, VC7.1 calls an out-of-line implicit copy of the record. Mark both the record's and the
  CIntelligentPointer's copy ctors `__forceinline` (0x007935D0 / 0x00798410).
- map::operator[] matched first try in its STLport spelling (`lower_bound`, then `insert(i, value_type(k, _Tp()))`); the
  insert result is homed in the dead key slot (0x007934F0 / 0x00798330).
- `vector(size_type n)` needs `_STLP_alloc_proxy` as the end-of-storage member, with `allocate(n) = n ? malloc(n*sz) : 0`
  on it. A free allocator temporary moves `mov esi,ecx` (0x00799CD0).
- Interface 0xA7 = CTCSpecialAbilities; bit 7 of bitset word 5 is tested as `mov cl,[eax+34h] ; test cl,cl ; jns` by
  the plain inline GetInterface (0x007A03B0).
- `mov ecx,[eax+4] ; push arg ; call` straight after an inline GetInterface (no `mov eax,[eax+4] ; ... mov ecx,eax`)
  is the ONE-ternary store `*ppOut = (T*)((it == last || type < it->type) ? last : it)->component;` (0x007A0490).
- A member handle compared against a global and then passed on wants a named local; otherwise VC compares against
  memory and re-reads the member (0x007A0490).
- STLport sort_heap with a 12-byte comparator: a C3DVector user copy ctor stops VC hoisting the comparator's three
  loads out of the loop (0x007A3B60). In the sibling __partial_sort, the same ctor copies Y/Z through `fld/fstp`
  (parked, 0x007A3FA0).
- CHitParameters (Position +2, Direction +0x0e) is only reachable with `#pragma pack(push, 1)` on C3DVector, which is the
  real unaligned PDB layout (0x007A3BC0).
- FinalCleanup-style bodies that do two GetInterface lookups on ParentThing re-read the caster into eax in retail. VC7.1
  reuses the dead `this` register for the second lookup, and no source form fixed it (0x0079D2D0, 0x0079EAA0; same as
  the queued CTCBulletTime::Pause 0x0079CF30).

Review at landing:
- 0x0078F820 / 0x00794D90 carry `__declspec(align(8))` on a loop local to reproduce `and esp,-8`. Check this is
  acceptable purity-wise; it is a layout attribute, not asm.
- 0x007A2F90 is landed as `CThingCompare_Nearest::operator()<CThingAICreature>`. Ego_r ordering says
  `CThingCompare_NearestToPos::operator()<CThing>` (same 12-byte layout, so the bytes are unaffected).
- 0x007A0400 is landed as `CTCWillLightning::ResumeForCutscene`. The PDB has no such member; the body is
  `StartLoopingSound`, the mirror of 0x007A0490.
- Some rows model their class locally although an engine header exists:
  - 0x00789A10 (engine/CTCHeroRangedAttacks.h has ProjectileWeaponZoomParameter at the same +0x40) could be retyped onto
    the header.
  - 0x00787C35 / 0x007881AD (CTCTrapBase.h) and 0x007A03B0 / 0x007A0490 (CTCWillLightning.h) carry PDB offsets that
    are 0xc off from retail (TrapOpener +0x38 vs +0x44; LoopingSound +0x98 vs +0xa4), so the local models stay.
- No CHitParameters engine header exists. The landed 3-argument ctor 0x0087FF60 models the map as 8 bytes plus an
  "AllocatorState" word; 0x007A3BC0 uses the 12-byte STLport map and a 12-byte vector instead (same offsets).
- Sort-family rows use `_STL::` as the line-1 module (finish.py takes it as the module); the catalog module is
  `_global`.
