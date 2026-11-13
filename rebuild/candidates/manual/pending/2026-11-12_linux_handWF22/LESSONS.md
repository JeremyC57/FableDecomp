## 0x0063B3F0

- (wf22 0x0063B3F0) An `__ftol2` result VC7.1 reuses where retail truncates again: read the first use through an inline 2D view (`(long)p->ToC2DVector().Y`) -- the temporary splits the load's value number; casts, aliases, helpers and volatile all still CSE.
- (wf22 0x0063B3F0) Frame order of bool flags (valid lowest) and which branch falls into a shared join follow from where the flag is SET: `valid = true` at the end of each branch plus `if (valid && ...)` beat a `goto done` skip.

## 0x00F3A890

- Same-TU out-of-line callees taking a local struct by reference: put their real bodies before the row under auto_inline(off) -- 'not captured' lets VC7.1 forward the local's fields across unrelated calls and kill dead ctor stores (0x00F3A890).

## 0x009C9280

- (wf22, 0x009C9280) A same-TU callee inlined at one site and called at others (GetTextInfo / IsLoaded) may be source written out in place at that site, not an inline-budget split: hide the body and write the expanded expression where retail expands it.
- (wf22, 0x009C9280) Two returns that retail tail-merges (stub `push 1; push reg; jmp`) merge only when c2 allocates both tails alike: a named `bool loaded` for the first test did it; a callee body that is visible but still called (CBasicString::Compare) lets c2 keep a loaded array base across the call.

