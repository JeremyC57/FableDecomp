# Lessons from chunk 187
- A block-scoped local (out-parameter / LTCG result object) lets VC7.1 share its frame slot with later temporaries; a function-scope named local does not (0x0087FD20, frame of 0x0052EA30).
- A same-TU callee body under `#pragma auto_inline(off)` lets an x87 value stay on the FPU stack across the call (0x006E9D90 GetRing).
- An inline stream read for a scalar should return by value (`ReadULong()`); an address-taken local forces a store on the fast path.
- A switch over 4 sparse small cases becomes a jump table; retail if-chains are written as if/else.
- `a - b` / `a + b` with inline C3DVector operators: the right operand is evaluated first; put the operand retail computes first on the right.
- Positive nesting (`if (ok) { ... } return false;`) moves early-return blocks to the end like retail.
- Register rotation family (second AddAction `lea ecx`, delete `mov edx`) in CCombatSequence OnCreate rows with an out-of-line second ctor, and `movzx` vs `xor/mov` zero extension, did not move with source changes -- lane gaps.
