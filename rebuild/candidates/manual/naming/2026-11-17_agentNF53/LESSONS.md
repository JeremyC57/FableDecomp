# NF53 lessons (round 7, engine-side tail of the name-fix queue)

1. **Read the RTTI base list before porting a "pass `this` as owner" call.** `ghidra_out/rtti_bases.tsv` showed
   CContainmentVolume : CMovableResource@0 and C3DMesh2 : C3DMeshStats : CMovableResource@0. The AllocateResource
   respell (`PAVCMovableResource`) was then an honest base-class conversion, with no casts. One opt-in line in the
   flattened header was enough.
2. **A whole-chain port is often only 1 to 3 grades when a landed sibling already uses the shared headers.** Rows ported
   that way here: 008c4990 (the 0x0096DBC0 define set), 0095b3e0 (0x00972990), 00ba1a70 (0x00B33B50) and
   00e33720 (0x00E33EA0). Find the sibling by grepping `rebuild/src/compiled` for the opt-in macro of the callee you
   need, and copy its `#define` block.
3. **Check the catalog label and the landed callers before trusting a row's own self name.** 0x00C1EA60 was spelled
   CTCCreatureNavigation::IsPosNavLineOfSightTo(C3DVector*, Thing*). Its label, and the landed caller 0x00C21780,
   both say CSoundSample::LoadData(CSoundEventManager*, CStreamBuffer*). The retail argument pushes confirmed which
   pointer is which.
4. **The callee's ABI shows the caller's real argument layout.** At 0x00C119D0, 0x00C1BAC0 is a plain `ret` with
   no stack argument. That made the pushed tag CSampleRef::Bind's fourth argument, with `this` set to the key itself,
   not the "key+4, three arguments" stand-in. Both readings give the same caller bytes, but only one fits the shared
   header.
5. **A shared header can bring its own class key.** libpk/CCodaFrameStreamer.h has `class CCodaFrame`, but landed
   rows use `struct CCodaFrame`. Including the header respelled an unrelated binding (Pop, PAU to PAV). Always diff
   the whole relocation set (`xr.sh`), not just the target. A port that creates a new wrong binding is a block, not a
   win.
6. **Body evidence beats a queue claim.** 0x00C0DF20 sprintf()s `"%p"`, so its argument is `const void*` and the
   claimed wide-string spelling was rejected. 0x00C17940 carries a retail `PKSerialiseLoad.h` trace string, so the
   claimed ReadMetadataData was rejected. Read the target body and its strings before accepting a libpk claim that
   disagrees with the landed leaf.
7. **The same stack slot can be reused across scopes.** At 008a7980, retail homes the two key temporaries and the
   filter object in the dead `script` argument slot. Seven source shapes of the temporary never shared that slot, so
   the row was blocked after the binding was solved. Without the earlier local-variable alias (`&key`) this residue
   is not reachable from honest C++ at this budget.
8. **A changed parent row means a funclet regrade.** Grep `#include ".*_<addr>.cpp"` again at the end: rows ported
   late brought five more funclets: four from 00c172f0 and 00c30f60, and one (01213c1a) from 00c2e610.
9. **Keep shell chains exit-clean.** The slips were an `ls` or `cat` of a missing path at the end of a chain, a python
   IndexError, and a script named `dis.py` that shadowed the stdlib. Put `; true` after probes that may miss, and
   never name a helper after a stdlib module.
