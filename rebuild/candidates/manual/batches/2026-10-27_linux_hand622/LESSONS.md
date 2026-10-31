# Chunk 622 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A function-scope local passed by address to a call inside a loop can swap frame words with an inlined lookup key. Declare it
  inside the call's block and VC7.1 puts it above the key, as retail does (0x00923070, `CThingObject* food;` at each
  GetServingPoint call, 22 lines -> RELOCATION_MATCH). The same holds for a `bool reached;` out-flag: block-scoped, it shares the
  HasTC bool temps' word (0x00927F30).
- A virtual tail call guarded by a null test, with the false value loaded first (`xor al,al ; test ecx,ecx ; je ret ; jmp [eax+N]`),
  is `bool result = false; if (p) result = p->Virt(); return result;`. The ternary and if/return forms put `xor al,al` after the
  jump (0x004C7870).
- The real `CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound` template (landed 0x0040F020 body), as a `__declspec(noinline)`
  member with its `const K&` key, keeps the same frame-sharing effect as the old `const long*` CInterfaceMap stand-in (0x00923070).
  Use the canonical spelling instead of the stand-in.
- A frame that is 4-8 bytes too big in a row that calls LowerBound: first add the same-TU noinline LowerBound body, then turn each
  `GetTC<T>(type)->Method(args)` whose arguments read memory into `T* p; thing->GetInterface(&p, type); p->Method(args);`. The
  argument load then follows the lookup and reuses the key word (0x007B3120: 0x24 -> 0x20 -> 0x1c).
- A callee-saved permutation does not change between `#pragma optimize("y",off)` + `("s",on)` and `/O1 /Oy-` (0x00590235).

Review at landing:
- 0x00923070: the source carries a same-TU noinline copy of the 0x0040F020 LowerBound body. The lander must not land that
  symbol a second time (it is landed at 0x0040F020).
- The parked 00927f30 / 007b3120 / 008de7a0 sources still name 0x0040F020 with stand-in spellings (`CInterfaceMap::LowerBound(const long*)`
  or a CVectorMap specialisation). Respell them before landing.
- Landed callers of 0x00661FF0 / 0x00662000 / 0x007B8AA0 / 0x0040F020 use the wrong spellings listed in NAME_FIXES.tsv.
- 0x006C2170 has two upstream bests (1345v1329/93 parked here, and 1329v1329/166 alt in 2026-10-31_parked_retry2_sec2c).
