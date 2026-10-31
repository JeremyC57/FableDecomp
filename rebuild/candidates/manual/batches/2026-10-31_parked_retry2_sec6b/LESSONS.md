# Lessons from retry sec6b

- `#pragma inline_depth(0)` binds lexically to the call site being compiled. A region around a template definition does not
  reach its instantiation; depth 0 also outlines placement operator new inside a new-expression (00f277f0).
- `__forceinline` does not override depth 0 at a site, but it does beat the inline budget: in long functions the last return's
  dtors go out of line from budget exhaustion, and forceinline dtors bring them back (00cddcb0, 63 -> 14 lines). On a quest Main
  it also makes every exit share one inline cleanup, so it is not a fix where retail mixes inline and out-of-line exits (00d43db0).
- An implicit dtor still re-stores the vptr; model the dtor explicitly only when retail shows it.
- Chained assignment `a = b = 0` stores in descending order; separate statements store ascending (00c61610).
- STLport `erase(begin(), end())` through the copy helper keeps retail's unfolded `cmp eax,eax` (00f3ba70).
- do-while stops VC7.1's loop unroll; const& fill in resize; hoisting a common `changed = true` defeats cross-jumping.
- A `for (i = 0; i < n; ++i) p *= 10` loop with constant n folds away; `while (n--)` / do-while keeps retail's counted loop
  (00b1d550). VC7.1 also folds `x * c1 * c2` into one constant, so retail's two multiplies come from something else.
- Headers that declare a class without the method: `#define Class ClassLayout` + include + `#undef` + `class Class : public
  ClassLayout { methods }` gives the right mangled name while keeping the shared layout (00b277a0, 00b30b50, 00be6880, 00bfad90).
- Catalog extents often run into int3 padding and the next function; check for the ret and cut with addextra.py before grading
  (8 rows this sweep).
- `this` held in edx across calls, a dtor dropped after a by-const& call, and fields forwarded across a call are LTCG
  inter-procedural effects; don't spend edits on them.
