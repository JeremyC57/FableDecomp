# Chunk 247 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An unaligned function address means a size TU: start with `#pragma optimize("s", on)`. Seven of this chunk's unaligned rows needed it (004175e5, 00418c3b, 00427dc9, 0045ce48, 0045f178, 0046267d, 00646c44). Add `optimize("y", off)` only when retail uses `push ebp ; mov ebp,esp`. A /Os function with a large frame and a biased `lea ebp,[esp-N]` needs `s` alone.
- Under /GS, VC7.1 moves `unsigned char`/char arrays next to the cookie. Opaque `unsigned char _body[N]` pads in a local model then reorder the frame: retail's CMemoryFile and find-handle locals stayed in declaration order only after their bodies became `long` arrays (00418c3b).
- `bool saving = mode == 2 || mode == 3; if (saving) {...} else {...}` keeps the then-block first with `cmp/je ; cmp/jne`. The inline `||` condition, the inverted test and an early return all put the else-block first, and `switch` gives a `jl`/`jg` range check (0045f178).
- When retail keeps a local's words in callee-saved registers across a call that takes the local's ADDRESS (`UpdateCost(&ptr)`, `GetSubDef(&out)`), the callee's body is in the same TU. Give it a same-TU `__declspec(noinline)` stand-in under `auto_inline(off)` that does not let the address escape (00acd960, 008dacb0 load order, 008e7b90).
- An inline accessor such as `CThing* GetParent() const { return ParentThing; }` used once (for `&GetParent()->GetPosition()`) swapped the whole esi/edi map of 008e7a00 to retail's. Try it when `this`/temp registers are mirrored.
- A conditional temporary in an `&&` (by-value def copy, then a string) gives the `mov ebx,1 ... mov ebx,3` destruction-flag pattern with the result in a frame byte. Write it as `bool found = CDefCopy(def)->GetSubDef(&out) && out.Object->GetEffectFromString("X", &type);`.
- CCountedPointer with a deleter record (`{Count, Deleter, Object}`, release = `dec [info] ; mov ecx,[info+8] ; call [info+4] ; operator delete(info)`): the by-value argument is built in its stack slot only when the class has a user copy constructor. Its converting constructor must be `if (Data) Handle = new CHandle(Data); else Handle = 0;`, because a ternary merges the two handle stores (0085c7d0).
- STLport map/multimap in retail is 12 bytes: header node, count, compare. begin is `[header+8]`, the value is at node `+0x10`, and the increment is the cdecl 0x004292C0. Cache `end()` in a local or VC re-reads the header every iteration (0045f178).
- x87 `a*b` operand order is NOT controlled by source order for cross products (64 permutations gave identical code). Local floats per component fixed two of three components in 0085c7d0.
- Frame-slot packing of an 8-byte CIntelligentPointer temporary (vptr + item) is still unsolved. Retail overlays it on an earlier int out-parameter's slot, while VC7.1 here overlays it on the CCharString temp slot (008e7a00, 008dacb0). Declaration order, block scopes, named temporaries and helper inlines did not move it.

Review at landing:
- Same-TU stand-in bodies: GetSubDef (008e7b90, and the parked 008dacb0) and COptimisedPrimitive::UpdateCost (parked 00acd960). The landed 0x008C4170 idiom applies.
- Local models beside existing engine headers: CObserver (0052d7b0), CNavQuadTreeNode (00a76060), CMemoryFile (009e94a0, and 00418c3b, where retail's 0x24-byte CMemoryFile differs from the generated PDB 0x28 header), CModeStandBase (00862ec0). Each source carries a `// Layout:` note.
- The test for 00427dc9 uses std::map. The VC lane passes it.
- The 00ae8680 source and test fields were renamed (`m_entriesBegin`/`m_entriesEnd`) so that no identifier contains the purity checker's `_emit` substring.
- 00418c3b and 0045f178 carry `// Flags: /GS`.
