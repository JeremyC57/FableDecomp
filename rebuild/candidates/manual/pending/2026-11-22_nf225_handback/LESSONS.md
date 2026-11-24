# NF225 lessons (one line each)
- A size-body opt-in (explicit specialization of the header's own body under `optimize("s", on)`) lets an /O2 row expand a member at the site while its COMDAT is the size TU owner's body; VC7.1 then also keeps the nested ReleaseData / ShareData calls out of line in that COMDAT, so no DECL_ONLY is needed inside it.
- Prove size-pragma condition 1 by hashing every COMDAT (relocation fields zeroed) with and without the macros: only the owner copies may move.
- Before an owner check, add the new spellings to owners.tsv: pk.py prints NO-OWNER for a symbol in neither owners.tsv nor reloc_bindings, which hides a NEQ copy.
- VC7.1 PDBs have no S_INLINESITE records at all; the usable inline evidence is S_FRAMEPROC "marked inline" (inline-specified), "inlined" (expanded somewhere), and which compiland holds the kept copy.
- Porting a binder onto a header can flip an unrelated callee's spelling (here the protected ctor: QAE -> IAE); check every callee the header would respell before choosing a port over an in-place respell.
