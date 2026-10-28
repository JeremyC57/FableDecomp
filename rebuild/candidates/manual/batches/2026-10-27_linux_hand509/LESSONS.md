# Lessons (chunk 509)

## GOTCHAS one-liners
- A by-reference helper (IS_THING_ALIVE) that gets an address-taken local: give it a same-TU noinline body and VC7.1 keeps the local in a register and reuses its slot; declared-only, the local is reloaded from memory and the frame grows by 4.
- `GetPlayer(x)->HasMode(c)` pushes `c` before the GetPlayer call; retail calling GetPlayer first then `push c` means a separate local (`CPlayer* p = GetPlayer(x); if (p->HasMode(c))`).
- `test byte ptr [thing+N], mask` is the std::bitset reference proxy (non-const operator[]); `mov ; shr ; test 1` is the const `_M_test` path. A const GetTC whose bit test reads `test byte ptr` uses the proxy (const_cast) while still calling the const LowerBound.
- STLport helpers containing loops are never inlined by VC7.1 unless __forceinline; mark them that way when retail inlined them.
- The empty comparator of an STLport sort: an in-TU auto_inline(off) body of operator() removes the 1-byte comparator copy VC makes otherwise.
- A catalog row that uses registers it never set and pops what it never pushed is a mid-function fragment: SKIP it and record the real start.

## Review at landing
- RELOC_CONFLICT: `??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z` binds to 0x0043DBD0 in 0x62CF90 but to its byte-identical twin 0x0043DBF0 in landed 0x0064A568 (marked "unproven" there). Evidence for 0x43DBD0 here: the caller homes the GetPControlledCreature result (CThingPlayerCreature*, PDB-mangled return type) right after the call and passes that slot with no conversion temporary. Landed rows also call 0x43DBD0 as `IS_THING_ALIVE_0043dbd0<CThing*>` and corrections label it `<const CThing*>`. Settle the twin pair (two separate instantiations; VC7.1 did not fold them) before landing; the spelling was not changed to silence the warning.
- STLport `_M_fill_insert` access is inconsistent catalog-wide (QAE vs IAE). 0x51D630 follows its existing IAE binding; the other rows use QAE (public in STLport 4.6). Pick one at landing.
- 0x5239CD should be re-cut as 0x5239C0 (379 bytes).
