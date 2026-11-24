# NF323 lessons (2026-11-24, Clone family + add-only declarations)
- The NF320 Q1 rule ("the emitted ??_7 matches retail slot for slot") is fully scriptable: compile, then compare each emitted slot's
  relocation symbol with the RTTI-port label of retail's slot target (tools/cloneproof.py). It caught nothing new after the models were
  built from the retail table, because the models were generated FROM the table (tools/ovlist.py + tools/mkmodel.py).
- Build the override list from the retail vftable, not from the header: clonetri.py's "slots whose label is not a base-chain member" is
  exactly the set a derived model must declare.
- labels_rtti_port_dtors.tsv carries the DONOR's dtor kind; 10 of 21 Clone classes are labelled ??_E while retail's body is scalar.
  Compare the ??_G body with retail's slot-0 bytes and treat the slot as the class's dtor, whatever the label kind.
- VC7.1 vftables always reference ??_E (a weak alias of ??_G without `new X[]`), so a slot-0 "DIFF" by name is expected.
- A header with an access the PDBs contradict (GetUpFromSitting's protected Clone) only shows up when the mangled self is compared
  with the PDB spelling; fix it in an `#ifdef` new / `#else` old opt-in, not by editing the line.
- The CCombatActionBase retail branch is selected by unrelated switches; FABLE_RETAIL_KRAKEN_ROAR is read by that header alone, so it
  is the cleanest selector (it needs DEF_CLASS_BASE, CSoundPair.h and a CAnimationEntry declaration).
- The repo include tree can move under you mid-pass (a landing touched ~45 headers): compare every orig/hdr copy with the repo before
  the final grade and rebuild the frozen trees from the current repo.
- recheck.py must run with the repo as cwd (it reads rebuild/build_candidates.ps1 relatively); mkpay.py does this, a bare call does not.
- probe.py strips only `_RTTI_DECLS` defines; with other opt-in names (`_TRANSFER_DECL`, `_RUMBLE_DECLS`) the probe sees the class's own
  declaration and passes trivially: extend the strip pattern for every new macro name.
- Link controls that are T3_PORT_QUEUE column-2 rows cannot be compiled (they need a lock): note them instead.
