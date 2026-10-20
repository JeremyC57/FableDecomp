# Chunk 208 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Same-TU template/helper callee (CDefPointer ctor, list walker, GetTC): putting a stand-in callee body above the graded function under `#pragma auto_inline(off)` gives VC7.1 callee knowledge, so the def/list pointer stays in a register and ecx is not reloaded after the call (0x0077CBE0, 0x006C5E90, 0x00C21620).
- A find-iterator scoped in its own `{ }` block shares its frame word with later temporaries; a function-scope iterator gets a separate slot and the frame grows (0x0055F3B0).
- Clearing a CIntelligentPointer member with an implicit-conversion `PX = 0` gives the retail sequence; an explicit `.Release()`/reset call does not (0x005C39F0).
- In an /EHsc function with several early returns, a `goto` single exit stops VC from copying the EH epilogue into each return (0x00C3CD00).
- Declaration order sets frame-slot order: declare the morality local first. Write two separate `if`s instead of `a || b` to get retail's branch layout (0x005EE6BF).
- Reading the def through a named local and indexing with `m_pFirst[i]` stops VC from CSE-ing a zero register across the loop (0x0057D8BE).
- Indexed record stores `hits[count].X = ...` instead of a reference local, plus a named local read after the sort call, reproduce retail's addressing and scheduling (0x008169B0).
- Vertex-buffer fill: named byte-count and locked-pointer locals fix the register allocation (0x00B61FF0).
- In a size-optimised (`optimize("s", on)`) TU, small helpers like GFLimit need `__forceinline` to inline as they do in retail (0x0057B327).
- A CRGBColour with a user-declared ctor makes GetColour return through a hidden pointer, as in retail. Fetch the manager local before the implicit CCharString conversion to get retail's call order (0x00584F15).
- Bool-ish queries that really return `int` give `test eax,eax`; declaring them `bool` gives `test al,al` (0x00C21620).
- Binding a temporary action to `const CCreatureActionBase&` (SetCurrentAction takes a const ref) gives retail's temporary layout; a by-value or derived-type parameter does not (0x0078A2C0).
- Remaining gaps no source form reached in 4 attempts: biased /GS ebp frame (0x00581B33), EH state tag written over a live slot (0x0071CE80), inlined null-tested release order (0x005F6A6F), lea operand order (0x006D8300), and pure register swaps / scheduling (several rows).

Review at landing:
- Label conflicts:
  - 0x006869D0 is GetDefinitionManager in some landed code and GetDefinitions in others.
  - Landed BuildBuffers calls 0x00B61FF0 "BuildSolidBaseBand"; the evidence says BuildStarFieldVB.
  - Landed 0x0047D6F2 is probably IsSaveMenuReceivingInput (the sibling of 0x0047D648 IsLoadMenuReceivingInput).
  - CycleAssignedAbilities is declared `bool` in one place and `long` in another.
- Stand-in callee bodies stay in the sources (0x0077CBE0, 0x006C5E90, 0x00C21620). The lander must grade only the `// Leaf:` symbol, not the stand-ins.
- 005c39f0 uses `#pragma pack(1)` on the intelligent-pointer classes only.
- 00c3cd00 needs `// Flags: /EHsc`.
- Class rename: 0x00C3C4E0's class is CVtable_012b4f74 (the base ctor stores vtable 0x12B4F74), not CVtable_012b4f38.
