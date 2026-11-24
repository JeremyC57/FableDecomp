# NF307 lessons (one line each)
- Witnesses matter as much as entries: build work/other_agents_witnesses.txt from the neighbours' spellings columns (tools/mkoff.py); 4 of 40 entries here had their only wrong binder in it, and two more units had to stop one row short (0x00713EE0).
- The compiled census finds UNTABLED wrong binders: entry 36's witness was one of six rows binding the same PBV LookAtPos stand-in; fix them in the same pass or the delete line is wrong.
- A remaining wrong binder turns a delete into a rewitness: assemble.py now reads work/tc_skip.tsv (old symbol, target) and takes the hand line from tc_extra.tsv.
- Pick-any copies renamed by a re-scope: check whether the leaf references them (objdump -r: only .debug$F here), then compare same-named COMDATs across every compiled object (tools/cmpcomdat.py): 0x00449F90 already carried byte-equal ??0 / ??1CPlayerSave@NPlayerManager copies.
- A self with a wrong local type for a parameter (long for the PDB enum) can still have binders that bind the PDB spelling and are unresolved at HEAD: the HEAD link shows it, and the self fix then resolves them (entry 5: five binders).
- A local `typedef long <PDB enum>` respells every symbol of the TU that names it, the row's own leaf included (0x007BB150): it is a self fix, not a binder fix.
- A "TU emitter" stub (`CTCMultiStrike::CTCMultiStrike(CThing&) { }` in 0x007CD810) is a strong definition under the PDB name beside the real self: a name census on the new spelling finds it; a binder respelled onto that name inherits the duplicate.
- lock.sh needs S EXPORTED (`export S=...` after sourcing env.sh), not only set.
