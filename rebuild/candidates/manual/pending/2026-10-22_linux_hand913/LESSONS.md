# Chunk 913 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Two identical early-exit quit tests** (`if (quit) { quit = false; LeaveGame(); return; }` at the top and again before the state change).
  - VC7.1 merges them into the FIRST block, with the second test jumping back up, only when the second copy comes from an inline helper (the SetStateAfterTime body as `SetState(44)`) or from an if/else.
  - Two plain early-return copies put the merged block at the second site. Seen at 005eb080.
- **Size-TU def-pointer loops** (005aceda, parked): the counted def pointer's destructor and release must be `__forceinline`.
  - Without it VC calls them out of line and keeps the `||` result in a frame byte.
  - With it the conditional-temporary flag word, the slot swap and the `bl` bool match retail exactly.
  - A by-reference `CCharString` argument written as a string literal takes the outer temporary slot. A named `CCharString` local in the branch takes retail's slot.
- **Interface lookup**: `T* p = 0; if (owner->HasTC(t)) p = owner->GetTC<T>(t);` keeps the absent-case zero in the callee-saved register (ebx).
  - The out-parameter form gives the pointer a frame word that the miss path re-reads.
  - Only the pointer-returning form freed the register in 005aceda.
- **Player slot taking `int`**: a sound-player slot declared with an `int` parameter (not `bool`) gives retail's `xor ecx,ecx ; test bl,bl ; sete cl` for `SetEnabled(!disabled)` (00a3e350).
- **Branch order for a range test**: `if (s >= 28) { if (s > 37) { if (s == 38) A(); } else virt(); }` puts the virtual tail call last, as retail does (007da360). The `<= 37` form inverts the blocks.
- **Byte-identical Smash bodies** (007eeb20 / 007f4e10): two classes share the same Smash body. The label propagation names both after the first class. Check the vtable owner (`rtti-vtables.tsv`) before using the label.

Review at landing:
- 005aceda (1067 bytes) took 10 grades across several strategies. Every remaining diff is register allocation or block placement (`this` homing at entry, callee-saved pushes after the first block, latch `lea` hoisting). It is a candidate for the LTCG and size-TU lane-gap list.
- 00567d60: the only diff is the trailing int3 after a noreturn call, which the grader strips from retail. The bytes are otherwise exact; decide whether the lander should accept it.
- Local models are used next to existing engine headers for CTCInventoryTrade (the PDB puts PTop_Level_List at +0x270; retail uses +0x220) and for CTCStockItem / CTCInventoryItem method views (`*_Methods` structs over the header). The `// Layout:` comments give the evidence.
- Manifest label 005d9aa0 "GetHairCardDef" should be GetTattooCardDef (the landed ProcessTradeAction 0x005AEBFE already names it so).
