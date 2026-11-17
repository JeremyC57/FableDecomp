# NF92 lessons (round 7)

- **Opaque out-of-line callee vs. retail register reuse.** If retail keeps a local in a register across a call that takes it by `const&`
  (0x004C8050: ptc_base in ebp across list::remove) and shares the argument's frame word with a later temporary, the callee's body
  was visible in retail's TU. Make the body visible with noinline: the implicit same-TU instance, as with FABLE_RETAIL_STLP_LIST_REMOVE_NOINLINE.
  A const local or an rvalue argument changes nothing (2 grades spent).
- **Look for the original helper before writing out a condition.** A `!p || p->IsDead()` test laid out the merged erase at the bottom.
  The original IS_THING_ALIVE<T> (engine/ThingAlive.h, FableWin.pdb instances) put it where retail has it, in one grade. Grep FableWin
  for `IS_THING_ALIVE@V?$CIntelligentPointer@V<Class>` first.
- **Loop counters.** For `for (n ...; n < v.size(); ...)` with Ego_d `n` as long, compare against the unsigned size() directly
  (`jbe` / `jb`). A `(long)` cast gives `test eax,0FFFFFFF8h ; jle`.
- **Header-inline expansion question (q11224).**
  - Check the .text call sites and .rdata references of the expanded function.
  - Run a same-TU probe (out-of-line definition + qualified call) to prove the expansion.
  - Check FableWin.pdb placement: the .cpp bodies cluster apart from the header-inline COMDAT group.
- **Link proof.** Use /NODEFAULTLIB with /FORCE:UNRESOLVED and `LINK=` cleared. Without /NODEFAULTLIB, the link stops at LNK1104
  libcp.lib before reaching LNK2005.
- **Re-creating an overlay.** Never re-create an overlay directory with a fresh `cp` of the repo header. Edit the existing hdr/ copy.
  After any overlay change, check that every FABLE_RETAIL_* in rows/ resolves in rebuild/include or hdr/.
- **STLport tree recipe.** The tree trio for a new value type (operator= / _M_copy / _M_create_node) each grades RELOCATION_MATCH
  first time from the 0x004F9660 / 0x004F99DE / 0x004F9A6E recipes. Only the value type's header and its CCharString /
  CDefStringTable defines change.
