# Chunk 209 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A literal pushed directly in every arm (`push imm ; jmp common`) with the constructor's `push -1` hoisted above the whole chain means one `return` per arm with identical bodies: VC7.1 head-merges the common prefix and tail-merges the rest. An if-chain assigning a `const char*` materialises the pick in eax (`mov eax,imm ; push eax`, +1 byte); a 4-case switch becomes a jump table. (005ccbbd)
- `find(const unsigned long&)` / pair construction that writes the key into a fresh frame word from a register (`mov [ebp-0Ch],edi`) is a conversion temporary: the subject local is `long`, not `unsigned long` (an unsigned local would be homed once and its address passed). (005cdccb)
- A CDefPointer local with a destructor forces `push ebp ; mov ebp,esp ; push ecx` even in a no-frame size TU; when retail has no frame pointer, hold the def through a raw AddRef/Release inline pair instead. (005bd4ea)
- A double member in a class with a vptr pads the vptr to 8 under the default pack; `#pragma pack(push,4)` around the class gives the retail offsets. (005bd4ea)
- `(HeadTurnState >> 2) & 1` compiles to `sar` unless the word is unsigned (`shr`). (00753840)
- A call result consumed by a member chain (`GetDisplayEngine()->m_pFontBank`) only lands in the retail register when it is a named local declared before the pushes. (0059dabb)
- Unmerged `return false` tails and lazy pushes disappear when the body is one nested `if` with a single `return false`. (007f1410)
- `x < g_one` (global float) gives `fcomp ; test ah,5 ; jp`; `>=` gives `test ah,1 ; jne`, and `!(x < 1)` the right test with inverted layout. (007f14f0)
- Behaviour-test models of by-value ref-counted getters need a real copy ctor: VC7.1 has no NRVO, so `return p;` copies bitwise and destroys the local, dropping a reference under VC while g++ elides it. (005aebfe test)
- Byte purity check: a `volatile` cast slipped into a manager-pointer model and still matched; grep src/ for volatile/__asm before finishing.
- Lane gaps seen repeatedly in this chunk (do not spend attempts on them): QFE never shares a frame slot between sibling-scope locals or dead argument slots (7 rows), callee-saved register assignment swaps with identical instructions (7 rows), loop-invariant `base+const` hoisting with the def spilled (3 rows), biased `lea ebp,[esp-70h]` frames (1 row).

Review at landing:
- 0x5D9AA0 is GetTattooCardDef, not GetHairCardDef (0x5D9A10); 0x6868A0 is CTCBase::FrameUpdate ($E2); 0x5D83B3 is map<unsigned long,long>::find, not CMemoryAllocatorVariableSize_FindNode.
- 005bd4ea's raw AddRef/Release pair should be reconciled with the landed CDefPointer header (the header's dtor form forces a frame pointer here).
- 0059d761/0059d1b1 are byte twins; whichever form eventually matches lands both.
