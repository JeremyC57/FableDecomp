# NF248 lessons
- The PDBs need the 4k repack: `llvm-pdbutil dump -publics` on `$S/codex27/pdbx/{FableWin,Ego_r}.4k.pdb` (tools/pub.sh). Always pass
  an anchored pattern and a small N: one broad grep printed 40 template spellings.
- `census.py` reads columns 2 / 3: reshape QUEUE_LINES first (work/ql3.tsv); a 0x0X prefix must be stripped.
- Vtable-slot binders are data: tools/dref.py finds the slot, the vtable start and its emitter rows; then vtcheck.py on each
  emitter object. Most emitters model only the first slots, so leaf-only renames of later slots have no referencer.
- Making a local-model member virtual: drop an explicit `void* __vftable;` member, and check the base chain is polymorphic from +0
  (an empty non-polymorphic base lands after the vptr and shifts every `this` adjustment).
- A file-static inline accessor keeps a retail re-read that a reference local lets VC7.1 CSE away.
- A test that defines a `const char x[] = ""` the row references needs an `extern` declaration first (const globals are internal).
- pre.sh (clang) before every grade: a missing define (DEF_CLASS_BASE, ETCInterfaceType) spins the lane cl until timeout; kill it with
  tools/killmine.sh.
