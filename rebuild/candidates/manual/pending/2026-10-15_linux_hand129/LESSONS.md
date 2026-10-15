# Lessons from hand chunk 129
- The out-parameter GetInterface template is very often used WITHOUT testing its result (`GetInterface(&p, T); p->Call();`): the
  absent arm reads the uninitialised out pointer from the key's frame word and VC tail-duplicates the call into both arms. 13 of
  the 40 rows were this shape.
- An out-pointer getter whose pointer must share the single `push ecx` frame word with a later inlined lookup key belongs in an
  inline accessor (its local is then an inner-scope local); a plain function-scope local takes its own frame word.
- A float local that retail keeps in an oversized frame (3 words for one value) is a vector with only one live component:
  `C3DVector rise(0, 0, x)` -- the dead zero stores vanish but the frame words stay (008c0790).
- A component reached through a member pointer as `this` of a virtual call (`thing->m_pPhysics->SetPosition`) needs an inline
  accessor to load straight into ecx (same rule as GOTCHAS 24a/24c, confirmed twice here).
- A string literal handed to a lookup that is one argument of a multi-argument base constructor: the implicit conversion
  temporary is built first and keeps the neighbouring immediate stores in place; the inline by-value helper hoists those stores
  above the string constructor (the opposite of 24a, where the string was the LAST evaluated argument).
- 0x006924B0 takes the animation name by `const CCharString&`; the CCreatureActionBase constructor family (0x00693B30) reproduces
  with initialiser-list members in declaration order however the byte stores interleave.
- In combat-action constructors the vtable store position relative to init-list scalars is not a reliable "base vs derived" signal:
  the same init list put +0x118 before and +0x114 after the vtable store (Kraken loops).
- Misattributed destructors: find the single caller by scanning for E8 rel32 targets, then map the member offset through the
  owner's PDB layout with retail 12-byte containers (006f2f60 = ~CombatDialogueManager).
