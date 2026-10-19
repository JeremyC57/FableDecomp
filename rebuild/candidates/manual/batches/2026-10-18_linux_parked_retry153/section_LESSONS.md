# retry201A lessons

- 0087FF60: audit allocator thunk imports before authoring. BFEA0E is malloc, BFEA1A scalar new; masked parity cannot distinguish them. Native CHitParameters map8 instead of donor16 shifts subsequent fields by8 while its vector remains16.
- 0055B8F0: VC7.1 struct-return through a typed PMF can insert copies from the returned eax pointer. Project the actual hidden result pointer explicitly through a genuine member-function pointer to reproduce the native callee-popped argument/eax-return ABI without fake vtable padding. Full host PMF representation must remain intact in fixtures.
- 0055B8F0: converting two float coordinates through the real long-coordinate constructor preserves all four truncations before the half-open comparison. Named mouse x/y locals reproduce the early pair load. Expression order determines x87 load order.
- 00D9C4A0: named string argument scope creates the string before reading the Thing virtual table and destroys it before testing the persisted hit bool. The actual fable_string.h and correct bool/const signatures match retail; replica strings and char substitutions are unacceptable masked wins.
- 0055BC30: read list next/previous into locals before unlink stores; alias-visible reloads otherwise add instructions. Correct semantic 203-byte source can still differ because VC7.1 shrinks callee-saved pushes into a branch.

Review at landing: exact source/hash versus payload, native ABI hidden-result projection, fullwidth PMFs, all external relocations, native8 versus donor16 map, const frame dispatch, bool quest flag, and rejected D9C4A0 new3 history. BC30 reached original cap10 including source-generation failure new6; do not regrade variants automatically.
