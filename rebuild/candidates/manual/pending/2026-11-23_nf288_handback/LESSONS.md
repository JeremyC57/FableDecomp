# NF288 lessons (one line each)
- Read the binding table from the objects, not from the sources: one `bind.py` pass (HEAD object of every census row, xr at the entry address + the self's DEF) gives the whole work list; the source census's BAD lines include other overloads (0x007E7580's GetText() const is 0x009C7A40) and misses stand-in selves.
- Scan every binder object for a DEF of the entry's method before editing: three same-TU strong copies (0x00560D00, 0x0058509F, 0x0066BD00 / 0x0066BDF0) turned would-be access fixes into T3.
- A `const` self respell compiles only if the body's callees are const too: 0x005FC840's callee self 0x005FBD20 is a stand-in signature (T3), 0x00582027 needed a const operator[] on its local container model (byte-equal).
- D1b covers unchanged landed tests only: when the CHANGED source itself fails clang in an unchanged own line, try NOWCHAR first (0x0059A238's CWideString + L" " passes under it), else prepare the edit and ask (0x004962C0's `unsigned int(...)`).
- A self move can strand another agent's binder: check the census rows against NF287's entries before moving a self (0x006D5260 / 0x006D5850).
- NOWCHAR settles row-local `wchar16` and `fable_string.h` wide-compare clang failures in old rows (4 controls); a shared-header clang error with no VC7.1 analogue (engine/CTCObjectAugmentations.h line 107, `explicit instantiation cannot be inline`) is not D1: leave that row out of the compile set.
- Claim locks for controls only after deciding which controls might become changed rows, or precheck their tests first: re-locking cost a release / precheck / re-claim round.
- Four grade lanes of ~22 rows each grade 88 HEAD controls in about three minutes; the whole census-to-recheck loop for 30 entries is ~25 tool calls.
