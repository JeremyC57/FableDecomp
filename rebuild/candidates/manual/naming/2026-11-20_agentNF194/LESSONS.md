# NF194 lessons (one line each)
- A stand-in row's namespace-local CCountedPointer can move onto engine/CCountedPointer.h with per-TU decl-only / visible-dtor opt-ins and nothing else: the callee spellings (ShareData, ~CCountedPointer, (T*) ctor) follow the template's namespace, and the self leaf's hidden-result type moves with them (record it as a leaf-only s1 line).
- Census a data global by source grep of every REFS row before trusting reloc_bindings: two of the three "wrong" table spellings of 0x01375748 and both 0x0143E90C wrong spellings in the queue note were already gone (stale witnesses); compile the witness rows to prove it.
- History comments carry old spellings: grep declarations (extern lines), not names, or the census over-counts (141 of 143 "CScriptDef* pGlobalScriptDef" hits were comments).
- The exe-scan REFS list can hold false positives (0x0058578B for 0x00418DCA): a compiled probe that binds nothing at the address settles it.
- A shared-header string port of an old quest row can grade RELOCATION_MATCH and still be held: TEXT_EQ_INLINE emits a 20-byte ??8?$CBasicString@D pick-any copy while the owner 0x004115A0 is 88 bytes. Run tabdef/cmporc before celebrating.
- For CTimer::operator= in a script TU, copy the same-TU sibling's FABLE_FORCEINLINE definition; the plain `inline` one is called out of line (0x00E0A510, 759v778).
- A bare `cat $f` with an empty $f waits on stdin: guard file lists before cat in pipelines.
