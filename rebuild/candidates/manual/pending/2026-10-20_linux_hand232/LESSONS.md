# Chunk 232 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `for (i = 0; i != N; ++i)` linear table scans give retail's `jne` back-edge (0x0056EDD2); `i < N` gives `jl` and re-lays the tail.
- STLport `clear()` as an inline member `erase(begin(), end())` keeps end() addressed through the vector-pointer register (0x0047E07F, 0x00602E30); a direct `v.erase(v.begin(), v.end())` re-reads it through `this`.
- Inline `_Destroy(first, last)` element loops + `if (start) free(start)` reproduce STLport vector destructors in a class dtor; a user-declared inline base dtor keeps the base vtable store (0x00499FB0).
- `cmp [mem],0` / `push dword ptr [mem]` with an esp frame and a tail `jmp [eax+4]` is `#pragma optimize("s", on)` WITHOUT `("y", off)` (0x0057238F; the speed default was 368v318).
- Two inline GetInterface lookups in one body: write the entry pick as ONE ternary `*out = ((it == end || wanted < it->type) ? end : it)->component` (0x005EE887); the if-form gives the second lookup a `jle ; mov eax,esi` select.
- Mixed const (0x4365B0) and non-const (0x40F020) LowerBound lookups on the same ParentThing: route the const ones through an inline `const CThing* GetConstParent() const` (0x00583BD5).
- Count-down loop with the count in the dead arg slot (`dec [ebp+8] ; jne`) and a pointer stepping 20-byte items: `unsigned long count = v.size(); T* p = v._M_start; for (i = 0; i < count; ++i, ++p)` with size() before the pointer (0x005BFB96). CDefPointer `operator=` (and by-value temp dtors) need `__forceinline` to stay inline in size TUs (0x005BFB96, 0x005F4B30).
- A same-TU stand-in callee that fills the caller's local vector must also READ it (a capacity test), or VC7.1 drops the caller's end-of-storage zero store (8 bytes short); with it VC keeps the vector start in a callee-saved register across unrelated calls like retail (0x0061AC60, 0x0061AB30).
- `if/else` of two complete `Reset(CreateComponent("lit", false))` statements tail-merges into retail's single dead-arg-slot temporary; a ternary of CCharString temporaries adds construction-flag bits (0x005FB7E0). Implicit `const char*` -> `const CCharString&` builds the temp before the other pushes and lets VC CSE a twice-used literal into a register (0x005C0121).
- A swap of two vector elements that retail re-addresses after the first store is three operator[] statements, not std::swap (0x005F694D).
- `IS_THING_ALIVE(const T&)` on a pointer retail keeps in a register AND copies to a frame word is a converted-pointer temporary: hold a CThingCreatureBase* and call IS_THING_ALIVE<CThing*> (0x005BE284). RTM 3077 did sink a callee-saved `push ebx` into the only branch using it there.
- `if (a) {...} else { ...; if (ok) { ...; return true; } } return false;` puts the false block right after the first branch with the later failure jumping back (0x005BF654).
- STLport vector locals zero end, finish, start in that order: `_STLP_alloc_proxy` end member in the init list, finish/start zeroed in the ctor body (0x00591A66).
- Under RTM 3077 /O2 an 8-byte struct local/temporary whose address is taken inside a loop makes VC7.1 build an `and esp,-8` aligned ebp frame (probe-proven; a 12-byte struct or no loop gives the plain frame). Retail's CThingFilter_InMap loops (0x005FBEA0, 0x005FBFA0) have no aligned frame.
- Shared CTestQuest helpers for the landing: CTCInventoryQuests::GetTestQuestsToDisplay is called with a same-TU stand-in in 0x0061AC60/0x0061AB30; the real body (0x0061A8A0) is parked.

Review at landing:
- Same-TU stand-in bodies (not the real code): GetTestQuestsToDisplay in src/0061ac60.cpp and src/0061ab30.cpp; differ/005fbd90.cpp (GetAllThings), differ/005ee99d probe (GetAbilityPDef), differ/0061a8a0.cpp (_M_insert_overflow).
- Local models used where engine headers exist (retail layouts differ: 12-byte STLport vectors / smaller bases): CTCInGameMenu (0047e07f), CTCInventoryExperience (0056fad7, 0057229b, 0057238f), CTCInventoryAbilities (005ee887), CTCInventoryClothing (parked rows).
- Behaviour tests for 005bfb96 and 0047e07f model only the members the logic touches (simplified layouts).
- 0x00415DD0 is declared as a second `CDefString::operator CCharString()` (landed sources use 0x00415D70 for that name); confirm which spelling owns which address.
- 0x005D9380 row holds three functions (ledger).
- Several rows needed all 4 grades and are parked on recurring gaps: def-release load register (005fb7e0, 005f694d, 005bf654), live-range frame packing (005b413d, 005b4795, 005fbfa0), register-allocation swaps (005be028, 005be284, 005b690b, 005b5be6, 005c0121, 0061a8a0).
