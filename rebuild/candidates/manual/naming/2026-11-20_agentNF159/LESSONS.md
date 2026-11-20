# NF159 lessons (round 10)

- **Adding a virtual to a local model with a double member moves everything.** The implicit vptr follows the class alignment (8), so
  members shift by 4. Make the pre-double span opaque (`char _baseNN[...]`) rather than reordering (0x005EA100, 1 grade).
- **An empty local base after a derived vptr lands at +4.** If retail does `lea ecx,[esi+4]` vs `mov ecx,esi`, the local base is the
  problem: give it the virtual dtor retail's base has (`virtual ~CTCBase();`) so the vptr is the base's (0x007D34A0).
- **??_G cannot be produced by a non-owner row.** The scalar deleting destructor comes out only with the vtable (ctor/dtor TU).
  A `delete p` in another TU emits neither ??_7 nor ??_G (work/exp/e1.cpp). Do not spend grades on it: mark blocked/port.
- **Keep families whole.** A callee spelling change (QAE -> UAE for a virtual override) has to move every landed binder of that address
  in the same hand-back; census with the HEAD probe first (work/probe.tsv), then coll.py after each row.
- **Tavern header ports need the full retail-mode set:** FABLE_RETAIL_CTCTAVERNGAME + C3DVECTOR + C3DVECTOR_PACKED + C2DVECTOR +
  CRIGHTHANDEDSET; CardBase-family headers also need FABLE_RETAIL_DEF_CLASS_BASE; CTCHeroStats members past the default layout need
  FABLE_RETAIL_CTCHEROSTATS_SERIALISE. Run the cpp precheck first: a missing mode is a cheap #error, not a spun cl.
- **Token check with stderr discarded.** `cpp ... 2>&1 | md5sum` hashes the warning paths too and reports a false difference; use `2>/dev/null`.
- **A same-TU stand-in body blocks the respell.** When a binder DEFINES a noinline stand-in of its callee (because retail had the
  callee's body in the same TU), giving it the PDB spelling makes a second strong definition of the callee row's symbol. Census the
  binder's defined symbols (objdump) before planning a respell; such rows go to the TU-emitter decision (q11671: 0x006A2370, 0x006A23E0).
- **The same-TU shape often has a shared-header opt-in.** 0x006A2220's noinline InsertGrow stand-in is exactly
  stlport/fable_containers.h + FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE (visible body, out of line): one grade, and the stand-in
  extern goes away.
- **A short `// Leaf:` line hides the self symbol from coll.py.** `??0CAIBrain` or `GetAllCarriedThings (...)` makes coll.py say
  NEW-SPELLING even on an exact match, and pd2/q.py may grade the largest function. Read the self object's real symbol (hp.sh + objdump)
  and write the full mangled leaf when the row is yours.
- **`?A0x<hash>` anonymous-namespace names differ per build path.** They show up as gone/new pairs in a scratch build; they are not renames.
- **Run vland with $S/wine/env.sh sourced.** Without it verify_and_land reads the Windows default root and dies on functions.tsv.
- **Exe-scan "abs" hits can be instruction bytes.** 0x004804DE's hit for 0x006A1060 was `83 4D 60 10 | 6A 00`; check the bytes
  before treating a row as a binder.
