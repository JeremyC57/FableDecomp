# NF331 lessons (2026-11-24)
- Size the gate from the member's own constructs: for operator= the definers are out-of-class bodies, in-class bodies of local models, local
  models (implicit assignment) and the IMPLICIT_ASSIGNMENT opt-in. Cross-check it against the previous unit's link evidence (every object
  that emitted the symbol there must be in the gate) before compiling: that is the check that guards a "0 strong definers" claim.
- A census can be much smaller than the gate: 91 of 497 objects emit operator=; most rows that model CScriptThing never assign one.
- An equal-bytes copy is only equal when every relocation binds the self's symbol (here `??3@YAXPAX@Z` at 0x00BFE9BC): read the relocation
  shapes, not only BYTES_EQUAL. All 70 equal copies bind exactly the self's symbol.
- Differing copies of an inline assignment of a pimpl handle are almost all one shape: the inner counted-pointer operator= left as a call
  (25 bytes) where retail expands it (74). That is an inline-budget / visibility fact of each TU, not of the member.
- A heredoc that expands `$(for ...; do echo ...; done)` writes one object per line into the generated script, so only the first object
  reaches the link: write the list to a file and expand it with `$(cat list)` inside a quoted heredoc.
- Compiles on this lane run at well under a second each (497 census objects in about 3 minutes over 4 background parts).
