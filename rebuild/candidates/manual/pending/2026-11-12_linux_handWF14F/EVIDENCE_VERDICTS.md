# Workflow wf14f verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 009f20b0 -- VERIFIED, author attempts 1

- What made it match: In loop 2 the lookup-entry store + `break` is written out in BOTH arms (fixed-size range match / variable size) instead of once after a `continue` chain. VC7.1 merges the two tails after register allocation, but before merging `size` has one more weighted reference block than `a`, so size takes edx and `a` (needs a byte register) takes ebx. That splits the zero constant before heap's turn; its prologue + loop-1 piece then outranks heap and i, so zero keeps ebx through loop 1 and heap / i / the fixed-size bool stay in memory, as in retail. Found with a c2prio trace of a hacked variant (extra size reference): loop 2 turned into retail's registers and the zero piece landed at -26 next to heap's -18. Two spelling facts for the port: the fixed-size bit must be a bool local `((info.InternalFlags >> 1) & 1) != 0` in loop 2 (a plain `if` folds to `test byte [eax],2`), and the PDB accessor IsAllocatorFixedSize(index) matches in loop 1 but not in loop 2 (pointer base +0 instead of +0x14), so the row does not use it.

