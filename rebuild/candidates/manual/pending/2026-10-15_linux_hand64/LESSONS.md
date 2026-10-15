# Chunk 64 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A vector walk holding two pointers (the entry and `lea edi,[esi+8]` into it): no single-iterator form reproduces it;
  an explicit second pointer stepped by the entry size, in a do/while behind an entry test, with the end re-read into a
  local before the increment, does.
- A list walk passing an iterator to a callee that retail keeps in a register across the call (whole-program
  optimisation): walk the node pointer and write the iterator object after each step (a plain `for (...; ++it) Clear(it)`
  reloads the iterator from the stack).
- An identity matrix whose stores come out in reverse = chained assignments (`a = b = ... = 0.0f; d0 = d1 = d2 = 1.0f;`,
  a chain stores right to left).
- `MAX(MAX(x,y),z)` with the inner compare evaluated twice is a macro, not a function.
- `xor edx,edx ; mov dx,[..] ; shr edx,N ; test dl,1` = an inline `bool Is...() { return bit; }` accessor; a direct
  bitfield test gives `test byte ptr`.
- A by-value CCharString read through a call's returned pointer and destroyed before the next statement = a temporary
  passed to an inline helper taking `const CCharString&` (a named local reloads, merges the null tests, dies later).
- A walk reading `current->prev->data` with the stop re-read from the sentinel = an STLport `reverse_iterator` loop.
- An empty functor stored as a zero byte before being pushed = value-initialised `F()` (no user constructor).

Review at landing: the already-landed 0x00AF3F30 is GetNoActiveParticles, not GetNoActiveDecals -- fix its name/comment
(bytes unaffected). Parked 006aee40 / 006d5850: the wave-24l shared dead-slot gap (item + empty tag in one slot).
15 label corrections in NOTES.md (e.g. 004c7d50 is CThing::OnDie, 00a39cc0 CWADFile::Open, 00cacb80 the DirectShow CUnknown ctor).
