# Chunk 84 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The landed out-parameter `GetInterface(type, &out)` template (0x007504E0 shape) must be `__forceinline` when it sits
  under an outer guard (`if (ChestOpen)`, 0x00750730) or inside an `&&` chain (`type == 2 && GetInterface(48, &c)`,
  0x00734050): VC7.1 otherwise emits it out of line and calls it. At the top level of the body plain `inline` is enough (0x0074E500).
- A zero-initialised result local before the interface lookup (`long gold = 0; if (GetInterface(204, &g)) gold = g->GetGold();`)
  gives the hoisted `xor eax,eax` ahead of the presence test (0x0074E500).
- A functor with a USER constructor passed by value to a `/Gr` (`__fastcall`) `std::for_each` instantiation is returned through
  the hidden pointer in ecx with `first` in edx and the 8-byte functor pushed as two dwords; read `.MaxStage` off the call
  result (0x00733910 -> for_each 0x00738F30).
- `(long)(g_FramesPerSecond * 2.0f)` on a `long` global = `fild ; fadd st0,st0 ; __ftol2` (0x00734050); `seconds * g_rate`
  in `long` = `mov reg,[def+n] ; imul reg,[rate]`, and `-(a * rate)` = `imul ; neg` (0x00749EA0).
- A one-word iterator out-parameter local zeroed before the finder call keeps the zero store scheduled AFTER the pushes
  (`push eax ; lea edx,[esp+4] ; push edx ; mov dword ptr [esp+8],0`) (0x00733750, 0x00734880).
- `return Members.FindNode(id) != Members.Header;` for a `bool` result = `xor edx,edx ; setne dl ; mov al,dl` (the header
  re-read after the call), and two SEQUENTIAL `if (...) return false;` tests place the shared `xor al,al` exit right after the
  second test (0x00812F20).
- CTCCrateStack getters: `return p != 0;` as `long` = `neg ; sbb ; neg`; `return p ? 1 : 2;` = `neg ; sbb eax,eax ; add eax,2`
  (0x007B6660, 0x007B6690).
- An implicit memberwise struct assignment containing a CCharString inlines as integer moves for the floats/longs plus the
  out-of-line `CCharString::operator=` for the string (0x00749E40).
- A CIntelligentPointer member at an unaligned offset (+0x31 in CTCChest, +0x2a in CTCDRegionExit, +0x11 in
  CTCCreatureGeneratorCreator) needs `#pragma pack(push, 1)` around the pointer classes only (24p precedent).
- A float `if (a == X && b == Y) return 0.0f; return arg;` leaves the unused second parameter visible only in `ret 8`; the 0.0f
  is the 0x0122DEDC pool load (0x00733410).

Review at landing:
- engine/CTCCreatureGenerator.h (Ego_r.pdb) is 0x10 bytes high from GenParams and 0x1c from PDef on vs retail (retail CArray
  0xc, STLport list 4, vectors 12): 00749e40, 00749ea0, 0074a190 use a LOCAL model next to the existing header.
- engine/CTCCombat.h is used directly for all CTCCombat rows (offsets agree: ComboChainPos +0xc, ComboChainID +0x10,
  CombatManager +0x20, ParentCreature +0x24); the CThing / CThingCreatureBase / interface-map parts are local models.
- 0074fde0 is graded with leaf `??_GCTCContainer` (non-polymorphic first-member model, GOTCHAS 24m) -- check the lander
  extracts `??_G` and not a `??1`.
- 006543af needs `#pragma optimize("y", off)` + `#pragma optimize("s", on)`; 00453de0 needs `#pragma optimize("s", on)`.
- Catalog row 00750960 holds two functions (a 5-byte `jmp [eax+24h]` thunk and `mov al,1 ; ret` at 0x00750970): split it.
- Many chunk labels are propagated across byte-identical bodies (13 rows relabelled, see NOTES.md "Label corrections"):
  land each under its real class, not the chunk label.
- No row needed more than 4 grades (005e3c60 used exactly 4 and was parked).
