## 0x00523CF0

- A local vector's zero stores in order start, eos, finish = a clear() right after construction (the clear re-stores _M_finish) (0x00523CF0).
- A same-TU callee taking a local vector by reference keeps the vector from escaping only with its REAL body visible (savers + STLport _M_range_insert noinline) (0x00523CF0).

## 0x006C8E90

- VC7.1 return-block placement (0x006C8E90): when retail tail-duplicates `return true` but lets `return false` fall into a shared `return result` exit (or the reverse), try the logic as nested single-exit blocks (`do { ... {ret=X; break;} } while (0); return ret;`, the shape of inlined helpers) instead of return statements; a wrap `if (type==1) {...} return false;` plus an && chain gives the early-out placement.

## 0x00AD9F10 (not landed)

LESSON: a callee's address-taken out-param (InsertBlock(index, added&)) only gets load-hoisted past an unrelated store when the callee's real body is visible in the TU (auto_inline(off)); a register-colour residue around such a local is callee visibility

