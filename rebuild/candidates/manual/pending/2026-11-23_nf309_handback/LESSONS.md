# NF309 lessons (for the next naming batch agent)

- **`>>` passes clang and fails VC7.1.** The clang ms-compat precheck accepts `CCountedPointer<CArray<T>>`, but cl 13.10 rejects it
  (SRC_FAIL at grade time). Write `> >` in every nested template argument before the precheck.
- **Stale table lines are cheap wins.** A witness that was re-authored but whose table line was not updated leaves a line that binds
  nothing. tools/stale.py finds the candidates. Prove each one by reading the witness HEAD object's external symbols (it references the PDB
  spelling, not the line's symbol), then grep landed row sources for the old name. A test-local mock with the same plain name is not a
  binding.
- **An implicitly virtual override already binds UAE.** If the base (a shared header or a virtual base) declares the function virtual, the
  derived override is virtual with no keyword. Read the HEAD object before counting such a binder as needing a fix.
- **Check the target, not just the binder, against stay-off.** Entry 169 was respelled before anyone noticed that its target was an NF307
  witness. Run so_check.py on the target address too.
- **Deleter targets are reached by absolute references, not calls.** Use tools/absrows.py (absolute reference -> catalog row) for
  CCountedPointer DeleteData / Destroy_ targets. xs_all lists only calls.
- **A mid-run landing changes the ground.** After another agent lands, rerun mkstayoff.py, confirm orig/ still equals HEAD, recheck
  TABLE_CHANGES against the new table, and check whether the newly landed rows call any of your table targets.
- **Never re-precheck in the call that launches a grade.** Even a comment-only edit belongs in its own earlier call.
- **A self fix can duplicate a binder's strong same-TU copy** (NF281 D2). Link HEAD against new before writing it up. LNK4006 means revert.
- **A source-regex name census misses constructors with initialiser lists, and templated definitions.** Confirm a 0 or broad hit by reading
  the target row.
- **T3_PORT_QUEUE has unit lines.** A line whose column 2 is not an address (nf284 `deferred-12`) lists its T3 targets in column 3.
  Parse every column of the queue for stay-off, not just column 2, and check the TARGETS of your table lines as well as your rows.
- **Column layouts differ between sections of the same file.** RENAMES section 1 has the Leaf in column 4; section 2 has new_symbol in
  column 3. A census that reads the wrong column silently checks nothing. Print the count of names checked and compare it with the
  number you expect.
- **Respell stale callee names in comments too** (`// Callees: X::OldName @ addr`). Do it before the final grade: the precheck goes in its own
  call, then the regrade.
