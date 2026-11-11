# Chunk Q14 lessons (GOTCHAS-ready)

- 0x00876CA0: three divisions of a C3DVector's components by the same global in one return expression compile (no /Op) to ONE `fld 1.0; fdiv [g]` reciprocal stored to a frame word, applied Z first -- spell the division as written (`(a-b)/g_step` x3, as the landed twin 0x00876E10), never a hand-made `inv` local.
- 0x00876CA0: a by-value `C3DVector p = (*it)->GetPos();` through the shared header's user-declared copy ctor gives retail's fld/fstp copy; a local POD model gives integer moves.
- 0x00987040: `return !c ? true : expr;` is retail's jne / `mov eax,1` / `xor eax,eax` tail; `c || ...` gives je/xor, `c ? expr : true` je, an early `return true` a separate `mov al,1`, a `bool result` local `mov al`.
- 0x00B80CE0: cloth GetDeltaTransform = `delta.SetInverse(CurrentTransform); delta.PreMultiply(LastTransform);` (last * inverse(current)), not inverse*last; on the shared CMatrix3x4 it is 682v682 with only PreMultiply's E12/E13 sum order left.
- q.py / gg.sh: a source with no `// Leaf:` line passes the next token as the leaf and grades the largest symbol -- always give the full mangled Leaf.
- 0x00844740 / 0x008EF200: retail does NOT CSE a repeated `sin*0.25` / `radius*radius` across two statements; every VC7.1 /O2 source spelling tried (operand swaps, member vs param, GFSqr) is CSE'd -- probably a different compile unit shape (out-of-line callee or volatile-free reload); parked.
