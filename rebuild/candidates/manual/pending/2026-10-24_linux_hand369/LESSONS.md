# Chunk 369 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Size TUs (`optimize("s", on)`) call small inline helpers out of line: STLport's copy/fill loops, the GFSerialise traits
  TransferBinaryIn/Out, the one-argument `resize(n)`, pair's copy constructor (0x0046ECE7 without it: 33v46) and a class's
  implicit operator= (0x0046EFB8, 0x004719CE) all needed `__forceinline` to inline as retail. The implicit constructor and
  destructor of a traits holding a CCharString did inline without it (0x00472841).
- `T()` for a POD with no constructor is zeroed (`xor eax,eax ; stosd`); a specialised traits constructor that stores members
  gives `and dword ptr [..],0` (0x0046C31C); an empty user constructor leaves the temporary unwritten.
- Empty tag temporaries: `__false_type` without a constructor is value-initialised with `stosb`; with an empty user
  constructor nothing is written. Retail's `__destroy_aux` tags are never written, so model the tag with `__false_type() { }`.
- STLport 4.x `pair` has an explicit member-wise copy constructor and an implicit (bitwise) operator=: copy-construction
  copies members (a float through `fld`/`fstp` once integer registers run out, 0x0046EB9E) while assignment is `movsd`.
- An enum is not a POD to STLport's type traits: vector<EOpinionAttitudeType>'s copy goes through the placement-new loop with
  its null test (0x00471F97).
- Leaf substring trap: `uninitialized_fill_n` also matches the out-of-line `__uninitialized_fill_n`; name the leaf by its
  mangled prefix (`??$uninitialized_fill_n`). Likewise `?erase@` and `??0?$vector@` pick a member/ctor precisely.
- Make the CBasicString<char> compare 0x00411570's body visible in the TU (under `auto_inline(off)`): VC then knows the call
  writes no memory and keeps the key's string block hoisted in a register across it (declaration only: reloads, 69v73).
- CCharString::operator< for the map: `if (other.PStringData == PStringData) return false; if (!PStringData) return true;
  if (!other.PStringData) return false; return Compare(..) < 0;` -- the operand order of the first test fixes `cmp ebx,eax`.
- STLport map `find` on node pointers byte-matches as `if (y == header || comp(k, key(y))) y = header; return y;`; the nested
  `if (y != header) { if (comp) y = header; }` and the `?:` return both cost a branch target.
- The STL free algorithms in 0x0046xxxx-0x0047xxxx are `__fastcall` (`__copy_ptrs`, `uninitialized_fill_n`,
  `__introsort_loop`, `__adjust_heap`), but `distance` @ 0x004753B1 is `__cdecl`: declare each callee's convention from its
  call site.

Review at landing:
- 0x00472BDD parked DIFFER(77v77): only the never-written tag's slot (`[ebp+0Fh]` v `[ebp+0Bh]`).
- The tree rows (0x0047536C .. 0x004759AD) and 0x0046ECE7 are named map<CCharString, long>; the PDB's map<CCharString,
  EGameAction> (CPlayerGuiDef::GameActionValues) would fold to identical bytes -- the caller chain (TransferMap<CCharString,
  long> 0x00475026, landed) is the attribution.
- 0x0047547D / 0x004754C6 / 0x00475861 / 0x004758E3 / 0x004759AD carry a same-TU body of 0x00411570 (the landed compare);
  the lander should keep the leaf on the tree member, not the stand-in.
- Landed labels to correct (see NOTES.md): 0x00469D47, 0x0046B0D6, 0x0046AADE, 0x0046ABC4, 0x0046EC57, 0x00450A14,
  0x0046638B, 0x00471C79, 0x00471C9A, 0x0047280C.
