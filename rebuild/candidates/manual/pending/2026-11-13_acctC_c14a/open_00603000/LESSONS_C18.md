# agentC18 LESSONS (one line each)
- (0x00603000/0x005FF190) When one inline-cost value fits every call/expand decision of a callee in two rows of one TU while frequency fixes would differ per row, the residue is the callee's inline COST, not the call sites' structure.
- (Reset<CComponent>) c2 costs a callee before jump threading: `if (Info) ReleaseData();` adds +6 to Reset's cost yet expands to the default bytes; check any such text against the callee's out-of-line COMDAT (0x00429C6F), where an opaque inner call keeps the redundancy visible.
- (c2 frequency) A `return` that takes flow out of a loop is as cold as the flow it takes; only a do-while(0)-wrapped return is hot, and it takes nothing -- no plain spelling makes an in-loop return hot and the code after it cold.
- (c2 frequency) Nested `if (allLoaded) { rest }` removes the return's own cleanup copy (the loop exit jumps to the function end); retail's `test edi,edi; je; push edi; jmp <end+n>` is a cross-jumped real `return`.
- (row 2) Retail jump tables after `ret` disassemble as `pop edi` runs: count them before calling an extra epilogue.
