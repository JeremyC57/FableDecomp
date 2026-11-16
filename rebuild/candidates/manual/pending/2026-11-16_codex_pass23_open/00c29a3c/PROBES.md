# Pass23 vector parent

Read prior pass22 source/test/notes and boundary audit. Parent00c298d0 is explicitly authorized by README Test ports. Assigned00c29a3c starts at the rethrow tail, not a function entry; correct parent length is542. Only178bytes of the incorrectly positioned542-byte reservation lie in this parent; no unrelated trailing functions are claimed.

- Regrade on package headers first, original RTM standard headers second: whole parent542bytes masked equal.
- Reproduce the reported compile problem with the legacy harness standard headers: C2039, _Insert_n absent from std::vector. /showIncludes confirms those headers came from the harness SDK, not the package (which has no top-level vector file).
- Keep project include/ FIRST and select genuine VC7.1 RTM standard headers SECOND: compiles and matches. The supplied package's claimed STL-shadowing cause could not be reproduced with its actual files. Document both header hashes/include traces; do not invent a replacement STL or silently reverse include order.
- Strengthen the test: native /Od /W3 without /EHsc now exercises2700cases on actual std::vector<CHandleInfo> through the shared libpk header, plus the2700existing verbatim-algorithm adapter cases. The latter preserve injectable-helper fault coverage. GCC64 runs2700adapter cases plus6exception cases; it does not test the VC7.1-specific class layout.
- Fresh full graph:33owned code/data sections all masked equal,95operands,13external runtime contracts pass. No surrogate types or fabricated EH bodies. Native6layout assertions and58symbol conflict checks pass.
- Fresh wrapper for reserved00c29a30 reproduces the same parent, generated catch label at+0x160, and all12reserved bytes. Both native HandlerType pointers confirm the shared catch/rethrow region. This is parent coverage, not an independent function or additional542bytes.

No production source-shape rewrite was needed. Compiler configuration and test integration changes are recorded separately from reconstruction attempts.
